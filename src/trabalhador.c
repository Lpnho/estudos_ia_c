#include "trabalhador.h"

status_t trabalhador_inicializar(
    tarefa_desc_t obter_tarefa_pendente,
    tarefa_desc_t finalizar_tarefa)
{
    if (obter_tarefa_pendente.func == NULL ||
        finalizar_tarefa.func == NULL)
    {
        return status_erro("trabalhador_inicializar: callbacks não implementadas: NULL\n");
    }

    trabalhador_t *trabalhador = malloc(sizeof(trabalhador_t));
    if (trabalhador == NULL)
    {
        return status_erro("trabalhador_inicializar: falha ao alocar memoria\n");
    }
    trabalhador->executando_tarefa = false;
    trabalhador->obter_tarefa_pendente = obter_tarefa_pendente;
    trabalhador->finalizar_tarefa = finalizar_tarefa;

    if (!pthread_mutex_init(&trabalhador->mutex, NULL))
    {
        free(trabalhador);
        return status_erro("trabalhador_inicializar: pthread_mutex_init falhou\n");
    }

    if (!pthread_cond_init(&trabalhador->lock, NULL))
    {
        pthread_mutex_destroy(&trabalhador->mutex);
        free(trabalhador);
        return status_erro("trabalhador_inicializar: pthread_cond_init falhou\n");
    }
    atomic_init(&trabalhador->rodando, true);

    if (!pthread_create(&trabalhador->thread, NULL, trabalhador_executar, (void *)&trabalhador))
    {
        pthread_mutex_destroy(&trabalhador->mutex);
        pthread_cond_destroy(&trabalhador->lock);
        free(trabalhador);
        return status_erro("trabalhador_inicializar: pthread_create falhou\n");
    }
    return status_sucesso(trabalhador);
}

void trabalhador_destruir(trabalhador_t *trabalhador)
{
    if (trabalhador == NULL)
        return;

    atomic_store(&trabalhador->rodando, false);

    pthread_mutex_lock(&trabalhador->lock);
    pthread_cond_signal(&trabalhador->lock);
    pthread_mutex_unlock(&trabalhador->lock);

    pthread_join(trabalhador->thread, NULL);

    pthread_mutex_destroy(&trabalhador->mutex);
    pthread_cond_destroy(&trabalhador->lock);
    free(trabalhador);
}

void *trabalhador_executar(void *arg)
{
    trabalhador_t *trabalhador = (trabalhador_t *)arg;
    tarefa_pendente_t *tarefa = NULL;
    while (atomic_load_explicit(&trabalhador->rodando, memory_order_acquire))
    {
        pthread_mutex_lock(&trabalhador->mutex);
        while (!trabalhador->executando_tarefa)
        {
            pthread_cond_wait(&trabalhador->lock, &trabalhador->mutex);
        }
        tarefa_desc_executar(&trabalhador->obter_tarefa_pendente);

        tarefa = (tarefa_pendente_t *)(trabalhador->obter_tarefa_pendente.ret);
        if (tarefa != NULL)
        {
            tarefa_pendente_incr_referencia(tarefa);
            tarefa_pendente_executar_tarefa(tarefa);
            tarefa_pendente_decr_referencia(tarefa);
        }

        pthread_mutex_unlock(&trabalhador->mutex);
        tarefa_desc_executar(&trabalhador->finalizar_tarefa);
    }

    pthread_exit(NULL);
    return NULL;
}

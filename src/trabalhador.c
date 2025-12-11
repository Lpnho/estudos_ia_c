#include "trabalhador.h"

status_t trabalhador_inicializar(pthread_t *thread, sem_t *sinal_termino)
{
    if (thread == NULL)
    {
        return status_erro("trabalhador_inicializar: thread trabalhadora NULL\n");
    }
    if (sinal_termino == NULL)
    {
        return status_erro("trabalhador_inicializar: semafono trabalhadora NULL\n");
    }

    trabalhador_t *trabalhador = malloc(sizeof(trabalhador_t));
    if (trabalhador == NULL)
    {
        return status_erro("trabalhador_inicializar: falha ao alocar memoria\n");
    }
    trabalhador->executando_tarefa = false;
    trabalhador->semaforo_termino = sinal_termino;
    trabalhador->thread = thread;

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

    if (trabalhador->thread != NULL)
    {
        pthread_join(*trabalhador->thread, NULL);
    }

    pthread_mutex_destroy(&trabalhador->mutex);
    pthread_cond_destroy(&trabalhador->lock);
    free(trabalhador);
}

void *trabalhador_executar(void *arg)
{
    trabalhador_t *trabalhador = (trabalhador_t *)arg;

    while (atomic_load_explicit(&trabalhador->rodando, memory_order_acquire))
    {
        pthread_mutex_lock(&trabalhador->mutex);
        while (!trabalhador->executando_tarefa)
        {
            pthread_cond_wait(&trabalhador->lock, &trabalhador->mutex);
        }
        if (trabalhador->tarefa != NULL)
        {
            tarefa_pendente_executar_tarefa(trabalhador->tarefa);
        }

        pthread_mutex_unlock(&trabalhador->mutex);
    }

    pthread_exit(NULL);
    return NULL;
}

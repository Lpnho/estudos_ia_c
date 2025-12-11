#include "escalonador.h"


status_t escalonador_inicializar(uint32_t n_threads)
{
    escalonador_t *escalonador = (escalonador_t *)malloc(sizeof(escalonador_t) + sizeof(pthread_t) * n_threads);
    if (escalonador == NULL)
    {
        return status_erro("escalonador_inicializar: Falha ao alocar memória: retorno NULL\n");
    }
    escalonador->n_threads = n_threads;
    escalonador->threads = (pthread_t *)(escalonador + 1);

    atomic_init(&escalonador->rodando, true);

    if (!sem_init(&escalonador->sem_iniciar_trabalhadores, true, 0))
    {
        free(escalonador);
        return status_erro("escalonador_inicializar: Falha ao iniciar semaforo: retorno NULL\n");
    }

    if (!sem_init(&escalonador->sem_trabalhadores_ociosos, true, n_threads))
    {
        sem_destroy(&escalonador->sem_iniciar_trabalhadores);
        free(escalonador);
        return status_erro("escalonador_inicializar: Falha ao iniciar semaforo: retorno NULL\n");
    }

    status_t result = sinc_fila_inicializar();
    if (!result.sucesso)
    {
        sem_destroy(&escalonador->sem_iniciar_trabalhadores);
        sem_destroy(&escalonador->sem_trabalhadores_ociosos);
        free(escalonador);
        return result;
    }

    escalonador->fila_tarefas = (sinc_fila_t *)result.resultado;

    return status_sucesso(escalonador);
}

void escalonador_destruir(escalonador_t *escalonador)
{
    if (escalonador == NULL)
        return;

    sem_destroy(&escalonador->sem_iniciar_trabalhadores);
    sem_destroy(&escalonador->sem_trabalhadores_ociosos);

    sinc_fila_destruir(escalonador->fila_tarefas);
    free(escalonador);
}

status_t escalonador_agendar_tarefa(escalonador_t *escalonador, tarefa_desc_t tarefa)
{
    if (escalonador == NULL)
    {
        return status_erro("agendar_tarefa: Escalonador NULL\n");
    }

    status_t result = tarefa_pendente_inicializar(tarefa);
    if (!result.sucesso)
    {
        return result;
    }

    sinc_fila_adic(escalonador->fila_tarefas, result.resultado);

    // sem_post_multiple(&escalonador->semaforo, barreira->recursos);

    return status_erro("agendar_tarefa: Não implementado\n");
}


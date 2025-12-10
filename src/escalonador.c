#include "escalonador.h"
#include <pthread.h>

status_t escalonador_inicializar(uint32_t n_threads)
{
    escalonador_t *escalonador = (escalonador_t *)malloc(sizeof(escalonador_t) + sizeof(pthread_t) * n_threads);
    if (escalonador == NULL)
    {
        return status_erro("escalonador_inicializar: Falha ao alocar memória: retorno NULL\n");
    }
    escalonador->n_threads = n_threads;
    escalonador->threads = (pthread_t *)(escalonador + 1);

    if (!sem_init(&escalonador->semaforo, true, 0))
    {
        free(escalonador);
        return status_erro("escalonador_inicializar: Falha ao iniciar semaforo: retorno NULL\n");
    }

    status_t result = sinc_fila_inicializar();
    if (!result.sucesso)
    {
        sem_destroy(&escalonador->semaforo);
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

    sem_destroy(&escalonador->semaforo);
    sinc_fila_destruir(escalonador->fila_tarefas);
    free(escalonador);
}

status_t tarefa_pendente_inicializar(tarefa_t tarefa, barreira_t *barreira)
{
    if (tarefa == NULL)
    {
        return status_erro("tarefa_pendente_inicializar: tarefa NULL\n");
    }

    if (barreira == NULL)
    {
        return status_erro("tarefa_pendente_inicializar: barreira NULL\n");
    }

    tarefa_pendente_t *tp = malloc(sizeof(tarefa_pendente_t));
    if (tp == NULL)
    {
        return status_erro("tarefa_pendente_inicializar: falha ao alocar memoria\n");
    }

    tp->tarefa = tarefa;
    tp->barreira = barreira;

    return status_sucesso(tp);
}

void tarefa_pendente_destruir(tarefa_pendente_t *tp)
{
    if (tp == NULL)
        return;

    free(tp);
}

status_t agendar_tarefa(escalonador_t *escalonador, tarefa_t tarefa, barreira_t *barreira)
{
    if (escalonador == NULL)
    {
        return status_erro("agendar_tarefa: Escalonador NULL\n");
    }

    status_t result = tarefa_pendente_inicializar(tarefa, barreira);
    if (!result.sucesso)
    {
        return result;
    }

    sinc_fila_adic(escalonador->fila_tarefas, result.resultado);

    sem_post_multiple(&escalonador->semaforo, barreira->recursos);

    return status_erro("agendar_tarefa: Não implementado\n");
}

void *trabalhador(void *arg)
{

    return NULL;
}

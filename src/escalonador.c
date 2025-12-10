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

    status_t result = fila_inicializar();
    if (!result.sucesso)
    {
        free(escalonador);
        return result;
    }

    escalonador->fila_tarefas = (fila_t *)result.resultado;

    return status_sucesso(escalonador);
}

void escalonador_destruir(escalonador_t *escalonador)
{
    if (escalonador == NULL)
        return;
    fila_destruir(escalonador->fila_tarefas);
    free(escalonador);
}

status_t agendar_tarefa(escalonador_t *escalonador, tarefa_t tarefa, barreira_t *barreira)
{
    return status_erro("agendar_tarefa: Não implementado\n");
}

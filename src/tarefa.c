#include "tarefa.h"

static void tarefa_pendente_destruir(void *ptr_tarefa);

status_t tarefa_pendente_inicializar(tarefa_desc_t tarefa)
{
    if (tarefa.func == NULL)
    {
        return status_erro("tarefa_pendente_inicializar: tarefa NULL\n");
    }

    tarefa_pendente_t *tp = malloc(sizeof(tarefa_pendente_t));
    if (tp == NULL)
    {
        return status_erro("tarefa_pendente_inicializar: falha ao alocar memoria\n");
    }
    tp->finalizado = false;
    tp->tarefa = tarefa;

    if (!pthread_mutex_init(&tp->mutex, NULL))
    {
        free(tp);
        return status_erro("tarefa_pendente_inicializar: pthread_mutex_init falhou\n");
    }

    if (!pthread_cond_init(&tp->lock, NULL))
    {
        pthread_mutex_destroy(&tp->mutex);
        free(tp);
        return status_erro("tarefa_pendente_inicializar: pthread_cond_init falhou\n");
    }

    status_t res = cr_inicializar(tarefa_pendente_destruir, tp);
    if (!res.sucesso)
    {
        tarefa_pendente_destruir(tp);
        return res;
    }
    tp->cnt_referencias = (contador_referencia_t *)res.resultado;
    return status_sucesso(tp);
}

status_t tarefa_pendente_executar_tarefa(tarefa_pendente_t *tarefa)
{
    if (tarefa == NULL)
        return status_erro("tarefa_pendente_executar_tarefa: tarefa NULL");

    void *ret = tarefa->tarefa.func(tarefa->tarefa.arg);
    pthread_mutex_lock(&tarefa->lock);
    tarefa->tarefa.ret = ret;
    tarefa->finalizado = true;
    pthread_cond_broadcast(&tarefa->lock);
    pthread_mutex_unlock(&tarefa->lock);

    return status_sucesso(NULL);
}

status_t tarefa_pendente_aguardar_tarefa(tarefa_pendente_t *tarefa)
{
    if (tarefa == NULL)
        return status_erro("tarefa_pendente_executar_tarefa: tarefa NULL");

    pthread_mutex_lock(&tarefa->lock);
    while (!tarefa->finalizado)
    {
        pthread_cond_wait(&tarefa->lock, &tarefa->mutex);
    }
    void *result = tarefa->tarefa.ret;
    pthread_mutex_unlock(&tarefa->lock);

    return status_sucesso(result);
}

status_t tarefa_pendente_incr_referencia(tarefa_pendente_t *tarefa)
{
    return cr_incr_referencia(tarefa->cnt_referencias);
}

status_t tarefa_pendente_decr_referencia(tarefa_pendente_t *tarefa)
{
    return cr_decr_referencia(tarefa->cnt_referencias);
}

static void tarefa_pendente_destruir(void *ptr_tarefa)
{
    tarefa_pendente_t *tarefa = (tarefa_pendente_t *)ptr_tarefa;
    if (tarefa == NULL)
        return;
    pthread_mutex_lock(&tarefa->lock);
    if (!tarefa->finalizado)
    {
        tarefa->finalizado = true;
    }
    pthread_cond_broadcast(&tarefa->lock);
    pthread_mutex_unlock(&tarefa->lock);

    pthread_mutex_destroy(&tarefa->mutex);
    pthread_cond_destroy(&tarefa->lock);
    free(tarefa);
}
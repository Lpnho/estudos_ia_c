#include "barreira.h"

static void barreira_destuir(void *ptr_barreira);

status_t barreira_inicializar(uint32_t recursos)
{
    barreira_t *barreira = malloc(sizeof(barreira_t));
    if (!barreira)
    {
        return status_erro("barreira_inicializar: Falha ao alocar memória\n");
    }

    barreira->recursos = recursos;
    barreira->notificado = false;

    if (!pthread_mutex_init(&barreira->mutex, NULL))
    {
        free(barreira);
        return status_erro("barreira_inicializar: pthread_mutex_init falhou\n");
    }

    if (!pthread_cond_init(&barreira->lock, NULL))
    {
        pthread_mutex_destroy(&barreira->mutex);
        free(barreira);
        return status_erro("barreira_inicializar: pthread_cond_init falhou\n");
    }

    status_t res = cr_inicializar(barreira_destuir, barreira);
    if (!res.sucesso)
    {
        barreira_destuir(barreira);
        return res;
    }
    barreira->cnt_referencias = (contador_referencia_t *)res.resultado;
    return status_sucesso(barreira);
}

status_t barreira_incr_referencia(barreira_t *barreira)
{
    return cr_incr_referencia(barreira->cnt_referencias);
}

status_t barreira_decr_referencia(barreira_t *barreira)
{
    return cr_decr_referencia(barreira->cnt_referencias);
}

status_t barreira_decr_recurso(barreira_t *barreira)
{
    if (!barreira)
    {
        return status_erro("barreira_decrementar_recurso: barreira NULL");
    }

    pthread_mutex_lock(&barreira->mutex);

    if (barreira->recursos > 0)
    {
        barreira->recursos--;
    }

    barreira->notificado = (barreira->recursos == 0);

    if (!barreira->notificado)
    {
        pthread_cond_signal(&barreira->lock);
    }

    pthread_mutex_unlock(&barreira->mutex);
    return status_sucesso(NULL);
}

status_t barreira_aguardar(barreira_t *barreira)
{
    if (!barreira)
    {
        return status_erro("barreira_aguardar: barreira NULL");
    }

    pthread_mutex_lock(&barreira->mutex);

    while (!barreira->notificado)
    {
        pthread_cond_wait(&barreira->lock, &barreira->mutex);
    }

    pthread_mutex_unlock(&barreira->mutex);
    return status_sucesso(NULL);
}

// Critico. Só destroi via contador de referências
static void barreira_destuir(void *ptr_barreira) 
{
    barreira_t *barreira = (barreira_t *)ptr_barreira;
    if (!barreira)
        return;

    pthread_mutex_destroy(&barreira->mutex);
    pthread_cond_destroy(&barreira->lock);
    free(barreira);
}
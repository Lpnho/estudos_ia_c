#include "barreira.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "status.h"

status_t barreira_inicializar(uint32_t recursos)
{
    barreira_t *barreira = malloc(sizeof(barreira_t));
    if (!barreira)
    {
        return status_erro("barreira_inicializar: Falha ao alocar memória\n");
    }

    barreira->n_referencias = 0;
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

    return status_sucesso(barreira);
}

void barreira_destuir(barreira_t *barreira)
{
    if (!barreira)
        return;

    pthread_mutex_destroy(&barreira->mutex);
    pthread_cond_destroy(&barreira->lock);
    free(barreira);
}

status_t barreira_incrementar_referencia(barreira_t *barreira)
{
    if (!barreira)
    {
        return status_erro("barreira_incrementar_referencia: barreira NULL");
    }

    pthread_mutex_lock(&barreira->mutex);
    barreira->n_referencias++;
    pthread_mutex_unlock(&barreira->mutex);

    return status_sucesso(NULL);
}

status_t barreira_decrementar_referencia(barreira_t *barreira)
{
    if (!barreira)
    {
        return status_erro("barreira_decrementar_referencia: barreira NULL");
    }

    pthread_mutex_lock(&barreira->mutex);

    if (barreira->n_referencias > 0)
    {
        barreira->n_referencias--;
    }

    if (barreira->notificado && barreira->n_referencias == 0)
    {
        pthread_cond_signal(&barreira->lock);
    }

    pthread_mutex_unlock(&barreira->mutex);
    return status_sucesso(NULL);
}

status_t barreira_decrementar_recurso(barreira_t *barreira)
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

    if (barreira->notificado && barreira->n_referencias == 0)
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

    while (!barreira->notificado || barreira->n_referencias != 0)
    {
        pthread_cond_wait(&barreira->lock, &barreira->mutex);
    }

    pthread_mutex_unlock(&barreira->mutex);
    return status_sucesso(NULL);
}

status_t barreira_renovar_recurso(barreira_t *barreira, uint32_t recursos)
{
    if (!barreira)
    {
        return status_erro("barreira_renovar_recurso: barreira NULL");
    }

    pthread_mutex_lock(&barreira->mutex);

    barreira->n_referencias = 0;
    barreira->recursos = recursos;
    barreira->notificado = false;

    pthread_mutex_unlock(&barreira->mutex);
    return status_sucesso(barreira);
}

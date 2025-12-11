#include "barreira.h"


static void barreira_destuir(barreira_t *barreira);

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
    barreira_incrementar_referencia(barreira);
    return status_sucesso(barreira);
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
    pthread_mutex_unlock(&barreira->mutex);
    
    if(barreira->n_referencias == 0)
    {
        barreira_destuir(barreira);
    }
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

static void barreira_destuir(barreira_t *barreira)
{
    if (!barreira)
        return;

    pthread_mutex_destroy(&barreira->mutex);
    pthread_cond_destroy(&barreira->lock);
    free(barreira);
}
#include "sinc_fila.h"

status_t sinc_fila_inicializar()
{
    sinc_fila_t *fila = (sinc_fila_t *)malloc(sizeof(sinc_fila_t));
    if (fila == NULL)
    {
        return status_erro("sinc_fila_inicializar: Falha ao alocar memória\n");
    }
    status_t result = fila_inicializar();
    if (!result.sucesso)
    {
        free(fila);
        return result;
    }
    fila->fila = (fila_t *)result.resultado;

    if (!pthread_mutex_init(&fila->mutex, NULL))
    {
        fila_destruir(fila->fila);
        free(fila);
        return status_erro("sinc_fila_inicializar: Falha ao iniciar mutex: retorno NULL\n");
    }

    return status_sucesso(fila);
}

void sinc_fila_destruir(sinc_fila_t *fila)
{
    if (fila == NULL)
        return;

    fila_destruir(fila->fila);
    pthread_mutex_destroy(&fila->mutex);
    free(fila);
}

status_t sinc_fila_adic(sinc_fila_t *fila, void *arg)
{
    if (fila == NULL)
        return status_erro("sinc_fila_adic: fila NULL\n");

    pthread_mutex_lock(&fila->mutex);
    status_t result = fila_adic(fila->fila, arg);
    pthread_mutex_unlock(&fila->mutex);
    return result;
}

status_t sinc_fila_remo(sinc_fila_t *fila)
{
    if (fila == NULL)
        return status_erro("sinc_fila_adic: fila NULL\n");

    pthread_mutex_lock(&fila->mutex);
    status_t result = fila_remo(fila->fila);
    pthread_mutex_unlock(&fila->mutex);
    return result;
}

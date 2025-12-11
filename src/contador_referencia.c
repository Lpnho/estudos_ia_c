#include "contador_referencia.h"

static void cr_destruir(contador_referencia_t *cr);

status_t cr_inicializar(destrutor_t destrutor, void *arg)
{
    contador_referencia_t *cr = malloc(sizeof(contador_referencia_t));
    if (!cr)
    {
        return status_erro("cr_inicializar: Falha ao alocar memória\n");
    }

    if (destrutor == NULL)
    {
        return status_erro("cr_inicializar: destrutor NULL\n");
    }

    if (!pthread_mutex_init(&cr->mutex, NULL))
    {
        return status_erro("cr_inicializar: pthread_mutex_init falhou\n");
    }
    cr->n_referencias = 1;
    cr->destrutor.arg = arg;
    cr->destrutor.func = destrutor;
    return status_sucesso(cr);
}

status_t cr_incr_referencia(contador_referencia_t *cr)
{
    if (!cr)
    {
        return status_erro("cr_incr_referencia: contador NULL");
    }

    pthread_mutex_lock(&cr->mutex);
    cr->n_referencias++;
    pthread_mutex_unlock(&cr->mutex);

    return status_sucesso(NULL);
}

status_t cr_decr_referencia(contador_referencia_t *cr)
{
    if (!cr)
    {
        return status_erro("cr_incr_referencia: contador NULL");
    }

    pthread_mutex_lock(&cr->mutex);

    if (cr->n_referencias > 0)
    {
        cr->n_referencias--;
    }
    pthread_mutex_unlock(&cr->mutex);

    if (cr->n_referencias == 0)
    {
        cr->destrutor.func(cr->destrutor.arg);
        cr_destruir(cr);
    }
    return status_sucesso(NULL);
}

static void cr_destruir(contador_referencia_t *cr)
{
    if (cr == NULL)
        return;
    pthread_mutex_destroy(&cr->mutex);
    free(cr);
}
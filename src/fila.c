#include "fila.h"
#include <stdlib.h>

status_t fila_inicializar()
{
    fila_t *fila = (fila_t *)malloc(sizeof(fila_t));
    if (fila == NULL)
    {
        return status_erro("fila_inicializar: Falha ao alocar memória\n");
    }

    fila->n_elementos = 0;
    fila->primeiro = NULL;
    fila->ultimo = NULL;

    return status_sucesso(fila);
}

void fila_destruir(fila_t *fila)
{
    if (fila == NULL)
        return;

    no_fila_t *no = fila->primeiro;
    while (no != NULL)
    {
        no_fila_t *proximo = no->proximo;
        free(no);
        no = proximo;
    }

    free(fila);
}

status_t fila_adic(fila_t *fila, void *arg)
{
    if (fila == NULL)
        return status_erro("fila_adic: fila NULL\n");

    if (fila->n_elementos != 0 &&
        (fila->primeiro == NULL || fila->ultimo == NULL))
    {
        return status_erro("fila_adic: inconsistência detectada\n");
    }

    no_fila_t *no = (no_fila_t *)malloc(sizeof(no_fila_t));
    if (no == NULL)
    {
        return status_erro("fila_adic: Falha ao alocar memória\n");
    }

    no->valor = arg;
    no->proximo = NULL;

    if (fila->n_elementos == 0)
    {
        fila->primeiro = no;
        fila->ultimo = no;
    }
    else
    {
        fila->ultimo->proximo = no;
        fila->ultimo = no;
    }

    fila->n_elementos++;
    return status_sucesso(no);
}

status_t fila_remo(fila_t *fila)
{
    if (fila == NULL)
        return status_erro("fila_remo: fila NULL\n");

    if (!fila->n_elementos || fila->primeiro == NULL)
        return status_erro("fila_remo: fila vazia\n");

    no_fila_t *removido = fila->primeiro;
    void *valor = removido->valor;

    fila->primeiro = removido->proximo;
    fila->n_elementos--;

    if (!fila->n_elementos)
        fila->ultimo = NULL;

    free(removido);
    return status_sucesso(valor);
}

bool fila_vazia(fila_t *fila)
{
    return !fila->n_elementos;
}

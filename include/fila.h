#pragma once
#include <stdint.h>
#include "status.h"

typedef struct no_fila_t
{
    struct no_fila_t *proximo;
    void *valor;
} no_fila_t;

typedef struct fila_t
{
    no_fila_t *primeiro;
    no_fila_t *ultimo;
    uint32_t n_elementos;
} fila_t;

status_t fila_inicializar();
void fila_destruir(fila_t *fila);

status_t fila_adic(fila_t *fila, void* arg);
status_t fila_remo(fila_t *fila);

bool fila_vazia(fila_t* fila);
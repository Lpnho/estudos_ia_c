#pragma once
#include "fila.h"
#include "status.h"
#include <pthread.h>
#include <stdlib.h>

typedef struct sinc_fila_t
{
    fila_t *fila;
    pthread_mutex_t mutex;
} sinc_fila_t;



status_t sinc_fila_inicializar();
void sinc_fila_destruir(sinc_fila_t *fila);

status_t sinc_fila_adic(sinc_fila_t *fila, void* arg);
status_t sinc_fila_remo(sinc_fila_t *fila);
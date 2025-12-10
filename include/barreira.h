#pragma once
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include "status.h"

typedef struct barreira_t
{
    bool notificado;
    uint32_t n_referencias;
    uint32_t recursos;
    pthread_mutex_t mutex;
    pthread_cond_t lock;
} barreira_t;

status_t barreira_inicializar(uint32_t recursos);
void barreira_destuir(barreira_t *barreira);


status_t barreira_incrementar_referencia(barreira_t *barreira);
status_t barreira_decrementar_referencia(barreira_t *barreira);

status_t barreira_renovar_recurso(barreira_t *barreira, uint32_t recursos);
status_t barreira_decrementar_recurso(barreira_t *barreira);

status_t barreira_aguardar(barreira_t *barreira);

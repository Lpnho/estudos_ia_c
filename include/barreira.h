#pragma once
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include "status.h"
#include "contador_referencia.h"

typedef struct barreira_t
{
    bool notificado;
    uint32_t recursos;
    pthread_mutex_t mutex;
    pthread_cond_t lock;
    contador_referencia_t* cnt_referencias;
} barreira_t;

status_t barreira_inicializar(uint32_t recursos);

status_t barreira_incr_referencia(barreira_t *barreira);
status_t barreira_decr_referencia(barreira_t *barreira);

status_t barreira_decr_recurso(barreira_t *barreira);

status_t barreira_aguardar(barreira_t *barreira);

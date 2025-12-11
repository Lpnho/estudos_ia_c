#pragma once
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

#include "status.h"

typedef void (*destrutor_t)(void *args);

typedef struct destrutor_desc_t
{
    destrutor_t func;
    void *arg;
} destrutor_desc_t;

typedef struct contador_referencia_t
{
    uint32_t n_referencias;
    destrutor_desc_t destrutor;
    pthread_mutex_t mutex;
} contador_referencia_t;

status_t cr_inicializar(destrutor_t destrutor, void* arg);

status_t cr_incr_referencia(contador_referencia_t *cr);
status_t cr_decr_referencia(contador_referencia_t *cr);

#pragma once
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdatomic.h>

#include "status.h"
#include "barreira.h"
#include "fila.h"
#include "sinc_fila.h"
#include "tarefa_desc.h"
#include "tarefa_pendente.h"

typedef struct trabalhador_t
{
    bool executando_tarefa;
    atomic_bool rodando;

    pthread_mutex_t mutex;
    pthread_cond_t lock;

    pthread_t thread;

    tarefa_desc_t obter_tarefa_pendente;
    tarefa_desc_t finalizar_tarefa;
} trabalhador_t;

status_t trabalhador_inicializar(
    tarefa_desc_t obter_tarefa_pendente,
    tarefa_desc_t finalizar_tarefa
);

void trabalhador_destruir(trabalhador_t *escalonador);

void *trabalhador_executar(void *);

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
#include "tarefa.h"


typedef struct escalonador_t
{
    uint32_t n_threads;
    sem_t sem_iniciar_trabalhadores;
    sem_t sem_trabalhadores_ociosos;

    atomic_bool rodando;
    sinc_fila_t *fila_tarefas;
    pthread_t *threads;
} escalonador_t;

status_t escalonador_inicializar(uint32_t n_threads);
void escalonador_destruir(escalonador_t *escalonador);

status_t escalonador_agendar_tarefa(escalonador_t *escalonador, tarefa_desc_t tarefa);
status_t escalonador_agendar_multiplas_tarefas(escalonador_t *escalonador, tarefa_desc_t tarefa);

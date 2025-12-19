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
#include "trabalhador.h"

#define N_THREADS_TRABALHADORES 10

typedef struct escalonador_t
{
    uint32_t n_trabalhadores;

    sem_t sem_trabalhos_pendentes;
    sem_t sem_trabalhadores_ociosos;

    atomic_bool rodando;
    sinc_fila_t *fila_tarefas;
    sinc_fila_t *fila_tarefas_multiplas;
    trabalhador_t **trabalhadores;
} escalonador_t;

status_t escalonador_inicializar(uint32_t n_trabalhadores);
void escalonador_destruir(escalonador_t *escalonador);

status_t escalonador_agendar_tarefa(escalonador_t *escalonador, tarefa_desc_t tarefa);
status_t escalonador_agendar_multiplas_tarefas(escalonador_t *escalonador, tarefa_desc_t tarefa);

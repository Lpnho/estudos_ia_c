#pragma once
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "status.h"
#include "barreira.h"
#include "fila.h"

typedef void *(*tarefa_t)(barreira_t *barreira, void *args);

typedef struct tarefa_pendente_t
{
    tarefa_t tarefa;
    barreira_t *barreira;
    void *args;
} tarefa_pendente_t;

typedef struct escalonador_t
{
    uint32_t n_threads;
    pthread_t *threads;
    fila_t* fila_tarefas;
} escalonador_t;



status_t escalonador_inicializar(uint32_t n_threads);
void escalonador_destruir(escalonador_t *escalonador);

status_t agendar_tarefa(escalonador_t *escalonador, tarefa_t tarefa, barreira_t *barreira);

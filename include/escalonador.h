#pragma once
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include "status.h"
#include "barreira.h"
#include "fila.h"
#include "sinc_fila.h"

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
    sinc_fila_t* fila_tarefas;
    
    sem_t semaforo;
} escalonador_t;



status_t escalonador_inicializar(uint32_t n_threads);
void escalonador_destruir(escalonador_t *escalonador);

status_t tarefa_pendente_inicializar(tarefa_t tarefa, barreira_t *barreira);
void tarefa_pendente_destruir(tarefa_pendente_t*escalonador);

status_t agendar_tarefa(escalonador_t *escalonador, tarefa_t tarefa, barreira_t *barreira);
void* trabalhador(void*);



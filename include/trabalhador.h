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

typedef struct trabalhador_t
{
    bool executando_tarefa;
    atomic_bool rodando;

    pthread_mutex_t mutex;
    pthread_cond_t lock;
    
    tarefa_pendente_t* tarefa;

    sem_t *semaforo_termino;
    pthread_t *thread;
} trabalhador_t;

status_t trabalhador_inicializar(pthread_t *thread, sem_t *sinal_termino);
void trabalhador_destruir(trabalhador_t *escalonador);

void *trabalhador_executar(void *);

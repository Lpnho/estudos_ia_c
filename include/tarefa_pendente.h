#pragma once
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdbool.h>

#include "status.h"
#include "tarefa_desc.h"
#include "contador_referencia.h"
#include "barreira.h"

typedef struct tarefa_pendente_t
{
    bool finalizado;
    tarefa_desc_t tarefa;

    pthread_cond_t lock;
    pthread_mutex_t mutex;
    contador_referencia_t *cnt_referencias;
} tarefa_pendente_t;

status_t tarefa_pendente_inicializar(tarefa_desc_t tarefa);

status_t tarefa_pendente_executar_tarefa(tarefa_pendente_t *tarefa);

status_t tarefa_pendente_aguardar_tarefa(tarefa_pendente_t *tarefa);

status_t tarefa_pendente_incr_referencia(tarefa_pendente_t *tarefa);

status_t tarefa_pendente_decr_referencia(tarefa_pendente_t *tarefa);

status_t tarefa_desc_executar(tarefa_desc_t *tarefa);

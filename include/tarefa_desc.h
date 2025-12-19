#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "status.h"

typedef void *(*tarefa_t)(void *args);

typedef struct tarefa_desc_t
{
    tarefa_t func;
    void *arg;
    void *ret;
} tarefa_desc_t;

status_t tarefa_desc_executar(tarefa_desc_t *tarefa);

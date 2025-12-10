#pragma once
#include <stdbool.h>

typedef struct status_t
{
    bool sucesso;
    const char* mensagem;
    void* resultado;
} status_t;

status_t status_sucesso(void* resultado);
status_t status_erro(const char* mensagem);
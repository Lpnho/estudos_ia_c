#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "status.h"
#include "cperceptron_conf.h"
#include "fativacao.h"


typedef struct camada_t 
{
    uint32_t n_neuronios;
    uint32_t n_entradas;
    funcao_ativacao f_ativacao;
    precisao_t *b;
    precisao_t *a;
    precisao_t *z;
    precisao_t *w;
} camada_t;

status_t camada_inicializar_recursos();
void camada_liberar_recursos();

status_t camada_inicializar(uint32_t n_entrada, uint32_t n_neuronios, funcao_ativacao fa);
void camada_destruir(camada_t *camada);

status_t camada_feedforward(camada_t*, precisao_t *dados);



#pragma once
#include "cperceptron_conf.h"
#include "status.h"
#include "camada.h"
#include <stdio.h>

typedef struct mlp_t
{
    uint32_t n_camadas;
    camada_t **camadas;
    uint32_t indice;
} mlp_t;

status_t mlp_inicializar(uint32_t n_camadas);

void mlp_destruir(mlp_t *mlp);

status_t mlp_adic_camada(mlp_t *builder, camada_t *camada);

status_t mlp_feedforward(mlp_t *mlp, precisao_t *dados);
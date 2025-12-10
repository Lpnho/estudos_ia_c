#include "cperceptron.h"

status_t mlp_inicializar(uint32_t n_camadas)
{
    mlp_t *result = malloc(sizeof(mlp_t) + n_camadas * sizeof(camada_t **));
    if (result == NULL)
    {
        return status_erro("Falha na alocação de memória para mlp!\n");
    }
    result->indice = 0;
    result->n_camadas = n_camadas;
    result->camadas = (camada_t **)(result + 1);
    return status_sucesso(result);
}

void mlp_destruir(mlp_t *mlp)
{
    if (mlp == NULL)
    {
        return;
    }

    if (mlp->indice != 0)
    {
        for (uint32_t i = 0; i < mlp->indice; ++i)
        {
            camada_destruir(mlp->camadas[i]);
        }
    }
    free(mlp);
}

status_t mlp_adic_camada(mlp_t *builder, camada_t *camada)
{
    if (builder->indice >= builder->n_camadas)
    {
        return status_erro("O número de camadas ultrapassa o valor informado! Atualize o n_camadas.\n");
    }

    builder->camadas[builder->indice] = camada;
    return status_sucesso(NULL);
}

status_t mlp_feedforward(mlp_t *mlp, precisao_t *dados)
{
    if (dados == NULL)
    {
        return status_erro("Os dados de entrada não podem ser nulos");
    }
}
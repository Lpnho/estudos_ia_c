#include "camada.h"
#include <string.h>

status_t camada_inicializar_recursos()
{
    srand(time(NULL));
    return status_sucesso(NULL);
}

void camada_liberar_recursos()
{
}

status_t camada_inicializar(uint32_t n_entrada, uint32_t n_neuronios, funcao_ativacao fa)
{
    if (!n_entrada || !n_neuronios)
    {
        return status_erro("Dimensão inválida!\n");
    }
    size_t tamanho_bloco_memoria = sizeof(camada_t) +
                                   3 * sizeof(precisao_t) * n_neuronios +
                                   sizeof(precisao_t) * n_entrada * n_neuronios;

    void *data = malloc(tamanho_bloco_memoria);

    if (data == NULL)
    {
        return status_erro("Erro de alocação de memória durante criação de camada.\n");
    }
    camada_t *camada = data;
    camada->n_neuronios = n_neuronios;
    camada->n_entradas = n_entrada;
    camada->f_ativacao = fa;

    camada->b = (precisao_t *)(camada + 1);
    camada->a = camada->b + n_neuronios;
    camada->z = camada->a + n_neuronios;
    camada->w = camada->z + n_neuronios;

    memset((void *)camada->b, 0, n_neuronios * sizeof(precisao_t)); // limpa o vies
    for (uint32_t i = 0; i < camada->n_neuronios * camada->n_entradas; ++i)
    {
        camada->w[i] = 2.0 * (((precisao_t)rand()) / ((precisao_t)RAND_MAX)) - 1.0; // normalizado entre -1 e 1
    }

    return status_sucesso((void *)camada);
}

void camada_destruir(camada_t *camada)
{
    if (camada != NULL)
    {
        camada->n_neuronios = 0;
        camada->n_entradas = 0;

        camada->b = NULL;
        camada->a = NULL;
        camada->z = NULL;
        camada->w = NULL;
        free(camada);
    }
}

status_t camada_feedforward(camada_t *camada, precisao_t *dados)
{
    if (dados == NULL)
    {
        return status_erro("camada_feedforward: Erro, dados de entrada inválidos: NULL.\n");
    }

    for (uint32_t neuronio_i = 0; neuronio_i < camada->n_neuronios; ++neuronio_i)
    {
        for (uint32_t dados_entrada_i = 0; dados_entrada_i < camada->n_entradas; ++dados_entrada_i)
        {
            camada->z[neuronio_i] += dados[dados_entrada_i];
        }
        // camada->a = 
    }
}

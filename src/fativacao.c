#include "fativacao.h"
#include <stdlib.h>
#include <math.h>
#include "cperceptron_conf.h"
#include "status.h"

status_t fa_inicializar_recursos()
{
#ifdef HABILITAR_SOFTMAX_BUFFER
    return fa_redimensionar_buffer_softmax(SOFTMAX_INICIAL_BUFFER_TAM);
#endif
    return status_sucesso(NULL);
}

void fa_liberar_recursos()
{
#ifdef HABILITAR_SOFTMAX_BUFFER
    fa_liberar_buffer_softmax();
#endif
}

// Classificação binária simples
PRECISAO fa_step(PRECISAO x)
{
    return (x < 0.0) ? 0.0 : 1.0;
}

// Classificação binária
PRECISAO fa_sigmoid(PRECISAO x)
{
    return 1.0 / (1.0 + EXPONENCIAL(-x));
}

// ?
PRECISAO fa_relu(PRECISAO x)
{
    return (x > 0.0) ? x : 0.0;
}

// Para ajustes normalizados entre o 0
PRECISAO fa_tanh(PRECISAO x)
{
    return tanhf(x);
}

// ?
PRECISAO fa_leaky_relu(PRECISAO x)
{
    return (x > 0.0) ? x : 0.01 * x;
}

#ifdef HABILITAR_SOFTMAX_BUFFER
uint32_t sofmax_buffer_tam = SOFTMAX_INICIAL_BUFFER_TAM;
PRECISAO *sofmax_buffer = NULL;

status_t fa_redimensionar_buffer_softmax(uint32_t n)
{
    if (n <= sofmax_buffer_tam)
    {
        return status_sucesso(NULL);
    }

    sofmax_buffer_tam = n;
    if (sofmax_buffer == NULL)
    {
        sofmax_buffer = malloc(sofmax_buffer_tam * sizeof(PRECISAO));
        return (sofmax_buffer == NULL) ? status_erro("Falha em manipular buffer do softmax!\n") : status_sucesso(NULL);
    }
    void *aux = realloc(sofmax_buffer, sofmax_buffer_tam * sizeof(PRECISAO));
    if (aux == NULL)
    {
        return status_erro("Falha em manipular buffer do softmax. Erro ao usar realloc .\n");
    }
    sofmax_buffer = aux;
    return status_sucesso(NULL);
}

void fa_liberar_buffer_softmax()
{
    if (sofmax_buffer != NULL)
    {
        free(sofmax_buffer);
        sofmax_buffer = NULL;
        sofmax_buffer_tam = 0;
    }
}
#endif

status_t fa_softmax(PRECISAO *x, PRECISAO *saida, uint32_t n)
{
    if (x == NULL || saida == NULL)
    {
        return status_erro("Softmax: ponteiro nulo.\n");
    }

    if (n == 0)
    {
        return status_erro("Softmax: n = 0.\n");
    }

#ifdef HABILITAR_SOFTMAX_BUFFER
    status_t result = fa_redimensionar_buffer_softmax(n);
    if (!result.sucesso)
    {
        return result;
    }
#endif

    PRECISAO soma = 0.0;
    for (uint32_t i = 0; i < n; ++i)
    {
#ifdef HABILITAR_SOFTMAX_BUFFER

        sofmax_buffer[i] = EXPONENCIAL(x[i]);
        soma += sofmax_buffer[i];
#else
        soma += EXPONENCIAL(x[i]);
#endif
    }
    for (uint32_t i = 0; i < n; ++i)
    {
#ifdef HABILITAR_SOFTMAX_BUFFER

        saida[i] = sofmax_buffer[i] / soma;
#else
        saida[i] = EXPONENCIAL(x[i]) / soma;
#endif
    }
    return status_sucesso(NULL);
}

PRECISAO fa_dstep(PRECISAO x)
{
    return (x == 0.0) ? 1.0 : 0;
}
PRECISAO fa_dsigmoid(PRECISAO x)
{
    PRECISAO sigmoid_x = fa_sigmoid(x);
    return fa_dsigmoid_opt(sigmoid_x);
}
PRECISAO fa_dsigmoid_opt(PRECISAO sigmoid_x)
{
    return sigmoid_x / (1 - sigmoid_x);
}
PRECISAO fa_drelu(PRECISAO x)
{
    return (x > 0.0) ? 1 : 0.0;
}
PRECISAO fa_dtanh(PRECISAO x)
{
    PRECISAO tanh_x = fa_tanh(x);
    return fa_dtanh_opt(tanh_x);
}
PRECISAO fa_dtanh_opt(PRECISAO tanh_x)
{
    return 1 - (tanh_x * tanh_x);
}
PRECISAO fa_dleaky_relu(PRECISAO x)
{
    return (x > 0.0) ? 1 : 0.01;
}
status_t fa_dsoftmax(PRECISAO *x, PRECISAO *saida, uint32_t n)
{
    return status_erro("Não implementado");
}
#pragma once
#include "cperceptron_conf.h"
#include "status.h"

typedef enum funcao_ativacao
{
    FA_STEP,
    FA_SIGMOID,
    FA_SOFTMAX,
    FA_RELU,
    FA_TANH,
    FA_LEAKY_RELU

} funcao_ativacao;




status_t fa_inicializar_recursos(); 

void fa_liberar_recursos();

PRECISAO fa_step(PRECISAO);

PRECISAO fa_sigmoid(PRECISAO);

PRECISAO fa_relu(PRECISAO);

PRECISAO fa_tanh(PRECISAO);

PRECISAO fa_leaky_relu(PRECISAO);

status_t fa_softmax(PRECISAO *x, PRECISAO *saida, uint32_t n);


PRECISAO fa_dstep(PRECISAO);

PRECISAO fa_dsigmoid(PRECISAO);

PRECISAO fa_dsigmoid_opt(PRECISAO sigmoid_x);

PRECISAO fa_drelu(PRECISAO);

PRECISAO fa_dtanh(PRECISAO);

PRECISAO fa_dtanh_opt(PRECISAO tanh_x);

PRECISAO fa_dleaky_relu(PRECISAO);

status_t fa_dsoftmax(PRECISAO *x, PRECISAO *saida, uint32_t n);



#ifdef HABILITAR_SOFTMAX_BUFFER
extern uint32_t sofmax_buffer_tam;
extern PRECISAO *sofmax_buffer;

status_t fa_redimensionar_buffer_softmax(uint32_t n);
void fa_liberar_buffer_softmax();
#endif

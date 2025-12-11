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

#ifdef HABILITAR_SOFTMAX_BUFFER
extern uint32_t sofmax_buffer_tam;
extern precisao_t *sofmax_buffer;

status_t fa_redimensionar_buffer_softmax(uint32_t n);
void fa_liberar_buffer_softmax();
#endif


status_t fa_inicializar_recursos(); 

void fa_liberar_recursos();

precisao_t fa_step(precisao_t);

precisao_t fa_sigmoid(precisao_t);

precisao_t fa_relu(precisao_t);

precisao_t fa_tanh(precisao_t);

precisao_t fa_leaky_relu(precisao_t);

status_t fa_softmax(precisao_t *x, precisao_t *saida, uint32_t n);


precisao_t fa_dstep(precisao_t);

precisao_t fa_dsigmoid(precisao_t);

precisao_t fa_dsigmoid_opt(precisao_t sigmoid_x);

precisao_t fa_drelu(precisao_t);

precisao_t fa_dtanh(precisao_t);

precisao_t fa_dtanh_opt(precisao_t tanh_x);

precisao_t fa_dleaky_relu(precisao_t);

status_t fa_dsoftmax(precisao_t *x, precisao_t *saida, uint32_t n);




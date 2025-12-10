#pragma once
#include <stdint.h>

#define PRECISAO_32 1
#define PRECISAO_64 0

#ifdef PRECISAO_32
    #define PRECISAO float
    #define EXPONENCIAL expf

#elif PRECISAO_64
    #define PRECISAO double
    #define EXPONENCIAL exp
#else
    #error "Defina PRECISAO_32 ou PRECISAO_64 como 1"
#endif

typedef PRECISAO precisao_t;


#define HABILITAR_SOFTMAX_BUFFER 1
#ifdef HABILITAR_SOFTMAX_BUFFER
    #define SOFTMAX_INICIAL_BUFFER_TAM 1000
#endif
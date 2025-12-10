#include "status.h"
#include <stdlib.h>

status_t status_sucesso(void *resultado)
{
    status_t res;
    res.sucesso = true;
    res.mensagem = NULL;
    res.resultado = resultado;
    return res;
}
status_t status_erro(const char *mensagem)
{
    status_t res;
    res.sucesso = false;
    res.mensagem = mensagem;
    res.resultado = NULL;
    return res;
}
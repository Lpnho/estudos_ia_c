#include "tarefa_desc.h"

status_t tarefa_desc_executar(tarefa_desc_t *tarefa)
{
    if (tarefa->func == NULL)
    {
        tarefa->ret = NULL;
        return status_erro("tarefa_desc_executar: Função inválida.\n");
    }
    tarefa->ret = tarefa->func(tarefa->arg);
    return status_sucesso(tarefa->ret);
}
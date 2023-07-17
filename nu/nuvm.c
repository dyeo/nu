#include "nu.h"

#include <stdarg.h>

// --------------------------------------------------------------------------------------------------------------------------------

nu_op *nu_new_op(uint8_t op, ...)
{
    nu_op *r = nu_malloc(nu_op);
    NU_ASSERT(r != NULL, "heap allocation error");
    r->op = op;

    va_list args;
    void *arg;

    uint8_t c = 0;
    va_start(args, op);
    while ((arg = va_arg(args, void *)) != NULL)
    {
        c++;
    }
    va_end(args);

    size_t i = 0;
    r->args = nu_calloc(void*, c);
    va_start(args, op);
    while ((arg = va_arg(args, void *)) != NULL)
    {
        r->args[i++] = arg;
    }
    va_end(args);

    return r;
}

void nu_free_op(nu_op *op)
{
    op->op = 0;
    free(op->args);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_vm *nu_new_vm()
{
    nu_vm *r = nu_malloc(nu_vm);
    NU_ASSERT(r != NULL, "heap allocation error");
    for (size_t i = 0; i < NU_REGISTERS; ++i)
    {
        r->reg[i] = NU_NONE;
    }
    r->glb = nu_new_obj();
    r->loc = nu_new_obj();
    r->opl = 0;
    r->opc = 16;
    r->ops = nu_calloc(nu_op *, r->opc);
    r->opp = 0;
    return r;
}

void nu_free_vm(nu_vm *vm)
{
    nu_free_obj(vm->glb);
    nu_free_obj(vm->loc);
    vm->opl = vm->opc = 0;
    for (size_t i = 0; i < vm->opl; ++i)
    {
        nu_free_op(vm->ops[i]);
    }
    free(vm->ops);
    free(vm);
}

// --------------------------------------------------------------------------------------------------------------------------------
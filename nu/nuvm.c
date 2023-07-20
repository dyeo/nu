#include "nu.h"

#include <stdarg.h>

// --------------------------------------------------------------------------------------------------------------------------------

nu_vm *nu_new_vm()
{
    nu_vm *r = nu_malloc(nu_vm);
    NU_ASSERT(r != NULL, "heap allocation error");
    for (size_t i = 0; i < NU_REGISTERS; ++i)
    {
        r->reg[i] = NU_NONE;
    }
    r->res = NU_NONE;
    r->glb = nu_new_obj();
    r->loc = nu_new_obj();
    r->opl = 0;
    r->opc = 16;
    r->op = nu_calloc(uint8_t, r->opc);
    r->opp = r->op;
    r->ops = 0;
    return r;
}

void nu_free_vm(nu_vm *vm)
{
    nu_free_obj(vm->glb);
    nu_free_obj(vm->loc);
    vm->opl = 0;
    vm->opc = 0;
    free(vm->op);
    vm->opp = 0;
    vm->ops = 0;
    free(vm);
}

// --------------------------------------------------------------------------------------------------------------------------------

void nu_vm_add_op(nu_vm *vm, const uint8_t *op, const size_t len)
{
    size_t diff = vm->opp - vm->op;
    if (vm->opl + len > vm->opc)
    {
        size_t new_opc = vm->opc;
        while (new_opc < vm->opl + len)
            new_opc *= 2;
        vm->opc = new_opc;
        vm->op = nu_realloc(uint8_t, vm->op, vm->opc);
    }
    memcpy(vm->op + vm->opl, op, len);
    vm->opl += len;
    vm->opp = vm->op + diff;
    vm->ops = vm->opl - diff;
}

void nu_vm_run(nu_vm *vm)
{
    uint8_t op = NU_VM_POP(vm, uint8_t);
    NU_ASSERT(op >= 0 && op < NU_OPS_COUNT, "Invalid opcode");
    nu_op_ptr[op](vm);
}

// --------------------------------------------------------------------------------------------------------------------------------
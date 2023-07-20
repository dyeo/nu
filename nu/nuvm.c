#include "nu.h"

#include <stdarg.h>

// --------------------------------------------------------------------------------------------------------------------------------

nu_vm *nu_vm_new()
{
    nu_vm *r = nu_malloc(nu_vm);
    NU_ASSERT(r != NULL, "heap allocation error");
    for (size_t i = 0; i < NU_REGISTERS; ++i)
    {
        r->reg[i] = NU_NONE;
    }
    r->res = NU_NONE;
    r->glb = nu_obj_new();
    r->loc = nu_obj_new();
    r->opl = 0;
    r->opc = 16;
    r->op = nu_calloc(r->opc, uint8_t);
    r->opp = r->op;
    r->opr = 0;
    return r;
}

void nu_vm_free(nu_vm *vm)
{
    nu_obj_free(vm->glb);
    nu_obj_free(vm->loc);
    vm->opl = 0;
    vm->opc = 0;
    free(vm->op);
    vm->opp = 0;
    vm->opr = 0;
    free(vm);
}

// --------------------------------------------------------------------------------------------------------------------------------

inline static size_t _nu_vm_rsz_op(nu_vm *vm, const size_t add)
{
    size_t diff = vm->opp - vm->op;
    if (vm->opl + add > vm->opc)
    {
        size_t new_opc = vm->opc;
        while (new_opc < vm->opl + add)
            new_opc *= 2;
        vm->opc = new_opc;
        vm->op = nu_realloc(vm->op, vm->opc, uint8_t);
    }
    return diff;
}

void nu_vm_add_op(nu_vm *vm, const uint8_t *op, const size_t len)
{
    size_t diff = _nu_vm_rsz_op(vm, len);
    nu_copy(vm->op + vm->opl, op, len, uint8_t);
    vm->opl += len;
    vm->opp = vm->op + diff;
    vm->opr = vm->opl - diff;
}

void nu_vm_ins_op(nu_vm *vm, const uint8_t *op, const ptrdiff_t pos, const size_t len)
{
    size_t diff = _nu_vm_rsz_op(vm, len);
    nu_move(vm->op + pos + len, vm->op + pos, vm->opl - pos, uint8_t);
    nu_copy(vm->op + pos, op, len, uint8_t);
    vm->opl += len;
    vm->opp = vm->op + diff;
    vm->opr = vm->opl - diff;
}

void nu_vm_run(nu_vm *vm)
{
    uint8_t op = NU_VM_POP(vm, uint8_t);
    NU_ASSERT(op >= 0 && op < NU_OPS_COUNT, "Invalid opcode");
    nu_op_ptr[op](vm);
}

// --------------------------------------------------------------------------------------------------------------------------------
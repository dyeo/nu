#include "nu.h"

// --------------------------------------------------------------------------------------------------------------------------------

inline void _nu_load_reg(nu_vm *vm, const uint8_t reg, nu_val *val)
{
    if (reg == NU_NULLREG)
    {
        return;
    }
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    nu_decref(vm->reg[reg]);
    vm->reg[reg] = val;
    nu_incref(val);
}

inline void _nu_load_res(nu_vm *vm, nu_val *val)
{
    nu_decref(vm->res);
    vm->res = val;
    nu_incref(val);
}

// --------------------------------------------------------------------------------------------------------------------------------

void nu_op_noop(nu_vm *vm)
{
}

void nu_op_load(nu_vm *vm)
{
    const uint8_t reg = NU_VM_POP(vm, uint8_t);
    nu_val *val = NU_VM_POP(vm, nu_val *);
    _nu_load_reg(vm, reg, val);
}

void nu_op_swap(nu_vm *vm)
{
    const uint8_t reg0 = NU_VM_POP(vm, uint8_t);
    const uint8_t reg1 = NU_VM_POP(vm, uint8_t);
    NU_ASSERT(reg0 >= 0 && reg0 < NU_REGISTERS, "Invalid register 0");
    NU_ASSERT(reg1 >= 0 && reg1 < NU_REGISTERS, "Invalid register 1");
    nu_val *tmp = vm->reg[reg0];
    if (reg0 != NU_NULLREG)
    {
        vm->reg[reg0] = vm->reg[reg1];
    }
    if (reg1 != NU_NULLREG)
    {
        vm->reg[reg1] = tmp;
    }
}

void nu_op_cmpv(nu_vm *vm)
{
    const uint8_t reg = NU_VM_POP(vm, uint8_t);
    const nu_val *val = NU_VM_POP(vm, nu_val *);
    const uint8_t how = NU_VM_POP(vm, uint8_t);
    nu_bool *res = nu_cmp(vm->reg[reg], val, how);
    _nu_load_res(vm, res);
}

void nu_op_cmpr(nu_vm *vm)
{
    const uint8_t reg0 = NU_VM_POP(vm, uint8_t);
    const uint8_t reg1 = NU_VM_POP(vm, uint8_t);
    const uint8_t how = NU_VM_POP(vm, uint8_t);
    nu_bool *res = nu_cmp(vm->reg[reg0], vm->reg[reg1], how);
    _nu_load_res(vm, res);
}

void nu_op_jmp(nu_vm *vm)
{
    const uint8_t *opp = NU_VM_POP(vm, uint8_t *);
    const nu_bool *res = nu_eq(NU_TRUE, vm->res);
    const uint8_t *amts[] = {vm->opp + 1, opp};
    vm->opp = amts[res == NU_TRUE];
}

void nu_op_jne(nu_vm *vm)
{
    const uint8_t *opp = NU_VM_POP(vm, uint8_t *);
    const nu_bool *res = nu_eq(NU_FALSE, vm->res);
    const uint8_t *amts[] = {vm->opp + 1, opp};
    vm->opp = amts[res == NU_TRUE];
}

void nu_op_njmp(nu_vm *vm)
{
    uint8_t *opp = NU_VM_POP(vm, uint8_t *);
    vm->opp = opp;
}

void nu_op_add(nu_vm *vm)
{
    const uint8_t reg = NU_VM_POP(vm, uint8_t);
    const nu_val *val = NU_VM_POP(vm, nu_val *);
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    nu_val *res = nu_add(vm->reg[reg], val);
    _nu_load_res(vm, res);
}

void nu_op_sub(nu_vm *vm)
{
    const uint8_t reg = NU_VM_POP(vm, uint8_t);
    const nu_val *val = NU_VM_POP(vm, nu_val *);
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    nu_val *res = nu_sub(vm->reg[reg], val);
    _nu_load_res(vm, res);
}

void nu_op_mul(nu_vm *vm)
{
    const uint8_t reg = NU_VM_POP(vm, uint8_t);
    const nu_val *val = NU_VM_POP(vm, nu_val *);
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    nu_val *res = nu_mul(vm->reg[reg], val);
    _nu_load_res(vm, res);
}

void nu_op_div(nu_vm *vm)
{
    const uint8_t reg = NU_VM_POP(vm, uint8_t);
    const nu_val *val = NU_VM_POP(vm, nu_val *);
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    nu_val *res = nu_div(vm->reg[reg], val);
    _nu_load_res(vm, res);
}

void nu_op_mod(nu_vm *vm)
{
    const uint8_t reg = NU_VM_POP(vm, uint8_t);
    const nu_val *val = NU_VM_POP(vm, nu_val *);
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    nu_val *res = nu_mod(vm->reg[reg], val);
    _nu_load_res(vm, res);
}

void nu_op_addv(nu_vm *vm)
{
    const uint8_t reg = NU_VM_POP(vm, uint8_t);
    const nu_val *key = NU_VM_POP(vm, nu_val *);
    const nu_val *val = NU_VM_POP(vm, nu_val *);
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    bool is_obj = nu_is_obj(vm->reg[reg]);
    bool is_arr = nu_is_arr(vm->reg[reg]);
    NU_ASSERT(is_obj || is_arr, "Register val is not an obj or an arr");
    NU_ASSERT(is_arr && nu_is_num(key), "Register val is arr, expected num key");
    const nu_bool *res = nu_literal_bool[nu_add_val(vm->reg, key, val)];
    _nu_load_res(vm, res);
}

void nu_op_setv(nu_vm *vm)
{
    const uint8_t reg = NU_VM_POP(vm, uint8_t);
    const nu_val *key = NU_VM_POP(vm, nu_val *);
    const nu_val *val = NU_VM_POP(vm, nu_val *);
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    bool is_obj = nu_is_obj(vm->reg[reg]);
    bool is_arr = nu_is_arr(vm->reg[reg]);
    NU_ASSERT(is_obj || is_arr, "Register val is not an obj or an arr");
    NU_ASSERT(is_arr && nu_is_num(key), "Register val is arr, expected num key");
    const nu_bool *res = nu_literal_bool[nu_set_val(vm->reg, key, val)];
    _nu_load_res(vm, res);
}

void nu_op_getv(nu_vm *vm)
{
    const uint8_t reg = NU_VM_POP(vm, uint8_t);
    const nu_val *key = NU_VM_POP(vm, nu_val *);
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register 0");
    bool is_obj = nu_is_obj(vm->reg[reg]);
    bool is_arr = nu_is_arr(vm->reg[reg]);
    NU_ASSERT(is_obj || is_arr, "Register val is not an obj or an arr");
    NU_ASSERT(is_arr && nu_is_num(key), "Register val is arr, expected num key");
    nu_val *res = nu_get_val(vm->reg[reg], key);
    _nu_load_res(vm, res);
}

void nu_op_delv(nu_vm *vm)
{
    const uint8_t reg = NU_VM_POP(vm, uint8_t);
    const nu_val *key = NU_VM_POP(vm, nu_val *);
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register 0");
    bool is_obj = nu_is_obj(vm->reg[reg]);
    bool is_arr = nu_is_arr(vm->reg[reg]);
    NU_ASSERT(is_obj || is_arr, "Register val is not an obj or an arr");
    NU_ASSERT(is_arr && nu_is_num(key), "Register val is arr, expected num key");
    nu_val *res = nu_del_val(vm->reg[reg], key);
    _nu_load_res(vm, res);
}

void nu_op_setg(nu_vm *vm)
{
    const nu_val *key = NU_VM_POP(vm, nu_val *);
    const nu_val *val = NU_VM_POP(vm, nu_val *);
    const nu_bool *res = nu_literal_bool[nu_obj_set_val(vm->glb, key, val)];
    _nu_load_res(vm, res);
}

void nu_op_setl(nu_vm *vm)
{
    const nu_val *key = NU_VM_POP(vm, nu_val *);
    const nu_val *val = NU_VM_POP(vm, nu_val *);
    const nu_bool *res = nu_literal_bool[nu_obj_set_val(vm->loc, key, val)];
    _nu_load_res(vm, res);
}

void nu_op_getg(nu_vm *vm)
{
    const nu_val *key = NU_VM_POP(vm, nu_val *);
    nu_val *res;
    if (key == NU_NONE)
    {
        res = nu_obj_get_val(vm->glb, key);
    }
    else
    {
        res = vm->glb;
    }
    _nu_load_res(vm, res);
}

void nu_op_getl(nu_vm *vm)
{
    const nu_val *key = NU_VM_POP(vm, nu_val *);
    nu_val *res;
    if (key == NU_NONE)
    {
        res = nu_obj_get_val(vm->loc, key);
    }
    else
    {
        res = vm->loc;
    }
    _nu_load_res(vm, res);
}

void nu_op_delg(nu_vm *vm)
{
    const nu_val *key = NU_VM_POP(vm, nu_val *);
    nu_val *res = nu_obj_del_val(vm->glb, key);
    _nu_load_res(vm, res);
}

void nu_op_dell(nu_vm *vm)
{
    const nu_val *key = NU_VM_POP(vm, nu_val *);
    nu_val *res = nu_obj_del_val(vm->loc, key);
    _nu_load_res(vm, res);
}

// --------------------------------------------------------------------------------------------------------------------------------

#include "nu.h"

// --------------------------------------------------------------------------------------------------------------------------------

void nu_vm_noop(nu_vm *vm)
{
    vm->opp++;
}

void _nu_vm_load(nu_vm *vm, const uint8_t reg, nu_val *val)
{
    if (reg == NU_NULLREG)
    {
        return;
    }
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    if (vm->reg[reg] != NU_NONE)
    {
        nu_decref(vm->reg[reg]);
    }
    nu_incref(val);
    vm->reg[reg] = val;
}

void nu_vm_load(nu_vm *vm, const uint8_t reg, nu_val *val)
{
    _nu_vm_load(vm, reg, val);
    vm->opp++;
}

void nu_vm_swap(nu_vm *vm, const uint8_t reg0, const uint8_t reg1)
{
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
    vm->opp++;
}

void nu_vm_cmpv(nu_vm *vm, const uint8_t reg, const nu_val *val, const uint8_t how)
{
    const nu_bool *res = nu_cmp(vm->reg[reg], val, how);
    _nu_vm_load(vm, reg, val);
    vm->opp++;
}

void nu_vm_cmpr(nu_vm *vm, const uint8_t reg0, const uint8_t reg1, const uint8_t how)
{
    const nu_bool *res = nu_cmp(vm->reg[reg0], vm->reg[reg1], how);
    _nu_vm_load(vm, reg0, res);
    vm->opp++;
}

void nu_vm_jmp(nu_vm *vm, const uint8_t reg, const size_t opp)
{
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    const nu_bool *res = nu_eq(NU_TRUE, vm->reg[reg]);
    const size_t amts[] = {vm->opp + 1, opp};
    vm->opp = amts[res == NU_TRUE];
}

void nu_vm_njmp(nu_vm *vm, const size_t opp)
{
    vm->opp = opp;
}

void nu_vm_add(nu_vm *vm, const uint8_t reg, const nu_val *val)
{
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    nu_val *res = nu_add(vm->reg[reg], val);
    _nu_vm_load(vm, reg, res);
    vm->opp++;
}

void nu_vm_sub(nu_vm *vm, const uint8_t reg, const nu_val *val)
{
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    nu_val *res = nu_sub(vm->reg[reg], val);
    _nu_vm_load(vm, reg, res);
    vm->opp++;
}

void nu_vm_mul(nu_vm *vm, const uint8_t reg, const nu_val *val)
{
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    nu_val *res = nu_mul(vm->reg[reg], val);
    _nu_vm_load(vm, reg, res);
    vm->opp++;
}

void nu_vm_div(nu_vm *vm, const uint8_t reg, const nu_val *val)
{
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    nu_val *res = nu_div(vm->reg[reg], val);
    _nu_vm_load(vm, reg, res);
    vm->opp++;
}

void nu_vm_mod(nu_vm *vm, const uint8_t reg, const nu_val *val)
{
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    nu_val *res = nu_mod(vm->reg[reg], val);
    _nu_vm_load(vm, reg, res);
    vm->opp++;
}

void nu_vm_addv(nu_vm *vm, const uint8_t reg, const nu_val *key, const nu_val *val)
{
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    bool is_obj = nu_is_obj(vm->reg[reg]);
    bool is_arr = nu_is_arr(vm->reg[reg]);
    NU_ASSERT(is_obj || is_arr, "Register val is not an obj or an arr");
    NU_ASSERT(is_arr && nu_is_num(key), "Register val is arr, expected num key");
    nu_add_val(vm->reg, key, val);
    vm->opp++;
}

void nu_vm_setv(nu_vm *vm, const uint8_t reg, const nu_val *key, const nu_val *val)
{
    NU_ASSERT(reg >= 0 && reg < NU_REGISTERS, "Invalid register");
    bool is_obj = nu_is_obj(vm->reg[reg]);
    bool is_arr = nu_is_arr(vm->reg[reg]);
    NU_ASSERT(is_obj || is_arr, "Register val is not an obj or an arr");
    NU_ASSERT(is_arr && nu_is_num(key), "Register val is arr, expected num key");
    nu_set_val(vm->reg, key, val);
    vm->opp++;
}

void nu_vm_getv(nu_vm *vm, const uint8_t reg0, const nu_val *key, const uint8_t reg1)
{
    NU_ASSERT(reg0 >= 0 && reg0 < NU_REGISTERS, "Invalid register 0");
    NU_ASSERT((reg1 >= 0 && reg1 < NU_REGISTERS) || reg1 == NU_NULLREG, "Invalid register 0");
    bool is_obj = nu_is_obj(vm->reg[reg0]);
    bool is_arr = nu_is_arr(vm->reg[reg0]);
    NU_ASSERT(is_obj || is_arr, "Register val is not an obj or an arr");
    NU_ASSERT(is_arr && nu_is_num(key), "Register val is arr, expected num key");
    nu_val *res = nu_get_val(vm->reg[reg0], key);
    _nu_vm_load(vm, reg1, res);
    vm->opp++;
}

void nu_vm_delv(nu_vm *vm, const uint8_t reg0, const nu_val *key, const uint8_t reg1)
{
    NU_ASSERT(reg0 >= 0 && reg0 < NU_REGISTERS, "Invalid register 0");
    NU_ASSERT((reg1 >= 0 && reg1 < NU_REGISTERS) || reg1 == NU_NULLREG, "Invalid register 0");
    bool is_obj = nu_is_obj(vm->reg[reg0]);
    bool is_arr = nu_is_arr(vm->reg[reg0]);
    NU_ASSERT(is_obj || is_arr, "Register val is not an obj or an arr");
    NU_ASSERT(is_arr && nu_is_num(key), "Register val is arr, expected num key");
    nu_val *res = nu_del_val(vm->reg[reg0], key);
    _nu_vm_load(vm, reg1, res);
    vm->opp++;
}

// --------------------------------------------------------------------------------------------------------------------------------

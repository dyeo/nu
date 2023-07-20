#ifndef __NU_VM_OPS_INL__
#define __NU_VM_OPS_INL__

#include "nu.h"

#define NU_OP_NOOP 0
#define NU_OP_LOAD 1
#define NU_OP_SWAP 2
#define NU_OP_CMPV 3
#define NU_OP_CMPR 4
#define NU_OP_JMP 5
#define NU_OP_JNE 6
#define NU_OP_NJMP 7
#define NU_OP_ADD 8
#define NU_OP_SUB 9
#define NU_OP_MUL 10
#define NU_OP_DIV 11
#define NU_OP_MOD 12
#define NU_OP_ADDV 13
#define NU_OP_SETV 14
#define NU_OP_GETV 15
#define NU_OP_DELV 16
#define NU_OP_SETG 17
#define NU_OP_SETL 18
#define NU_OP_GETG 19
#define NU_OP_GETL 20
#define NU_OP_DELG 21
#define NU_OP_DELL 22
#define NU_OPS_COUNT 23

const static str_t nu_op_name[] = 
{
	[NU_OP_NOOP] = "noop",
	[NU_OP_LOAD] = "load",
	[NU_OP_SWAP] = "swap",
	[NU_OP_CMPV] = "cmpv",
	[NU_OP_CMPR] = "cmpr",
	[NU_OP_JMP] = "jmp",
	[NU_OP_JNE] = "jne",
	[NU_OP_NJMP] = "njmp",
	[NU_OP_ADD] = "add",
	[NU_OP_SUB] = "sub",
	[NU_OP_MUL] = "mul",
	[NU_OP_DIV] = "div",
	[NU_OP_MOD] = "mod",
	[NU_OP_ADDV] = "addv",
	[NU_OP_SETV] = "setv",
	[NU_OP_GETV] = "getv",
	[NU_OP_DELV] = "delv",
	[NU_OP_SETG] = "setg",
	[NU_OP_SETL] = "setl",
	[NU_OP_GETG] = "getg",
	[NU_OP_GETL] = "getl",
	[NU_OP_DELG] = "delg",
	[NU_OP_DELL] = "dell"
};

const static nu_op_fptr nu_op_ptr[] = 
{
	[NU_OP_NOOP] = &nu_op_noop,
	[NU_OP_LOAD] = &nu_op_load,
	[NU_OP_SWAP] = &nu_op_swap,
	[NU_OP_CMPV] = &nu_op_cmpv,
	[NU_OP_CMPR] = &nu_op_cmpr,
	[NU_OP_JMP] = &nu_op_jmp,
	[NU_OP_JNE] = &nu_op_jne,
	[NU_OP_NJMP] = &nu_op_njmp,
	[NU_OP_ADD] = &nu_op_add,
	[NU_OP_SUB] = &nu_op_sub,
	[NU_OP_MUL] = &nu_op_mul,
	[NU_OP_DIV] = &nu_op_div,
	[NU_OP_MOD] = &nu_op_mod,
	[NU_OP_ADDV] = &nu_op_addv,
	[NU_OP_SETV] = &nu_op_setv,
	[NU_OP_GETV] = &nu_op_getv,
	[NU_OP_DELV] = &nu_op_delv,
	[NU_OP_SETG] = &nu_op_setg,
	[NU_OP_SETL] = &nu_op_setl,
	[NU_OP_GETG] = &nu_op_getg,
	[NU_OP_GETL] = &nu_op_getl,
	[NU_OP_DELG] = &nu_op_delg,
	[NU_OP_DELL] = &nu_op_dell
};

#endif

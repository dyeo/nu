#ifndef __NU_H__
#define __NU_H__

#ifdef __cplusplus
extern "C" {
#endif

// --------------------------------------------------------------------------------------------------------------------------------

#include "utils.h"
#include "utf8.h"
#include "nurbt.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// --------------------------------------------------------------------------------------------------------------------------------

#define NU_T_NONE 0
#define NU_T_BOOL 1
#define NU_T_NUM  2
#define NU_T_STR  3
#define NU_T_FN   4
#define NU_T_ARR  5
#define NU_T_OBJ  6
#define NU_T_THR  7


// --------------------------------------------------------------------------------------------------------------------------------
// Object Definitions
// --------------------------------------------------------------------------------------------------------------------------------

#define NU_VAL_HEADER size_t type : NU_BASE_TYPE_BITS, refs : NU_BASE_REFS_BITS
#define NU_BASE_HEADER union { struct { NU_VAL_HEADER; }; nu_val base; }

typedef struct nu_val
{
    NU_VAL_HEADER;
} nu_val;

typedef struct nu_bool
{
    NU_BASE_HEADER;
    bool data;
} nu_bool;

typedef struct nu_num
{
    NU_BASE_HEADER;
    num_t data;
} nu_num;

typedef struct nu_str
{
    NU_BASE_HEADER;
    size_t len; // string character length
    size_t cap; // string byte length
    char *data;
} nu_str;

typedef struct nu_fn
{
    NU_BASE_HEADER;
} nu_fn;

typedef struct nu_arr
{
    NU_BASE_HEADER;
    size_t len; // array length
    size_t cap; // array capacity
    nu_val **data;
} nu_arr;

typedef struct nu_obj
{
    NU_BASE_HEADER;
    size_t len; // # of nodes
    rb_tree *data;
} nu_obj;

typedef struct nu_thr
{
    NU_BASE_HEADER;
} nu_thr;


// --------------------------------------------------------------------------------------------------------------------------------
// Function Pointer Definitions
// --------------------------------------------------------------------------------------------------------------------------------

NU_NEW_FPTR(nu_cmpr_t, const nu_bool *, const nu_val *, const nu_val *);
NU_NEW_FPTR(nu_oper_t, nu_val *, nu_val *, const nu_val *);


// --------------------------------------------------------------------------------------------------------------------------------
// Constants
// nu.c
// --------------------------------------------------------------------------------------------------------------------------------

const extern nu_val nu_none;
const extern nu_bool nu_true;
const extern nu_bool nu_false;
const extern nu_num nu_zero;
const extern nu_num nu_one;
const extern nu_str nu_empty;

#define NU_NONE (&nu_none)
#define NU_TRUE (&nu_true)
#define NU_FALSE (&nu_false)
#define NU_ZERO (&nu_zero)
#define NU_ONE (&nu_one)
#define NU_EMPTY (&nu_empty)

const extern nu_bool *nu_literal_bool[2];

inline static const nu_val *nu_oper_none(nu_val *_0, nu_val *_1) { return NU_NONE; }


// --------------------------------------------------------------------------------------------------------------------------------
// Initialization, Finalization, & Interpreter State
// nu.c
// --------------------------------------------------------------------------------------------------------------------------------

bool nu_initialize();
bool nu_finalize();
bool nu_is_initialized();


// --------------------------------------------------------------------------------------------------------------------------------
// Generic Object Methods
// nu.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_num *nu_len(const nu_val *val);
nu_num *nu_cap(const nu_val *val);

nu_num *nu_hash(const nu_val *val);
const char *nu_c_repr(const nu_val *val);

bool nu_set_val(nu_val *cnt, nu_val *key, nu_val *val);
nu_val *nu_get_val(nu_val *cnt, nu_val *key);
bool nu_add_val(nu_val *cnt, nu_val *key, nu_val *val);
nu_val *nu_del_val(nu_val *cnt, nu_val *key);

const nu_bool *nu_lt(const nu_val *lhs, const nu_val *rhs);
const nu_bool *nu_le(const nu_val *lhs, const nu_val *rhs);
const nu_bool *nu_eq(const nu_val *lhs, const nu_val *rhs);
const nu_bool *nu_ne(const nu_val *lhs, const nu_val *rhs);
const nu_bool *nu_ge(const nu_val *lhs, const nu_val *rhs);
const nu_bool *nu_gt(const nu_val *lhs, const nu_val *rhs);

#define NU_LT 0
#define NU_LE 1
#define NU_EQ 2
#define NU_NE 3
#define NU_GE 4
#define NU_GT 5

const nu_bool *nu_cmp(const nu_val *lhs, const nu_val *rhs, const uint8_t how);

nu_val *nu_add(nu_val *lhs, const nu_val *rhs);
nu_val *nu_sub(nu_val *lhs, const nu_val *rhs);
nu_val *nu_mul(nu_val *lhs, const nu_val *rhs);
nu_val *nu_div(nu_val *lhs, const nu_val *rhs);
nu_val *nu_mod(nu_val *lhs, const nu_val *rhs);

inline static bool nu_is_none(nu_val *val) { return val->type == NU_T_NONE; }
inline static bool nu_is_bool(nu_val *val) { return val->type == NU_T_BOOL; }
inline static bool nu_is_num(nu_val *val) { return val->type == NU_T_NUM; }
inline static bool nu_is_str(nu_val *val) { return val->type == NU_T_STR; }
inline static bool nu_is_fn(nu_val *val) { return val->type == NU_T_FN; }
inline static bool nu_is_arr(nu_val *val) { return val->type == NU_T_ARR; }
inline static bool nu_is_obj(nu_val *val) { return val->type == NU_T_OBJ; }
inline static bool nu_is_thr(nu_val *val) { return val->type == NU_T_THR; }


// --------------------------------------------------------------------------------------------------------------------------------
// Memory Management
// nu.c
// --------------------------------------------------------------------------------------------------------------------------------

void nu_free(nu_val *val);
inline static bool nu_opt_free(nu_val *val) { if(val->refs == 0) { nu_free(val); return true; } return false; }

inline static bool nu_incref(nu_val *val) { if (nu_is_none(val)) { return false; } else { NU_ASSERT(val->refs != NU_REFS_MAX, "val has too many refs"); val->refs++; return true; } }
inline static bool nu_decref(nu_val *val) { if (nu_is_none(val)) { return false; } else { NU_ASSERT(val->refs != 0, "val has no refs"); val->refs--; return true; } }


// --------------------------------------------------------------------------------------------------------------------------------
// Boolean Methods
// nubool.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_bool *nu_new_bool(bool data);
void nu_free_bool(nu_bool *bol);

bool nu_to_bool_c(nu_val *val);
nu_bool *nu_to_bool(nu_val *val);


// --------------------------------------------------------------------------------------------------------------------------------
// Number Methods
// nunum.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_num *nu_new_num(num_t data);
void nu_free_num(nu_num *num);

uint8_t nu_c_to_uint8(nu_val *val);
int8_t nu_c_to_int8(nu_val *val);
uint16_t nu_c_to_uint16(nu_val *val);
int16_t nu_c_to_int16(nu_val *val);
uint32_t nu_c_to_uint32(nu_val *val);
int32_t nu_c_to_int32(nu_val *val);
uint64_t nu_c_to_uint64(nu_val *val);
int64_t nu_c_to_int64(nu_val *val);
size_t nu_c_to_size_t(nu_val *val);
float nu_c_to_float(nu_val *val);
double nu_c_to_double(nu_val *val);

nu_num *nu_to_num(nu_val *val);


// --------------------------------------------------------------------------------------------------------------------------------
// String Methods
// nustr.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_str *nu_new_str(const char *data);
void nu_free_str(nu_str *str);

nu_str *nu_get_val_str(nu_str *str, nu_num *key);


// --------------------------------------------------------------------------------------------------------------------------------
// Array Methods
// nuarr.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_arr *nu_new_arr(size_t cap);
void nu_free_arr(nu_arr *arr);

inline static nu_num *nu_len_arr(nu_arr *arr) { return nu_new_num((num_t)arr->len); }
inline static nu_num *nu_cap_arr(nu_arr *arr) { return nu_new_num((num_t)arr->cap); }

bool nu_set_val_arr(nu_arr *arr, nu_num *idx, nu_val *val);
nu_val *nu_get_val_arr(nu_arr *arr, nu_num *idx);
bool nu_add_val_arr(nu_arr *arr, nu_num *idx, nu_val *val);
nu_val *nu_del_val_arr(nu_arr *arr, nu_num *idx);

bool nu_c_set_val_arr(nu_arr *arr, size_t i, nu_val *val);
nu_val *nu_c_get_val_arr(nu_arr *arr, size_t i);
bool nu_c_add_val_arr(nu_arr *arr, size_t i, nu_val *val);
nu_val *nu_c_del_val_arr(nu_arr *arr, size_t i);

inline static void nu_push_val_arr(nu_arr *arr, nu_val *val) { nu_c_add_val_arr(arr, arr->len, val); }
inline static nu_val *nu_pop_val_arr(nu_arr *arr) { return nu_c_del_val_arr(arr, arr->len - 1); }
inline static void nu_enqueue_val_arr(nu_arr *arr, nu_val *val) { nu_c_add_val_arr(arr, 0, val); }
inline static nu_val *nu_dequeue_val_arr(nu_arr *arr) { return nu_c_del_val_arr(arr, arr->len - 1); }


// --------------------------------------------------------------------------------------------------------------------------------
// Object Methods
// nuobj.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_obj *nu_new_obj();
void nu_free_obj(nu_obj *obj);

inline static nu_num *nu_len_obj(nu_obj *obj) { return nu_new_num((num_t)obj->len); }
inline static nu_num *nu_cap_obj(nu_obj *arr) { return nu_new_num((num_t)SIZE_MAX); }

inline static size_t nu_c_len_obj(nu_obj *obj) { return obj->len; }
inline static size_t nu_c_cap_obj(nu_obj *arr) { return SIZE_MAX; }

bool nu_set_val_obj(nu_obj *obj, nu_val *key, nu_val *val);
nu_val *nu_get_val_obj(nu_obj *obj, nu_val *key);
inline static bool nu_add_val_obj(nu_obj *obj, nu_val *key, nu_val *val) { return nu_set_val_obj(obj, key, val); }
nu_val *nu_del_val_obj(nu_obj *obj, nu_val *key);


// --------------------------------------------------------------------------------------------------------------------------------
// Miscellaneous Methods
// numisc.c
// --------------------------------------------------------------------------------------------------------------------------------

inline static nu_str *nu_repr(const nu_val *val) { return nu_new_str(nu_c_repr(val)); }


// --------------------------------------------------------------------------------------------------------------------------------
// Virtual Machine
// nuvm.c
// --------------------------------------------------------------------------------------------------------------------------------

// operation
typedef struct nu_op
{
    uint8_t op;
    void **args;
} nu_op;

nu_op *nu_new_op(uint8_t op, ...);
void nu_free_op(nu_op *op);

// virtual machine
typedef struct nu_vm
{
    nu_val *reg[NU_REGISTERS]; // registers

    nu_obj *glb; // global variables
    nu_obj *loc; // local variables
    
    size_t opl; // operation length
    size_t opc; // operation capacity
    nu_op **ops; // operations array
    
    size_t opp; // operation pointer
} nu_vm;

nu_vm *nu_new_vm();
void nu_free_vm(nu_vm *vm);


// --------------------------------------------------------------------------------------------------------------------------------
// Virtual Machine Operations
// nuvmops.c
// --------------------------------------------------------------------------------------------------------------------------------

#include "nuvmop.inl"

// no-op
NU_VM_OP_0(0x00, noop);

// load register with value
NU_VM_OP_2(0x01, load, const uint8_t, nu_val *);

// swap register values
NU_VM_OP_2(0x02, swap, const uint8_t, const uint8_t);

// compare register to value and set register to result
NU_VM_OP_3(0x03, cmpv, const uint8_t, const nu_val *, const uint8_t);

// compare register to other register and set register to result
NU_VM_OP_3(0x04, cmpr, const uint8_t, const uint8_t, const uint8_t);

// jump to op if value in register evaluates to true
NU_VM_OP_2(0x05, jmp, const uint8_t, const size_t);

// non-conditional jump to op
NU_VM_OP_1(0x06, njmp, const size_t);

// add value to value in register
NU_VM_OP_2(0x07, add, const uint8_t, const nu_val *);

// subtract value from value in register
NU_VM_OP_2(0x08, sub, const uint8_t, const nu_val *);

// multiply register value by other value
NU_VM_OP_2(0x09, mul, const uint8_t, const nu_val *);

// divide register value by other value
NU_VM_OP_2(0x0A, div, const uint8_t, const nu_val *);

// modulus register value by other value
NU_VM_OP_2(0x0B, mod, const uint8_t, const nu_val *);

// add value to obj or arr in register
NU_VM_OP_3(0x0C, addv, const uint8_t, const nu_val *, const nu_val *);

// set value in obj or arr in register
NU_VM_OP_3(0x0D, setv, const uint8_t, const nu_val *, const nu_val *);

// get value in obj or arr in one register and load it to another
NU_VM_OP_3(0x0E, getv, const uint8_t, const nu_val *, const uint8_t);

// delete value in obj or arr in one register and load it to another
NU_VM_OP_3(0x0F, delv, const uint8_t, const nu_val *, const uint8_t);


// --------------------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif//__NU_H__
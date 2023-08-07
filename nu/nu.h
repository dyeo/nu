#ifndef __NU_H__
#define __NU_H__

#ifdef __cplusplus
extern "C" {
#endif

// --------------------------------------------------------------------------------------------------------------------------------

#include "utils.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "lstr.h"

// --------------------------------------------------------------------------------------------------------------------------------

#define NU_TYPE_LIST                                                                                                              \
    NU_X(none)                                                                                                                    \
    NU_X(bool)                                                                                                                    \
    NU_X(num)                                                                                                                     \
    NU_X(str)                                                                                                                     \
    NU_X(fn)                                                                                                                      \
    NU_X(arr)                                                                                                                     \
    NU_X(obj)                                                                                                                     \
    NU_X(thr)

typedef enum nu_type_t
{
#define NU_X(V) nu_##V##_t,
    NU_TYPE_LIST
#undef NU_X
        NU_TYPES_COUNT
};

#define NU_NONE_T 0
#define NU_BOOL_T 1
#define NU_NUM_T 2
#define NU_STR_T 3
#define NU_FN_T 4
#define NU_ARR_T 5
#define NU_OBJ_T 6
#define NU_THR_T 7

// --------------------------------------------------------------------------------------------------------------------------------
// Object Definitions
// --------------------------------------------------------------------------------------------------------------------------------

#define NU_VAL_HEADER nusize_t type : NU_BASE_TYPE_BITS, refs : NU_BASE_REFS_BITS
#define NU_BASE_HEADER                                                                                                            \
    union                                                                                                                         \
    {                                                                                                                             \
        struct                                                                                                                    \
        {                                                                                                                         \
            NU_VAL_HEADER;                                                                                                        \
        };                                                                                                                        \
        nu_val base;                                                                                                              \
    }

// --------------------------------------------------------------------------------------------------------------------------------

typedef struct nu_val
{
    NU_VAL_HEADER;
} nu_val;

#define NU_PAST(VAL, TYPE) (TYPE *) (((nu_val *) VAL) + sizeof(nusize_t))

// --------------------------------------------------------------------------------------------------------------------------------

typedef struct nu_bool
{
    NU_BASE_HEADER;
    bool data;
} nu_bool;

#define NU_BOOL_INIT(V, DATA)                                                                                                     \
    do                                                                                                                            \
    {                                                                                                                             \
        V->type = NU_BOOL_T;                                                                                                      \
        V->refs = 0u;                                                                                                             \
        V->data = (DATA);                                                                                                         \
    } while (0)

// --------------------------------------------------------------------------------------------------------------------------------

typedef struct nu_num
{
    NU_BASE_HEADER;
    num_t data;
} nu_num;

#define NU_NUM_INIT(V, DATA)                                                                                                      \
    do                                                                                                                            \
    {                                                                                                                             \
        V->type = NU_NUM_T;                                                                                                       \
        V->refs = 0u;                                                                                                             \
        V->data = (DATA);                                                                                                         \
    } while (0)

// --------------------------------------------------------------------------------------------------------------------------------

typedef struct nu_str
{
    NU_BASE_HEADER;
    lstr_t data;
} nu_str;

#define NU_STR_INIT(V, DATA)                                                                                                      \
    do                                                                                                                            \
    {                                                                                                                             \
        V->type = NU_STR_T;                                                                                                       \
        V->data = (DATA);                                                                                                         \
    } while (0)

// --------------------------------------------------------------------------------------------------------------------------------

typedef struct nu_fn
{
    NU_BASE_HEADER;
} nu_fn;

#define NU_FN_INIT(V)                                                                                                             \
    do                                                                                                                            \
    {                                                                                                                             \
        V->type = NU_FN_T;                                                                                                        \
        V->refs = 0u;                                                                                                             \
    } while (0)

// --------------------------------------------------------------------------------------------------------------------------------

typedef struct nu_arr
{
    NU_BASE_HEADER;
    nu_val **data; // array (stb)
} nu_arr;

#define NU_ARR_INIT(V, DATA)                                                                                                      \
    do                                                                                                                            \
    {                                                                                                                             \
        V->type = NU_ARR_T;                                                                                                       \
        V->data = (DATA);                                                                                                         \
    } while (0)

// --------------------------------------------------------------------------------------------------------------------------------

typedef struct nu_kvp
{
    size_t hsh;  // hash key
    nu_val *key; // actual key
    nu_val *val; // val
} nu_kvp;

typedef struct nu_obj
{
    NU_BASE_HEADER;
    nu_kvp *data; // hashtable (stb)
} nu_obj;

#define NU_OBJ_INIT(V, DATA)                                                                                                      \
    do                                                                                                                            \
    {                                                                                                                             \
        V->type = NU_OBJ_T;                                                                                                       \
        V->refs = 0u;                                                                                                             \
        V->data = (DATA);                                                                                                         \
    } while (0)

// --------------------------------------------------------------------------------------------------------------------------------

typedef struct nu_thr
{
    NU_BASE_HEADER;
    nu_fn *fn;
} nu_thr;

#define NU_THR_INIT(V, FN)                                                                                                        \
    do                                                                                                                            \
    {                                                                                                                             \
        V->type = NU_THR_T;                                                                                                       \
        V->refs = 0u;                                                                                                             \
        V->fn   = (FN);                                                                                                           \
    } while (0)

// --------------------------------------------------------------------------------------------------------------------------------
// Function Pointer Definitions
// --------------------------------------------------------------------------------------------------------------------------------

NU_FPTR_NEW(nu_cmpr_t, const nu_bool *, const nu_val *, const nu_val *);
NU_FPTR_NEW(nu_oper_t, nu_val *, nu_val *, const nu_val *);

// --------------------------------------------------------------------------------------------------------------------------------
// Literals
// nu.h
// --------------------------------------------------------------------------------------------------------------------------------

static const nu_val nu_none   = {NU_NONE_T, 0ul};
static const nu_bool nu_true  = {NU_BOOL_T, 0ul, true};
static const nu_bool nu_false = {NU_BOOL_T, 0ul, false};
static const nu_num nu_zero   = {NU_NUM_T, 0ul, 0.0};
static const nu_num nu_one    = {NU_NUM_T, 0ul, 1.0};
static const nu_str nu_empty  = {NU_STR_T, 0ul, ""};

#define NU_NONE (&nu_none)
#define NU_TRUE (&nu_true)
#define NU_FALSE (&nu_false)
#define NU_ZERO (&nu_zero)
#define NU_ONE (&nu_one)
#define NU_EMPTY (&nu_empty)

#define NU_TRUE_VAL (&nu_true.base)
#define NU_FALSE_VAL (&nu_false.base)
#define NU_ZERO_VAL (&nu_zero.base)
#define NU_ONE_VAL (&nu_one.base)
#define NU_EMPTY_VAL (&nu_empty.base)

static const nu_bool *nu_literal_bool[2] = {NU_FALSE, NU_TRUE};

inline static const nu_val *nu_none_oper(nu_val *_0, nu_val *_1)
{
    return NU_NONE;
}

inline static bool nu_is_literal(const nu_val *val)
{
    return val == NU_NONE || val == NU_TRUE_VAL || val == NU_FALSE_VAL || val == NU_ZERO_VAL || val == NU_ONE_VAL ||
           val == NU_EMPTY_VAL;
}

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
size_t nu_hash_c(const nu_val *val);
str_t nu_repr_c(const nu_val *val);

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
nu_val *nu_mod(nu_val *lhs, const nu_val *rhs);
nu_val *nu_div(nu_val *lhs, const nu_val *rhs);

inline static bool nu_is_none(nu_val *val)
{
    return val->type == NU_NONE_T;
}
inline static bool nu_is_bool(nu_val *val)
{
    return val->type == NU_BOOL_T;
}
inline static bool nu_is_num(nu_val *val)
{
    return val->type == NU_NUM_T;
}
inline static bool nu_is_str(nu_val *val)
{
    return val->type == NU_STR_T;
}
inline static bool nu_is_fn(nu_val *val)
{
    return val->type == NU_FN_T;
}
inline static bool nu_is_arr(nu_val *val)
{
    return val->type == NU_ARR_T;
}
inline static bool nu_is_obj(nu_val *val)
{
    return val->type == NU_OBJ_T;
}
inline static bool nu_is_thr(nu_val *val)
{
    return val->type == NU_THR_T;
}

nu_val *nu_coerce_c(nu_val *lhs, uint8_t type);
#define nu_coerce(VAL, TYPE) ((TYPE *) nu_coerce_c(VAL, TYPE##_t))

// --------------------------------------------------------------------------------------------------------------------------------
// Memory Management
// nu.c
// --------------------------------------------------------------------------------------------------------------------------------

void nu_free(nu_val *val);
inline static bool nu_opt_free(nu_val *val)
{
    if (val->refs > 0)
    {
        return false;
    }
    nu_free(val);
    return true;
}

inline static bool nu_incref(nu_val *val)
{
    if (nu_is_literal(val))
    {
        return false;
    }
    NU_ASSERT(val->refs != NU_REFS_MAX, "val has too many refs");
    val->refs++;
    return true;
}
inline static bool nu_decref(nu_val *val)
{
    if (nu_is_literal(val))
    {
        return false;
    }
    NU_ASSERT(val->refs != 0, "val has no refs");
    val->refs--;
    return true;
}

// --------------------------------------------------------------------------------------------------------------------------------
// Boolean Methods
// nubool.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_bool *nu_bool_new(bool data);
void nu_bool_free(nu_bool *bol);

bool nu_bool_get_c(nu_val *val);
nu_bool *nu_bool_get(nu_val *val);

// --------------------------------------------------------------------------------------------------------------------------------
// Number Methods
// nunum.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_num *nu_num_new(num_t data);
void nu_num_free(nu_num *num);

uint8_t nu_uint8_get_c(nu_val *val);
int8_t nu_int8_get_c(nu_val *val);
uint16_t nu_uint16_get_c(nu_val *val);
int16_t nu_int16_get_c(nu_val *val);
uint32_t nu_uint32_get_c(nu_val *val);
int32_t nu_int32_get_c(nu_val *val);
uint64_t nu_uint64_get_c(nu_val *val);
int64_t nu_int64_get_c(nu_val *val);
size_t nu_size_t_get_c(nu_val *val);
float nu_float_get_c(nu_val *val);
double nu_double_get_c(nu_val *val);

nu_num *nu_num_get(nu_val *val);

// --------------------------------------------------------------------------------------------------------------------------------
// String Methods
// nustr.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_str *nu_str_new(str_t data);
void nu_str_free(nu_str *str);

nu_str *nu_str_get_val(nu_str *str, nu_num *key);

// --------------------------------------------------------------------------------------------------------------------------------
// Function Methods
// nufn.c
// --------------------------------------------------------------------------------------------------------------------------------

// TODO: implement

// --------------------------------------------------------------------------------------------------------------------------------
// Array Methods
// nuarr.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_arr *nu_arr_new(size_t cap);
void nu_arr_free(nu_arr *arr);

inline static nu_num *nu_arr_len(nu_arr *arr)
{
    return nu_num_new((num_t) arrlen(arr->data));
}
inline static nu_num *nu_arr_cap(nu_arr *arr)
{
    return nu_num_new((num_t) arrcap(arr->data));
}

bool nu_arr_set_val(nu_arr *arr, nu_num *idx, nu_val *val);
nu_val *nu_arr_get_val(nu_arr *arr, nu_num *idx);
bool nu_arr_add_val(nu_arr *arr, nu_num *idx, nu_val *val);
nu_val *nu_arr_del_val(nu_arr *arr, nu_num *idx);

bool nu_arr_set_val_c(nu_arr *arr, size_t i, nu_val *val);
nu_val *nu_arr_get_val_c(nu_arr *arr, size_t i);
bool nu_arr_add_val_c(nu_arr *arr, size_t i, nu_val *val);
nu_val *nu_arr_del_val_c(nu_arr *arr, size_t i);

inline static void nu_arr_push_val(nu_arr *arr, nu_val *val)
{
    nu_arr_add_val_c(arr, arrlen(arr->data), val);
}
inline static nu_val *nu_arr_pop_val(nu_arr *arr)
{
    return nu_arr_del_val_c(arr, arrlen(arr->data) - 1);
}
inline static void nu_arr_enq_val(nu_arr *arr, nu_val *val)
{
    nu_arr_add_val_c(arr, 0, val);
}
inline static nu_val *nu_arr_deq_val(nu_arr *arr)
{
    return nu_arr_del_val_c(arr, arrlen(arr->data) - 1);
}

inline static void nu_arr_clear(nu_arr *arr)
{
    while (arrlen(arr->data) > 0)
    {
        nu_arr_pop_val(arr);
    }
}

bool nu_arr_has_val_c(const nu_arr *arr, const nu_val *val);
inline static nu_bool *nu_arr_has_val(const nu_arr *arr, const nu_val *val)
{
    return nu_bool_new(nu_arr_has_val_c(arr, val));
}

// --------------------------------------------------------------------------------------------------------------------------------
// Object Methods
// nuobj.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_obj *nu_obj_new();
void nu_obj_free(nu_obj *obj);

inline static nu_num *nu_obj_len(nu_obj *obj)
{
    return nu_num_new((num_t) hmlen(obj->data));
}
inline static nu_num *nu_obj_cap(nu_obj *arr)
{
    return nu_num_new((num_t) SIZE_MAX);
}

inline static size_t nu_obj_len_c(nu_obj *obj)
{
    return hmlen(obj->data);
}
inline static size_t nu_obj_cap_c(nu_obj *arr)
{
    return SIZE_MAX;
}

bool nu_obj_set_val(nu_obj *obj, nu_val *key, nu_val *val);
nu_val *nu_obj_get_val(nu_obj *obj, const nu_val *key);
inline static bool nu_obj_add_val(nu_obj *obj, nu_val *key, nu_val *val)
{
    return nu_obj_set_val(obj, key, val);
}
nu_val *nu_obj_del_val(nu_obj *obj, nu_val *key);

nu_val **nu_obj_keys_c(nu_obj *obj);
nu_val **nu_obj_vals_c(nu_obj *obj);
nu_arr **nu_obj_pairs_c(nu_obj *obj);

nu_arr *nu_obj_keys(nu_obj *obj);
nu_arr *nu_obj_vals(nu_obj *obj);
nu_arr *nu_obj_pairs(nu_obj *obj);

bool nu_obj_has_key_c(nu_obj *obj, const nu_val *key);
inline static nu_bool *nu_obj_has_key(const nu_obj *obj, const nu_val *key)
{
    return nu_bool_new(nu_obj_has_key_c(obj, key));
}

bool nu_obj_has_val_c(nu_obj *obj, const nu_val *val);
inline static nu_bool *nu_obj_has_val(const nu_obj *obj, const nu_val *val)
{
    return nu_bool_new(nu_obj_has_val_c(obj, val));
}

void nu_obj_clear(nu_obj *obj);

// --------------------------------------------------------------------------------------------------------------------------------
// Thread Methods
// nuthr.c
// --------------------------------------------------------------------------------------------------------------------------------

// TODO: implement

// --------------------------------------------------------------------------------------------------------------------------------
// Misc Methods
// nu.c
// --------------------------------------------------------------------------------------------------------------------------------

inline static nu_str *nu_repr(const nu_val *val)
{
    return nu_str_new(nu_repr_c(val));
}

#define nu_val_new(v)                                                                                                             \
    _Generic((v),                                                                                                                 \
        bool: nu_bool_new((bool) (v)),                                                                                            \
        char: nu_str_new(char[2]{(v), '\0'}),                                                                                     \
        unsigned char: nu_num_new((num_t) (v)),                                                                                   \
        short: nu_num_new((num_t) (v)),                                                                                           \
        unsigned short: nu_num_new((num_t) (v)),                                                                                  \
        int: nu_num_new((num_t) (v)),                                                                                             \
        unsigned int: nu_num_new((num_t) (v)),                                                                                    \
        long: nu_num_new((num_t) (v)),                                                                                            \
        unsigned long: nu_num_new((num_t) (v)),                                                                                   \
        long long: nu_num_new((num_t) (v)),                                                                                       \
        unsigned long long: nu_num_new((num_t) (v)),                                                                              \
        float: nu_num_new((num_t) (v)),                                                                                           \
        double: nu_num_new((num_t) (v)),                                                                                          \
        str_t: nu_str_new(v),                                                                                                     \
        default: NU_NONE)

// --------------------------------------------------------------------------------------------------------------------------------
// Virtual Machine
// nuvm.c
// --------------------------------------------------------------------------------------------------------------------------------

typedef struct nu_vm
{
    nu_val *reg[NU_REGISTERS]; // registers
    nu_val *res;               // result register

    nu_obj *glb; // global variables
    nu_obj *loc; // local variables

    size_t opl;  // operation length
    size_t opc;  // operation capacity
    uint8_t *op; // opcodes

    uint8_t *opp; // current operation pointer
    size_t opr;   // operation length remaining
} nu_vm;

#define NU_VM_POP(VM, TYPE) (TYPE) nu_pop_bytes(&(VM->opp), &(VM->opr), sizeof(TYPE))

nu_vm *nu_vm_new();
void nu_vm_free(nu_vm *vm);

void nu_vm_add_op(nu_vm *vm, const uint8_t *op, const size_t len);
void nu_vm_ins_op(nu_vm *vm, const uint8_t *op, const ptrdiff_t pos, const size_t len);

void nu_vm_run(nu_vm *vm);

// --------------------------------------------------------------------------------------------------------------------------------
// Virtual Machine Operations
// nuop.c
// --------------------------------------------------------------------------------------------------------------------------------

NU_FPTR_NEW(nu_op_fptr, void, nu_vm *);

#define NU_OP_LIST                                                                                                                \
    NU_X(noop)                                                                                                                    \
    NU_X(load)                                                                                                                    \
    NU_X(swap)                                                                                                                    \
    NU_X(cmpv)                                                                                                                    \
    NU_X(cmpr)                                                                                                                    \
    NU_X(jmp)                                                                                                                     \
    NU_X(jne)                                                                                                                     \
    NU_X(njmp)                                                                                                                    \
    NU_X(add)                                                                                                                     \
    NU_X(sub)                                                                                                                     \
    NU_X(mul)                                                                                                                     \
    NU_X(div)                                                                                                                     \
    NU_X(mod)                                                                                                                     \
    NU_X(addv)                                                                                                                    \
    NU_X(setv)                                                                                                                    \
    NU_X(getv)                                                                                                                    \
    NU_X(delv)                                                                                                                    \
    NU_X(setg)                                                                                                                    \
    NU_X(getg)                                                                                                                    \
    NU_X(delg)                                                                                                                    \
    NU_X(setl)                                                                                                                    \
    NU_X(getl)                                                                                                                    \
    NU_X(dell)

typedef enum nu_op
{
#define NU_X(N) nu_##N##_op,
    NU_OP_LIST
#undef NU_X
        NU_OPS_COUNT
} nu_op;

#define NU_X(N) void nu_op_##N(nu_vm *vm);
NU_OP_LIST;
#undef NU_X

#define NU_X(N) #N,
static const str_t nu_op_str[] = {NU_OP_LIST};
#undef NU_X

#define NU_X(N) nu_op_##N,
static const nu_op_fptr nu_op_ptr[] = {NU_OP_LIST};
#undef NU_X

// --------------------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif //__NU_H__
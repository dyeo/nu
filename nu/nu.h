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

#define NU_NONE_T 0
#define NU_BOOL_T 1
#define NU_NUM_T  2
#define NU_STR_T  3
#define NU_FN_T   4
#define NU_ARR_T  5
#define NU_OBJ_T  6
#define NU_THR_T  7


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

NU_DEF_FPTR(nu_oper_t, nu_val *, nu_val *, nu_val *);


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

inline static nu_val *nu_oper_none(nu_val *_0, nu_val *_1) { return (nu_val *)(&nu_none); }

#define NU_NONE (&nu_none)


// --------------------------------------------------------------------------------------------------------------------------------
// Initialization, Finalization, & Interpreter State
// nu.c
// --------------------------------------------------------------------------------------------------------------------------------

bool nu_initialize();
bool nu_finalize();


// --------------------------------------------------------------------------------------------------------------------------------
// Memory Management
// nu.c
// --------------------------------------------------------------------------------------------------------------------------------

inline static void nu_incref(nu_val *val) { NU_ASSERT(val->refs != SIZE_MAX, "val has too many refs"); val->refs++; }
inline static void nu_decref(nu_val *val) { NU_ASSERT(val->refs != 0, "val has no refs"); val->refs--; }
inline static bool nu_opt_incref(nu_val *val) { if(val != NU_NONE) { nu_incref(val); return true; } return false; }
inline static bool nu_opt_decref(nu_val *val) { if(val != NU_NONE) { nu_decref(val); return true; } return false; }

void nu_free(nu_val *val);
inline static bool nu_opt_free(nu_val *val) { if(val->refs > 0) { nu_free(val); return true; } return false; }


// --------------------------------------------------------------------------------------------------------------------------------
// Generic Object Methods
// nu.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_num *nu_len(const nu_val *val);
nu_num *nu_cap(const nu_val *val);

nu_num *nu_hash(const nu_val *val);
const char *nu_repr(const nu_val *val);

bool nu_set_val(nu_val *cnt, nu_val *key, nu_val *val);
nu_val *nu_get_val(nu_val *cnt, nu_val *key);
bool nu_add_val(nu_val *cnt, nu_val *key, nu_val *val);
nu_val *nu_del_val(nu_val *cnt, nu_val *key);

nu_val *nu_lt(nu_val *lhs, nu_val *rhs);
nu_val *nu_le(nu_val *lhs, nu_val *rhs);
nu_val *nu_eq(nu_val *lhs, nu_val *rhs);
nu_val *nu_ne(nu_val *lhs, nu_val *rhs);
nu_val *nu_ge(nu_val *lhs, nu_val *rhs);
nu_val *nu_gt(nu_val *lhs, nu_val *rhs);

nu_val *nu_add(nu_val *lhs, nu_val *rhs);
nu_val *nu_sub(nu_val *lhs, nu_val *rhs);
nu_val *nu_mul(nu_val *lhs, nu_val *rhs);
nu_val *nu_div(nu_val *lhs, nu_val *rhs);
nu_val *nu_mod(nu_val *lhs, nu_val *rhs);

inline static bool nu_is_none(nu_val *val) { return val->type == NU_NONE_T; }
inline static bool nu_is_bool(nu_val *val) { return val->type == NU_BOOL_T; }
inline static bool nu_is_num(nu_val *val) { return val->type == NU_NUM_T; }
inline static bool nu_is_str(nu_val *val) { return val->type == NU_STR_T; }
inline static bool nu_is_fn(nu_val *val) { return val->type == NU_FN_T; }
inline static bool nu_is_arr(nu_val *val) { return val->type == NU_ARR_T; }
inline static bool nu_is_obj(nu_val *val) { return val->type == NU_OBJ_T; }
inline static bool nu_is_thr(nu_val *val) { return val->type == NU_THR_T; }


// --------------------------------------------------------------------------------------------------------------------------------
// Boolean Methods
// nubool.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_bool *nu_bool_new(bool data);
void nu_bool_free(nu_bool *bol);

bool nu_to_bool_c(nu_val *val);
nu_bool *nu_to_bool(nu_val *val);


// --------------------------------------------------------------------------------------------------------------------------------
// Number Methods
// nunum.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_num *nu_num_new(num_t data);
void nu_num_free(nu_num *num);

uint8_t nu_to_uint8(nu_val *val);
int8_t nu_to_int8(nu_val *val);
uint16_t nu_to_uint16(nu_val *val);
int16_t nu_to_int16(nu_val *val);
uint32_t nu_to_uint32(nu_val *val);
int32_t nu_to_int32(nu_val *val);
uint64_t nu_to_uint64(nu_val *val);
int64_t nu_to_int64(nu_val *val);
size_t nu_to_size_t(nu_val *val);
float nu_to_float(nu_val *val);
double nu_to_double(nu_val *val);

nu_num *nu_to_num(nu_val *val);


// --------------------------------------------------------------------------------------------------------------------------------
// String Methods
// nustr.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_str *nu_str_new(const char *data);
void nu_str_free(nu_str *str);

nu_str *nu_str_get_val(nu_str *str, nu_num *key);

// --------------------------------------------------------------------------------------------------------------------------------
// Array Methods
// nuarr.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_arr *nu_arr_new(size_t cap);
void nu_arr_free(nu_arr *arr);

inline static nu_num *nu_arr_len(nu_arr *arr) { return nu_num_new((num_t)arr->len); }
inline static nu_num *nu_arr_cap(nu_arr *arr) { return nu_num_new((num_t)arr->cap); }

bool nu_arr_set_val(nu_arr *arr, nu_num *idx, nu_val *val);
nu_val *nu_arr_get_val(nu_arr *arr, nu_num *idx);
bool nu_arr_add_val(nu_arr *arr, nu_num *idx, nu_val *val);
nu_val *nu_arr_del_val(nu_arr *arr, nu_num *idx);

void nu_arr_push_val(nu_arr *arr, nu_val *val);
nu_val *nu_arr_pop_val(nu_arr *arr);
void nu_arr_enqueue_val(nu_arr *arr, nu_val *val);
nu_val *nu_arr_dequeue_val(nu_arr *arr);

bool nu_arr_set_val_i(nu_arr *arr, size_t i, nu_val *val);
nu_val *nu_arr_get_val_i(nu_arr *arr, size_t i);
bool nu_arr_add_val_i(nu_arr *arr, size_t i, nu_val *val);
nu_val *nu_arr_del_val_i(nu_arr *arr, size_t i);


// --------------------------------------------------------------------------------------------------------------------------------
// Object Methods
// nuobj.c
// --------------------------------------------------------------------------------------------------------------------------------

nu_obj *nu_obj_new();
void nu_obj_free(nu_obj *obj);

inline static nu_num *nu_obj_len(nu_obj *obj) { return nu_num_new((num_t)obj->len); }
inline static nu_num *nu_obj_cap(nu_obj *arr) { return nu_num_new((num_t)SIZE_MAX); }

bool nu_obj_set_val(nu_obj *obj, nu_val *key, nu_val *val);
nu_val *nu_obj_get_val(nu_obj *obj, nu_val *key);
inline static bool nu_obj_add_val(nu_obj *obj, nu_val *key, nu_val *val) { return nu_obj_set_val(obj, key, val); }
nu_val *nu_obj_del_val(nu_obj *obj, nu_val *key);


// --------------------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif//__NU_H__
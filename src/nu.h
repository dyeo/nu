#ifndef __NU_H__
#define __NU_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"
#include "utf8.h"
#include "nurbt.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#if _WIN64 || _LP64 || __LP64__ || __amd64__ || __amd64 || __x86_64__ || __x86_64 || _M_AMD_64 || __aarch64__
#define NU_64_BIT
#define NU_BASE_TYPE_BITS 3
#define NU_BASE_REFS_BITS 61
typedef double num_t;
#else
#define NU_32_BIT
#define NU_BASE_TYPE_BITS 3
#define NU_BASE_REFS_BITS 29
typedef float num_t;
#endif

#define NU_NEW(T) (T*)malloc(sizeof(T))
#define NU_ANEW(T, C) (T*)malloc(sizeof(T)*(C))

#define NU_NONE_T 0
#define NU_BOOL_T 1
#define NU_NUM_T  2
#define NU_STR_T  3
#define NU_FN_T   4
#define NU_ARR_T  5
#define NU_OBJ_T  6
#define NU_THR_T  7

#define NU_BASE_HEADER size_t type : NU_BASE_TYPE_BITS, refs : NU_BASE_REFS_BITS

/**
 * Object Definitions
 */

typedef struct nu_base
{
    NU_BASE_HEADER;
} nu_base;

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
    const char *data;
} nu_str;

typedef struct nu_arr
{
    NU_BASE_HEADER;
    size_t length;
    nu_base *data;
} nu_arr;

typedef struct nu_obj
{
    NU_BASE_HEADER;
    rb_tree *data;
} nu_obj;

/**
 * Function Pointer Definition
 */

#define NU_OP_DEF(name, ret, ...) typedef ret (*name)(__VA_ARGS__)

#define NU_FPTR(ret, ...) ret (*)(__VA_ARGS__)

NU_OP_DEF(nu_oper_t, nu_base *, nu_base *, nu_base *);

/**
 * Constants
 */

const extern nu_base nu_none;
const extern nu_bool nu_true;
const extern nu_bool nu_false;
const extern nu_num nu_zero;
const extern nu_num nu_one;
const extern nu_str nu_empty;

inline static nu_base *nu_oper_none(nu_base *_0, nu_base *_1) { return (nu_base *)(&nu_none); }

#define NU_NONE (&nu_none)
#define NU_NONE_OF(T) (T)(&nu_none)

/**
 * Initialization, Finalization, & Interpreter State
 * nu.c
 */

bool nu_initialize();
bool nu_finalize();


/**
 * Generic Object Methods
 * nubase.c
 */

nu_num *nu_hash(nu_base *o);
const char *nu_repr(nu_base *o);

nu_base *nu_lt(nu_base *l, nu_base *r);
nu_base *nu_le(nu_base *l, nu_base *r);
nu_base *nu_eq(nu_base *l, nu_base *r);
nu_base *nu_ne(nu_base *l, nu_base *r);
nu_base *nu_ge(nu_base *l, nu_base *r);
nu_base *nu_gt(nu_base *l, nu_base *r);

inline static bool nu_is_none(nu_base *o) { return o->type == NU_NONE_T; }
inline static bool nu_is_bool(nu_base *o) { return o->type == NU_BOOL_T; }
inline static bool nu_is_num(nu_base *o) { return o->type == NU_NUM_T; }
inline static bool nu_is_str(nu_base *o) { return o->type == NU_STR_T; }
inline static bool nu_is_fn(nu_base *o) { return o->type == NU_FN_T; }
inline static bool nu_is_arr(nu_base *o) { return o->type == NU_ARR_T; }
inline static bool nu_is_obj(nu_base *o) { return o->type == NU_OBJ_T; }
inline static bool nu_is_thr(nu_base *o) { return o->type == NU_THR_T; }

/**
 * Boolean Methods
 * nubool.c
 */

nu_bool *nu_new_bool(bool v);

bool nu_to_bool(nu_base *o);


/**
 * Number Methods
 * nunum.c
 */

nu_num *nu_new_num(num_t v);

uint8_t nu_to_uint8(nu_base *v);
int8_t nu_to_int8(nu_base *v);
uint16_t nu_to_uint16(nu_base *v);
int16_t nu_to_int16(nu_base *v);
uint32_t nu_to_uint32(nu_base *v);
int32_t nu_to_int32(nu_base *v);
uint64_t nu_to_uint64(nu_base *v);
int64_t nu_to_int64(nu_base *v);
float nu_to_float(nu_base * v);
double nu_to_double(nu_base * v);

nu_base *nu_add(nu_base *l, nu_base *r);
nu_base *nu_sub(nu_base *l, nu_base *r);
nu_base *nu_mul(nu_base *l, nu_base *r);
nu_base *nu_div(nu_base *l, nu_base *r);
nu_base *nu_mod(nu_base *l, nu_base *r);


/**
 * String Methods
 * nustr.c
 */

nu_str *nu_new_str(const char *v);

#ifdef __cplusplus
}
#endif

#endif//__NU_H__
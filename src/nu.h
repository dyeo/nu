#ifndef __NU_H__
#define __NU_H__

#ifdef __cplusplus
extern "C" {
#endif

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

#define NU_NONE_T   0ul
#define NU_BOOL_T   1ul
#define NU_NUM_T    2ul
#define NU_STR_T    3ul
#define NU_FN_T     4ul
#define NU_ARR_T    5ul
#define NU_OBJ_T    6ul
#define NU_THR_T    7ul

#define NU_BASE_HEADER() \
    size_t type : NU_BASE_TYPE_BITS; \
    size_t refs : NU_BASE_REFS_BITS;

/**
 * Object Definitions
 */

typedef struct nu_base {
    NU_BASE_HEADER()
} nu_base;

typedef struct nu_bool {
    NU_BASE_HEADER()
    bool value;
} nu_bool;

typedef struct nu_num {
    NU_BASE_HEADER()
    num_t value;
} nu_num;

typedef struct nu_str {
    NU_BASE_HEADER()
    uint8_t *value;
} nu_str;

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

nu_base *nu_lt(nu_base *l, nu_base *r);
nu_base *nu_le(nu_base *l, nu_base *r);
nu_base *nu_eq(nu_base *l, nu_base *r);
nu_base *nu_ne(nu_base *l, nu_base *r);
nu_base *nu_ge(nu_base *l, nu_base *r);
nu_base *nu_gt(nu_base *l, nu_base *r);

bool nu_is_none(nu_base *o);
bool nu_is_bool(nu_base *o);
bool nu_is_num(nu_base *o);
bool nu_is_str(nu_base *o);
bool nu_is_fn(nu_base *o);
bool nu_is_arr(nu_base *o);
bool nu_is_obj(nu_base *o);
bool nu_is_thr(nu_base *o);

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

double nu_to_double(nu_base *v);
long nu_to_long(nu_base *v);
float nu_to_float(nu_base *v);
int nu_to_int(nu_base *v);

nu_base *nu_add(nu_base *l, nu_base *r);
nu_base *nu_sub(nu_base *l, nu_base *r);
nu_base *nu_mul(nu_base *l, nu_base *r);
nu_base *nu_div(nu_base *l, nu_base *r);
nu_base *nu_mod(nu_base *l, nu_base *r);

#ifdef __cplusplus
}
#endif

#endif//__NU_H__
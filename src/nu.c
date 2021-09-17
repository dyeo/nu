#include "nu.h"

#include <assert.h>
#include <stdio.h>
#include <float.h>

// --------------------------------------------------------------------------------------------------------------------------------

const nu_val nu_none = { NU_NONE_T, 0ul };
const nu_bool nu_true = { NU_BOOL_T, 0ul, true };
const nu_bool nu_false = { NU_BOOL_T, 0ul, false };
const nu_num nu_zero = { NU_NUM_T, 0ul, 0.0 };
const nu_num nu_one = { NU_NUM_T, 0ul, 1.0 };
const nu_str nu_empty = { NU_STR_T, 0ul, "" };

// --------------------------------------------------------------------------------------------------------------------------------

bool nu_initialize()
{}

bool nu_finalize()
{}

// --------------------------------------------------------------------------------------------------------------------------------

NU_DEF_FPTR(nu_free_fptr, void, nu_val*);

void _nu_none_free(nu_val *o) {}

nu_free_fptr _nu_free_ptr[8] = {
	(nu_free_fptr)_nu_none_free,
	(nu_free_fptr)nu_free_bool,
	(nu_free_fptr)nu_free_num,
	(nu_free_fptr)nu_free_str,
	(nu_free_fptr)_nu_none_free,
	(nu_free_fptr)_nu_none_free,
	(nu_free_fptr)nu_free_obj,
	(nu_free_fptr)_nu_none_free
};

void nu_free(nu_val *o)
{
    _nu_free_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_DEF_FPTR(nu_repr_fptr, const char *, const nu_val *);

const char *_nu_none_repr(const nu_val *o)
{
    const char *t = "none";
    size_t n = strlen(t);
    return strcpy(NU_ANEW(const char, n), t);
}
const char *_nu_bool_repr(const nu_bool *o)
{
    const char *t = o->data ? "true" : "false";
    size_t n = strlen(t);
    return strcpy(NU_ANEW(const char, n), t);
}
const char *_nu_num_repr(const nu_num *o)
{
    const int size = 3 + DBL_MANT_DIG - DBL_MIN_EXP;
    const char *buf = NU_ANEW(const char, size);
    snprintf(buf, size, "%f", o->data);
    return buf;
}
const char *_nu_str_repr(const nu_str *o)
{
    size_t n = strlen(o->data);
    return strcpy(NU_ANEW(const char, n), o->data);
}
const char *_nu_fn_repr(const nu_fn *o)
{
    return NULL;
}
const char *_nu_arr_repr(const nu_arr *o)
{
    return NULL;
}
const char *_nu_obj_repr(const nu_obj *o)
{
    return NULL;
}
const char *_nu_thr_repr(const nu_thr *o)
{
    return NULL;
}
nu_repr_fptr _nu_repr_ptr[8] = {
    (nu_repr_fptr)_nu_none_repr,
    (nu_repr_fptr)_nu_bool_repr,
    (nu_repr_fptr)_nu_num_repr,
    (nu_repr_fptr)_nu_str_repr,
    (nu_repr_fptr)_nu_fn_repr,
    (nu_repr_fptr)_nu_arr_repr,
    (nu_repr_fptr)_nu_obj_repr,
    (nu_repr_fptr)_nu_thr_repr
};

const char *nu_repr(const nu_val *o)
{
    return _nu_repr_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_DEF_FPTR(nu_hash_fptr, nu_num *, nu_val *);

nu_num *_nu_none_hash(nu_val *o)
{
    return NU_NONE;
}
nu_num *_nu_bool_hash(nu_bool *o)
{
    return nu_new_num(o->data != 0);
}
nu_num *_nu_num_hash(nu_num *o)
{
    return nu_new_num(o->data);
}
nu_num *_nu_str_hash(nu_str *o)
{
    return nu_new_num((num_t)hashN(o->data));
}
nu_num *_nu_fn_hash(nu_val *o)
{
    return NU_NONE; // hashable???
}
nu_num *_nu_arr_hash(nu_val *o)
{
    return nu_new_num(0); // TODO: hash all array elems
}
nu_num *_nu_obj_hash(nu_val *o)
{
    return nu_new_num(0); // TODO: custom hash function lookup 
}
nu_num *_nu_thr_hash(nu_val *o)
{
    return NU_NONE; // hashable???
}

nu_hash_fptr _nu_hash_ptr[8] = {
    (nu_hash_fptr)_nu_none_hash,
    (nu_hash_fptr)_nu_bool_hash,
    (nu_hash_fptr)_nu_num_hash,
    (nu_hash_fptr)_nu_str_hash,
    (nu_hash_fptr)_nu_fn_hash,
    (nu_hash_fptr)_nu_arr_hash,
    (nu_hash_fptr)_nu_obj_hash,
    (nu_hash_fptr)_nu_thr_hash
};

nu_num *nu_hash(const nu_val *o)
{
    if(o == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_hash_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_lt(nu_bool *l, nu_bool *r)
{
    return NU_NONE;
}
nu_val *_nu_num_lt(nu_num *l, nu_num *r)
{
    return nu_new_bool(l->data < r->data);
}
nu_val *_nu_str_lt(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_fn_lt(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_arr_lt(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_obj_lt(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_thr_lt(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

nu_oper_t _nu_lt_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_bool_lt,
    (nu_oper_t)_nu_num_lt,
    (nu_oper_t)_nu_str_lt,
    (nu_oper_t)_nu_fn_lt,
    (nu_oper_t)_nu_arr_lt,
    (nu_oper_t)_nu_obj_lt,
    (nu_oper_t)_nu_thr_lt
};

nu_val *nu_lt(nu_val *l, nu_val *r)
{
    if(r == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_lt_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_le(nu_bool *l, nu_bool *r)
{
    return NU_NONE;
}
nu_val *_nu_num_le(nu_num *l, nu_num *r)
{
    return nu_new_bool(l->data <= r->data);
}
nu_val *_nu_str_le(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_fn_le(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_arr_le(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_obj_le(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_thr_le(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

nu_oper_t _nu_le_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_bool_le,
    (nu_oper_t)_nu_num_le,
    (nu_oper_t)_nu_str_le,
    (nu_oper_t)_nu_fn_le,
    (nu_oper_t)_nu_arr_le,
    (nu_oper_t)_nu_obj_le,
    (nu_oper_t)_nu_thr_le
};

nu_val *nu_le(nu_val *l, nu_val *r)
{
    if(r == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_le_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_eq(nu_bool *l, nu_bool *r)
{
    return nu_new_bool(l->data == r->data);
}
nu_val *_nu_num_eq(nu_num *l, nu_num *r)
{
    return nu_new_bool(l->data == r->data);
}
nu_val *_nu_str_eq(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_fn_eq(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_arr_eq(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_obj_eq(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_thr_eq(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

nu_oper_t _nu_eq_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_bool_eq,
    (nu_oper_t)_nu_num_eq,
    (nu_oper_t)_nu_str_eq,
    (nu_oper_t)_nu_fn_eq,
    (nu_oper_t)_nu_arr_eq,
    (nu_oper_t)_nu_obj_eq,
    (nu_oper_t)_nu_thr_eq
};

nu_val *nu_eq(nu_val *l, nu_val *r)
{
    if(r == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_eq_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_ne(nu_bool *l, nu_bool *r)
{
    return nu_new_bool(l->data != r->data);
}
nu_val *_nu_num_ne(nu_num *l, nu_num *r)
{
    return nu_new_bool(l->data != r->data);
}
nu_val *_nu_str_ne(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_fn_ne(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_arr_ne(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_obj_ne(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_thr_ne(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

nu_oper_t _nu_ne_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_bool_ne,
    (nu_oper_t)_nu_num_ne,
    (nu_oper_t)_nu_str_ne,
    (nu_oper_t)_nu_fn_ne,
    (nu_oper_t)_nu_arr_ne,
    (nu_oper_t)_nu_obj_ne,
    (nu_oper_t)_nu_thr_ne
};

nu_val *nu_ne(nu_val *l, nu_val *r)
{
    if(r == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_ne_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_ge(nu_bool *l, nu_bool *r)
{
    return NU_NONE;
}
nu_val *_nu_num_ge(nu_num *l, nu_num *r)
{
    return nu_new_bool(l->data >= r->data);
}
nu_val *_nu_str_ge(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_fn_ge(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_arr_ge(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_obj_ge(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_thr_ge(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

nu_oper_t _nu_ge_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_bool_ge,
    (nu_oper_t)_nu_num_ge,
    (nu_oper_t)_nu_str_ge,
    (nu_oper_t)_nu_fn_ge,
    (nu_oper_t)_nu_arr_ge,
    (nu_oper_t)_nu_obj_ge,
    (nu_oper_t)_nu_thr_ge
};

nu_val *nu_ge(nu_val *l, nu_val *r)
{
    if(r == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_ge_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_gt(nu_bool *l, nu_bool *r)
{
    return NU_NONE;
}
nu_val *_nu_num_gt(nu_num *l, nu_num *r)
{
    return nu_new_bool(l->data > r->data);
}
nu_val *_nu_str_gt(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_fn_gt(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_arr_gt(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_obj_gt(nu_val *l, nu_val *r)
{
    return NU_NONE;
}
nu_val *_nu_thr_gt(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

nu_oper_t _nu_gt_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_bool_gt,
    (nu_oper_t)_nu_num_gt,
    (nu_oper_t)_nu_str_gt,
    (nu_oper_t)_nu_fn_gt,
    (nu_oper_t)_nu_arr_gt,
    (nu_oper_t)_nu_obj_gt,
    (nu_oper_t)_nu_thr_gt
};

nu_val *nu_gt(nu_val *l, nu_val *r)
{
    if(r == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_gt_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

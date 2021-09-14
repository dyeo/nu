#include "nu.h"

// --------------------------------------------------------------------------------------------------------------------------------

NU_OP_DEF(nu_hash_fptr, nu_num *, nu_base *);

nu_num *_nu_bool_hash(nu_bool *o)
{
    return ((nu_base *)o) == &nu_none ? &nu_none : nu_new_num(o->data);
}
nu_num *_nu_num_hash(nu_num *o)
{
    return ((nu_base *)o) == &nu_none ? &nu_none : nu_new_num(o->data);
}
nu_num *_nu_str_hash(nu_str *o)
{
    return ((nu_base *)o) == &nu_none ? &nu_none : nu_new_num((num_t)utfhash(o->data));
}
nu_num *_nu_fn_hash(nu_base *o)
{
    return ((nu_base *)o) == &nu_none ? &nu_none : &nu_none;
}
nu_num *_nu_arr_hash(nu_base *o)
{
    return ((nu_base *)o) == &nu_none ? &nu_none : &nu_none;
}
nu_num *_nu_obj_hash(nu_base *o)
{
    return ((nu_base *)o) == &nu_none ? &nu_none : &nu_none;
}
nu_num *_nu_thr_hash(nu_base *o)
{
    return ((nu_base *)o) == &nu_none ? &nu_none : &nu_none;
}

nu_hash_fptr _nu_hash_ptr[8] = {
    (nu_hash_fptr)nu_oper_none,
    (nu_hash_fptr)_nu_bool_hash,
    (nu_hash_fptr)_nu_num_hash,
    (nu_hash_fptr)_nu_str_hash,
    (nu_hash_fptr)_nu_fn_hash,
    (nu_hash_fptr)_nu_arr_hash,
    (nu_hash_fptr)_nu_obj_hash,
    (nu_hash_fptr)_nu_thr_hash
};

nu_num *nu_hash(nu_base *o)
{
    return _nu_hash_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_base *_nu_bool_lt(nu_bool *l, nu_bool *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_num_lt(nu_num *l, nu_num *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->data < r->data);
}
nu_base *_nu_str_lt(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_fn_lt(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_arr_lt(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_obj_lt(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_thr_lt(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
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

nu_base *nu_lt(nu_base *l, nu_base *r)
{
    return _nu_lt_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_base *_nu_bool_le(nu_bool *l, nu_bool *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_num_le(nu_num *l, nu_num *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->data <= r->data);
}
nu_base *_nu_str_le(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_fn_le(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_arr_le(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_obj_le(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_thr_le(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
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

nu_base *nu_le(nu_base *l, nu_base *r)
{
    return _nu_le_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_base *_nu_bool_eq(nu_bool *l, nu_bool *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->data == r->data);
}
nu_base *_nu_num_eq(nu_num *l, nu_num *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->data == r->data);
}
nu_base *_nu_str_eq(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_fn_eq(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_arr_eq(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_obj_eq(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_thr_eq(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
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

nu_base *nu_eq(nu_base *l, nu_base *r)
{
    return _nu_eq_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_base *_nu_bool_ne(nu_bool *l, nu_bool *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->data != r->data);
}
nu_base *_nu_num_ne(nu_num *l, nu_num *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->data != r->data);
}
nu_base *_nu_str_ne(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_fn_ne(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_arr_ne(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_obj_ne(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_thr_ne(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
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

nu_base *nu_ne(nu_base *l, nu_base *r)
{
    return _nu_ne_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_base *_nu_bool_ge(nu_bool *l, nu_bool *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_num_ge(nu_num *l, nu_num *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->data >= r->data);
}
nu_base *_nu_str_ge(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_fn_ge(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_arr_ge(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_obj_ge(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_thr_ge(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
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

nu_base *nu_ge(nu_base *l, nu_base *r)
{
    return _nu_ge_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_base *_nu_bool_gt(nu_bool *l, nu_bool *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_num_gt(nu_num *l, nu_num *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->data > r->data);
}
nu_base *_nu_str_gt(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_fn_gt(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_arr_gt(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_obj_gt(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_thr_gt(nu_base *l, nu_base *r)
{
    return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
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

nu_base *nu_gt(nu_base *l, nu_base *r)
{
    return _nu_gt_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

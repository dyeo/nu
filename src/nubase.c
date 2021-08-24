#include "nu.h"

nu_base *_nu_none_lt(nu_base *l, nu_base *r) { return &nu_none; }
nu_base *_nu_bool_lt(nu_bool *l, nu_bool *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_num_lt(nu_num *l, nu_num *r) { return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->value < r->value); }
nu_base *_nu_str_lt(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_fn_lt(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_arr_lt(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_obj_lt(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_thr_lt(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }

nu_op_t _nu_lt_ptr[8] = {
    (nu_op_t)_nu_none_lt,
    (nu_op_t)_nu_bool_lt,
    (nu_op_t)_nu_num_lt,
    (nu_op_t)_nu_str_lt,
    (nu_op_t)_nu_fn_lt,
    (nu_op_t)_nu_arr_lt,
    (nu_op_t)_nu_obj_lt,
    (nu_op_t)_nu_thr_lt};

nu_base *nu_lt(nu_base *l, nu_base *r) { return _nu_lt_ptr[l->type](l, r); }

nu_base *_nu_none_le(nu_base *l, nu_base *r) { return &nu_none; }
nu_base *_nu_bool_le(nu_bool *l, nu_bool *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_num_le(nu_num *l, nu_num *r) { return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->value <= r->value); }
nu_base *_nu_str_le(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_fn_le(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_arr_le(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_obj_le(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_thr_le(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }

nu_op_t _nu_le_ptr[8] = {
    (nu_op_t)_nu_none_le,
    (nu_op_t)_nu_bool_le,
    (nu_op_t)_nu_num_le,
    (nu_op_t)_nu_str_le,
    (nu_op_t)_nu_fn_le,
    (nu_op_t)_nu_arr_le,
    (nu_op_t)_nu_obj_le,
    (nu_op_t)_nu_thr_le};

nu_base *nu_le(nu_base *l, nu_base *r) { return _nu_le_ptr[l->type](l, r); }

nu_base *_nu_none_eq(nu_base *l, nu_base *r) { return &nu_none; }
nu_base *_nu_bool_eq(nu_bool *l, nu_bool *r) { return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->value == r->value); }
nu_base *_nu_num_eq(nu_num *l, nu_num *r) { return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->value == r->value); }
nu_base *_nu_str_eq(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_fn_eq(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_arr_eq(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_obj_eq(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_thr_eq(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }

nu_op_t _nu_eq_ptr[8] = {
    (nu_op_t)_nu_none_eq,
    (nu_op_t)_nu_bool_eq,
    (nu_op_t)_nu_num_eq,
    (nu_op_t)_nu_str_eq,
    (nu_op_t)_nu_fn_eq,
    (nu_op_t)_nu_arr_eq,
    (nu_op_t)_nu_obj_eq,
    (nu_op_t)_nu_thr_eq};

nu_base *nu_eq(nu_base *l, nu_base *r) { return _nu_eq_ptr[l->type](l, r); }

nu_base *_nu_none_ne(nu_base *l, nu_base *r) { return &nu_none; }
nu_base *_nu_bool_ne(nu_bool *l, nu_bool *r) { return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->value != r->value); }
nu_base *_nu_num_ne(nu_num *l, nu_num *r) { return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->value != r->value); }
nu_base *_nu_str_ne(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_fn_ne(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_arr_ne(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_obj_ne(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_thr_ne(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }

nu_op_t _nu_ne_ptr[8] = {
    (nu_op_t)_nu_none_ne,
    (nu_op_t)_nu_bool_ne,
    (nu_op_t)_nu_num_ne,
    (nu_op_t)_nu_str_ne,
    (nu_op_t)_nu_fn_ne,
    (nu_op_t)_nu_arr_ne,
    (nu_op_t)_nu_obj_ne,
    (nu_op_t)_nu_thr_ne};

nu_base *nu_ne(nu_base *l, nu_base *r) { return _nu_ne_ptr[l->type](l, r); }

nu_base *_nu_none_ge(nu_base *l, nu_base *r) { return &nu_none; }
nu_base *_nu_bool_ge(nu_bool *l, nu_bool *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_num_ge(nu_num *l, nu_num *r) { return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->value >= r->value); }
nu_base *_nu_str_ge(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_fn_ge(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_arr_ge(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_obj_ge(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_thr_ge(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }

nu_op_t _nu_ge_ptr[8] = {
    (nu_op_t)_nu_none_ge,
    (nu_op_t)_nu_bool_ge,
    (nu_op_t)_nu_num_ge,
    (nu_op_t)_nu_str_ge,
    (nu_op_t)_nu_fn_ge,
    (nu_op_t)_nu_arr_ge,
    (nu_op_t)_nu_obj_ge,
    (nu_op_t)_nu_thr_ge};

nu_base *nu_ge(nu_base *l, nu_base *r) { return _nu_ge_ptr[l->type](l, r); }

nu_base *_nu_none_gt(nu_base *l, nu_base *r) { return &nu_none; }
nu_base *_nu_bool_gt(nu_bool *l, nu_bool *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_num_gt(nu_num *l, nu_num *r) { return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->value > r->value); }
nu_base *_nu_str_gt(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_fn_gt(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_arr_gt(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_obj_gt(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }
nu_base *_nu_thr_gt(nu_base *l, nu_base *r) { return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none; }

nu_op_t _nu_gt_ptr[8] = {
    (nu_op_t)_nu_none_gt,
    (nu_op_t)_nu_bool_gt,
    (nu_op_t)_nu_num_gt,
    (nu_op_t)_nu_str_gt,
    (nu_op_t)_nu_fn_gt,
    (nu_op_t)_nu_arr_gt,
    (nu_op_t)_nu_obj_gt,
    (nu_op_t)_nu_thr_gt};

nu_base *nu_gt(nu_base *l, nu_base *r) { return _nu_gt_ptr[l->type](l, r); }
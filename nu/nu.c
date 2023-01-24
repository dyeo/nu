#include "nu.h"

#include <assert.h>
#include <stdio.h>
#include <float.h>

#include "nurbt.h"

// --------------------------------------------------------------------------------------------------------------------------------

const nu_val nu_none =   { NU_NONE_T, 0ul };
const nu_bool nu_true =  { NU_BOOL_T, 0ul, true };
const nu_bool nu_false = { NU_BOOL_T, 0ul, false };
const nu_num nu_zero =   { NU_NUM_T, 0ul, 0.0 };
const nu_num nu_one =    { NU_NUM_T, 0ul, 1.0 };
const nu_str nu_empty =  { NU_STR_T, 0ul, 0, 1, "" };

/// @brief Const array to convert a boolean value efficiently to one of the true or false constants.
const nu_bool *nu_bool_literal[2] = { NU_FALSE, NU_TRUE };

// --------------------------------------------------------------------------------------------------------------------------------

bool nu_initialize()
{
}

bool nu_finalize()
{
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_DEF_FPTR(nu_free_fptr, void, nu_val *);

void _nu_none_free(nu_val *o) {}

nu_free_fptr _nu_free_ptr[8] = {
    (nu_free_fptr)_nu_none_free,
    (nu_free_fptr)nu_bool_free,
    (nu_free_fptr)nu_num_free,
    (nu_free_fptr)nu_str_free,
    (nu_free_fptr)_nu_none_free,
    (nu_free_fptr)nu_arr_free,
    (nu_free_fptr)nu_obj_free,
    (nu_free_fptr)_nu_none_free};
void nu_free(nu_val *o)
{
    _nu_free_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_DEF_FPTR(nu_len_fptr, nu_num *, const nu_val *);

nu_num *_nu_none_len(nu_val *o)
{
    return (nu_num *)NU_NONE;
}

nu_len_fptr _nu_len_ptr[8] = {
    (nu_len_fptr)_nu_none_len,
    (nu_len_fptr)_nu_none_len,
    (nu_len_fptr)_nu_none_len,
    (nu_len_fptr)_nu_none_len,
    (nu_len_fptr)_nu_none_len,
    (nu_len_fptr)nu_arr_len,
    (nu_len_fptr)nu_obj_len,
    (nu_len_fptr)_nu_none_len};
nu_num *nu_len(const nu_val *o)
{
    _nu_len_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_DEF_FPTR(nu_cap_fptr, nu_num *, const nu_val *);

nu_num *_nu_none_cap(nu_val *o)
{
    return (nu_num *)NU_NONE;
}

nu_cap_fptr _nu_cap_ptr[8] = {
    (nu_cap_fptr)_nu_none_cap,
    (nu_cap_fptr)_nu_none_cap,
    (nu_cap_fptr)_nu_none_cap,
    (nu_cap_fptr)_nu_none_cap,
    (nu_cap_fptr)_nu_none_cap,
    (nu_cap_fptr)nu_arr_cap,
    (nu_cap_fptr)nu_obj_cap,
    (nu_cap_fptr)_nu_none_cap};
nu_num *nu_cap(const nu_val *o)
{
    _nu_cap_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_DEF_FPTR(nu_hash_fptr, nu_num *, nu_val *);

nu_num *_nu_none_hash(nu_val *o)
{
    return NU_NONE;
}
nu_num *_nu_bool_hash(nu_bool *o)
{
    return nu_num_new(o->data != 0);
}
nu_num *_nu_num_hash(nu_num *o)
{
    return nu_num_new(o->data);
}
nu_num *_nu_str_hash(nu_str *o)
{
    return nu_num_new((num_t)hashN(o->data, o->cap));
}
nu_num *_nu_fn_hash(nu_fn *o)
{
    return NU_NONE; // hashable???
}
nu_num *_nu_arr_hash(nu_arr *o)
{
    size_t h = _FNV_OFFSET_N;
    size_t i;
    for (i = 0; i < o->len; ++i)
    {
        h = (h ^ nu_to_size_t(nu_hash(o->data[i]))) * _FNV_PRIME_N;
    }
    return nu_num_new((double)h); // TODO: hash all array elems
}
nu_num *_nu_obj_hash(nu_obj *o)
{
    // nu_val *hfn = nu_obj_get_val(o, nu_str_new("$hash"));
    // if(hfn != NU_NONE)
    //{
    //     return (nu_num *)nu_fn_call(hfn, o);
    // }
    return NU_NONE;
}
nu_num *_nu_thr_hash(nu_thr *o)
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
    (nu_hash_fptr)_nu_thr_hash};
nu_num *nu_hash(const nu_val *o)
{
    return _nu_hash_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_DEF_FPTR(nu_repr_fptr, const char *, const nu_val *);

const char *_nu_none_repr(const nu_val *o)
{
    return utfdup("none");
}
const char *_nu_bool_repr(const nu_bool *o)
{
    return utfdup(o->data ? "true" : "false");
}
const char *_nu_num_repr(const nu_num *o)
{
    size_t size;
    char *buf;
    if (is_int(o->data))
    {
        size = snprintf(NULL, 0, NU_INT_FMT, (i_num_t)o->data);
        buf = nu_calloc(char, size + 1);
        snprintf(buf, size + 1, NU_INT_FMT, (i_num_t)o->data);
    }
    else
    {
        size = snprintf(NULL, 0, NU_FLP_FMT, o->data);
        buf = nu_calloc(char, size + 1);
        snprintf(buf, size + 1, NU_FLP_FMT, o->data);
    }
    buf[size] = NULL;
    return buf;
}
const char *_nu_str_repr(const nu_str *o)
{
    char *buf = nu_calloc(char, o->cap + 3);
    snprintf(buf, o->cap + 3, "\"%s\"", o->data);
    buf[o->cap + 2] = NULL;
    return buf;
}
const char *_nu_fn_repr(const nu_fn *o)
{
    return NULL;
}
const char *_nu_arr_repr(const nu_arr *o)
{
    char **elems = nu_calloc(char *, o->len);
    size_t *sizes = nu_calloc(size_t, o->len);
    size_t i = 0, total_size = 0;
    total_size += 4; // account for "[ " and " ]"
    for (i; i < o->len; ++i)
    {
        char *elem = elems[i] = nu_repr(o->data[i]);
        size_t size = sizes[i] = strlen(elems[i]);
        total_size += size;
        if (i < o->len - 1)
        {
            total_size += 2; // account for ", "
        }
    }
    total_size++;

    char *result = nu_calloc(char, total_size);
    i = 0;
    size_t j = 0;
    strncpy(result + j, "[ ", 2);
    j += 2;
    for (i; i < o->len; ++i)
    {
        char *elem = elems[i];
        size_t size = sizes[i];
        strncpy(result + j, elem, size);
        j += size;
        if (i < o->len - 1)
        {
            strncpy(result + j, ", ", 2);
            j += 2;
        }
    }
    strncpy(result + j, " ]", 2);
    j += 2;
    result[j] = 0;
    return result;
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
    (nu_repr_fptr)_nu_thr_repr};
const char *nu_repr(const nu_val *o)
{
    return _nu_repr_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_DEF_FPTR(nu_set_val_fptr, bool, nu_val *, nu_val *, nu_val *);

bool _nu_none_set_val(nu_val *_0, nu_val *_1, nu_val *_2)
{
    return false;
}

nu_set_val_fptr _nu_set_val_ptr[8] = {
    (nu_set_val_fptr)_nu_none_set_val,
    (nu_set_val_fptr)_nu_none_set_val,
    (nu_set_val_fptr)_nu_none_set_val,
    (nu_set_val_fptr)_nu_none_set_val,
    (nu_set_val_fptr)_nu_none_set_val,
    (nu_set_val_fptr)nu_arr_set_val,
    (nu_set_val_fptr)nu_obj_set_val,
    (nu_set_val_fptr)_nu_none_set_val};
bool nu_set_val(nu_val *cnt, nu_val *key, nu_val *val)
{
    return _nu_set_val_ptr[cnt->type](cnt, key, val);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_DEF_FPTR(nu_get_val_fptr, nu_val *, nu_val *, nu_val *);

nu_val *_nu_none_get_val(nu_val *_0, nu_val *_1)
{
    return NU_NONE;
}

nu_get_val_fptr _nu_get_val_ptr[8] = {
    (nu_get_val_fptr)_nu_none_get_val,
    (nu_get_val_fptr)_nu_none_get_val,
    (nu_get_val_fptr)_nu_none_get_val,
    (nu_get_val_fptr)_nu_none_get_val,
    (nu_get_val_fptr)_nu_none_get_val,
    (nu_get_val_fptr)nu_arr_get_val,
    (nu_get_val_fptr)nu_obj_get_val,
    (nu_get_val_fptr)_nu_none_get_val};
nu_val *nu_get_val(nu_val *cnt, nu_val *key)
{
    return _nu_get_val_ptr[cnt->type](cnt, key);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_DEF_FPTR(nu_add_val_fptr, bool, nu_val *, nu_val *, nu_val *);

bool _nu_none_add_val(nu_val *_0, nu_val *_1, nu_val *_2)
{
    return false;
}

nu_add_val_fptr _nu_add_val_ptr[8] = {
    (nu_add_val_fptr)_nu_none_add_val,
    (nu_add_val_fptr)_nu_none_add_val,
    (nu_add_val_fptr)_nu_none_add_val,
    (nu_add_val_fptr)_nu_none_add_val,
    (nu_add_val_fptr)_nu_none_add_val,
    (nu_add_val_fptr)nu_arr_add_val,
    (nu_add_val_fptr)nu_obj_add_val,
    (nu_add_val_fptr)_nu_none_add_val};
bool nu_add_val(nu_val *cnt, nu_val *key, nu_val *val)
{
    return _nu_add_val_ptr[cnt->type](cnt, key, val);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_DEF_FPTR(nu_del_val_fptr, nu_val *, nu_val *, nu_val *);

nu_val *_nu_none_del_val(nu_val *_0, nu_val *_1)
{
    return NU_NONE;
}

nu_del_val_fptr _nu_del_val_ptr[8] = {
    (nu_del_val_fptr)_nu_none_del_val,
    (nu_del_val_fptr)_nu_none_del_val,
    (nu_del_val_fptr)_nu_none_del_val,
    (nu_del_val_fptr)_nu_none_del_val,
    (nu_del_val_fptr)_nu_none_del_val,
    (nu_del_val_fptr)nu_arr_del_val,
    (nu_del_val_fptr)nu_obj_del_val,
    (nu_del_val_fptr)_nu_none_del_val};
nu_val *nu_del_val(nu_val *cnt, nu_val *key)
{
    return _nu_del_val_ptr[cnt->type](cnt, key);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_lt(nu_bool *l, nu_bool *r)
{
    return NU_NONE;
}
nu_val *_nu_num_lt(nu_num *l, nu_num *r)
{
    return nu_bool_literal[l->data < r->data];
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
    (nu_oper_t)_nu_thr_lt};
nu_val *nu_lt(nu_val *l, nu_val *r)
{
    if (r == NU_NONE)
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
    return nu_bool_literal[l->data <= r->data];
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
    (nu_oper_t)_nu_thr_le};

nu_val *nu_le(nu_val *l, nu_val *r)
{
    if (r == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_le_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_eq(nu_bool *l, nu_bool *r)
{
    return nu_bool_literal[l->data == r->data];
}
nu_val *_nu_num_eq(nu_num *l, nu_num *r)
{
    return nu_bool_literal[l->data == r->data];
}
nu_val *_nu_str_eq(nu_str *l, nu_str *r)
{
    if (l->len != r->len)
        return nu_bool_literal[false];
    size_t i = 0;
    for (i; i < l->len; ++i)
    {
    }
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
    (nu_oper_t)_nu_thr_eq};

nu_val *nu_eq(nu_val *l, nu_val *r)
{
    if (r == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_eq_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_ne(nu_bool *l, nu_bool *r)
{
    return nu_bool_literal[l->data != r->data];
}
nu_val *_nu_num_ne(nu_num *l, nu_num *r)
{
    return nu_bool_literal[l->data != r->data];
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
    (nu_oper_t)_nu_thr_ne};

nu_val *nu_ne(nu_val *l, nu_val *r)
{
    if (r == NU_NONE)
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
    return nu_bool_literal[l->data >= r->data];
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
    (nu_oper_t)_nu_thr_ge};

nu_val *nu_ge(nu_val *l, nu_val *r)
{
    if (r == NU_NONE)
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
    return nu_bool_literal[l->data > r->data];
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
    (nu_oper_t)_nu_thr_gt};

nu_val *nu_gt(nu_val *l, nu_val *r)
{
    if (r == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_gt_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_add(nu_bool *l, nu_bool *r)
{
    return nu_bool_literal[(l->data + r->data) != 0];
}

nu_val *_nu_num_add(nu_num *l, nu_num *r)
{
    return nu_num_new(l->data + r->data);
}

nu_val *_nu_str_add(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

nu_val *_nu_arr_add(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

const nu_oper_t _nu_add_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_bool_add,
    (nu_oper_t)_nu_num_add,
    (nu_oper_t)_nu_str_add,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_arr_add,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none};

nu_val *nu_add(nu_val *l, nu_val *r)
{
    if (r == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_add_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_sub(nu_bool *l, nu_bool *r)
{
    return nu_bool_literal[l->data - r->data];
}

nu_val *_nu_num_sub(nu_num *l, nu_num *r)
{
    return nu_num_new(l->data - r->data);
}

nu_val *_nu_str_sub(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

const nu_oper_t _nu_sub_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_bool_sub,
    (nu_oper_t)_nu_num_sub,
    (nu_oper_t)_nu_str_sub,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none};

nu_val *nu_sub(nu_val *l, nu_val *r)
{
    if (r == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_sub_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_mul(nu_bool *l, nu_bool *r)
{
    return nu_bool_literal[l->data * r->data];
}

nu_val *_nu_num_mul(nu_num *l, nu_num *r)
{
    return nu_num_new(l->data * r->data);
}

nu_val *_nu_str_mul(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

const nu_oper_t _nu_mul_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_bool_mul,
    (nu_oper_t)_nu_num_mul,
    (nu_oper_t)_nu_str_mul,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none};

nu_val *nu_mul(nu_val *l, nu_val *r)
{
    if (r == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_mul_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_none_div(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

nu_val *_nu_bool_div(nu_bool *l, nu_bool *r)
{
    return nu_bool_literal[l->data / r->data];
}

nu_val *_nu_num_div(nu_num *l, nu_num *r)
{
    return nu_num_new(l->data / r->data);
}

const nu_oper_t _nu_div_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_bool_div,
    (nu_oper_t)_nu_num_div,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none};

nu_val *nu_div(nu_val *l, nu_val *r)
{
    if (r == NU_NONE)
    {
        return NU_NONE;
    }
    return _nu_div_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_mod(nu_bool *l, nu_bool *r)
{
    return nu_bool_literal[l->data % r->data];
}

nu_val *_nu_num_mod(nu_num *l, nu_num *r)
{
    return nu_num_new(fmod(l->data, r->data));
}

nu_oper_t _nu_mod_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_bool_mod,
    (nu_oper_t)_nu_num_mod,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none};

nu_val *nu_mod(nu_val *l, nu_val *r)
{
    return _nu_mod_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

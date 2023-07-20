#include "nu.h"

#include <assert.h>
#include <stdio.h>
#include <float.h>

#include "nurbt.h"

// --------------------------------------------------------------------------------------------------------------------------------

bool nu_initialized = false;

bool nu_initialize()
{
    nu_initialized = true;
    return true;
}

bool nu_finalize()
{
    nu_initialized = false;
    return false;
}

bool nu_is_initialized()
{
    return nu_initialized;
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_FPTR_NEW(nu_free_fptr, void, nu_val *);

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
    if (nu_is_literal(o))
    {
        return;
    }
    _nu_free_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_FPTR_NEW(nu_len_fptr, nu_num *, const nu_val *);

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

NU_FPTR_NEW(nu_cap_fptr, nu_num *, const nu_val *);

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

NU_FPTR_NEW(nu_hash_fptr, nu_num *, nu_val *);

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
        h = (h ^ nu_size_t_get_c(nu_hash(o->data[i]))) * _FNV_PRIME_N;
    }
    return nu_num_new((double)h); // TODO: hash all array elems
}
nu_num *_nu_obj_hash(nu_obj *o)
{
    // nu_val *hfn = nu_get_val_obj(o, nu_new_str("$hash"));
    // if(hfn != NU_NONE)
    //{
    //     return (nu_num *)nu_call_fn(hfn, o);
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

NU_FPTR_NEW(nu_repr_fptr, str_t , const nu_val *);

str_t _nu_none_repr(const nu_val *o)
{
    return utfdup("none");
}
str_t _nu_bool_repr(const nu_bool *o)
{
    return utfdup(o->data ? "true" : "false");
}
str_t _nu_num_repr(const nu_num *o)
{
    size_t size;
    char *buf;
    if (is_int(o->data))
    {
        size = snprintf(NULL, 0, NU_INT_FMT, (i_num_t)o->data);
        buf = nu_calloc(size + 1, char);
        snprintf(buf, size + 1, NU_INT_FMT, (i_num_t)o->data);
    }
    else
    {
        size = snprintf(NULL, 0, NU_FLP_FMT, o->data);
        buf = nu_calloc(size + 1, char);
        snprintf(buf, size + 1, NU_FLP_FMT, o->data);
    }
    buf[size] = NULL;
    return buf;
}
str_t _nu_str_repr(const nu_str *o)
{
    char *buf = nu_calloc(o->cap + 3, char);
    snprintf(buf, o->cap + 3, "\"%s\"", o->data);
    buf[o->cap + 2] = NULL;
    return buf;
}
str_t _nu_fn_repr(const nu_fn *o)
{
    return NULL;
}
str_t _nu_arr_repr(const nu_arr *o)
{
    char **elems = nu_calloc(o->len, char *);
    size_t *sizes = nu_calloc(o->len, size_t);
    size_t i = 0, total_size = 0;
    total_size += 4; // account for "[ " and " ]"
    for (i; i < o->len; ++i)
    {
        char *elem = elems[i] = nu_repr_c(o->data[i]);
        size_t size = sizes[i] = strlen(elems[i]);
        total_size += size;
        if (i < o->len - 1)
        {
            total_size += 2; // account for ", "
        }
    }
    total_size++;

    char *result = nu_calloc(total_size, char);
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
str_t _nu_obj_repr(const nu_obj *o)
{
    return NULL;
}
str_t _nu_thr_repr(const nu_thr *o)
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
str_t nu_repr_c(const nu_val *o)
{
    return _nu_repr_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_FPTR_NEW(nu_set_val_fptr, bool, nu_val *, nu_val *, nu_val *);

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

NU_FPTR_NEW(nu_get_val_fptr, nu_val *, nu_val *, nu_val *);

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

NU_FPTR_NEW(nu_add_val_fptr, bool, nu_val *, nu_val *, nu_val *);

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

NU_FPTR_NEW(nu_del_val_fptr, nu_val *, nu_val *, nu_val *);

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

const nu_bool *_nu_bool_lt(const nu_bool *l, const nu_bool *r)
{
    return nu_literal_bool[l->data < r->data];
}
const nu_bool *_nu_num_lt(const nu_num *l, const nu_num *r)
{
    return nu_literal_bool[l->data < r->data];
}
const nu_bool *_nu_str_lt(const nu_str *l, const nu_str *r)
{
    return NU_FALSE; // TODO: implement this
}
const nu_bool *_nu_fn_lt(const nu_fn *l, const nu_fn *r)
{
    return NU_FALSE;
}
const nu_bool *_nu_arr_lt(const nu_arr *l, const nu_arr *r)
{
    return nu_literal_bool[l->len < r->len];
}
const nu_bool *_nu_obj_lt(const nu_obj *l, const nu_obj *r)
{
    return nu_literal_bool[l->len < r->len];
}
const nu_bool *_nu_thr_lt(const nu_val *l, const nu_val *r)
{
    return NU_FALSE;
}

nu_cmpr_t _nu_lt_ptr[8] = {
    (nu_cmpr_t)nu_oper_none,
    (nu_cmpr_t)_nu_bool_lt,
    (nu_cmpr_t)_nu_num_lt,
    (nu_cmpr_t)_nu_str_lt,
    (nu_cmpr_t)_nu_fn_lt,
    (nu_cmpr_t)_nu_arr_lt,
    (nu_cmpr_t)_nu_obj_lt,
    (nu_cmpr_t)_nu_thr_lt};

const nu_bool *nu_lt(const nu_val *l, const nu_val *r)
{
    return _nu_lt_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

const nu_bool *_nu_bool_le(const nu_bool *l, const nu_bool *r)
{
    return nu_literal_bool[l->data < r->data];
}
const nu_bool *_nu_num_le(const nu_num *l, const nu_num *r)
{
    return nu_literal_bool[l->data <= r->data];
}
const nu_bool *_nu_str_le(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_fn_le(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_arr_le(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_obj_le(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_thr_le(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}

nu_cmpr_t _nu_le_ptr[8] = {
    (nu_cmpr_t)nu_oper_none,
    (nu_cmpr_t)_nu_bool_le,
    (nu_cmpr_t)_nu_num_le,
    (nu_cmpr_t)_nu_str_le,
    (nu_cmpr_t)_nu_fn_le,
    (nu_cmpr_t)_nu_arr_le,
    (nu_cmpr_t)_nu_obj_le,
    (nu_cmpr_t)_nu_thr_le};

const nu_bool *nu_le(const nu_val *l, const nu_val *r)
{
    return _nu_le_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

const nu_bool *_nu_bool_eq(const nu_bool *l, const nu_bool *r)
{
    return nu_literal_bool[l->data == r->data];
}
const nu_bool *_nu_num_eq(const nu_num *l, const nu_num *r)
{
    return nu_literal_bool[l->data == r->data];
}
const nu_bool *_nu_str_eq(const nu_str *l, const nu_str *r)
{
    if (l->type != r->type || l->len != r->len)
    {
        return NU_FALSE;
    }
    size_t i = 0;
    for (i; i < l->len; ++i)
    {
        if (l->data[i] != r->data[i])
        {
            return NU_FALSE;
        }
    }
    return NU_TRUE;
}
const nu_bool *_nu_fn_eq(const nu_fn *l, const nu_fn *r)
{
    return nu_literal_bool[l == r];
}
const nu_bool *_nu_arr_eq(const nu_arr *l, const nu_arr *r)
{
    if (l->type != r->type || l->len != r->len)
    {
        return NU_FALSE;
    }
    size_t i = 0;
    for (i; i < l->len; ++i)
    {
        if (!nu_eq(l->data[i], r->data[i]))
        {
            return NU_FALSE;
        }
    }
    return NU_TRUE;
}
const nu_bool *_nu_obj_eq(const nu_obj *l, const nu_obj *r)
{
    if (l->type != r->type || l->len != r->len)
    {
        return NU_FALSE;
    }
    size_t i = 0;
    for (i; i < l->len; ++i)
    {
        // TODO: implement
    }
    return NU_TRUE;
}
const nu_bool *_nu_thr_eq(const nu_thr *l, const nu_thr *r)
{
    return nu_literal_bool[l == r];
}

nu_cmpr_t _nu_eq_ptr[8] = {
    (nu_cmpr_t)nu_oper_none,
    (nu_cmpr_t)_nu_bool_eq,
    (nu_cmpr_t)_nu_num_eq,
    (nu_cmpr_t)_nu_str_eq,
    (nu_cmpr_t)_nu_fn_eq,
    (nu_cmpr_t)_nu_arr_eq,
    (nu_cmpr_t)_nu_obj_eq,
    (nu_cmpr_t)_nu_thr_eq};

const nu_bool *nu_eq(const nu_val *l, const nu_val *r)
{
    return _nu_eq_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

const nu_bool *_nu_bool_ne(const nu_bool *l, const nu_bool *r)
{
    return nu_literal_bool[l->data != r->data];
}
const nu_bool *_nu_num_ne(const nu_num *l, const nu_num *r)
{
    return nu_literal_bool[l->data != r->data];
}
const nu_bool *_nu_str_ne(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_fn_ne(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_arr_ne(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_obj_ne(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_thr_ne(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}

nu_cmpr_t _nu_ne_ptr[8] = {
    (nu_cmpr_t)nu_oper_none,
    (nu_cmpr_t)_nu_bool_ne,
    (nu_cmpr_t)_nu_num_ne,
    (nu_cmpr_t)_nu_str_ne,
    (nu_cmpr_t)_nu_fn_ne,
    (nu_cmpr_t)_nu_arr_ne,
    (nu_cmpr_t)_nu_obj_ne,
    (nu_cmpr_t)_nu_thr_ne};

const nu_bool *nu_ne(nu_val *l, nu_val *r)
{
    return _nu_ne_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

const nu_bool *_nu_bool_ge(const nu_bool *l, const nu_bool *r)
{
    return NU_NONE;
}
const nu_bool *_nu_num_ge(const nu_num *l, const nu_num *r)
{
    return nu_literal_bool[l->data >= r->data];
}
const nu_bool *_nu_str_ge(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_fn_ge(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_arr_ge(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_obj_ge(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_thr_ge(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}

nu_cmpr_t _nu_ge_ptr[8] = {
    (nu_cmpr_t)nu_oper_none,
    (nu_cmpr_t)_nu_bool_ge,
    (nu_cmpr_t)_nu_num_ge,
    (nu_cmpr_t)_nu_str_ge,
    (nu_cmpr_t)_nu_fn_ge,
    (nu_cmpr_t)_nu_arr_ge,
    (nu_cmpr_t)_nu_obj_ge,
    (nu_cmpr_t)_nu_thr_ge};

const nu_bool *nu_ge(const nu_val *l, const nu_val *r)
{
    return _nu_ge_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

const nu_bool *_nu_bool_gt(const nu_bool *l, const nu_bool *r)
{
    return nu_literal_bool[l->data > r->data];
}
const nu_bool *_nu_num_gt(const nu_num *l, const nu_num *r)
{
    return nu_literal_bool[l->data > r->data];
}
const nu_bool *_nu_str_gt(const nu_str *l, const nu_str *r)
{
    return nu_literal_bool[l->len > r->len];
}
const nu_bool *_nu_fn_gt(const nu_fn *l, const nu_fn *r)
{
    return NU_NONE;
}
const nu_bool *_nu_arr_gt(const nu_arr *l, const nu_arr *r)
{
    return nu_literal_bool[l->len > r->len];
}
const nu_bool *_nu_obj_gt(const nu_obj *l, const nu_obj *r)
{
    return nu_literal_bool[l->len > r->len];
}
const nu_bool *_nu_thr_gt(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}

nu_cmpr_t _nu_gt_ptr[8] = {
    (nu_cmpr_t)nu_oper_none,
    (nu_cmpr_t)_nu_bool_gt,
    (nu_cmpr_t)_nu_num_gt,
    (nu_cmpr_t)_nu_str_gt,
    (nu_cmpr_t)_nu_fn_gt,
    (nu_cmpr_t)_nu_arr_gt,
    (nu_cmpr_t)_nu_obj_gt,
    (nu_cmpr_t)_nu_thr_gt};

const nu_bool *nu_gt(const nu_val *l, const nu_val *r)
{
    return _nu_gt_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_cmpr_t _nu_cmp_ptr[8] = {
    (nu_cmpr_t)nu_lt,
    (nu_cmpr_t)nu_le,
    (nu_cmpr_t)nu_eq,
    (nu_cmpr_t)nu_ne,
    (nu_cmpr_t)nu_ge,
    (nu_cmpr_t)nu_gt};

const nu_bool *nu_cmp(const nu_val *l, const nu_val *r, uint8_t how)
{
    return _nu_cmp_ptr[how](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_bool *_nu_bool_add(nu_bool *l, nu_bool *r)
{
    return nu_literal_bool[(l->data + r->data) != 0];
}

nu_num *_nu_num_add(nu_num *l, nu_num *r)
{
    return nu_num_new(l->data + r->data);
}

nu_fn *_nu_fn_add(nu_fn *l, nu_fn *r)
{
    return NU_NONE;
}

nu_str *_nu_str_add(nu_str *l, nu_str *r)
{
    char *res = nu_calloc(l->cap + r->cap - 1, char);
    strncpy_s(res, l->cap - 1, l->data, l->cap - 1);
    strncpy_s(res + l->cap-1, r->cap - 1, r->data, r->cap - 1);
    return nu_str_new(res);
}

nu_arr *_nu_arr_add(nu_arr *l, nu_arr *r)
{
    nu_arr *res = nu_arr_new(l->cap + r->cap);
    for (int i = 0; i < l->len; ++i)
    {
        nu_arr_push_val(res, nu_arr_get_val_c(l, i));
    }
    for (int i = 0; i < r->len; ++i)
    {
        nu_arr_push_val(res, nu_arr_get_val_c(r, i));
    }
    return res;
}

nu_obj *_nu_obj_add(nu_obj *l, nu_obj *r)
{
    // TODO: implement
    return NU_NONE;
}

const nu_oper_t _nu_add_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_bool_add,
    (nu_oper_t)_nu_num_add,
    (nu_oper_t)_nu_str_add,
    (nu_oper_t)_nu_fn_add,
    (nu_oper_t)_nu_arr_add,
    (nu_oper_t)_nu_obj_add,
    (nu_oper_t)nu_oper_none};

nu_val *nu_add(nu_val *l, const nu_val *r)
{
    return _nu_add_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_sub(nu_bool *l, nu_bool *r)
{
    return nu_literal_bool[l->data - r->data];
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
    return _nu_sub_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_mul(nu_bool *l, nu_bool *r)
{
    return nu_literal_bool[l->data * r->data];
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
    return _nu_mul_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_none_div(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

nu_val *_nu_bool_div(nu_bool *l, nu_bool *r)
{
    return nu_literal_bool[l->data / r->data];
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
    return _nu_div_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_mod(nu_bool *l, nu_bool *r)
{
    return nu_literal_bool[l->data % r->data];
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

#include "nu.h"

#include <assert.h>
#include <stdio.h>
#include <float.h>

#include "nurbt.h"

// --------------------------------------------------------------------------------------------------------------------------------

const nu_val nu_none = {NU_T_NONE, 0ul};
const nu_bool nu_true = {NU_T_BOOL, 0ul, true};
const nu_bool nu_false = {NU_T_BOOL, 0ul, false};
const nu_num nu_zero = {NU_T_NUM, 0ul, 0.0};
const nu_num nu_one = {NU_T_NUM, 0ul, 1.0};
const nu_str nu_empty = {NU_T_STR, 0ul, 0, 1, ""};

/// @brief Const array to convert a boolean value efficiently to one of the true or false constants.
const nu_bool *nu_literal_bool[2] = {NU_FALSE, NU_TRUE};

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

NU_NEW_FPTR(nu_free_fptr, void, nu_val *);

void _nu_free_none(nu_val *o) {}

nu_free_fptr _nu_free_ptr[8] = {
    (nu_free_fptr)_nu_free_none,
    (nu_free_fptr)nu_free_bool,
    (nu_free_fptr)nu_free_num,
    (nu_free_fptr)nu_free_str,
    (nu_free_fptr)_nu_free_none,
    (nu_free_fptr)nu_free_arr,
    (nu_free_fptr)nu_free_obj,
    (nu_free_fptr)_nu_free_none};
void nu_free(nu_val *o)
{
    _nu_free_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_NEW_FPTR(nu_len_fptr, nu_num *, const nu_val *);

nu_num *_nu_len_none(nu_val *o)
{
    return (nu_num *)NU_NONE;
}

nu_len_fptr _nu_len_ptr[8] = {
    (nu_len_fptr)_nu_len_none,
    (nu_len_fptr)_nu_len_none,
    (nu_len_fptr)_nu_len_none,
    (nu_len_fptr)_nu_len_none,
    (nu_len_fptr)_nu_len_none,
    (nu_len_fptr)nu_len_arr,
    (nu_len_fptr)nu_len_obj,
    (nu_len_fptr)_nu_len_none};
nu_num *nu_len(const nu_val *o)
{
    _nu_len_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_NEW_FPTR(nu_cap_fptr, nu_num *, const nu_val *);

nu_num *_nu_cap_none(nu_val *o)
{
    return (nu_num *)NU_NONE;
}

nu_cap_fptr _nu_cap_ptr[8] = {
    (nu_cap_fptr)_nu_cap_none,
    (nu_cap_fptr)_nu_cap_none,
    (nu_cap_fptr)_nu_cap_none,
    (nu_cap_fptr)_nu_cap_none,
    (nu_cap_fptr)_nu_cap_none,
    (nu_cap_fptr)nu_cap_arr,
    (nu_cap_fptr)nu_cap_obj,
    (nu_cap_fptr)_nu_cap_none};
nu_num *nu_cap(const nu_val *o)
{
    _nu_cap_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_NEW_FPTR(nu_hash_fptr, nu_num *, nu_val *);

nu_num *_nu_hash_none(nu_val *o)
{
    return NU_NONE;
}
nu_num *_nu_hash_bool(nu_bool *o)
{
    return nu_new_num(o->data != 0);
}
nu_num *_nu_hash_num(nu_num *o)
{
    return nu_new_num(o->data);
}
nu_num *_nu_hash_str(nu_str *o)
{
    return nu_new_num((num_t)hashN(o->data, o->cap));
}
nu_num *_nu_hash_fn(nu_fn *o)
{
    return NU_NONE; // hashable???
}
nu_num *_nu_hash_arr(nu_arr *o)
{
    size_t h = _FNV_OFFSET_N;
    size_t i;
    for (i = 0; i < o->len; ++i)
    {
        h = (h ^ nu_c_to_size_t(nu_hash(o->data[i]))) * _FNV_PRIME_N;
    }
    return nu_new_num((double)h); // TODO: hash all array elems
}
nu_num *_nu_hash_obj(nu_obj *o)
{
    // nu_val *hfn = nu_get_val_obj(o, nu_new_str("$hash"));
    // if(hfn != NU_NONE)
    //{
    //     return (nu_num *)nu_call_fn(hfn, o);
    // }
    return NU_NONE;
}
nu_num *_nu_hash_thr(nu_thr *o)
{
    return NU_NONE; // hashable???
}

nu_hash_fptr _nu_hash_ptr[8] = {
    (nu_hash_fptr)_nu_hash_none,
    (nu_hash_fptr)_nu_hash_bool,
    (nu_hash_fptr)_nu_hash_num,
    (nu_hash_fptr)_nu_hash_str,
    (nu_hash_fptr)_nu_hash_fn,
    (nu_hash_fptr)_nu_hash_arr,
    (nu_hash_fptr)_nu_hash_obj,
    (nu_hash_fptr)_nu_hash_thr};
nu_num *nu_hash(const nu_val *o)
{
    return _nu_hash_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_NEW_FPTR(nu_repr_fptr, const char *, const nu_val *);

const char *_nu_repr_none(const nu_val *o)
{
    return utfdup("none");
}
const char *_nu_repr_bool(const nu_bool *o)
{
    return utfdup(o->data ? "true" : "false");
}
const char *_nu_repr_num(const nu_num *o)
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
const char *_nu_repr_str(const nu_str *o)
{
    char *buf = nu_calloc(char, o->cap + 3);
    snprintf(buf, o->cap + 3, "\"%s\"", o->data);
    buf[o->cap + 2] = NULL;
    return buf;
}
const char *_nu_repr_fn(const nu_fn *o)
{
    return NULL;
}
const char *_nu_repr_arr(const nu_arr *o)
{
    char **elems = nu_calloc(char *, o->len);
    size_t *sizes = nu_calloc(size_t, o->len);
    size_t i = 0, total_size = 0;
    total_size += 4; // account for "[ " and " ]"
    for (i; i < o->len; ++i)
    {
        char *elem = elems[i] = nu_c_repr(o->data[i]);
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
const char *_nu_repr_obj(const nu_obj *o)
{
    return NULL;
}
const char *_nu_repr_thr(const nu_thr *o)
{
    return NULL;
}
nu_repr_fptr _nu_repr_ptr[8] = {
    (nu_repr_fptr)_nu_repr_none,
    (nu_repr_fptr)_nu_repr_bool,
    (nu_repr_fptr)_nu_repr_num,
    (nu_repr_fptr)_nu_repr_str,
    (nu_repr_fptr)_nu_repr_fn,
    (nu_repr_fptr)_nu_repr_arr,
    (nu_repr_fptr)_nu_repr_obj,
    (nu_repr_fptr)_nu_repr_thr};
const char *nu_c_repr(const nu_val *o)
{
    return _nu_repr_ptr[o->type](o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_NEW_FPTR(nu_set_val_fptr, bool, nu_val *, nu_val *, nu_val *);

bool _nu_set_val_none(nu_val *_0, nu_val *_1, nu_val *_2)
{
    return false;
}

nu_set_val_fptr _nu_set_val_ptr[8] = {
    (nu_set_val_fptr)_nu_set_val_none,
    (nu_set_val_fptr)_nu_set_val_none,
    (nu_set_val_fptr)_nu_set_val_none,
    (nu_set_val_fptr)_nu_set_val_none,
    (nu_set_val_fptr)_nu_set_val_none,
    (nu_set_val_fptr)nu_set_val_arr,
    (nu_set_val_fptr)nu_set_val_obj,
    (nu_set_val_fptr)_nu_set_val_none};
bool nu_set_val(nu_val *cnt, nu_val *key, nu_val *val)
{
    return _nu_set_val_ptr[cnt->type](cnt, key, val);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_NEW_FPTR(nu_get_val_fptr, nu_val *, nu_val *, nu_val *);

nu_val *_nu_get_val_none(nu_val *_0, nu_val *_1)
{
    return NU_NONE;
}

nu_get_val_fptr _nu_get_val_ptr[8] = {
    (nu_get_val_fptr)_nu_get_val_none,
    (nu_get_val_fptr)_nu_get_val_none,
    (nu_get_val_fptr)_nu_get_val_none,
    (nu_get_val_fptr)_nu_get_val_none,
    (nu_get_val_fptr)_nu_get_val_none,
    (nu_get_val_fptr)nu_get_val_arr,
    (nu_get_val_fptr)nu_get_val_obj,
    (nu_get_val_fptr)_nu_get_val_none};
nu_val *nu_get_val(nu_val *cnt, nu_val *key)
{
    return _nu_get_val_ptr[cnt->type](cnt, key);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_NEW_FPTR(nu_add_val_fptr, bool, nu_val *, nu_val *, nu_val *);

bool _nu_add_val_none(nu_val *_0, nu_val *_1, nu_val *_2)
{
    return false;
}

nu_add_val_fptr _nu_add_val_ptr[8] = {
    (nu_add_val_fptr)_nu_add_val_none,
    (nu_add_val_fptr)_nu_add_val_none,
    (nu_add_val_fptr)_nu_add_val_none,
    (nu_add_val_fptr)_nu_add_val_none,
    (nu_add_val_fptr)_nu_add_val_none,
    (nu_add_val_fptr)nu_add_val_arr,
    (nu_add_val_fptr)nu_add_val_obj,
    (nu_add_val_fptr)_nu_add_val_none};
bool nu_add_val(nu_val *cnt, nu_val *key, nu_val *val)
{
    return _nu_add_val_ptr[cnt->type](cnt, key, val);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_NEW_FPTR(nu_del_val_fptr, nu_val *, nu_val *, nu_val *);

nu_val *_nu_del_val_none(nu_val *_0, nu_val *_1)
{
    return NU_NONE;
}

nu_del_val_fptr _nu_del_val_ptr[8] = {
    (nu_del_val_fptr)_nu_del_val_none,
    (nu_del_val_fptr)_nu_del_val_none,
    (nu_del_val_fptr)_nu_del_val_none,
    (nu_del_val_fptr)_nu_del_val_none,
    (nu_del_val_fptr)_nu_del_val_none,
    (nu_del_val_fptr)nu_del_val_arr,
    (nu_del_val_fptr)nu_del_val_obj,
    (nu_del_val_fptr)_nu_del_val_none};
nu_val *nu_del_val(nu_val *cnt, nu_val *key)
{
    return _nu_del_val_ptr[cnt->type](cnt, key);
}

// --------------------------------------------------------------------------------------------------------------------------------

const nu_bool *_nu_lt_bool(const nu_bool *l, const nu_bool *r)
{
    return nu_literal_bool[l->data < r->data];
}
const nu_bool *_nu_lt_num(const nu_num *l, const nu_num *r)
{
    return nu_literal_bool[l->data < r->data];
}
const nu_bool *_nu_lt_str(const nu_str *l, const nu_str *r)
{
    return NU_FALSE; // TODO: implement this
}
const nu_bool *_nu_lt_fn(const nu_fn *l, const nu_fn *r)
{
    return NU_FALSE;
}
const nu_bool *_nu_lt_arr(const nu_arr *l, const nu_arr *r)
{
    return nu_literal_bool[l->len < r->len];
}
const nu_bool *_nu_lt_obj(const nu_obj *l, const nu_obj *r)
{
    return nu_literal_bool[l->len < r->len];
}
const nu_bool *_nu_lt_thr(const nu_val *l, const nu_val *r)
{
    return NU_FALSE;
}

nu_cmpr_t _nu_lt_ptr[8] = {
    (nu_cmpr_t)nu_oper_none,
    (nu_cmpr_t)_nu_lt_bool,
    (nu_cmpr_t)_nu_lt_num,
    (nu_cmpr_t)_nu_lt_str,
    (nu_cmpr_t)_nu_lt_fn,
    (nu_cmpr_t)_nu_lt_arr,
    (nu_cmpr_t)_nu_lt_obj,
    (nu_cmpr_t)_nu_lt_thr};

const nu_bool *nu_lt(const nu_val *l, const nu_val *r)
{
    return _nu_lt_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

const nu_bool *_nu_le_bool(const nu_bool *l, const nu_bool *r)
{
    return nu_literal_bool[l->data < r->data];
}
const nu_bool *_nu_le_num(const nu_num *l, const nu_num *r)
{
    return nu_literal_bool[l->data <= r->data];
}
const nu_bool *_nu_le_str(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_le_fn(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_le_arr(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_le_obj(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_le_thr(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}

nu_cmpr_t _nu_le_ptr[8] = {
    (nu_cmpr_t)nu_oper_none,
    (nu_cmpr_t)_nu_le_bool,
    (nu_cmpr_t)_nu_le_num,
    (nu_cmpr_t)_nu_le_str,
    (nu_cmpr_t)_nu_le_fn,
    (nu_cmpr_t)_nu_le_arr,
    (nu_cmpr_t)_nu_le_obj,
    (nu_cmpr_t)_nu_le_thr};

const nu_bool *nu_le(const nu_val *l, const nu_val *r)
{
    return _nu_le_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

const nu_bool *_nu_eq_bool(const nu_bool *l, const nu_bool *r)
{
    return nu_literal_bool[l->data == r->data];
}
const nu_bool *_nu_eq_num(const nu_num *l, const nu_num *r)
{
    return nu_literal_bool[l->data == r->data];
}
const nu_bool *_nu_eq_str(const nu_str *l, const nu_str *r)
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
const nu_bool *_nu_eq_fn(const nu_fn *l, const nu_fn *r)
{
    return nu_literal_bool[l == r];
}
const nu_bool *_nu_eq_arr(const nu_arr *l, const nu_arr *r)
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
const nu_bool *_nu_eq_obj(const nu_obj *l, const nu_obj *r)
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
const nu_bool *_nu_eq_thr(const nu_thr *l, const nu_thr *r)
{
    return nu_literal_bool[l == r];
}

nu_cmpr_t _nu_eq_ptr[8] = {
    (nu_cmpr_t)nu_oper_none,
    (nu_cmpr_t)_nu_eq_bool,
    (nu_cmpr_t)_nu_eq_num,
    (nu_cmpr_t)_nu_eq_str,
    (nu_cmpr_t)_nu_eq_fn,
    (nu_cmpr_t)_nu_eq_arr,
    (nu_cmpr_t)_nu_eq_obj,
    (nu_cmpr_t)_nu_eq_thr};

const nu_bool *nu_eq(const nu_val *l, const nu_val *r)
{
    return _nu_eq_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

const nu_bool *_nu_ne_bool(const nu_bool *l, const nu_bool *r)
{
    return nu_literal_bool[l->data != r->data];
}
const nu_bool *_nu_ne_num(const nu_num *l, const nu_num *r)
{
    return nu_literal_bool[l->data != r->data];
}
const nu_bool *_nu_ne_str(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_ne_fn(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_ne_arr(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_ne_obj(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_ne_thr(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}

nu_cmpr_t _nu_ne_ptr[8] = {
    (nu_cmpr_t)nu_oper_none,
    (nu_cmpr_t)_nu_ne_bool,
    (nu_cmpr_t)_nu_ne_num,
    (nu_cmpr_t)_nu_ne_str,
    (nu_cmpr_t)_nu_ne_fn,
    (nu_cmpr_t)_nu_ne_arr,
    (nu_cmpr_t)_nu_ne_obj,
    (nu_cmpr_t)_nu_ne_thr};

const nu_bool *nu_ne(nu_val *l, nu_val *r)
{
    return _nu_ne_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

const nu_bool *_nu_ge_bool(const nu_bool *l, const nu_bool *r)
{
    return NU_NONE;
}
const nu_bool *_nu_ge_num(const nu_num *l, const nu_num *r)
{
    return nu_literal_bool[l->data >= r->data];
}
const nu_bool *_nu_ge_str(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_ge_fn(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_ge_arr(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_ge_obj(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_ge_thr(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}

nu_cmpr_t _nu_ge_ptr[8] = {
    (nu_cmpr_t)nu_oper_none,
    (nu_cmpr_t)_nu_ge_bool,
    (nu_cmpr_t)_nu_ge_num,
    (nu_cmpr_t)_nu_ge_str,
    (nu_cmpr_t)_nu_ge_fn,
    (nu_cmpr_t)_nu_ge_arr,
    (nu_cmpr_t)_nu_ge_obj,
    (nu_cmpr_t)_nu_ge_thr};

const nu_bool *nu_ge(const nu_val *l, const nu_val *r)
{
    return _nu_ge_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

const nu_bool *_nu_gt_bool(const nu_bool *l, const nu_bool *r)
{
    return NU_NONE;
}
const nu_bool *_nu_gt_num(const nu_num *l, const nu_num *r)
{
    return nu_literal_bool[l->data > r->data];
}
const nu_bool *_nu_gt_str(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_gt_fn(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_gt_arr(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_gt_obj(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}
const nu_bool *_nu_gt_thr(const nu_val *l, const nu_val *r)
{
    return NU_NONE;
}

nu_cmpr_t _nu_gt_ptr[8] = {
    (nu_cmpr_t)nu_oper_none,
    (nu_cmpr_t)_nu_gt_bool,
    (nu_cmpr_t)_nu_gt_num,
    (nu_cmpr_t)_nu_gt_str,
    (nu_cmpr_t)_nu_gt_fn,
    (nu_cmpr_t)_nu_gt_arr,
    (nu_cmpr_t)_nu_gt_obj,
    (nu_cmpr_t)_nu_gt_thr};

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
    _nu_cmp_ptr[how](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_add_bool(nu_bool *l, nu_bool *r)
{
    return nu_literal_bool[(l->data + r->data) != 0];
}

nu_val *_nu_add_num(nu_num *l, nu_num *r)
{
    return nu_new_num(l->data + r->data);
}

nu_val *_nu_add_str(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

nu_val *_nu_add_arr(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

const nu_oper_t _nu_add_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_add_bool,
    (nu_oper_t)_nu_add_num,
    (nu_oper_t)_nu_add_str,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_add_arr,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none};

nu_val *nu_add(nu_val *l, const nu_val *r)
{
    return _nu_add_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_sub_bool(nu_bool *l, nu_bool *r)
{
    return nu_literal_bool[l->data - r->data];
}

nu_val *_nu_sub_num(nu_num *l, nu_num *r)
{
    return nu_new_num(l->data - r->data);
}

nu_val *_nu_sub_str(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

const nu_oper_t _nu_sub_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_sub_bool,
    (nu_oper_t)_nu_sub_num,
    (nu_oper_t)_nu_sub_str,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none};

nu_val *nu_sub(nu_val *l, nu_val *r)
{
    return _nu_sub_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_mul_bool(nu_bool *l, nu_bool *r)
{
    return nu_literal_bool[l->data * r->data];
}

nu_val *_nu_mul_num(nu_num *l, nu_num *r)
{
    return nu_new_num(l->data * r->data);
}

nu_val *_nu_mul_str(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

const nu_oper_t _nu_mul_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_mul_bool,
    (nu_oper_t)_nu_mul_num,
    (nu_oper_t)_nu_mul_str,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)nu_oper_none};

nu_val *nu_mul(nu_val *l, nu_val *r)
{
    return _nu_mul_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_div_none(nu_val *l, nu_val *r)
{
    return NU_NONE;
}

nu_val *_nu_div_bool(nu_bool *l, nu_bool *r)
{
    return nu_literal_bool[l->data / r->data];
}

nu_val *_nu_div_num(nu_num *l, nu_num *r)
{
    return nu_new_num(l->data / r->data);
}

const nu_oper_t _nu_div_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_div_bool,
    (nu_oper_t)_nu_div_num,
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

nu_val *_nu_mod_bool(nu_bool *l, nu_bool *r)
{
    return nu_literal_bool[l->data % r->data];
}

nu_val *_nu_mod_num(nu_num *l, nu_num *r)
{
    return nu_new_num(fmod(l->data, r->data));
}

nu_oper_t _nu_mod_ptr[8] = {
    (nu_oper_t)nu_oper_none,
    (nu_oper_t)_nu_mod_bool,
    (nu_oper_t)_nu_mod_num,
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

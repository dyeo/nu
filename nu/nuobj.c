#include "nu.h"

#include <assert.h>

// --------------------------------------------------------------------------------------------------------------------------------

nu_obj *nu_obj_new()
{
    nu_obj *r = nu_malloc(nu_obj);
    NU_ASSERT(r != NULL, "heap allocation error");
    NU_OBJ_INIT(r, NULL);
    hmdefaults(r->data, nu_kvp_none);
    return r;
}

void nu_obj_free(nu_obj *o)
{
    o->type = NU_NONE_T;
    o->refs = 0;
    for (int i = 0; i < hmlen(o->data); ++i)
    {
        nu_decref(o->data[i].key);
        nu_decref(o->data[i].val);
    }
    hmfree(o->data);
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

bool nu_obj_set_val(nu_obj *o, nu_val *key, nu_val *val)
{
    if (o == NU_NONE || key == NU_NONE)
    {
        return false;
    }

    size_t hsh = nu_hash_c(key);
    nu_kvp res = hmgets(o->data, hsh);
    nu_decref(res.val);
    if (res.key == NU_NONE)
    {
        nu_incref(key);
        res.key = key;
    }
    res.hsh = hsh;
    nu_incref(val);
    res.val = val;
    hmputs(o->data, res);
    return true;
}

nu_val *nu_obj_get_val(nu_obj *o, const nu_val *key)
{
    if (o == NU_NONE || key == NU_NONE)
    {
        return NU_NONE;
    }

    size_t hsh = nu_hash_c(key);
    nu_kvp res = hmgets(o->data, hsh);
    return res.val;
}

nu_val *nu_obj_del_val(nu_obj *o, const nu_val *key)
{
    if (o == NU_NONE || key == NU_NONE)
    {
        return NU_NONE;
    }

    size_t hsh = nu_hash_c(key);
    nu_kvp res = hmgets(o->data, hsh);
    nu_decref(res.key);
    nu_decref(res.val);
    hmdel(o->data, hsh);
    return res.val;
}

// --------------------------------------------------------------------------------------------------------------------------------

size_t nu_obj_keys_c(nu_obj *o, nu_val **keys)
{
    int res = hmlen(o->data);
    arrsetcap(keys, res);
    for (int i = 0; i < res; ++i)
    {
        o->data = arrpush(keys, o->data[i].key);
    }
    return res;
}

size_t nu_obj_vals_c(nu_obj *o, nu_val **vals)
{
    int res = hmlen(o->data);
    arrsetcap(vals, res);
    for (int i = 0; i < res; ++i)
    {
        arrpush(vals, o->data[i].val);
    }
    return res;
}

nu_arr *nu_obj_keys(nu_obj *o)
{
    nu_val *keys = NULL;
    size_t len = nu_obj_keys_c(o, &keys);
    nu_arr *res = nu_malloc(nu_arr);
    NU_ARR_INIT(res, &keys);
    return res;
}

nu_arr *nu_obj_vals(nu_obj *o)
{
    nu_val *vals = NULL;
    size_t len = nu_obj_vals_c(o, &vals);
    nu_arr *res = nu_malloc(nu_arr);
    NU_ARR_INIT(res, &vals);
    return res;
}

bool nu_obj_has_key_c(nu_obj *o, nu_val *key)
{
    if (o == NU_NONE || key == NU_NONE)
    {
        return NU_NONE;
    }

    size_t hsh = nu_hash_c(key);
    nu_kvp res = hmgets(o->data, hsh);
    return res.hsh == hsh;
}

bool nu_obj_has_val_c(nu_obj *o, const nu_val *val)
{
    for (int i = 0; i < hmlen(o->data); ++i)
    {
        if (nu_eq(o->data[i].val, val))
        {
            return true;
        }
    }
    return false;
}

// --------------------------------------------------------------------------------------------------------------------------------

void nu_obj_clear(nu_obj *o)
{
    for (int i = 0; i < hmlen(o->data); ++i)
    {
        nu_decref(o->data[i].key);
        nu_decref(o->data[i].val);
    }
    hmfree(o->data);
}

// --------------------------------------------------------------------------------------------------------------------------------
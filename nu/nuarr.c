#include "nu.h"

// --------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------

nu_arr *nu_arr_new(size_t c)
{
    nu_arr *r = nu_malloc(nu_arr);
    NU_ASSERT(r != NULL, "heap allocation error");
    NU_ARR_INIT(r, 0, c == 0 ? 16 : c, nu_calloc(r->cap, nu_val *));
    return r;
}

void nu_arr_free(nu_arr *o)
{
    o->type = NU_NONE_T;
    o->len = o->cap = 0;
    for (size_t i = 0; i < o->len; ++i)
    {
        nu_decref(o->data[i]);
    }
    free(o->data);
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

bool nu_arr_set_val_c(nu_arr *a, size_t i, nu_val *v)
{
    if (i < a->len)
    {
        nu_val *old = a->data[i];
        nu_decref(old);
        nu_incref(v);
        a->data[i] = v;
        return true;
    }
    return false;
}

bool nu_arr_set_val(nu_arr *a, nu_num *i, nu_val *v)
{
    NU_ASSERT(nu_is_num(i), "cannot index array with non-number");
    return nu_arr_set_val_c(a, nu_size_t_get_c(i), v);
}

bool nu_arr_add_val_c(nu_arr *a, size_t i, nu_val *v)
{
    if (i > a->len)
    {
        return false;
    }
    nu_incref(v);
    if (a->len == a->cap)
    {
        a->cap *= 2;
        nu_val **tmp = nu_calloc(a->cap, nu_val *);
        NU_ASSERT(tmp != NULL, "heap allocation error");
        if (i > 0)
        {
            nu_copy(tmp, a->data, i, nu_val *);
        }
        tmp[i] = v;
        if (i < a->len)
        {
            nu_copy(tmp + i + 1, a->data + i, a->len - i, nu_val *);
        }
        free(a->data);
        a->data = tmp;
    }
    else
    {
        for (size_t j = i; j < a->len; ++j)
        {
            a->data[j + 1] = a->data[j];
        }
        a->data[i] = v;
    }
    a->len++;
    return true;
}

bool nu_arr_add_val(nu_arr *a, nu_num *i, nu_val *v)
{
    NU_ASSERT(nu_is_num(i), "cannot index array with non-number");
    return nu_arr_add_val_c(a, nu_size_t_get_c(i), v);
}

nu_val *nu_arr_get_val_c(nu_arr *a, size_t i)
{
    if (i < a->len)
    {
        return a->data[i];
    }
    return NU_NONE;
}

nu_val *nu_arr_get_val(nu_arr *a, nu_num *i)
{
    NU_ASSERT(nu_is_num(i), "cannot index array with non-number");
    return nu_arr_get_val_c(a, nu_size_t_get_c(i));
}

nu_val *nu_arr_del_val_c(nu_arr *a, size_t i)
{
    if (i < a->len)
    {
        nu_val *val = a->data[i];
        nu_decref(val);
        for (i; i < a->len - 1; ++i)
        {
            a->data[i] = a->data[i + 1];
        }
        a->len--;
        return val;
    }
    return NU_NONE;
}

nu_val *nu_arr_del_val(nu_arr *arr, nu_num *idx)
{
    NU_ASSERT(nu_is_num(idx), "cannot index array with non-number");
    return nu_arr_del_val_c(arr, nu_size_t_get_c(idx));
}

// --------------------------------------------------------------------------------------------------------------------------------

bool nu_arr_has_val_c(const nu_arr *arr, const nu_val *val)
{
    for (size_t i = 0; i < arr->len; ++i)
    {
        if (nu_eq(val, *(arr->data+i)))
        {
            return true;
        }
    }
    return false;
}

// --------------------------------------------------------------------------------------------------------------------------------

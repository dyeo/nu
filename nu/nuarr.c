#include "nu.h"

// --------------------------------------------------------------------------------------------------------------------------------

nu_arr *nu_new_arr(size_t c)
{
    nu_arr *r = nu_malloc(nu_arr);
    NU_ASSERT(r != NULL, "heap allocation error");
    r->type = NU_T_ARR;
    r->refs = 0u;
    r->len = 0u;
    r->cap = c == 0 ? 16 : c;
    r->data = nu_calloc(nu_val *, r->cap);
    return r;
}

void nu_free_arr(nu_arr *o)
{
    o->type = NU_T_NONE;
    o->len = o->cap = 0;
    for (size_t i = 0; i < o->len; ++i)
    {
        nu_decref(o->data[i]);
    }
    free(o->data);
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

bool nu_c_set_val_arr(nu_arr *a, size_t i, nu_val *v)
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

bool nu_set_val_arr(nu_arr *a, nu_num *i, nu_val *v)
{
    NU_ASSERT(nu_is_num(i), "cannot index array with non-number");
    return nu_c_set_val_arr(a, nu_c_to_size_t(i), v);
}

bool nu_c_add_val_arr(nu_arr *a, size_t i, nu_val *v)
{
    if (i > a->len)
    {
        return false;
    }
    nu_incref(v);
    if (a->len == a->cap)
    {
        a->cap *= 2;
        nu_val **tmp = nu_calloc(nu_val *, a->cap);
        NU_ASSERT(tmp != NULL, "heap allocation error");
        if (i > 0)
        {
            nu_copy(nu_val *, tmp, a->data, i);
        }
        tmp[i] = v;
        if (i < a->len)
        {
            nu_copy(nu_val *, tmp + i + 1, a->data + i, a->len - i);
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

bool nu_add_val_arr(nu_arr *a, nu_num *i, nu_val *v)
{
    NU_ASSERT(nu_is_num(i), "cannot index array with non-number");
    return nu_c_add_val_arr(a, nu_c_to_size_t(i), v);
}

nu_val *nu_c_get_val_arr(nu_arr *a, size_t i)
{
    if (i < a->len)
    {
        return a->data[i];
    }
    return NU_NONE;
}

nu_val *nu_get_val_arr(nu_arr *a, nu_num *i)
{
    NU_ASSERT(nu_is_num(i), "cannot index array with non-number");
    return nu_c_get_val_arr(a, nu_c_to_size_t(i));
}

nu_val *nu_c_del_val_arr(nu_arr *a, size_t i)
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

nu_val *nu_del_val_arr(nu_arr *arr, nu_num *idx)
{
    NU_ASSERT(nu_is_num(idx), "cannot index array with non-number");
    return nu_c_del_val_arr(arr, nu_c_to_size_t(idx));
}

// --------------------------------------------------------------------------------------------------------------------------------
#include "nu.h"

// --------------------------------------------------------------------------------------------------------------------------------

nu_arr *nu_arr_new(size_t c)
{
    nu_arr *a = nu_malloc(nu_arr);
    NU_ASSERT(a != NULL, "heap allocation error");
    NU_ARR_INIT(a, NULL);
    arrsetcap(a->data, c == 0 ? 16 : c);
    return a;
}

void nu_arr_free(nu_arr *a)
{
    a->type = NU_NONE_T;
    for (size_t i = 0; i < arrlen(a->data); ++i)
    {
        nu_decref(a->data[i]);
    }
    arrfree(a->data);
    free(a);
}

// --------------------------------------------------------------------------------------------------------------------------------

bool nu_arr_set_val_c(nu_arr *a, size_t i, nu_val *v)
{
    if (i < arrlen(a->data))
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
    if (i > arrlen(a->data))
    {
        return false;
    }
    
    nu_incref(v);
    arrins(a->data, i, v);
    return true;
}

bool nu_arr_add_val(nu_arr *a, nu_num *i, nu_val *v)
{
    NU_ASSERT(nu_is_num(i), "cannot index array with non-number");
    return nu_arr_add_val_c(a, nu_size_t_get_c(i), v);
}

nu_val *nu_arr_get_val_c(nu_arr *a, size_t i)
{
    if (i < arrlen(a->data))
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
    if (i >= arrlen(a->data))
    {
        return NU_NONE;
    }
    
    nu_val *res = a->data[i];
    arrdel(a->data, i);
    nu_decref(res);
    return res;
}

nu_val *nu_arr_del_val(nu_arr *arr, nu_num *idx)
{
    NU_ASSERT(nu_is_num(idx), "cannot index array with non-number");
    return nu_arr_del_val_c(arr, nu_size_t_get_c(idx));
}

// --------------------------------------------------------------------------------------------------------------------------------

bool nu_arr_has_val_c(const nu_arr *a, const nu_val *val)
{
    for (size_t i = 0; i < arrlen(a->data); ++i)
    {
        if (nu_eq(val, *(a->data+i)))
        {
            return true;
        }
    }
    return false;
}

// --------------------------------------------------------------------------------------------------------------------------------

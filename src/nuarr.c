#include "nu.h"

// --------------------------------------------------------------------------------------------------------------------------------

nu_arr *nu_arr_new(size_t c)
{
    nu_arr *r = nu_malloc(nu_arr);
    NU_ASSERT(r != NULL, "heap allocation error");
    r->type = NU_ARR_T;
    r->len = 0;
    if (c == 0)
        c = 16;
    r->cap = c;
    r->data = nu_calloc(nu_val *, r->cap);
    return r;
}

void nu_arr_free(nu_arr *o)
{
    o->type = NU_NONE_T;
    for (size_t i = 0; i < o->len; ++i)
    {
        nu_opt_decref(o->data[i]);
    }
    o->len = 0;
    free(o->data);
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

bool nu_arr_set_val_i(nu_arr *a, size_t i, nu_val *v)
{
    if (i < a->len)
    {
        nu_val *old = a->data[i];
        nu_opt_decref(old);
        nu_opt_incref(v);
        a->data[i] = v;
        return true;
    }
    return false;
}

bool nu_arr_set_val(nu_arr *a, nu_num *i, nu_val *v)
{
    NU_ASSERT(nu_is_num(i), "cannot index array with non-number");
    return nu_arr_set_val_i(a, nu_to_size_t(i), v);
}

bool nu_arr_add_val_i(nu_arr *a, size_t i, nu_val *v)
{
    if (i > a->len)
    {
        return false;
    }
    nu_opt_incref(v);
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

bool nu_arr_add_val(nu_arr *a, nu_num *i, nu_val *v)
{
    NU_ASSERT(nu_is_num(i), "cannot index array with non-number");
    return nu_arr_add_val_i(a, nu_to_size_t(i), v);
}

nu_val *nu_arr_get_val_i(nu_arr *a, size_t i)
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
    return nu_arr_get_val_i(a, nu_to_size_t(i));
}

nu_val *nu_arr_del_val_i(nu_arr *a, size_t i)
{
    printf("AAAA\n");
    if (i < a->len)
    {
        nu_val *val = a->data[i];
        nu_opt_decref(val);
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
    return nu_arr_del_val_i(arr, nu_to_size_t(idx));
}

// --------------------------------------------------------------------------------------------------------------------------------

void nu_arr_push_val(nu_arr *arr, nu_val *val)
{
    nu_arr_add_val_i(arr, arr->len, val);
}

nu_val *nu_arr_pop_val(nu_arr *arr)
{
    return nu_arr_del_val_i(arr, arr->len - 1);
}

void nu_arr_enqueue_val(nu_arr *arr, nu_val *val)
{
    nu_arr_add_val_i(arr, 0, val);
}

nu_val *nu_arr_dequeue_val(nu_arr *arr)
{
    return nu_arr_del_val_i(arr, arr->len - 1);
}

// --------------------------------------------------------------------------------------------------------------------------------
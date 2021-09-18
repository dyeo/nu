#include "nu.h"

// --------------------------------------------------------------------------------------------------------------------------------

nu_arr *nu_arr_new(size_t cap)
{
    nu_arr *r = NU_NEW(nu_arr);
    NU_ASSERT(r != NULL, "heap allocation error");
    r->type = NU_ARR_T;
    r->len = 0;
    r->cap = cap;
    r->data = NU_ANEW(nu_val *, r->cap);
    return r;
}

void nu_arr_free(nu_arr *o)
{
    o->type = NU_NONE_T;
    o->len = 0;
    free(o->data);
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

bool nu_arr_set_val(nu_arr *arr, nu_val *idx, nu_val *val)
{
    size_t i = nu_to_size_t(idx);
    if(i < arr->len)
    {
        nu_val *old = arr->data[i];
        nu_opt_decref(old);
        nu_opt_incref(val);
        arr->data[i] = val;
        return true;
    }
    return false;
}

bool nu_arr_add_val(nu_arr *arr, nu_val *idx, nu_val *val)
{
    size_t i = nu_to_size_t(idx);
    if(i > arr->len)
    {
        return false;
    }
    bool add_to_end = i == arr->len;
    if(arr->len == arr->cap)
    {
        if(arr->cap * 2 > SIZE_MAX)
        {
            NU_ASSERT(arr->cap < SIZE_MAX, "array too large");
            arr->cap = SIZE_MAX;
        }
        arr->cap *= 2;
        nu_val **tmp = NU_ANEW(nu_val *, arr->cap);
        memcpy(tmp, arr->data, arr->len);
        free(arr->data);
        arr->data = tmp;
    }
}

nu_val *nu_arr_get_val(nu_arr *arr, nu_val *idx)
{

}

bool nu_arr_del_val(nu_arr *arr, nu_val *idx)
{

}


// --------------------------------------------------------------------------------------------------------------------------------
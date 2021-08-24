#include "nu.h"

nu_bool *nu_new_bool(bool v)
{
    nu_bool *o = (nu_bool *)malloc(sizeof(nu_bool));
    o->type = NU_BOOL_T;
    o->refs = 0u;
    o->value = v;
    return o;
}

bool _nu_none_to_bool(nu_base *o) { return 0; }
bool _nu_bool_to_bool(nu_bool *o) { return o->value; }
bool _nu_num_to_bool(nu_num *o) { return o->value != 0.0; }
bool _nu_str_to_bool(nu_base *o) { return 0; }
bool _nu_fn_to_bool(nu_base *o) { return 0; }
bool _nu_arr_to_bool(nu_base *o) { return 0; }
bool _nu_obj_to_bool(nu_base *o) { return 0; }
bool _nu_thr_to_bool(nu_base *o) { return 0; }
bool (*_nu_to_bool_ptr[8])(nu_base *o) = {
    (bool (*)(nu_base *))_nu_none_to_bool,
    (bool (*)(nu_base *))_nu_bool_to_bool,
    (bool (*)(nu_base *))_nu_num_to_bool,
    (bool (*)(nu_base *))_nu_str_to_bool,
    (bool (*)(nu_base *))_nu_fn_to_bool,
    (bool (*)(nu_base *))_nu_arr_to_bool,
    (bool (*)(nu_base *))_nu_obj_to_bool,
    (bool (*)(nu_base *))_nu_thr_to_bool};

bool nu_to_bool(nu_base *b)
{
    return _nu_to_bool_ptr[b->type](b);
}
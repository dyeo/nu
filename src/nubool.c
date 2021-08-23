#include "nu.h"

nu_bool *nu_new_bool(bool v)
{
    nu_bool *o = malloc(sizeof(nu_bool));
    o->type = NU_BOOL_T;
    o->refs = 0u;
    o->value = v;
    return o;
}

NU_LOOKUP_FUNC(
    to_bool,
    bool,
    { return false; },
    { return o->value; },
    { return o->value != 0.0; },
    { return false; },
    { return false; },
    { return false; },
    { return false; },
    { return false; }
);

bool nu_to_bool(nu_base *b)
{
    return _nu_to_bool_ptr[b->type](b);
}
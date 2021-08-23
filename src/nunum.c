#include "nu.h"
#include <math.h>

nu_num *nu_new_num(num_t v) 
{ 
    nu_num *o = malloc(sizeof(nu_num));
    o->type = NU_NUM_T;
    o->refs = 0u;
    o->value = v;
    return o;
}

NU_LOOKUP_FUNC(
    to_num_t,
    num_t,
    { return 0.0; },
    { return o->value;},
    { return o->value; },
    { return 0.0; },
    { return 0.0; },
    { return 0.0; },
    { return 0.0; },
    { return 0.0; }
);

double nu_to_double(nu_base *v) { return (double)_nu_to_num_t_ptr[v->type](v); }
long nu_to_long(nu_base *v) { return (long)_nu_to_num_t_ptr[v->type](v); }
float nu_to_float(nu_base *v) { return (float)_nu_to_num_t_ptr[v->type](v); }
int nu_to_int(nu_base *v) { return (int)_nu_to_num_t_ptr[v->type](v); }

NU_LOOKUP_FUNC2(
    add,
    nu_base *,
    { return &nu_none; },
    { return r == &nu_none ? &nu_none : nu_new_bool(l->value + r->value); },
    { return r == &nu_none ? &nu_none : nu_new_num(l->value + r->value); },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; }
);

nu_base *nu_add(nu_base *l, nu_base *r)
{
    return _nu_add_ptr[l->type](l, r);
}

NU_LOOKUP_FUNC2(
    sub,
    nu_base *,
    { return &nu_none; },
    { return r == &nu_none ? &nu_none : nu_new_bool(l->value - r->value); },
    { return r == &nu_none ? &nu_none : nu_new_num(l->value - r->value); },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; }
);

nu_base *nu_sub(nu_base *l, nu_base *r)
{
    return _nu_sub_ptr[l->type](l, r);
}

NU_LOOKUP_FUNC2(
    mul,
    nu_base *,
    { return &nu_none; },
    { return r == &nu_none ? &nu_none : nu_new_bool(l->value * r->value); },
    { return r == &nu_none ? &nu_none : nu_new_num(l->value * r->value); },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; }
);

nu_base *nu_mul(nu_base *l, nu_base *r)
{
    return _nu_mul_ptr[l->type](l, r);
}

NU_LOOKUP_FUNC2(
    div,
    nu_base *,
    { return &nu_none; },
    { return r == &nu_none ? &nu_none : nu_new_bool(l->value / r->value); },
    { return r == &nu_none ? &nu_none : nu_new_num(l->value / r->value); },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; }
);

nu_base *nu_div(nu_base *l, nu_base *r)
{
    return _nu_div_ptr[l->type](l, r);
}

NU_LOOKUP_FUNC2(
    mod,
    nu_base *,
    { return &nu_none; },
    { return r == &nu_none ? &nu_none : nu_new_bool(l->value % r->value); },
    { return r == &nu_none ? &nu_none : nu_new_num(fmod(l->value, r->value)); },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; },
    { return r == &nu_none ? &nu_none : &nu_none; }
);

nu_base *nu_mod(nu_base *l, nu_base *r)
{
    return _nu_mod_ptr[l->type](l, r);
}

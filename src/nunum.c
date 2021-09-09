#include "nu.h"
#include <math.h>
#include <assert.h>

nu_num *nu_new_num(num_t v)
{
	nu_num *r = NU_NEW(nu_num);
	assert(r != NULL);
	r->type = NU_NUM_T;
	r->refs = 0u;
	r->data = v;
	return r;
}

NU_OP_DEF(nu_to_num_t_t, num_t, nu_base *);
num_t _nu_none_to_num_t(nu_base *o)
{
	return 0.0;
}
num_t _nu_bool_to_num_t(nu_bool *o)
{
	return o->data;
}
num_t _nu_num_to_num_t(nu_num *o)
{
	return o->data;
}
num_t _nu_str_to_num_t(nu_base *o)
{
	return 0.0;
}
num_t _nu_fn_to_num_t(nu_base *o)
{
	return 0.0;
}
num_t _nu_arr_to_num_t(nu_base *o)
{
	return 0.0;
}
num_t _nu_obj_to_num_t(nu_base *o)
{
	return 0.0;
}
num_t _nu_thr_to_num_t(nu_base *o)
{
	return 0.0;
}
const nu_to_num_t_t _nu_to_num_t_ptr[8] = {
	(nu_to_num_t_t)_nu_none_to_num_t,
	(nu_to_num_t_t)_nu_bool_to_num_t,
	(nu_to_num_t_t)_nu_num_to_num_t,
	(nu_to_num_t_t)_nu_str_to_num_t,
	(nu_to_num_t_t)_nu_fn_to_num_t,
	(nu_to_num_t_t)_nu_arr_to_num_t,
	(nu_to_num_t_t)_nu_obj_to_num_t,
	(nu_to_num_t_t)_nu_thr_to_num_t };

double nu_to_double(nu_base *v)
{
	return (double)_nu_to_num_t_ptr[v->type](v);
}
long nu_to_long(nu_base *v)
{
	return (long)_nu_to_num_t_ptr[v->type](v);
}
float nu_to_float(nu_base *v)
{
	return (float)_nu_to_num_t_ptr[v->type](v);
}
int nu_to_int(nu_base *v)
{
	return (int)_nu_to_num_t_ptr[v->type](v);
}

nu_base *_nu_bool_add(nu_bool *l, nu_bool *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->data + r->data);
}
nu_base *_nu_num_add(nu_num *l, nu_num *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_num(l->data + r->data);
}
nu_base *_nu_str_add(nu_base *l, nu_base *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
nu_base *_nu_arr_add(nu_base *l, nu_base *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
const nu_oper_t _nu_add_ptr[8] = {
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)_nu_bool_add,
	(nu_oper_t)_nu_num_add,
	(nu_oper_t)_nu_str_add,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)_nu_arr_add,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none };

nu_base *nu_add(nu_base *l, nu_base *r)
{
	return _nu_add_ptr[l->type](l, r);
}

nu_base *_nu_bool_sub(nu_bool *l, nu_bool *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->data - r->data);
}
nu_base *_nu_num_sub(nu_num *l, nu_num *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_num(l->data - r->data);
}
nu_base *_nu_str_sub(nu_base *l, nu_base *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
const nu_oper_t _nu_sub_ptr[8] = {
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)_nu_bool_sub,
	(nu_oper_t)_nu_num_sub,
	(nu_oper_t)_nu_str_sub,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none
};
nu_base *nu_sub(nu_base *l, nu_base *r)
{
	return _nu_sub_ptr[l->type](l, r);
}

nu_base *_nu_bool_mul(nu_bool *l, nu_bool *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->data * r->data);
}
nu_base *_nu_num_mul(nu_num *l, nu_num *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_num(l->data * r->data);
}
nu_base *_nu_str_mul(nu_base *l, nu_base *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : &nu_none;
}
const nu_oper_t _nu_mul_ptr[8] = {
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)_nu_bool_mul,
	(nu_oper_t)_nu_num_mul,
	(nu_oper_t)_nu_str_mul,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none
};
nu_base *nu_mul(nu_base *l, nu_base *r)
{
	return _nu_mul_ptr[l->type](l, r);
}

nu_base *_nu_none_div(nu_base *l, nu_base *r)
{
	return &nu_none;
}
nu_base *_nu_bool_div(nu_bool *l, nu_bool *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->data / r->data);
}
nu_base *_nu_num_div(nu_num *l, nu_num *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_num(l->data / r->data);
}
const nu_oper_t _nu_div_ptr[8] = {
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)_nu_bool_div,
	(nu_oper_t)_nu_num_div,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none
};
nu_base *nu_div(nu_base *l, nu_base *r)
{
	return _nu_div_ptr[l->type](l, r);
}

nu_base *_nu_bool_mod(nu_bool *l, nu_bool *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_bool(l->data % r->data);
}
nu_base *_nu_num_mod(nu_num *l, nu_num *r)
{
	return ((nu_base *)r) == &nu_none ? &nu_none : nu_new_num(fmod(l->data, r->data));
}
nu_oper_t _nu_mod_ptr[8] = {
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)_nu_bool_mod,
	(nu_oper_t)_nu_num_mod,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none
};

nu_base *nu_mod(nu_base *l, nu_base *r)
{
	return _nu_mod_ptr[l->type](l, r);
}

#include "nu.h"
#include <assert.h>

nu_bool *nu_new_bool(bool v)
{
	nu_bool *r = NU_NEW(nu_bool);
	assert(r != NULL);
	r->type = NU_BOOL_T;
	r->refs = 0u;
	r->data = v;
	return r;
}

bool _nu_none_to_bool(nu_base *o)
{
	return 0;
}
bool _nu_bool_to_bool(nu_bool *o)
{
	return o->data;
}
bool _nu_num_to_bool(nu_num *o)
{
	return o->data != 0.0;
}
bool _nu_str_to_bool(nu_base *o)
{
	return 0;
}
bool _nu_fn_to_bool(nu_base *o)
{
	return 0;
}
bool _nu_arr_to_bool(nu_base *o)
{
	return 0;
}
bool _nu_obj_to_bool(nu_base *o)
{
	return 0;
}
bool _nu_thr_to_bool(nu_base *o)
{
	return 0;
}
bool (*_nu_to_bool_ptr[8])(nu_base *o) = {
	(NU_FPTR(bool, nu_base *))_nu_none_to_bool,
	(NU_FPTR(bool, nu_base *))_nu_bool_to_bool,
	(NU_FPTR(bool, nu_base *))_nu_num_to_bool,
	(NU_FPTR(bool, nu_base *))_nu_str_to_bool,
	(NU_FPTR(bool, nu_base *))_nu_fn_to_bool,
	(NU_FPTR(bool, nu_base *))_nu_arr_to_bool,
	(NU_FPTR(bool, nu_base *))_nu_obj_to_bool,
	(NU_FPTR(bool, nu_base *))_nu_thr_to_bool };

bool nu_to_bool(nu_base *b)
{
	return _nu_to_bool_ptr[b->type](b);
}
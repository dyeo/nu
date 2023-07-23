#include "nu.h"
#include <assert.h>

// --------------------------------------------------------------------------------------------------------------------------------

nu_bool *nu_bool_new(bool v)
{
	nu_bool *r = nu_malloc(nu_bool);
	assert(r != NULL);
	NU_BOOL_INIT(r, v);
	return r;
}

void nu_bool_free(nu_bool *o)
{
	o->type = NU_NONE_T;
	o->refs = 0;
	o->data = 0;
	free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_FPTR_NEW(nu_to_bool_fptr, bool, nu_val *);

bool _nu_none_bool_get(nu_val *o)
{
	return 0;
}
bool _nu_bool_bool_get(nu_bool *o)
{
	return o->data;
}
bool _nu_num_bool_get(nu_num *o)
{
	return o->data != 0.0;
}
bool _nu_str_bool_get(nu_str *o)
{
	return o->len > 0;
}
bool _nu_fn_bool_get(nu_fn *o)
{
	return 0;
}
bool _nu_arr_bool_get(nu_arr *o)
{
	return arrlen(o->data) > 0;
}
bool _nu_obj_bool_get(nu_obj *o)
{
	return hmlen(o->data) > 0;
}
bool _nu_thr_bool_get(nu_thr *o)
{
	return 0;
}

bool (*_nu_bool_get_ptr[8])(nu_val *o) = {
	(nu_to_bool_fptr)_nu_none_bool_get,
	(nu_to_bool_fptr)_nu_bool_bool_get,
	(nu_to_bool_fptr)_nu_num_bool_get,
	(nu_to_bool_fptr)_nu_str_bool_get,
	(nu_to_bool_fptr)_nu_fn_bool_get,
	(nu_to_bool_fptr)_nu_arr_bool_get,
	(nu_to_bool_fptr)_nu_obj_bool_get,
	(nu_to_bool_fptr)_nu_thr_bool_get
};

bool nu_bool_get_c(nu_val *b)
{
	return _nu_bool_get_ptr[b->type](b);
}

nu_bool *nu_bool_get(nu_val *b)
{
	return nu_literal_bool[_nu_bool_get_ptr[b->type](b)];
}

// --------------------------------------------------------------------------------------------------------------------------------

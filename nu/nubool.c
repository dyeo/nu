#include "nu.h"
#include <assert.h>

// --------------------------------------------------------------------------------------------------------------------------------

nu_bool *nu_bool_new(bool v)
{
	nu_bool *r = nu_malloc(nu_bool);
	assert(r != NULL);
	r->type = NU_BOOL_T;
	r->refs = 0u;
	r->data = v;
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

NU_DEF_FPTR(nu_to_bool_fptr, bool, nu_val *);

bool _nu_none_to_bool(nu_val *o)
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
bool _nu_str_to_bool(nu_str *o)
{
	return o->len > 0;
}
bool _nu_fn_to_bool(nu_fn *o)
{
	return 0;
}
bool _nu_arr_to_bool(nu_arr *o)
{
	return o->len > 0;
}
bool _nu_obj_to_bool(nu_obj *o)
{
	return o->len > 0;
}
bool _nu_thr_to_bool(nu_thr *o)
{
	return 0;
}

bool (*_nu_to_bool_ptr[8])(nu_val *o) = {
	(nu_to_bool_fptr)_nu_none_to_bool,
	(nu_to_bool_fptr)_nu_bool_to_bool,
	(nu_to_bool_fptr)_nu_num_to_bool,
	(nu_to_bool_fptr)_nu_str_to_bool,
	(nu_to_bool_fptr)_nu_fn_to_bool,
	(nu_to_bool_fptr)_nu_arr_to_bool,
	(nu_to_bool_fptr)_nu_obj_to_bool,
	(nu_to_bool_fptr)_nu_thr_to_bool
};

bool nu_to_bool_c(nu_val *b)
{
	return _nu_to_bool_ptr[b->type](b);
}

nu_bool *nu_to_bool(nu_val *b)
{
	return nu_bool_literal[_nu_to_bool_ptr[b->type](b)];
}

// --------------------------------------------------------------------------------------------------------------------------------

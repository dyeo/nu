#include "nu.h"
#include <assert.h>

// --------------------------------------------------------------------------------------------------------------------------------

nu_bool *nu_new_bool(bool v)
{
	nu_bool *r = nu_malloc(nu_bool);
	assert(r != NULL);
	r->type = NU_T_BOOL;
	r->refs = 0u;
	r->data = v;
	return r;
}

void nu_free_bool(nu_bool *o)
{
	o->type = NU_T_NONE;
	o->refs = 0;
	o->data = 0;
	free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_NEW_FPTR(nu_to_bool_fptr, bool, nu_val *);

bool _nu_to_bool_none(nu_val *o)
{
	return 0;
}
bool _nu_to_bool_bool(nu_bool *o)
{
	return o->data;
}
bool _nu_to_bool_num(nu_num *o)
{
	return o->data != 0.0;
}
bool _nu_to_bool_str(nu_str *o)
{
	return o->len > 0;
}
bool _nu_to_bool_fn(nu_fn *o)
{
	return 0;
}
bool _nu_to_bool_arr(nu_arr *o)
{
	return o->len > 0;
}
bool _nu_to_bool_obj(nu_obj *o)
{
	return o->len > 0;
}
bool _nu_to_bool_thr(nu_thr *o)
{
	return 0;
}

bool (*_nu_to_bool_ptr[8])(nu_val *o) = {
	(nu_to_bool_fptr)_nu_to_bool_none,
	(nu_to_bool_fptr)_nu_to_bool_bool,
	(nu_to_bool_fptr)_nu_to_bool_num,
	(nu_to_bool_fptr)_nu_to_bool_str,
	(nu_to_bool_fptr)_nu_to_bool_fn,
	(nu_to_bool_fptr)_nu_to_bool_arr,
	(nu_to_bool_fptr)_nu_to_bool_obj,
	(nu_to_bool_fptr)_nu_to_bool_thr
};

bool nu_to_bool_c(nu_val *b)
{
	return _nu_to_bool_ptr[b->type](b);
}

nu_bool *nu_to_bool(nu_val *b)
{
	return nu_literal_bool[_nu_to_bool_ptr[b->type](b)];
}

// --------------------------------------------------------------------------------------------------------------------------------

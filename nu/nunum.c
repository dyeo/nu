#include "nu.h"
#include <math.h>

// --------------------------------------------------------------------------------------------------------------------------------

nu_num *nu_new_num(num_t v)
{
	nu_num *r = nu_malloc(nu_num);
    NU_ASSERT(r != NULL, "heap allocation error");
	r->type = NU_T_NUM;
	r->refs = 0u;
	r->data = v;
	return r;
}

void nu_free_num(nu_num *o)
{
	o->type = NU_T_NONE;
	o->refs = 0;
	o->data = 0;
	free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_NEW_FPTR(nu_to_num_t_fptr, num_t, nu_val *);

num_t _nu_to_num_t_none(nu_val *o)
{
	return 0.0;
}

num_t _nu_to_num_t_bool(nu_bool *o)
{
	return (o->data != 0);
}

num_t _nu_to_num_t_num(nu_num *o)
{
	return o->data;
}

num_t _nu_to_num_t_str(nu_val *o)
{
	return 0.0;
}

num_t _nu_to_num_t_fn(nu_val *o)
{
	return 0.0;
}

num_t _nu_to_num_t_arr(nu_val *o)
{
	return 0.0;
}

num_t _nu_to_num_t_obj(nu_val *o)
{
	return 0.0;
}

num_t _nu_to_num_t_thr(nu_val *o)
{
	return 0.0;
}

const nu_to_num_t_fptr _nu_to_num_t_ptr[8] = {
	(nu_to_num_t_fptr)_nu_to_num_t_none,
	(nu_to_num_t_fptr)_nu_to_num_t_bool,
	(nu_to_num_t_fptr)_nu_to_num_t_num,
	(nu_to_num_t_fptr)_nu_to_num_t_str,
	(nu_to_num_t_fptr)_nu_to_num_t_fn,
	(nu_to_num_t_fptr)_nu_to_num_t_arr,
	(nu_to_num_t_fptr)_nu_to_num_t_obj,
	(nu_to_num_t_fptr)_nu_to_num_t_thr
};

// --------------------------------------------------------------------------------------------------------------------------------

uint8_t nu_c_to_uint8(nu_val *v) { return (uint8_t)_nu_to_num_t_ptr[v->type](v); }
int8_t nu_c_to_int8(nu_val *v) { return (int8_t)_nu_to_num_t_ptr[v->type](v); }
uint16_t nu_c_to_uint16(nu_val *v) { return (uint16_t)_nu_to_num_t_ptr[v->type](v); }
int16_t nu_c_to_int16(nu_val *v) { return (int16_t)_nu_to_num_t_ptr[v->type](v); }
uint32_t nu_c_to_uint32(nu_val *v) { return (uint32_t)_nu_to_num_t_ptr[v->type](v); }
int32_t nu_c_to_int32(nu_val *v) { return (int32_t)_nu_to_num_t_ptr[v->type](v); }
uint64_t nu_c_to_uint64(nu_val *v) { return (uint64_t)_nu_to_num_t_ptr[v->type](v); }
int64_t nu_c_to_int64(nu_val *v) { return (int64_t)_nu_to_num_t_ptr[v->type](v); }
size_t nu_c_to_size_t(nu_val *v) { return (size_t)_nu_to_num_t_ptr[v->type](v); }
float nu_c_to_float(nu_val * v) { return (float)_nu_to_num_t_ptr[v->type](v); }
double nu_c_to_double(nu_val * v) { return (double)_nu_to_num_t_ptr[v->type](v); }

nu_num *nu_to_num(nu_val *v) { return nu_new_num(nu_c_to_double(v)); }

// --------------------------------------------------------------------------------------------------------------------------------

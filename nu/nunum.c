#include "nu.h"
#include <math.h>

// --------------------------------------------------------------------------------------------------------------------------------

nu_num *nu_num_new(num_t v)
{
	nu_num *r = nu_malloc(nu_num);
    NU_ASSERT(r != NULL, "heap allocation error");
	NU_NUM_INIT(r, v);
	return r;
}

void nu_num_free(nu_num *o)
{
	o->type = NU_NONE_T;
	o->refs = 0;
	o->data = 0;
	free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

NU_FPTR_NEW(nu_to_num_t_fptr, num_t, nu_val *);

num_t _nu_none_t_get(nu_val *o)
{
	return 0.0;
}

num_t _nu_bool_t_get(nu_bool *o)
{
	return (o->data != 0);
}

num_t _nu_num_t_get(nu_num *o)
{
	return o->data;
}

num_t _nu_str_t_get(nu_val *o)
{
	return 0.0;
}

num_t _nu_fn_t_get(nu_val *o)
{
	return 0.0;
}

num_t _nu_arr_t_get(nu_val *o)
{
	return 0.0;
}

num_t _nu_obj_t_get(nu_val *o)
{
	return 0.0;
}

num_t _nu_thr_t_get(nu_val *o)
{
	return 0.0;
}

const nu_to_num_t_fptr _nu_num_t_get_ptr[8] = {
	(nu_to_num_t_fptr)_nu_none_t_get,
	(nu_to_num_t_fptr)_nu_bool_t_get,
	(nu_to_num_t_fptr)_nu_num_t_get,
	(nu_to_num_t_fptr)_nu_str_t_get,
	(nu_to_num_t_fptr)_nu_fn_t_get,
	(nu_to_num_t_fptr)_nu_arr_t_get,
	(nu_to_num_t_fptr)_nu_obj_t_get,
	(nu_to_num_t_fptr)_nu_thr_t_get
};

// --------------------------------------------------------------------------------------------------------------------------------

uint8_t nu_uint8_get_c(nu_val *v) { return (uint8_t)_nu_num_t_get_ptr[v->type](v); }
int8_t nu_int8_get_c(nu_val *v) { return (int8_t)_nu_num_t_get_ptr[v->type](v); }
uint16_t nu_uint16_get_c(nu_val *v) { return (uint16_t)_nu_num_t_get_ptr[v->type](v); }
int16_t nu_int16_get_c(nu_val *v) { return (int16_t)_nu_num_t_get_ptr[v->type](v); }
uint32_t nu_uint32_get_c(nu_val *v) { return (uint32_t)_nu_num_t_get_ptr[v->type](v); }
int32_t nu_int32_get_c(nu_val *v) { return (int32_t)_nu_num_t_get_ptr[v->type](v); }
uint64_t nu_uint64_get_c(nu_val *v) { return (uint64_t)_nu_num_t_get_ptr[v->type](v); }
int64_t nu_int64_get_c(nu_val *v) { return (int64_t)_nu_num_t_get_ptr[v->type](v); }
size_t nu_size_t_get_c(nu_val *v) { return (size_t)_nu_num_t_get_ptr[v->type](v); }
float nu_float_get_c(nu_val * v) { return (float)_nu_num_t_get_ptr[v->type](v); }
double nu_double_get_c(nu_val * v) { return (double)_nu_num_t_get_ptr[v->type](v); }

nu_num *nu_num_get(nu_val *v) { return nu_num_new(nu_double_get_c(v)); }

// --------------------------------------------------------------------------------------------------------------------------------

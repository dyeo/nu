#include "nu.h"
#include <math.h>

// --------------------------------------------------------------------------------------------------------------------------------

nu_num *nu_num_new(num_t v)
{
	nu_num *r = NU_NEW(nu_num);
    NU_ASSERT(r != NULL, "heap allocation error");
	r->type = NU_NUM_T;
	r->refs = 0u;
	r->data = v;
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

NU_DEF_FPTR(nu_to_num_t_fptr, num_t, nu_val *);

num_t _nu_none_to_num_t(nu_val *o)
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

num_t _nu_str_to_num_t(nu_val *o)
{
	return 0.0;
}

num_t _nu_fn_to_num_t(nu_val *o)
{
	return 0.0;
}

num_t _nu_arr_to_num_t(nu_val *o)
{
	return 0.0;
}

num_t _nu_obj_to_num_t(nu_val *o)
{
	return 0.0;
}

num_t _nu_thr_to_num_t(nu_val *o)
{
	return 0.0;
}

const nu_to_num_t_fptr _nu_to_num_t_ptr[8] = {
	(nu_to_num_t_fptr)_nu_none_to_num_t,
	(nu_to_num_t_fptr)_nu_bool_to_num_t,
	(nu_to_num_t_fptr)_nu_num_to_num_t,
	(nu_to_num_t_fptr)_nu_str_to_num_t,
	(nu_to_num_t_fptr)_nu_fn_to_num_t,
	(nu_to_num_t_fptr)_nu_arr_to_num_t,
	(nu_to_num_t_fptr)_nu_obj_to_num_t,
	(nu_to_num_t_fptr)_nu_thr_to_num_t
};

// --------------------------------------------------------------------------------------------------------------------------------

uint8_t nu_to_uint8(nu_val *v) { return (uint8_t)_nu_to_num_t_ptr[v->type](v); }
int8_t nu_to_int8(nu_val *v) { return (int8_t)_nu_to_num_t_ptr[v->type](v); }
uint16_t nu_to_uint16(nu_val *v) { return (uint16_t)_nu_to_num_t_ptr[v->type](v); }
int16_t nu_to_int16(nu_val *v) { return (int16_t)_nu_to_num_t_ptr[v->type](v); }
uint32_t nu_to_uint32(nu_val *v) { return (uint32_t)_nu_to_num_t_ptr[v->type](v); }
int32_t nu_to_int32(nu_val *v) { return (int32_t)_nu_to_num_t_ptr[v->type](v); }
uint64_t nu_to_uint64(nu_val *v) { return (uint64_t)_nu_to_num_t_ptr[v->type](v); }
int64_t nu_to_int64(nu_val *v) { return (int64_t)_nu_to_num_t_ptr[v->type](v); }
size_t nu_to_size_t(nu_val *v) { return (size_t)_nu_to_num_t_ptr[v->type](v); }
float nu_to_float(nu_val * v) { return (float)_nu_to_num_t_ptr[v->type](v); }
double nu_to_double(nu_val * v) { return (double)_nu_to_num_t_ptr[v->type](v); }

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_add(nu_bool *l, nu_bool *r)
{
	return nu_bool_new((l->data + r->data) != 0);
}

nu_val *_nu_num_add(nu_num *l, nu_num *r)
{
	return nu_num_new(l->data + r->data);
}

nu_val *_nu_str_add(nu_val *l, nu_val *r)
{
	return NU_NONE;
}

nu_val *_nu_arr_add(nu_val *l, nu_val *r)
{
	return NU_NONE;
}

const nu_oper_t _nu_add_ptr[8] = {
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)_nu_bool_add,
	(nu_oper_t)_nu_num_add,
	(nu_oper_t)_nu_str_add,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)_nu_arr_add,
	(nu_oper_t)nu_oper_none,
	(nu_oper_t)nu_oper_none
};

nu_val *nu_add(nu_val *l, nu_val *r)
{
    if(r == NU_NONE)
    {
        return NU_NONE;
    }
	return _nu_add_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_sub(nu_bool *l, nu_bool *r)
{
	return nu_bool_new(l->data - r->data);
}

nu_val *_nu_num_sub(nu_num *l, nu_num *r)
{
	return nu_num_new(l->data - r->data);
}

nu_val *_nu_str_sub(nu_val *l, nu_val *r)
{
	return NU_NONE;
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

nu_val *nu_sub(nu_val *l, nu_val *r)
{
    if(r == NU_NONE)
    {
        return NU_NONE;
    }
	return _nu_sub_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_mul(nu_bool *l, nu_bool *r)
{
	return nu_bool_new(l->data * r->data);
}

nu_val *_nu_num_mul(nu_num *l, nu_num *r)
{
	return nu_num_new(l->data * r->data);
}

nu_val *_nu_str_mul(nu_val *l, nu_val *r)
{
	return NU_NONE;
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

nu_val *nu_mul(nu_val *l, nu_val *r)
{
    if(r == NU_NONE)
    {
        return NU_NONE;
    }
	return _nu_mul_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_none_div(nu_val *l, nu_val *r)
{
	return NU_NONE;
}

nu_val *_nu_bool_div(nu_bool *l, nu_bool *r)
{
	return nu_bool_new(l->data / r->data);
}

nu_val *_nu_num_div(nu_num *l, nu_num *r)
{
	return nu_num_new(l->data / r->data);
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

nu_val *nu_div(nu_val *l, nu_val *r)
{
    if(r == NU_NONE)
    {
        return NU_NONE;
    }
	return _nu_div_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_val *_nu_bool_mod(nu_bool *l, nu_bool *r)
{
	return nu_bool_new(l->data % r->data);
}

nu_val *_nu_num_mod(nu_num *l, nu_num *r)
{
	return nu_num_new(fmod(l->data, r->data));
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

nu_val *nu_mod(nu_val *l, nu_val *r)
{
    if(r == NU_NONE)
    {
        return NU_NONE;
    }
	return _nu_mod_ptr[l->type](l, r);
}

// --------------------------------------------------------------------------------------------------------------------------------

#include "nu.h"
#include "utils.h"

// --------------------------------------------------------------------------------------------------------------------------------

nu_str *_nu_str_new(const char *v, size_t c, size_t n)
{
    nu_str *r = nu_malloc(nu_str);
    NU_ASSERT(r != NULL, "heap allocation error");
    r->type = NU_STR_T;
    r->refs = 0u;
    r->cap = c;
    r->len = n;
    r->data = v;
    return r;
}

nu_str *nu_str_new(const char *v)
{
    size_t c, n;
    utfdlen(v, &c, &n);
    c++;
    return _nu_str_new(strncpy(nu_calloc(char, c), v, c), c, n);
}

void nu_str_free(nu_str *o)
{
	o->type = NU_NONE_T;
	o->refs = 0;
    o->len = 0;
    o->cap = NULL;
    free(o->data);
    o->data = NULL;
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_str *nu_str_get_val(nu_str *str, nu_num *idx)
{
    if(str == NU_NONE || idx == NU_NONE) return NU_NONE;
    size_t i = nu_to_size_t(idx);
    if(i > str->len) return NU_NONE;
    const char *c = str;
    size_t n = 0;
    for(;i-->0;)
    {
        n = cplen(*c);
        c += n;
    }
    return _nu_str_new(c, n, 1);
}

// --------------------------------------------------------------------------------------------------------------------------------

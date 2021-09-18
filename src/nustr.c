#include "nu.h"
#include "utils.h"

// --------------------------------------------------------------------------------------------------------------------------------

nu_str *nu_str_new(const char *v)
{
    nu_str *r = NU_NEW(nu_str);
    NU_ASSERT(r != NULL, "heap allocation error");
    r->type = NU_STR_T;
    utfdlen(v, &(r->cap), &(r->len));
    r->data = strncpy(NU_ANEW(char, r->cap), v, r->cap);
    return r;
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
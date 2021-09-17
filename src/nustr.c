#include "nu.h"
#include "utils.h"

// --------------------------------------------------------------------------------------------------------------------------------

nu_str *nu_new_str(const char *v)
{
    nu_str *r = NU_NEW(nu_str);
    NU_ASSERT(r != NULL, "heap allocation error");
    r->type = NU_STR_T;
    size_t n = strlen(v);
    r->len = n;
    r->data = strncpy(NU_ANEW(char, n), v, n);
    return r;
}

void nu_free_str(nu_str *o)
{
	o->type = NU_NONE_T;
	o->refs = 0;
    o->len = 0;
    free(o->data);
    o->data = NULL;
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------
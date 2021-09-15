#include "nu.h"

#include <assert.h>

nu_str *nu_new_str(const char *v)
{
    nu_str *r = NU_NEW(nu_str);
    assert(r != NULL);
    r->type = NU_STR_T;
    r->data = v;
    return r;
}
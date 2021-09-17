#include "nu.h"

// --------------------------------------------------------------------------------------------------------------------------------

nu_arr *nu_new_arr(nu_val **vals, size_t len)
{
    nu_arr *r = NU_NEW(nu_arr);
    NU_ASSERT(r != NULL, "heap allocation error");
    r->type = NU_OBJ_T;
    r->len = len;
    r->data = NU_ANEW(nu_val *, len);
    return r;
}

void nu_free_arr(nu_arr *o)
{
    o->type = NU_NONE_T;
    o->len = 0;
    free(o->data);
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------------------
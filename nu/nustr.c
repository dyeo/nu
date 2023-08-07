#include "nu.h"
#include "utils.h"

// --------------------------------------------------------------------------------------------------------------------------------

nu_str *nu_str_new(str_t str)
{
    nu_str *s = nu_malloc(nu_str);
    NU_ASSERT(s != NULL, "heap allocation error");
    NU_STR_INIT(s, lstr(str));
    return s;
}

void nu_str_free(nu_str *o)
{
    o->type = NU_NONE_T;
    o->refs = 0u;
    free(o->data);
    o->data = NULL;
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_str *nu_str_get_val(nu_str *str, nu_num *idx)
{
    if (str == NU_NONE || idx == NU_NONE)
    {
        return NU_NONE;
    }

    size_t i = nu_size_t_get_c(idx);
    if (i > arrlen(str->data))
    {
        return NU_NONE;
    }

    nu_str *res = nu_malloc(nu_str);
    NU_STR_INIT(res, lstrsub(res->data, i, 1));
    return res;
}

// --------------------------------------------------------------------------------------------------------------------------------

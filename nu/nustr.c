#include "nu.h"
#include "utils.h"

// --------------------------------------------------------------------------------------------------------------------------------

nu_str *_nu_str_new(str_t str, size_t blen, size_t clen)
{
    nu_str *r = nu_malloc(nu_str);
    NU_ASSERT(r != NULL, "heap allocation error");
    NU_STR_INIT(r, blen, clen, str);
    return r;
}

nu_str *nu_str_new(str_t str)
{
    size_t blen, clen;
    utfdlen(str, &blen, &clen);
    char *dest = nu_calloc(++blen, char);
    dest[blen - 1] = NULL;
    nu_copy(dest, str, blen - 1, char);
    return _nu_str_new(dest, blen, clen);
}

void nu_str_free(nu_str *o)
{
    o->type = NU_NONE_T;
    o->refs = 0u;
    o->len = 0u;
    o->cap = 0u;
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
    if (i > str->len)
    {
        return NU_NONE;
    }

    str_t c = str->data;
    size_t n = 0;
    for (; i-- > 0;)
    {
        n = cplen(*c);
        c += n;
    }
    return _nu_str_new(c, n, 1);
}

// --------------------------------------------------------------------------------------------------------------------------------

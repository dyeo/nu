#include "nu.h"
#include "utils.h"

// --------------------------------------------------------------------------------------------------------------------------------

nu_str *_nu_str_new(char *str, size_t blen, size_t clen)
{
    nu_str *r = nu_malloc(nu_str);
    NU_ASSERT(r != NULL, "heap allocation error");
    r->type = NU_STR_T;
    r->refs = 0u;
    r->cap = blen;
    r->len = clen;
    r->data = str;
    return r;
}

nu_str *nu_str_new(const char *str)
{
    size_t blen, clen;
    utfdlen(str, &blen, &clen);
    blen++;
    char *dest = nu_calloc(char, blen);
    dest[blen - 1] = NULL;
    strncpy(dest, str, blen - 1);
    return _nu_str_new(dest, blen, clen);
}

void nu_str_free(nu_str *o)
{
    o->type = NU_NONE_T;
    o->refs = 0u;
    o->len = 0u;
    o->cap = NULL;
    free(o->data);
    o->data = NULL;
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_str *nu_str_get_val(nu_str *str, nu_num *idx)
{
    if (str == NU_NONE || idx == NU_NONE)
        return NU_NONE;
    size_t i = nu_to_size_t(idx);
    if (i > str->len)
        return NU_NONE;
    const char *c = str;
    size_t n = 0;
    for (; i-- > 0;)
    {
        n = cplen(*c);
        c += n;
    }
    return _nu_str_new(c, n, 1);
}

// --------------------------------------------------------------------------------------------------------------------------------

#include "nu.h"
#include "utils.h"

// --------------------------------------------------------------------------------------------------------------------------------

nu_str *_nu_new_str(char *str, size_t blen, size_t clen)
{
    nu_str *r = nu_malloc(nu_str);
    NU_ASSERT(r != NULL, "heap allocation error");
    r->type = NU_T_STR;
    r->refs = 0u;
    r->cap = blen;
    r->len = clen;
    r->data = str;
    return r;
}

nu_str *nu_new_str(const char *str)
{
    size_t blen, clen;
    utfdlen(str, &blen, &clen);
    char *dest = nu_calloc(char, ++blen);
    dest[blen - 1] = NULL;
    strncpy(dest, str, blen - 1);
    return _nu_new_str(dest, blen, clen);
}

void nu_free_str(nu_str *o)
{
    o->type = NU_T_NONE;
    o->refs = 0u;
    o->len = 0u;
    o->cap = NULL;
    free(o->data);
    o->data = NULL;
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

nu_str *nu_get_val_str(nu_str *str, nu_num *idx)
{
    if (str == NU_NONE || idx == NU_NONE)
        return NU_NONE;
    size_t i = nu_c_to_size_t(idx);
    if (i > str->len)
        return NU_NONE;
    char *c = str->data;
    size_t n = 0;
    for (; i-- > 0;)
    {
        n = cplen(*c);
        c += n;
    }
    return _nu_new_str(c, n, 1);
}

// --------------------------------------------------------------------------------------------------------------------------------

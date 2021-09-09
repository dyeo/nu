#include "nu.h"

#include <assert.h>
#include <stdio.h>
#include <float.h>

const nu_base nu_none = { NU_NONE_T, 0ul };
const nu_bool nu_true = { NU_BOOL_T, 0ul, true };
const nu_bool nu_false = { NU_BOOL_T, 0ul, false };
const nu_num nu_zero = { NU_NUM_T, 0ul, 0.0 };
const nu_num nu_one = { NU_NUM_T, 0ul, 1.0 };
const nu_str nu_empty = { NU_STR_T, 0ul, "" };

bool nu_initialize()
{}

bool nu_finalize()
{}

const char *nu_repr(nu_base *o)
{
    assert(o != NULL);
    if (nu_is_none(o))
    {
        return "none";
    }
    if (nu_is_bool(o))
    {
        return ((nu_bool *)o)->data ? "true" : "false";
    }
    if (nu_is_num(o))
    {
        const int size = 3 + DBL_MANT_DIG - DBL_MIN_EXP;
        const char *buf = NU_ANEW(const char, size);
        snprintf(buf, size, "%f", ((nu_num *)o)->data);
        return buf;
    }
}

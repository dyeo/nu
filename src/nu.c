
#include "nu.h"

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
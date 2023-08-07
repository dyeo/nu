#include "../nu.h"

#include "test.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    test_group(nu_val, {
        nu_val *v = nu_str_new("hello");
        test_expr((v != NULL && v != NU_NONE), bool, true);
    });
    return 0;
}
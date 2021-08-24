#include "tests.h"
#include "../src/nu.h"

#include "stdio.h"

int main(int argc, char **argv)
{
    nu_num *a = nu_new_num(25);
    nu_num *b = nu_new_num(25);
    nu_bool *c = nu_le(a, b);
    printf("%i", nu_to_bool(c));
}
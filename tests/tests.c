#include "tests.h"
#include "../src/nu.h"
#include "../src/nurbt.h"

#include <stdio.h>

void rb_print(rb_node *curr)
{
    printf("%llu: %p\n", curr->key, curr->val);
}

int main(int argc, char **argv)
{
    nu_base *o = nu_new_num(42);
    printf("%s\n\n", nu_repr(o));

    rb_tree *a = rb_new_tree();
    for (int i = 0; i < 32; ++i)
    {
        rb_insert(a, rb_new_node(i, 0));
    }

    rb_in_order(a, rb_print);

    printf("\n");

    for (int i = 0; i < 32; ++i)
    {
        rb_delete(a, rb_search(a, i));
    }

    rb_in_order(a, rb_print);
}
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
    nu_num *n = nu_new_num(128);
    nu_str *s = nu_new_str("hello world!");
    printf("128 : %llu\n", nu_to_size_t(nu_hash(n)));
    printf("%s : %llu\n", s->data, nu_to_size_t(nu_hash(s)));

    nu_obj *o = nu_new_obj(NULL, NULL, 0);
    
    nu_set_val(o, n, s);
    nu_val *t = nu_get_val(o, n);
    const char *r = nu_repr(t);
    printf("%s\n", r);
    free(r);

    printf("%s : %llu\n", s->data, nu_to_size_t(nu_hash(s)));
    nu_free(s);
    printf("%i", o == &(o->base));


    NU_ASSERT(false, "this failed successfully");

    //nu_val *o = nu_new_num(42);
    //printf("%s\n\n", nu_repr(o));
    //
    //rb_tree *a = rb_new_tree();
    //for (int i = 0; i < 32; ++i)
    //{
    //    rb_insert(a, rb_new_node(i, 0));
    //}
    //
    //rb_in_order(a, rb_print);
    //
    //printf("\n");
    //
    //for (int i = 0; i < 32; ++i)
    //{
    //    rb_delete(a, rb_search(a, i));
    //}
    //
    //rb_in_order(a, rb_print);
}
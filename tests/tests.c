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
    printf("%lu\n", sizeof(nu_val *));
    nu_num *n = nu_num_new(128);
    nu_str *s = nu_str_new("hello world!");
    printf("128 : %llu\n", nu_to_size_t(nu_hash(n)));
    printf("%s : %llu\n", s->data, nu_to_size_t(nu_hash(s)));

    nu_obj *o = nu_obj_new(NULL, NULL, 0);
    
    nu_obj_set_val(o, n, s);
    nu_val *t = nu_obj_get_val(o, n);
    const char *r = nu_repr(t);
    printf("%s\n", r);
    free(r);

    printf("%s : %llu\n", s->data, nu_to_size_t(nu_hash(s)));
    nu_free(s);

    size_t SIZE = 100;
    printf("max: %lu\n", SIZE);
    nu_arr *arr = nu_arr_new(SIZE);
    for(int i = 0; i < SIZE; ++i)
    {
        nu_num *val = nu_num_new((num_t)i);
        nu_arr_push_val(arr, val);
    }
    printf("len: %lu\n", nu_to_size_t(nu_len(arr)));
    printf("cap: %lu\n", nu_to_size_t(nu_cap(arr)));
    if(nu_arr_del_val_i(arr, 53) != NU_NONE)
    {
        SIZE--;
    }
    printf("AAAA\n");
    for(int i = 0; i < SIZE; ++i)
    {
        nu_val *val = nu_arr_get_val_i(arr, i);
        printf("val: %lu\n", nu_to_size_t(val));
    }

    NU_ASSERT(false, "failed successfully");

    //nu_val *o = nu_num_new(42);
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
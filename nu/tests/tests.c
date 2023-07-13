#include "tests.h"
#include "../nu.h"
#include "../nurbt.h"

#include <stdio.h>

void rb_print(rb_node *curr)
{
    printf("%llu: %p\n", curr->key, curr->val);
}

int main(int argc, char **argv)
{
    // printf("%lu\n", sizeof(nu_val *));
    // nu_num *n = nu_num_new(128);
    // nu_str *s = nu_str_new("hello world!");
    // printf("128 : %llu\n", nu_to_size_t(nu_hash(n)));
    // printf("%s : %llu\n", s->data, nu_to_size_t(nu_hash(s)));
    //
    // nu_obj *o = nu_obj_new(NULL, NULL, 0);
    //
    // nu_obj_set_val(o, n, s);
    // nu_val *t = nu_obj_get_val(o, n);
    // const char *r = nu_repr(t);
    // printf("%s\n", r);
    // free(r);
    // nu_num *_hash = nu_hash(s);
    // size_t _hash_i = nu_to_size_t(_hash);
    //
    // printf("%s : %llu\n", s->data, _hash_i);
    // nu_free(s);

    size_t SIZE = 25;
    printf("max: %lu\n", SIZE);
    nu_arr *arr = nu_arr_new(SIZE);
    for (int i = 0; i < SIZE; ++i)
    {
        nu_arr_push_val(arr, nu_num_new((num_t)i));
        nu_arr_push_val(arr, nu_str_new("e"));
        if (i == SIZE / 2)
        {

            nu_arr *arr2 = nu_arr_new(SIZE / 2);
            for (int i = 0; i < SIZE / 2; ++i)
            {
                nu_num *val2 = nu_num_new((num_t)i);
                nu_arr_push_val(arr2, val2);
            }
            nu_arr_push_val(arr, arr2);
        }
    }
    printf("%s\n", nu_repr(arr));
    nu_free(arr);
    // printf("len: %lu\n", nu_to_size_t(nu_len(arr)));
    // printf("cap: %lu\n", nu_to_size_t(nu_cap(arr)));
    // if(nu_arr_del_val_i(arr, 53) != NU_NONE)
    //{
    //     SIZE--;
    // }
    // printf("AAAA\n");
    // for(int i = 0; i < SIZE; ++i)
    //{
    //     nu_val *val = nu_arr_get_val_i(arr, i);
    //     printf("val: %lu\n", nu_to_size_t(val));
    // }
}
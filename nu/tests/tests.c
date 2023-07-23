#include "tests.h"
#include "../nu.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    // printf("%lu\n", sizeof(nu_val *));
    // nu_num *n = nu_new_num(128);
    // nu_str *s = nu_new_str("hello world!");
    // printf("128 : %llu\n", nu_to_size_t(nu_hash(n)));
    // printf("%s : %llu\n", s->data, nu_to_size_t(nu_hash(s)));
    //
    // nu_obj *o = nu_new_obj(NULL, NULL, 0);
    //
    // nu_set_val_obj(o, n, s);
    // nu_val *t = nu_get_val_obj(o, n);
    // str_t r = nu_repr_c(t);
    // printf("%s\n", r);
    // free(r);
    // nu_num *_hash = nu_hash(s);
    // size_t _hash_i = nu_to_size_t(_hash);
    //
    // printf("%s : %llu\n", s->data, _hash_i);
    // nu_free(s);

    size_t SIZE = 32;
    printf("max: %lu\n", SIZE);
    nu_arr *arr = nu_arr_new(SIZE);
    for (int i = 0; i < SIZE; ++i)
    {
       nu_arr_push_val(arr, nu_num_new((num_t)i));
       nu_str *v = nu_str_new("e");
       nu_arr_push_val(arr, v);
       if (i == SIZE / 2)
       {
           nu_arr *arr2 = nu_arr_new(SIZE / 2);
           for (int i = 0; i < SIZE / 2; ++i)
           {
               nu_num *val2 = nu_num_new((num_t)i);
               nu_arr_push_val(arr2, val2);
               nu_arr_push_val(arr2, nu_str_new("e"));
           }
           nu_arr_push_val(arr, arr2);
       }
    }
    printf("%s\n", nu_repr_c(arr));
    printf("\n");
    nu_arr_clear(arr);
    printf("%s\n", nu_repr_c(arr));
    nu_free(arr);

    nu_obj *o = nu_obj_new();
    nu_obj_add_val(o, nu_str_new("a"), nu_num_new(1));
    nu_obj_add_val(o, nu_str_new("b"), nu_num_new(2));
    nu_obj_add_val(o, nu_str_new("c"), NU_NONE);
    nu_obj_add_val(o, nu_str_new("d"), nu_num_new(4));
    nu_arr *keys = nu_obj_vals(o);
    printf("%s\n", nu_repr_c(keys));
    // printf("len: %lu\n", nu_to_size_t(nu_len(arr)));
    // printf("cap: %lu\n", nu_to_size_t(nu_cap(arr)));
    // if(nu_del_val_arr_c(arr, 53) != NU_NONE)
    //{
    //     SIZE--;
    // }
    // printf("AAAA\n");
    // for(int i = 0; i < SIZE; ++i)
    //{
    //     nu_val *val = nu_get_val_arr_c(arr, i);
    //     printf("val: %lu\n", nu_to_size_t(val));
    // }
}
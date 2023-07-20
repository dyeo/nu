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
    // nu_num *n = nu_new_num(128);
    // nu_str *s = nu_new_str("hello world!");
    // printf("128 : %llu\n", nu_to_size_t(nu_hash(n)));
    // printf("%s : %llu\n", s->data, nu_to_size_t(nu_hash(s)));
    //
    // nu_obj *o = nu_new_obj(NULL, NULL, 0);
    //
    // nu_set_val_obj(o, n, s);
    // nu_val *t = nu_get_val_obj(o, n);
    // str_t r = nu_c_repr(t);
    // printf("%s\n", r);
    // free(r);
    // nu_num *_hash = nu_hash(s);
    // size_t _hash_i = nu_to_size_t(_hash);
    //
    // printf("%s : %llu\n", s->data, _hash_i);
    // nu_free(s);

    printf(NU_SZE_FMT"\n", NU_REFS_MAX);

    size_t SIZE = 32;
    printf("max: %lu\n", SIZE);
    nu_arr *arr = nu_new_arr(SIZE);
    for (int i = 0; i < SIZE; ++i)
    {
        nu_push_val_arr(arr, nu_new_num((num_t)i));
        nu_push_val_arr(arr, nu_new_str("e"));
        if (i == SIZE / 2)
        {
            nu_arr *arr2 = nu_new_arr(SIZE / 2);
            for (int i = 0; i < SIZE / 2; ++i)
            {
                nu_num *val2 = nu_new_num((num_t)i);
                nu_push_val_arr(arr2, val2);
                nu_push_val_arr(arr2, nu_new_str("e"));
            }
            nu_push_val_arr(arr, arr2);
        }
    }
    printf("%s\n", nu_c_repr(arr));
    printf("\n");
    for (int i = arr->len; i > 0; i--)
    {
        nu_pop_val_arr(arr);
    }
    printf("%s\n", nu_c_repr(arr));
    nu_free(arr);
    // printf("len: %lu\n", nu_to_size_t(nu_len(arr)));
    // printf("cap: %lu\n", nu_to_size_t(nu_cap(arr)));
    // if(nu_c_del_val_arr(arr, 53) != NU_NONE)
    //{
    //     SIZE--;
    // }
    // printf("AAAA\n");
    // for(int i = 0; i < SIZE; ++i)
    //{
    //     nu_val *val = nu_c_get_val_arr(arr, i);
    //     printf("val: %lu\n", nu_to_size_t(val));
    // }
}
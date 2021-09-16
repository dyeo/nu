#include "nu.h"

#include <assert.h>

nu_obj *nu_new_obj(nu_obj **keys, nu_obj **vals, size_t len)
{
    nu_obj *r = NU_NEW(nu_obj);
    assert(r != NULL);
    r->type = NU_OBJ_T;
    r->data = rb_new_tree();
    if (keys != NULL && vals != NULL && len > 0)
    {
        for (size_t i = 0; i < len; ++i)
        {
            nu_set_val(r, keys[i], vals[i]);
        }
    }
    return r;
}

void nu_set_val(nu_obj *obj, nu_base *key, nu_base *val)
{
    if (obj == NULL || obj == NU_NONE) return;
    if (key == NULL || key == NU_NONE) return;
    nu_num *hash = nu_hash(key);
    size_t hashv = nu_to_size_t(hash);
    if (hashv == 0) return;
    rb_node *snode = rb_search(obj->data, hashv);
    if (snode != RB_NIL)
    {
        nu_opt_decref(obj);
        nu_opt_free(snode->val);
        if (val == NU_NONE)
        {
            snode = rb_delete(obj->data, snode);
            rb_free_node(snode);
        }
        else
        {
            snode->val = (void *)val;
        }
    }
    else if (val != NU_NONE)
    {
        snode = rb_new_node(hashv, (void*)val);
        nu_incref(val);
        rb_insert(obj->data, snode);
    }
}

nu_base *nu_get_val(nu_obj *obj, nu_base *key)
{
    if (obj == NULL || obj == NU_NONE) return NU_NONE;
    if (key == NULL || key == NU_NONE) return NU_NONE;
    nu_num *hash = nu_hash(key);
    size_t hashv = nu_to_size_t(hash);
    if (hashv == 0) return NU_NONE;
    rb_node *snode = rb_search(obj->data, hashv);
    if(snode != RB_NIL)
    {
        return (nu_base*)snode->val;
    }
    return NU_NONE;
}
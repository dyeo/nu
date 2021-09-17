#include "nu.h"

#include <assert.h>

nu_obj *nu_new_obj(nu_val **keys, nu_val **vals, size_t len)
{
    nu_obj *r = NU_NEW(nu_obj);
    NU_ASSERT(r != NULL, "heap allocation error");
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

void _nu_iter_free(rb_node *n)
{
    nu_opt_free((nu_val*)n->val);
}

void nu_free_obj(nu_obj *o)
{
	o->type = NU_NONE_T;
	o->refs = 0;
    rb_free_tree_iter(o->data, _nu_iter_free);
	o->data = NULL;
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

void nu_set_val(nu_obj *obj, nu_val *key, nu_val *val)
{
    if (obj == NU_NONE) return;
    if (key == NU_NONE) return;
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

nu_val *nu_get_val(nu_obj *obj, nu_val *key)
{
    if (obj == NU_NONE) return NU_NONE;
    if (key == NU_NONE) return NU_NONE;
    nu_num *hash = nu_hash(key);
    size_t hashv = nu_to_size_t(hash);
    if (hashv == 0) return NU_NONE;
    rb_node *snode = rb_search(obj->data, hashv);
    if(snode != RB_NIL)
    {
        return (nu_val*)snode->val;
    }
    return NU_NONE;
}

// --------------------------------------------------------------------------------------------------------------------------------
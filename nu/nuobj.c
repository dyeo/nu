#include "nu.h"

#include <assert.h>

nu_obj *nu_new_obj()
{
    nu_obj *r = nu_malloc(nu_obj);
    NU_ASSERT(r != NULL, "heap allocation error");
    r->type = NU_T_OBJ;
    r->refs = 0u;
    r->len = 0;
    r->data = rb_new_tree();
    return r;
}

void _nu_free_iter(rb_node *n)
{
    nu_free_opt((nu_val*)n->val);
}

void nu_free_obj(nu_obj *o)
{
	o->type = NU_T_NONE;
	o->refs = 0;
    rb_free_tree_iter(o->data, _nu_free_iter);
	o->data = NULL;
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

bool nu_set_val_obj(nu_obj *obj, nu_val *key, nu_val *val)
{
    if (obj == NU_NONE || key == NU_NONE) return false;
    size_t hashv = nu_c_to_size_t(nu_hash(key));
    if (hashv == 0) return false;
    rb_node *snode = rb_search(obj->data, hashv);
    if (snode != RB_NIL)
    {
        nu_val *old = (nu_val*)snode->val;
        nu_opt_decref(old);
        if (val == NU_NONE)
        {
            obj->len--;
            snode = rb_delete(obj->data, snode);
            rb_free_node(snode);
        }
        else
        {
            snode->val = (void *)val;
        }
        return true;
    }
    else if (val != NU_NONE)
    {
        obj->len++;
        snode = rb_new_node(hashv, (void*)val);
        nu_incref(val);
        rb_insert(obj->data, snode);
        return true;
    }
    return false;
}

nu_val *nu_get_val_obj(nu_obj *obj, nu_val *key)
{
    if (obj == NU_NONE || key == NU_NONE) return NU_NONE;
    size_t hashv = nu_c_to_size_t(nu_hash(key));
    if (hashv == 0) return NU_NONE;
    rb_node *snode = rb_search(obj->data, hashv);
    if(snode != RB_NIL)
    {
        return (nu_val*)snode->val;
    }
    return NU_NONE;
}

nu_val *nu_del_val_obj(nu_obj *obj, nu_val *key)
{
    if (obj == NU_NONE || key == NU_NONE) return false;
    size_t hashv = nu_c_to_size_t(nu_hash(key));
    if (hashv == 0) return false;
    rb_node *snode = rb_search(obj->data, hashv);
    if(snode != RB_NIL)
    {
        nu_val *val = (nu_val*)snode->val;
        nu_opt_decref(val);
        snode = rb_delete(obj->data, snode);
        rb_free_node(snode);
        return val;
    }
    return NU_NONE;
}

// --------------------------------------------------------------------------------------------------------------------------------
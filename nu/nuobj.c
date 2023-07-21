#include "nu.h"

#include <assert.h>

nu_obj *nu_obj_new()
{
    nu_obj *r = nu_malloc(nu_obj);
    NU_ASSERT(r != NULL, "heap allocation error");
    NU_OBJ_INIT(r, 0, rb_new_tree(), rb_new_tree());
    return r;
}

void _nu_free_iter(rb_node *n)
{
    nu_decref(n->val);
    nu_opt_free(n->val);
}

void nu_obj_free(nu_obj *o)
{
    o->type = NU_NONE_T;
    o->refs = 0;
    o->len = 0;
    rb_free_tree_iter(o->keys, _nu_free_iter);
    rb_free_tree_iter(o->vals, _nu_free_iter);
    o->keys = o->vals = NULL;
    free(o);
}

// --------------------------------------------------------------------------------------------------------------------------------

bool nu_obj_set_val(nu_obj *obj, nu_val *key, nu_val *val)
{
    if (obj == NU_NONE || key == NU_NONE)
    {
        return false;
    }

    size_t hashv = nu_size_t_get_c(nu_hash(key));
    if (hashv == 0)
    {
        return false;
    }

    rb_node *knode = rb_search(obj->keys, hashv);
    rb_node *vnode = rb_search(obj->vals, hashv);
    if (knode != RB_NIL && vnode != RB_NIL)
    {
        nu_val *old = (nu_val *)vnode->val;
        nu_decref(old);
        if (val == NU_NONE)
        {
            obj->len--;
            vnode = rb_delete(obj->vals, vnode);
            rb_free_node(vnode);
        }
        else
        {
            nu_incref(val);
            vnode->val = (void *)val;
        }
        return true;
    }
    obj->len++;
    knode = rb_new_node(hashv, (void *)key);
    vnode = rb_new_node(hashv, (void *)val);
    nu_incref(key);
    nu_incref(val);
    rb_insert(obj->keys, knode);
    rb_insert(obj->vals, vnode);
    return true;
}

nu_val *nu_obj_get_val(nu_obj *obj, const nu_val *key)
{
    if (obj == NU_NONE || key == NU_NONE)
    {
        return NU_NONE;
    }

    size_t hashv = nu_size_t_get_c(nu_hash(key));
    if (hashv == 0)
    {
        return NU_NONE;
    }

    rb_node *snode = rb_search(obj->vals, hashv);
    if (snode != RB_NIL)
    {
        return (nu_val *)snode->val;
    }

    return NU_NONE;
}

nu_val *nu_obj_del_val(nu_obj *obj, const nu_val *key)
{
    if (obj == NU_NONE || key == NU_NONE)
    {
        return NU_NONE;
    }

    size_t hashv = nu_size_t_get_c(nu_hash(key));
    if (hashv == 0)
    {
        return NU_NONE;
    }

    rb_node *knode = rb_search(obj->keys, hashv);
    rb_node *vnode = rb_search(obj->vals, hashv);
    if (knode != RB_NIL && vnode != RB_NIL)
    {
        nu_val *key0 = (nu_val *)knode->val;
        nu_val *val = (nu_val *)vnode->val;
        nu_decref(key0);
        nu_decref(val);
        knode = rb_delete(obj->vals, knode);
        vnode = rb_delete(obj->vals, vnode);
        rb_free_node(knode);
        rb_free_node(vnode);
        return val;
    }

    return NU_NONE;
}

// --------------------------------------------------------------------------------------------------------------------------------

size_t nu_obj_keys_c(const nu_obj *obj, const nu_val **keys)
{
    size_t res = obj->len;
    *keys = nu_calloc(res, nu_val *);
    ptrdiff_t i = 0;
    rb_node *curr = rb_min(obj->keys->root);
    while (curr != RB_NIL)
    {
        *(keys + i++) = (nu_val *)curr->val;
        curr = rb_next(curr);
    }
    return res;
}

size_t nu_obj_vals_c(const nu_obj *obj, const nu_val **vals)
{
    size_t res = obj->len;
    *vals = nu_calloc(res, nu_val *);
    ptrdiff_t i = 0;
    rb_node *curr = rb_min(obj->vals->root);
    while (curr != RB_NIL)
    {
        *(vals + i++) = (nu_val *)curr->val;
        curr = rb_next(curr);
    }
    return res;
}

nu_arr *nu_obj_keys(const nu_obj *obj)
{
    nu_val *keys = NULL;
    size_t len = nu_obj_keys_c(obj, &keys);
    nu_arr *res = nu_malloc(nu_arr);
    NU_ARR_INIT(res, len, len, &keys);
    return res;
}

nu_arr *nu_obj_vals(const nu_obj *obj)
{
    nu_val *vals = NULL;
    size_t len = nu_obj_vals_c(obj, &vals);
    nu_arr *res = nu_malloc(nu_arr);
    NU_ARR_INIT(res, len, len, &vals);
    return res;
}

bool nu_obj_has_key_c(const nu_obj *obj, const nu_val *key)
{
    if (obj == NU_NONE || key == NU_NONE)
    {
        return NU_NONE;
    }

    size_t hashv = nu_size_t_get_c(nu_hash(key));
    if (hashv == 0)
    {
        return NU_NONE;
    }
    
    rb_node *knode = rb_search(obj->keys, hashv);
    return knode != RB_NIL;
}

bool nu_obj_has_val_c(const nu_obj *obj, const nu_val *val)
{
    rb_node *curr = rb_min(obj->vals->root);
    while (curr != RB_NIL)
    {
        if (nu_eq(val, curr->val))
        {
            return true;
        }
        curr = rb_next(curr);
    }
    return false;
}

// --------------------------------------------------------------------------------------------------------------------------------

void nu_obj_clear(nu_obj *obj)
{
    rb_free_tree_iter(obj->keys, _nu_free_iter);
    rb_free_tree_iter(obj->vals, _nu_free_iter);
    obj->len = 0;
    obj->keys = rb_new_tree();
    obj->vals = rb_new_tree();
}

// --------------------------------------------------------------------------------------------------------------------------------
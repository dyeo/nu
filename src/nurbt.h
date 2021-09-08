#ifndef __NU_RBT_H__
#define __NU_RBT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**/
typedef struct rb_node
{
	enum { red, black } color;
	struct rb_node *parent, *left, *right;
	size_t key;
	void *val;
} rb_node;

/**/
typedef struct rb_tree
{
	struct rb_node *root;
} rb_tree;

const extern rb_node rb_nil;

rb_tree *rb_new_tree();
rb_node *rb_new_node(size_t key, void *val);
rb_node *rb_min(rb_node *node);
rb_node *rb_max(rb_node *node);
rb_node *rb_next(rb_node *node);
rb_node *rb_prev(rb_node *node);
rb_node *rb_search(rb_tree *tree, size_t key);
rb_node *rb_insert(rb_tree *tree, rb_node *node);
rb_node *rb_delete(rb_tree *tree, rb_node *node);

#ifdef __cplusplus
}
#endif

#endif//__NU_RBT_H__
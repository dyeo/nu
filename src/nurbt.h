#ifndef __NU_RBT_H__
#define __NU_RBT_H__


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

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
#define RB_NIL (&rb_nil)

typedef void(*rb_traverse_fptr(rb_node *));

rb_tree *rb_new_tree();
void rb_free_tree(rb_tree *tree);
rb_node *rb_new_node(size_t key, void *val);
void rb_free_node(rb_node *node);
rb_node *rb_min(rb_node *node);
rb_node *rb_max(rb_node *node);
rb_node *rb_next(rb_node *node);
rb_node *rb_prev(rb_node *node);
rb_node *rb_search(rb_tree *tree, size_t key);
void rb_insert(rb_tree *tree, rb_node *node);
rb_node *rb_delete(rb_tree *tree, rb_node *node);
void rb_in_order(rb_tree *tree, rb_traverse_fptr func);

#ifdef __cplusplus
}
#endif

#endif//__NU_RBT_H__
#include "nurbt.h"

#include <stdlib.h>
#include <assert.h>

const rb_node rb_nil = {black, RB_NIL, RB_NIL, RB_NIL, 0ul, NULL};

rb_tree *rb_new_tree()
{
	rb_tree *r = (rb_tree *)malloc(sizeof(rb_tree));
	assert(r != NULL);
	r->root = RB_NIL;
	return r;
}

void rb_free_tree(rb_tree *tree)
{
	if (tree->root != RB_NIL)
	{
		rb_free_node(tree->root);
	}
}

void rb_free_tree_iter(rb_tree *tree, rb_traverse_fptr fptr)
{
	if (tree->root != RB_NIL)
	{
		rb_free_node_iter(tree->root, fptr);
	}
}

rb_node *rb_new_node(size_t key, void *val)
{
	rb_node *r = (rb_node *)malloc(sizeof(rb_node));
	assert(r != NULL);
	r->parent = RB_NIL;
	r->left = RB_NIL;
	r->right = RB_NIL;
	r->color = red;
	r->key = key;
	r->val = val;
	return r;
}

void rb_free_node(rb_node *node)
{
	if (node == RB_NIL)
	{
		return;
	}
	rb_free_node(node->left);
	rb_free_node(node->right);
	free(node);
}

void rb_free_node_iter(rb_node *node, rb_traverse_fptr fptr)
{
	if (node == RB_NIL)
	{
		return;
	}
	rb_free_node(node->left);
	rb_free_node(node->right);
	fptr(node);
	free(node);
}


rb_node *rb_min(rb_node *x)
{
	while (x->left != RB_NIL)
	{
		x = x->left;
	}
	return x;
}

rb_node *rb_max(rb_node *x)
{
	while (x->right != RB_NIL)
	{
		x = x->right;
	}
	return x;
}

rb_node *rb_search(rb_tree *t, size_t key)
{
	rb_node *curr = t->root;
	while (curr != RB_NIL && key != curr->key)
	{
		curr = key < curr->key ? curr->left : curr->right;
	}
	return curr;
}

rb_node *rb_next(rb_node *x)
{
	if (x->right != RB_NIL)
	{
		return rb_min(x->right);
	}
	rb_node *y = x->parent;
	while (y != RB_NIL && x == y->right)
	{
		x = y;
		y = y->parent;
	}
	return y;
}

rb_node *rb_prev(rb_node *x)
{
	if (x->left != RB_NIL)
	{
		return rb_max(x->left);
	}
	rb_node *y = x->parent;
	while (y != RB_NIL && x == y->left)
	{
		x = y;
		y = y->parent;
	}
	return y;
}

void rb_in_order(rb_tree *t, rb_traverse_fptr f)
{
	rb_node *curr = rb_min(t->root);
	while (curr != RB_NIL)
	{
		f(curr);
		curr = rb_next(curr);
	}
}

void rb_left_rotate(rb_tree *t, rb_node *x)
{
	if (x->right == RB_NIL)
	{
		return;
	}
	rb_node *y = x->right;
	x->right = y->left;
	if (y->left != RB_NIL)
	{
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == RB_NIL)
	{
		t->root = y;
	}
	else if (x == x->parent->left)
	{
		x->parent->left = y;
	}
	else
	{
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

void rb_right_rotate(rb_tree *t, rb_node *x)
{
	if (x->right == RB_NIL)
	{
		return;
	}
	rb_node *y = x->left;
	x->left = y->right;
	if (y->right != RB_NIL)
	{
		y->right->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == RB_NIL)
	{
		t->root = y;
	}
	else if (x == x->parent->right)
	{
		x->parent->right = y;
	}
	else
	{
		x->parent->left = y;
	}
	y->right = x;
	x->parent = y;
}

void rb_insert(rb_tree *t, rb_node *z)
{
	rb_node *y = RB_NIL;
	rb_node *x = t->root;
	while (x != RB_NIL)
	{
		y = x;
		if (z->key < x->key)
		{
			x = x->left;
		}
		else
		{
			x = x->right;
		}
	}
	z->parent = y;
	if (y == RB_NIL)
	{
		t->root = z;
	}
	else if (z->key < y->key)
	{
		y->left = z;
	}
	else
	{
		y->right = z;
	}
	if (x != RB_NIL)
	{
		x->color = red;
	}

	while (x != t->root && x->parent->color == red)
	{
		if (x->parent == x->parent->parent->left)
		{
			y = x->parent->parent->right;
			if (y->color == red)
			{
				x->parent->color = black;
				y->color = black;
				x->parent->parent->color = red;
				x = x->parent->parent;
			}
			else
			{
				if (x == x->parent->right)
				{
					x = x->parent;
					rb_left_rotate(t, x);
				}
				x->parent->color = black;
				x->parent->parent->color = red;
				rb_right_rotate(t, x->parent->parent);
			}
		}
		else
		{
			y = x->parent->parent->left;
			if (y->color == red)
			{
				x->parent->color = black;
				y->color = black;
				x->parent->parent->color = red;
				x = x->parent->parent;
			}
			else
			{
				if (x == x->parent->left)
				{
					x = x->parent;
					rb_right_rotate(t, x);
				}
				x->parent->color = black;
				x->parent->parent->color = red;
				rb_left_rotate(t, x->parent->parent);
			}
		}
	}
	t->root->color = black;
}

rb_node *rb_delete(rb_tree *t, rb_node *z)
{
	rb_node *x;
	rb_node *y;
	if (z->left == RB_NIL || z->right == RB_NIL)
	{
		y = z;
	}
	else
	{
		y = rb_next(z);
	}
	if (y->left != RB_NIL)
	{
		x = y->left;
	}
	else
	{
		x = y->right;
	}
	if (x->parent != RB_NIL)
	{
		x->parent = y->parent;
	}
	if (y->parent == RB_NIL)
	{
		t->root = x;
	}
	else if (y == y->parent->left)
	{
		y->parent->left = x;
	}
	else
	{
		y->parent->right = x;
	}
	if (y != z)
	{
		z->key = y->key;
		z->val = y->val;
	}

	if (y->color == black)
	{
		rb_node *w;
		while (x != t->root && x->color == black)
		{
			if (x == x->parent->left)
			{
				w = x->parent->right;
				if (w->color == red)
				{
					w->color = black;
					x->parent->color = red;
					rb_left_rotate(t, x->parent);
					w = x->parent->right;
				}
				if (w->left->color == black && w->right->color == black)
				{
					w->color = red;
					x = x->parent;
				}
				else
				{
					if (w->right->color == black)
					{
						w->left->color = black;
						w->color = red;
						rb_right_rotate(t, w);
						w = x->parent->right;
					}
					w->color = x->parent->color;
					x->parent->color = black;
					w->right->color = black;
					rb_left_rotate(t, x->parent);
					x = t->root;
				}
			}
			else
			{
				w = x->parent->left;
				if (w->color == red)
				{
					w->color = black;
					x->parent->color = red;
					rb_right_rotate(t, x->parent);
					w = x->parent->left;
				}
				if (w->right->color == black && w->left->color == black)
				{
					w->color = red;
					x = x->parent;
				}
				else
				{
					if (w->left->color == black)
					{
						w->right->color = black;
						w->color = red;
						rb_left_rotate(t, w);
						w = x->parent->left;
					}
					w->color = x->parent->color;
					x->parent->color = black;
					w->left->color = black;
					rb_right_rotate(t, x->parent);
					x = t->root;
				}
			}
		}
	}
	return y;
}

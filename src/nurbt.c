#include "nurbt.h"

#include <assert.h>

rb_node rb_nil = { &rb_nil, &rb_nil, &rb_nil, BLACK, 0, NULL };

rb_node *rb_new(size_t hash, void *data)
{
	rb_node *r = (rb_node *)malloc(sizeof(rb_node));
	assert(r != NULL);
	r->parent = RB_NIL;
	r->left = RB_NIL;
	r->right = RB_NIL;
	r->color = RED;
	r->hash = hash;
	r->data = data;
	return r;
}

rb_node *rb_search(rb_node **tree, size_t hash)
{
	rb_node *x = *tree;
	while (x != RB_NIL && x->hash != hash)
		x = (x->hash < hash) ? x->left : x->right;
	return x;
}

rb_node *rb_min(rb_node *node)
{
	while (node->left != RB_NIL)
		node = node->left;
	return node;
}

rb_node *rb_max(rb_node *node)
{
	while (node->right != RB_NIL)
		node = node->right;
	return node;
}

rb_node *_rb_successor(rb_node *node)
{
	if (node->right != RB_NIL)
		return rb_min(node->right);
	rb_node *y = node->parent;
	while (y != RB_NIL && node == y->right)
	{
		node = y;
		y = y->parent;
	}
	return y;
}

void _rb_rotate_left(rb_node **root, rb_node *node)
{
	rb_node *y = node->right;
	node->right = y->left;
	if (y->left != RB_NIL)
		y->left->parent = node;
	y->parent = node->parent;
	if (node->parent == RB_NIL)
		*root = y;
	else if (node == node->parent->left)
		node->parent->left = y;
	else
		node->parent->right = y;
	y->left = node;
	node->parent = y;
}

void _rb_rotate_right(rb_node **root, rb_node *node)
{
	rb_node *y = node->left;
	node->left = y->right;
	if (y->right != RB_NIL)
		y->right->parent = node;
	y->parent = node->parent;
	if (node->parent == RB_NIL)
		*root = y;
	else if (node == node->parent->left)
		node->parent->left = y;
	else
		node->parent->right = y;
	y->right = node;
	node->parent = y;
}

void rb_insert(rb_node **root, rb_node *node)
{
	rb_node *y = RB_NIL;
	rb_node *x = *root;
	while (x != RB_NIL)
	{
		y = x;
		x = (node->hash < x->hash) ? x->left : x->right;
	}
	node->parent = y;
	if (y == RB_NIL)
		*root = node;
	else if (node->hash < y->hash)
		y->left = node;
	else
		y->right = node;
	node->left = RB_NIL;
	node->right = RB_NIL;
	node->color = RED;
	while (node->parent->color == RED)
	{
		rb_node *y;
		if (node->parent == node->parent->parent->left)
		{
			y = node->parent->parent->right;
			if (y->color == RED)
			{
				node->parent->color = BLACK;
				y->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->right)
				{
					node = node->parent;
					_rb_rotate_left(root, node);
				}
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				_rb_rotate_right(root, node->parent->parent);
			}
		}
		else if (node->parent == node->parent->parent->right)
		{
			y = node->parent->parent->left;
			if (y->color == RED)
			{
				node->parent->color = BLACK;
				y->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->left)
				{
					node = node->parent;
					_rb_rotate_right(root, node);
				}
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				_rb_rotate_left(root, node->parent->parent);
			}
		}
	}
	(*root)->color = BLACK;
}

rb_node *rb_delete(rb_node **root, rb_node *node)
{
	rb_node *y, *x;
	if (node->left == RB_NIL || node->right == RB_NIL)
		y = node;
	else
		y = _rb_successor(node);
	if (y->left != RB_NIL)
		x = y->left;
	else
		x = y->right;
	x->parent = y->parent;
	if (y->parent == RB_NIL)
		*root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;
	if (y == node)
	{
		node->hash = y->hash;
		node->data = y->data;
	}
	if (y->color == BLACK)
	{
		// fixup
		while (x != *root && x->color == BLACK)
		{
			rb_node *w;
			if (x == x->parent->left)
			{
				w = x->parent->right;
				if (w->color == RED)
				{
					w->color = BLACK;
					x->parent->color = RED;
					_rb_rotate_left(root, x->parent);
					w = x->parent->right;
				}
				if (w->left->color == BLACK && w->right->color == BLACK)
				{
					w->color = RED;
					x = x->parent;
					x->parent = node->parent->parent;
				}
				else
				{
					if (w->right->color == BLACK)
					{
						w->left->color = BLACK;
						w->color = RED;
						_rb_rotate_right(root, w);
						w = x->parent->right;
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->right->color = BLACK;
					_rb_rotate_left(root, x->parent);
					x = *root;
				}
			}
			else
			{
				w = x->parent->left;
				if (w->color == RED)
				{
					w->color = BLACK;
					x->parent->color = RED;
					_rb_rotate_right(root, x->parent);
					w = x->parent->left;
				}
				if (w->right->color == BLACK && w->left->color == BLACK)
				{
					w->color = RED;
					x = x->parent;
					x->parent = node->parent->parent;
				}
				else
				{
					if (w->left->color == BLACK)
					{
						w->right->color = BLACK;
						w->color = RED;
						_rb_rotate_left(root, w);
						w = x->parent->left;
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->left->color = BLACK;
					_rb_rotate_right(root, x->parent);
					x = *root;
				}
			}
		}
		x->color = BLACK;
	}
	return y;
}

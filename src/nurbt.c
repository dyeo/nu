#include "nurbt.h"

#include <stdlib.h>
#include <assert.h>

#define nil &rb_nil
#define p(N) N->parent
#define left(N) N->left
#define right(N) N->right
#define color(N) N->color
#define key(N) N->key
#define val(N) N->val
#define root(N) N->root

const rb_node rb_nil = { black, nil, nil, nil, 0ul, NULL };

rb_node *rb_new_node(size_t key, void *val)
{
	rb_node *r = (rb_node *)malloc(sizeof(rb_node));
	assert(r != NULL);
	r->parent = nil;
	r->left = nil;
	r->right = nil;
	r->color = red;
	r->key = key;
	r->val = val;
	return r;
}

rb_node *rb_min(rb_node *x)
{
	while (left(x) != nil)
	{
		x = left(x);
	}
	return x;
}

rb_node *rb_max(rb_node *x)
{
	while (right(x) != nil)
	{
		x = right(x);
	}
	return x;
}

rb_node *rb_search(rb_tree *t, size_t key)
{
	rb_node *curr = root(t);
	while (curr != nil && key != key(curr))
	{
		curr = key < key(curr) ? left(curr) : right(curr);
	}
	return curr;
}

rb_node *rb_next(rb_node *x)
{
	if (right(x) != nil)
	{
		return rb_min(right(x));
	}
	rb_node *y = p(x);
	while (y != nil && x == right(y))
	{
		x = y;
		y = p(y);
	}
	return y;
}

rb_node *rb_prev(rb_node *x)
{
	if (left(x) != nil)
	{
		return rb_max(left(x));
	}
	rb_node *y = p(x);
	while (y != nil && x == left(y))
	{
		x = y;
		y = p(y);
	}
	return y;
}

rb_tree *rb_new_tree()
{
	rb_tree *r = (rb_tree *)malloc(sizeof(rb_tree));
	assert(r != NULL);
	r->root = nil;
	return r;
}

void rb_traverse(rb_tree *tree, rb_traverse_fptr func)
{
	rb_node *curr = rb_min(tree->root);
	do
	{
		func(curr);
	} while ((curr = rb_next(curr)) != &rb_nil);
}

void rb_left_rotate(rb_tree *t, rb_node *x)
{
	if (right(x) == nil)
	{
		return;
	}
	rb_node *y = right(x); // set y
	right(x) = left(y); // turn y's left subtree into x's right subtree
	if (left(y) != nil)
	{
		p(left(y)) = x;
	}
	p(y) = p(x); // link x's parent to y
	if (p(x) == nil)
	{
		root(t) = y;
	}
	else if (x == left(p(x)))
	{
		left(p(x)) = y;
	}
	else
	{
		right(p(x)) = y;
	}
	left(y) = x; // put x on y's left
	p(x) = y;
}

void rb_right_rotate(rb_tree *t, rb_node *x)
{
	if (right(x) == nil)
	{
		return;
	}
	rb_node *y = left(x); // set y
	left(x) = right(y); // turn y's right subtree into x's left subtree
	if (right(y) != nil)
	{
		p(right(y)) = x;
	}
	p(y) = p(x); // link x's parent to y
	if (p(x) == nil)
	{
		root(t) = y;
	}
	else if (x == right(p(x)))
	{
		right(p(x)) = y;
	}
	else
	{
		left(p(x)) = y;
	}
	right(y) = x; // put x on y's right
	p(x) = y;
}

void rb_insert(rb_tree *t, rb_node *z)
{
	rb_node *y = nil;
	rb_node *x = root(t);
	while (x != nil)
	{
		y = x;
		if (key(z) < key(x))
		{
			x = left(x);
		}
		else
		{
			x = right(x);
		}
	}
	p(z) = y;
	if (y == nil)
	{
		root(t) = z;
	}
	else if (key(z) < key(y))
	{
		left(y) = z;
	}
	else
	{
		right(y) = z;
	}
	if (x != nil)
	{
		color(x) = red;
	}

	while (x != root(t) && color(p(x)) == red)
	{
		if (p(x) == left(p(p(x))))
		{
			y = right(p(p(x)));
			if (color(y) == red)
			{
				color(p(x)) = black;
				color(y) = black;
				color(p(p(x))) = red;
				x = p(p(x));
			}
			else
			{
				if (x == right(p(x)))
				{
					x = p(x);
					rb_left_rotate(t, x);
				}
				color(p(x)) = black;
				color(p(p(x))) = red;
				rb_right_rotate(t, p(p(x)));
			}
		}
		else
		{
			y = left(p(p(x)));
			if (color(y) == red)
			{
				color(p(x)) = black;
				color(y) = black;
				color(p(p(x))) = red;
				x = p(p(x));
			}
			else
			{
				if (x == left(p(x)))
				{
					x = p(x);
					rb_right_rotate(t, x);
				}
				color(p(x)) = black;
				color(p(p(x))) = red;
				rb_left_rotate(t, p(p(x)));
			}
		}
	}
	color(root(t)) = black;
}

rb_node *rb_delete(rb_tree *t, rb_node *z)
{
	rb_node *x; rb_node *y;
	if (left(z) == nil || right(z) == nil)
	{
		y = z;
	}
	else
	{
		y = rb_next(z);
	}
	if (left(y) != nil)
	{
		x = left(y);
	}
	else
	{
		x = right(y);
	}
	if (p(x) != nil)
	{
		p(x) = p(y);
	}
	if (p(y) == nil)
	{
		root(t) = x;
	}
	else if (y == left(p(y)))
	{
		left(p(y)) = x;
	}
	else
	{
		right(p(y)) = x;
	}
	if (y != z)
	{
		key(z) = key(y);
		val(z) = val(y);
	}

	if (color(y) == black)
	{
		rb_node *w;
		while (x != root(t) && color(x) == black)
		{
			if (x == left(p(x)))
			{
				w = right(p(x));
				if (color(w) == red)
				{
					color(w) = black;
					color(p(x)) = red;
					rb_left_rotate(t, p(x));
					w = right(p(x));
				}
				if (color(left(w)) == black && color(right(w)) == black)
				{
					color(w) = red;
					x = p(x);
				}
				else
				{
					if (color(right(w)) == black)
					{
						color(left(w)) = black;
						color(w) = red;
						rb_right_rotate(t, w);
						w = right(p(x));
					}
					color(w) = color(p(x));
					color(p(x)) = black;
					color(right(w)) = black;
					rb_left_rotate(t, p(x));
					x = root(t);
				}
			}
			else
			{
				w = left(p(x));
				if (color(w) == red)
				{
					color(w) = black;
					color(p(x)) = red;
					rb_right_rotate(t, p(x));
					w = left(p(x));
				}
				if (color(right(w)) == black && color(left(w)) == black)
				{
					color(w) = red;
					x = p(x);
				}
				else
				{
					if (color(left(w)) == black)
					{
						color(right(w)) = black;
						color(w) = red;
						rb_left_rotate(t, w);
						w = left(p(x));
					}
					color(w) = color(p(x));
					color(p(x)) = black;
					color(left(w)) = black;
					rb_right_rotate(t, p(x));
					x = root(t);
				}
			}
		}
	}
	return y;
}

#undef nil
#undef p(N)
#undef left(N)
#undef right(N)
#undef color(N)
#undef key(N)
#undef val(N)

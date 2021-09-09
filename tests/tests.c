#include "tests.h"
#include "../src/nurbt.h"

#include <stdio.h>

void print_node(rb_node *curr)
{
	printf("%i\n", curr->key);
}

int main(int argc, char **argv)
{
	rb_tree *a = rb_new_tree();
	for (int i = 0; i < 24; ++i)
	{
		rb_insert(a, rb_new_node(i, NULL));
	}

	rb_traverse(a, print_node);

	//assert(rb_search(a, 50) != NIL);
	rb_node *s = rb_search(a, 10);
	rb_delete(a, s);

	printf("\n");

	rb_traverse(a, print_node);
	//nu_initialize();
	//nu_num *a = nu_new_num(25);
	//nu_num *b = nu_new_num(25);
	//nu_bool *c = nu_le(a, b);
	//printf("%i", nu_to_bool(c));
	//nu_finalize();
}
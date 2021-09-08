#include "tests.h"
#include "../src/nurbt.h"

#include <stdio.h>

int main(int argc, char **argv)
{
	rb_tree *a = rb_new_tree();
	for (int i = 0; i < 24; ++i)
	{
		a = rb_insert(&a, rb_new_node(i, NULL));
	}
	//assert(rb_search(a, 50) != NIL);
	rb_node *s = rb_search(&a, 23);
	rb_delete(a, s);
	//nu_initialize();
	//nu_num *a = nu_new_num(25);
	//nu_num *b = nu_new_num(25);
	//nu_bool *c = nu_le(a, b);
	//printf("%i", nu_to_bool(c));
	//nu_finalize();
}
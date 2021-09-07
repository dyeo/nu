#include "tests.h"
#include "../src/nu.h"
#include "../src/nurbt.h"

#include <assert.h>
#include <stdio.h>


int main(int argc, char **argv)
{
	rb_node *a = rb_new(0, NULL);
	for (int i = 1; i < 24; ++i)
	{
		rb_insert(&a, rb_new(i, NULL));
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
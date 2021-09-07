#ifndef __NU_RBT_H__
#define __NU_RBT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**/
typedef struct rb_node
{
	struct rb_node *parent;
	union
	{
		struct
		{
			struct rb_node *left;
			struct rb_node *right;
		};
		struct rb_node *child[2];
	};
	uint8_t color;
	size_t hash;
	void *data;
} rb_node;

extern rb_node rb_nil;
#define RB_NIL (&rb_nil)

#define BLACK 0
#define RED 1
#define LEFT 0
#define RIGHT 1
#define CHILDDIR(N) ( N == (N->parent)->right ? RIGHT : LEFT )

rb_node *rb_min(rb_node *node);
rb_node *rb_max(rb_node *node);
rb_node *rb_new(size_t hash, void *data);
rb_node *rb_search(rb_node **root, size_t hash);
void rb_insert(rb_node **root, rb_node *z);
rb_node *rb_delete(rb_node **root, rb_node *z);

#ifdef __cplusplus
}
#endif

#endif//__NU_RBT_H__
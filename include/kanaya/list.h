#ifndef kanaya_list_h
#define kanaya_list_h

#include <class.h>

typedef struct list_node {
	void *data;
	struct list_node *next;
	struct list_node *prev;
} list_node;

struct IList {
	int (*get_size)(struct IList *self);
	list_node *(*get_head)(struct IList *self);
	list_node *(*get_tail)(struct IList *self);
	list_node *(*ins_next)(struct IList *self, list_node *n, void *data);
	list_node *(*ins_prev)(struct IList *self, list_node *n, void *data);
	void (*del)(struct IList *self, list_node *node);
};

extern const void *IList;

#endif

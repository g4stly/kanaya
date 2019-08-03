#include <stdarg.h>
#include <string.h>
#include <class.h>
#include <kanaya/list.h>
#include "util.h"

// class definition
struct ListCls {
	struct cls super;
};

// class representation
struct RList {
	int size;
	list_node *head;
	list_node *tail;
	void (*destroy)(void *data);
	struct ListCls *cls;
};

// instance struct
struct List {
	struct RList representation;
	struct IList interface;
};


static int get_size(struct IList *_self)
{
	struct RList *self = (void *)_self - sizeof(struct RList);
	return self->size;
}

static list_node *get_head(struct IList *_self)
{
	struct RList *self = (void *)_self - sizeof(struct RList);
	return self->head;
}

static list_node *get_tail(struct IList *_self)
{
	struct RList *self = (void *)_self - sizeof(struct RList);
	return self->tail;
}

static list_node *ins_next(struct IList *_self, list_node *prev, void *data)
{
	struct List *self = (void *)_self - sizeof(struct RList);
	if (!prev && self->representation.size) return NULL;

	list_node *n = malloc(sizeof(list_node));
	if (!n) die("IList*->ins_next(): malloc():");
	memset(n, 0, sizeof(list_node));

	n->data = data;
	n->prev = prev;
	if (n->prev) {
		n->next = n->prev->next;
		n->prev->next = n;
	}
	if (n->next) {
		n->next->prev = n;
	}

	if (!self->representation.head || !n->prev) self->representation.head = n;
	if (!self->representation.tail || !n->next) self->representation.tail = n;
	self->representation.size++;

	return n;
}

static list_node *ins_prev(struct IList *_self, list_node *next, void *data)
{
	struct List *self = (void *)_self - sizeof(struct RList);
	if (!next && self->representation.size) return NULL;

	list_node *n = malloc(sizeof(list_node));
	if (!n) die("IList*->ins_prev(): malloc():");
	memset(n, 0, sizeof(list_node));

	n->data = data;
	n->next = next;
	if (n->next) {
		n->prev = n->next->prev;
		n->next->prev = n;
	}
	if (n->prev) {
		n->prev->next = n;
	}

	if (!self->representation.head || !n->prev) self->representation.head = n;
	if (!self->representation.tail || !n->next) self->representation.tail = n;
	self->representation.size++;

	return n;
}

static void del(struct IList *_self, list_node *n)
{
	struct List *self = (void *)_self - sizeof(struct RList);

	if (n->prev) n->prev->next = n->next;
	else self->representation.head = n->next;

	if (n->next) n->next->prev = n->prev;
	else self->representation.tail = n->prev;

	free(n);
}


static void *list_ctor(void *_self, va_list *ap)
{
	struct List *self = _self;

	self->representation.destroy = va_arg(*ap, void (*)(void *));

	self->interface.get_size = get_size;
	self->interface.get_head = get_head;
	self->interface.get_tail = get_tail;
	self->interface.ins_next = ins_next;
	self->interface.ins_prev = ins_prev;
	self->interface.del = del;

	// return pointer to the interface
	return &self->interface;
}

static void *list_dtor(void *_self)
{
	struct List *self = (void *)_self - sizeof(struct RList);

	list_node *next;
	list_node *current = self->representation.head;
	while (current) {
		if (self->representation.destroy && current->data)
			self->representation.destroy(current->data);
		next = current->next;
		self->interface.del(_self, current);
		current = next;
	}

	// return the original pointer passed to the ctor
	return self;
}

// class initialization
static const struct ListCls _List = {
	{
		sizeof(struct List),
		list_ctor,
		list_dtor
	}
};
const void *IList = &_List;

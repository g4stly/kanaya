#include <stdarg.h>
#include <string.h>
#include <class.h>
#include <kanaya/list.h>
#include <kanaya/stack.h>
#include "util.h"

struct StackCls {
	struct cls super;
};

struct RStack {
	struct IList *l;
	struct StackCls *cls;
};

struct Stack {
	struct RStack representation; 
	struct IStack interface;
};

static int get_size(struct IStack *_self)
{
	struct RStack *self = (void *)_self - sizeof(struct RStack);
	return self->l->get_size(self->l);
}

static int push(struct IStack *_self, void *data)
{
	struct RStack *self = (void *)_self - sizeof(struct RStack);
	struct IList *l = self->l;

	l->ins_prev(l, l->get_head(l), data); 
	return l->get_size(l);
}

static void *peek(struct IStack *_self)
{
	struct RStack *self = (void *)_self - sizeof(struct RStack);
	list_node *head = self->l->get_head(self->l);
	return head->data;
}

static void *pop(struct IStack *_self)
{
	struct RStack *self = (void *)_self - sizeof(struct RStack);
	list_node *head = self->l->get_head(self->l);
	void *rv = head->data;
	self->l->del(self->l, head);
	return rv;
}

static void *stack_ctor(void *_self, va_list *ap)
{
	struct Stack *self = _self;
	void *destroy = va_arg(*ap, void (*)(void *));

	self->representation.l = new(IList, destroy);

	self->interface.get_size = get_size;
	self->interface.push = push;
	self->interface.peek = peek;
	self->interface.pop = pop;

	return &self->interface;
}

static void *stack_dtor(void *_self)
{
	struct Stack *self = _self - sizeof(struct RStack);
	delete(self->representation.l);
	return self;
}

static const struct StackCls _Stack = {
	{
		sizeof(struct Stack),
		stack_ctor,
		stack_dtor
	}
};
const void *IStack = &_Stack;

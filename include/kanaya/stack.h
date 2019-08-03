#ifndef kanaya_stack_h_
#define kanaya_stack_h

struct IStack {
	int (*get_size)(struct IStack *self);
	int (*push)(struct IStack *self, void *data);
	void *(*peek)(struct IStack *self);
	void *(*pop)(struct IStack *self);
};

extern const void *IStack;

#endif

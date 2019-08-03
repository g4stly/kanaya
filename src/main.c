#include <stdio.h>
#include <kanaya.h>

void fn(void *data)
{
	printf("%s\n", (char *)data);
}

int main(int argc, char **argv)
{
	struct IList *l = new(IList, fn);
	l->ins_next(l, l->get_head(l), "World!");
	l->ins_prev(l, l->get_head(l), "Hello");
	delete(l);

	struct IStack *s = new(IStack, fn);
	s->push(s, "World!");
	s->push(s, "Hello");
	delete(s);
	return 0;
}

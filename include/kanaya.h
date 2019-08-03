#ifndef kanaya_h_
#define kanaya_h_

#include "class.h"
#include "kanaya/list.h"
#include "kanaya/stack.h"

// re-export 
void *new(const void *cls, ...);
const struct cls *get_cls(void *obj);
void delete(void *obj);

#endif

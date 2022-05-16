#ifndef SIMPLE_LIST
#define SIMPLE_LIST

#include <stdlib.h>

typedef struct _simple_list
{
    void *data;
    struct _simple_list *next;
} simple_list;

simple_list *prepend(simple_list *next, void *data);
simple_list *removeFromSimpleList(simple_list *prev);
simple_list *append(simple_list *prev, void *data);
simple_list *newSimpleList(void* value, simple_list *next);

#endif
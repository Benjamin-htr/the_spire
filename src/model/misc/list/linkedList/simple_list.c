#include <stdlib.h>
#include <stdio.h>

#include "simple_list.h"

simple_list *newSimpleList(void *data, simple_list *next)
{
    simple_list *res = malloc(sizeof(simple_list));
    res->next = next;
    res->data = data;
    return res;
}

simple_list *prepend(simple_list *next, void *data)
{
    return newSimpleList(data, next);
}

simple_list *append(simple_list *prev, void *data)
{
    simple_list *res = newSimpleList(data, prev->next);
    prev->next = res;
    return res;
}

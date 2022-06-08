#ifndef EVENT
#define EVENT

#include "../../../../misc/select/select.h"

typedef struct {
    char* dialogue;
    select* actions;
    void* data;
}event;


event * createEvent(char * dialogue, select* actions, void * data);
#endif
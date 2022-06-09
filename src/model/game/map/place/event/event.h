#ifndef EVENT
#define EVENT

#include "../../../../misc/select/select.h"

typedef struct {
    char* dialogue;
    select_* actions;
    void* data;
}event;

event *event_init(char* dialogue, select_* actions, void* data);

#endif
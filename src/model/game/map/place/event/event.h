#ifndef EVENT
#define EVENT

#include "../../../../misc/select/select.h"

typedef struct
{
    char *dialogue;
    option actions[2];
    void *data;
} event;

typedef struct
{
    char *dialogue;
    option actions[2];
    void *data;
} event_import;

event *event_init(char *dialogue, option actions[], void *data);
event *importEvent(event_import event_import_);
event *get_random_event();

extern event_import EVENT_ENCYCLOPEDIA[];

#endif
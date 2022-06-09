#ifndef PLACE
#define PLACE

#include "event/event.h"
#include "./../../entity/entity.h"
#include "./../../../misc/boolean/boolean.h"

typedef struct
{
    boolean isEvent;
    event* eventData;
    entity_t* enemyData;
}place;

place place_init(boolean isEvent, event *eventData, entity_t *enemyData);
place test();
void testPlace();

#endif
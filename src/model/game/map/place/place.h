#ifndef PLACE
#define PLACE

#include "event/event.h"

// #include "./../../entity/entity.h"
#include "./../../../misc/boolean/boolean.h"

typedef struct
{
    int isWhat;
    event *eventData;
    entity_t *enemyData;
} place;

place place_init(int isWhat, event *eventData, entity_t *enemyData);
place test();
void testPlace();

#endif
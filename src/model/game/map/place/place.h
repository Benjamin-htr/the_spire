#ifndef PLACE
#define PLACE

#include "../../entity/enemy/enemy.h"
#include "event/event.h"

typedef struct
{
    int isEvent;
    event* eventData;
    enemy* enemyData;
}place;

place place_init(int isEvent, event *eventData, enemy *enemyData);


#endif
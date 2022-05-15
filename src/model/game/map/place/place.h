#ifndef PLACE
#define PLACE

#include "../../entity/enemy/enemy.h"
#include "../../../misc/boolean/boolean.h"
#include "event/event.h"

typedef struct
{
    boolean isEvent;
    event* eventData;
    enemy* enemyData;
}place;


#endif
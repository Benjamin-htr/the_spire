#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "place.h"


place place_init(int isEvent, event *eventData, enemy *enemyData)
{
    place p;
    p.isEvent = isEvent;
    p.eventData = eventData;
    p.enemyData = enemyData;
    return p;
}
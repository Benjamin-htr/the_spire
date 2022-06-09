#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./event/event.h"
#include "place.h"


place place_init(boolean isEvent, event *eventData, entity_t *enemyData)
{
    place p;
    p.isEvent = isEvent;
    p.eventData = eventData;
    p.enemyData = enemyData;
    return p;
}

place test(){
    return place_init(false, NULL, NULL);
}

void testPlace(){
    place p = test();
    printf("%d\n", p.isEvent);
}

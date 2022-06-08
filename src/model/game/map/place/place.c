#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./event/event.h"
#include "place.h"


place place_init(int isEvent, event *eventData, enemy *enemyData)
{
    place p;
    p.isEvent = isEvent;
    p.eventData = eventData;
    p.enemyData = enemyData;
    return p;
}

place test(){
    return place_init(987, NULL, NULL);
}

int main(int argc, char const *argv[])
{
    place p = test();
    printf("%d\n", p.isEvent);
    return 0;
}

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "place.h"

// init de la place

place place_init(int isWhat, event *eventData, entity_t *enemyData)
{
    place p;
    p.isWhat = isWhat; // 0 = combat, 1 = event, 2 = sanctuaire, 3 miniboss
    p.eventData = eventData;
    p.enemyData = enemyData;
    return p;
}

// test de la place

place test()
{
    return place_init(0, NULL, NULL);
}

// test de la place

void testPlace()
{
    place p = test();
    printf("%d\n", p.isWhat);
}

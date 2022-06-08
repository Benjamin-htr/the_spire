#include "../model/game/entity/entity.h"
#include "../model/game/misc/stat/stat.h"
#include "../model/game/misc/effect/effect.h"
#include <stdio.h>
void testFunction()
{
    printf("\n==============================\n\tTEST DE STAT\n==============================\n");
    testStat();
    printf("\n==============================\n\tTEST DE EFFECT\n==============================\n");
    testEffect();
    printf("\n==============================\n\tTEST DE L'ENTITY\n==============================\n");
    testEntity();
}
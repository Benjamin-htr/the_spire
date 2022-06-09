#include "effect.h"
#include "../../entity/entity.h"
#include <stdlib.h>

effect_t *initEffectBlank()
{
    return malloc(sizeof(effect_t));
}
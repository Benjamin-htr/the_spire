#include "effect.h"
#include "../../entity/entity.h"
#include <stdlib.h>

effect *initEffectBlank()
{
    return malloc(sizeof(effect));
}
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "option.h"


option option_init(char* label, action_t action)
{
    option o;
    o.label = label;
    o.action = action;
    return o;
}

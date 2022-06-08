#ifndef STAT
#define STAT

#include "../../../misc/boolean/boolean.h"

typedef enum
{
    NONE,
    HP,
    DODGE,
    STR,
    MANA,
    ENERGY,
} stat_ID;

typedef enum
{
    PERCISTANT,
    TEMPORARY,
} statTemporalRange;

typedef enum
{
    CURR,
    MAX,
} statCurrOrMax;

typedef struct
{
    stat_ID id;
    int **stat_bar;
} stat_t;

stat_t *initStat(stat_ID id, int max, boolean currZero); // If curr is null curr = max
void displayStat(stat_t stat);
void testStat();
#endif
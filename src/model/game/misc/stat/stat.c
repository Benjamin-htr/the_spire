#include "stat.h"
#include <stdlib.h>
#include <stdio.h>

#include "../../../misc/boolean/boolean.h"

const char *STAT_NAME[] = {
    "NIY",
    "VIE",
    "ESQUIVE",
    "MANA",
    "ENERGY",
};

// CONSTRUCTOR

stat_t *initStat(stat_ID id, int max, boolean currZero)
{
    stat_t *stat = malloc(sizeof(stat_t));
    stat->id = id;
    stat->max = max;
    stat->current = (currZero) ? 0 : max;
    return stat;
}
// INIT ARRAY
stat_t **initEntityStatFromArray(int stats[4][2])
{
    stat_t **res = malloc(4 * sizeof(stat_t *));
    for (int stats_ID = 0; stats_ID < 4; stats_ID++)
    {
        res[stats_ID] = initStat(stats_ID + 1, stats[stats_ID][0], stats[stats_ID][1]);
    };
    return res;
}

// DECONSTRUCTOR

void freeStat(stat_t *statPt)
{
    free(statPt);
}
// Free Array
void freeEntityStatArray(stat_t **entityStatArray)
{
    for (int stats_ID = 0; stats_ID < 4; stats_ID++)
    {
        freeStat(entityStatArray[stats_ID]);
    }
    free(entityStatArray);
}

// METHOD
// update stat current and max checking if current isn't greather than max
void updateStat(stat_t *stat, int value, boolean isMax)
{
    if (isMax)
    {
        stat->max += value;
    }
    else
    {
        stat->current += value;
    }
    if (stat->max < stat->current)
    {
        stat->current = stat->max;
    }
}

// refill the stat to max
void refillStat(stat_t *stat)
{
    stat->current = stat->max;
}

// refill the stat to half max
void refillHalfStat(stat_t *stat)
{
    updateStat(stat, stat->max / 2, false);
}

// DISPLAY FUNCTION

void displayStat(stat_t *stat)
{
    printf(
        "\n%s\t%d/%d\n",
        STAT_NAME[stat->id],
        stat->current,
        stat->max);
}

// display an array of stat of size 4( used in Entity)
void displayEntityStatArray(stat_t **entityStatArray)
{
    printf("STATS: \n______\n");
    for (int stats_ID = 0; stats_ID < 4; stats_ID++)
    {
        displayStat(entityStatArray[stats_ID]);
    }
}

// TEST FUNCTION

void testStat()
{
    printf("\n==============================\n\tTEST DE STAT\n==============================\n");
    stat_t **test = initEntityStatFromArray((int[4][2]){
        {75, true},
        {999, true},
        {3, true},
        {100, true},
    });
    displayEntityStatArray(test);
    freeEntityStatArray(test);
}

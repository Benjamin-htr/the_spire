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
    int **tmpStats;
    tmpStats = calloc(2, sizeof *tmpStats);
    for (int currOrMax = 0; currOrMax < 2; currOrMax++)
    {
        tmpStats[currOrMax] = calloc(2, sizeof *(tmpStats[currOrMax]));
        if (currOrMax || !currZero)
        {
            for (size_t temporalRange = 0; temporalRange < 2; temporalRange++)
            {
                tmpStats[currOrMax][temporalRange] = max;
            }
        }
    }
    stat->stat_bar = tmpStats;
    return stat;
}

stat_t *initEntityStatFromArray(int stats[][2])
{
    stat_t *res = calloc(5, sizeof(stat_t *));
    for (size_t stats_ID = 0; stats_ID < 4; stats_ID++)
    {
        res[stats_ID] = *initStat(stats_ID + 1, stats[stats_ID][0], stats[stats_ID][1]);
    };
    return res;
}

// METHOD

void updateStat(stat_t *stat, int value, statCurrOrMax currOrMax, statTemporalRange temporalRange)
{
    stat->stat_bar[currOrMax][temporalRange] += value;
}

// DISPLAY FUNCTION

void displayStat(stat_t stat)
{
    printf(
        "\n%s\nw/O Effect : \t%d/%d\nw Effect : \t%d/%d\n",
        STAT_NAME[stat.id],
        stat.stat_bar[CURR][PERCISTANT],
        stat.stat_bar[MAX][PERCISTANT],
        stat.stat_bar[CURR][TEMPORARY],
        stat.stat_bar[MAX][TEMPORARY]);
}

// TEST FUNCTION

void testStat()
{
    stat_t *test = initStat(MANA, 20, false);
    displayStat(*test);
}

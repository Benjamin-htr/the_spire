#include "entity.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// CONSTRUCTORS
entity_t *initEntity(
    char *name,
    int stats[][2],
    int items[],
    int itemslength,
    int cards[][2],
    int diffCardSize,
    char *spriteName,
    int nbSpritePerLine,
    ENEMY_TYPE enemyType)
// for cards param first int is the card id 2nd one is the number of card
// for diffCardSize param is just the size of the array
{
    entity_t *res = malloc(sizeof(entity_t));
    res->name = name;
    res->enemyType = enemyType;
    res->stats = initEntityStatFromArray(stats);
    res->effects = initEntityEffectArray();
    res->cardDeck = createDeckFromArray(cards, diffCardSize);
    importEntityItemFromIdArray(res, itemslength, items);
    res->spriteName = spriteName;
    res->nbSpritePerLine = nbSpritePerLine;
    return res;
}

// free entity Memory
void freeEntity(entity_t *entity)
{
    freeEntityStatArray(entity->stats);
    freeEntityEffectArray(entity->effects);
    freeDeckListAndCard(entity->cardDeck);
    freeEntityItem(entity->items);
    free(entity);
}

// import a caracter from global array
entity_t *importCaracter(entity_import entitySkel)
{
    return initEntity(
        entitySkel.name,
        entitySkel.stats,
        entitySkel.items,
        entitySkel.itemslength,
        entitySkel.cardDeck,
        entitySkel.diffCardSize,
        entitySkel.spriteName,
        entitySkel.nbSpritePerLine,
        BOSS);
}

// import a caracter from enum Id
entity_t *importCaracterFromId(CARACTER_ID id)
{
    return importCaracter(CARATER_ENCYCLOPEDIA[id]);
}

// import an enemy from global array
entity_t *importEnemy(enemy_import enemySkel, ENEMY_TYPE enemyType)
{
    int hp =
        enemySkel.hpRange[0] + (rand() %
                                (enemySkel.hpRange[1] -
                                 enemySkel.hpRange[0]));
    int stats[5][2] = {
        {hp, false},
        {999, true},
        {999, false},
        {999, false},
    };
    return initEntity(
        enemySkel.name,
        stats,
        enemySkel.items,
        enemySkel.itemslength,
        enemySkel.cardDeck,
        enemySkel.diffCardSize,
        enemySkel.spriteName,
        enemySkel.nbSpritePerLine,
        enemyType);
}

// import an enemy of phase 1 from enum Id
entity_t *importEnemyPhase1FromId(ENEMY_PHASE_1_ID id)
{
    return importEnemy(ENEMY_PHASE_1_ENCYCLOPEDIA[id], COMMON_ENEMY);
}

// import a random enemy of phase 1
entity_t *getRandomEnemyPhase1()
{
    return importEnemyPhase1FromId(rand() % ENEMY_PHASE_1_ID_SIZE);
}

// import an enemy of phase 2 from enum Id
entity_t *importEnemyPhase2FromId(ENEMY_PHASE_2_ID id)
{
    return importEnemy(ENEMY_PHASE_2_ENCYCLOPEDIA[id], COMMON_ENEMY);
}

// import a random enemy of phase 2
entity_t *getRandomEnemyPhase2()
{
    return importEnemyPhase2FromId(rand() % ENEMY_PHASE_2_ID_SIZE);
}

// import a boss from enum Id
entity_t *importMiniBossFromId(MINIBOSS_ID id)
{
    return importEnemy(MINIBOSS_ENCYCLOPEDIA[id], MINIBOSS);
}

// import a random  mini boss
entity_t *getRandomMiniBoss()
{
    return importMiniBossFromId(rand() % MINIBOSS_ID_SIZE);
}
// import a boss from enum Id
entity_t *importBOSSFromId(BOSS_ID id)
{
    return importEnemy(BOSS_ENCYCLOPEDIA[id], BOSS);
}

// DISPLAY FUNCTION

void displayEntity(entity_t *entity)
{
    printf("NAME:\n_____\n%s\n", entity->name);
    displayEntityStatArray(entity->stats);
    displayEntityEffectArray(entity->effects);
    displayEntityItems(entity->items);
    printf("DECK: \n_____\n");
    displayDeck(entity->cardDeck);

    printf("============================================\n");
}

// METHODS
// variant of merge effect for the HP effect with negativ value
void takeDamage(entity_t *entity, int value)
{
    stat_t *entityDodge = getEntityStat(entity, DODGE);
    stat_t *entityHealth = getEntityStat(entity, HP);
    if (-value >= entityDodge->current)
    {
        value += entityDodge->current;
        entityDodge->current = 0;
        updateStat(entityHealth, value, false);
    }
    else
    {
        updateStat(entityDodge, value, false);
    }
}

// apply card effect to the launcher and the receiver
void applyCardEffect(card_t *card, entity_t *launcher, entity_t *receiver)
{
    printf("la carte %s est joué\n", card->name);
    for (int launcherEffectID = 0; launcherEffectID < card->launcherEffectsSize; launcherEffectID++)
    {
        mergeEffect(launcher, card->launcherEffects[launcherEffectID]);
    }
    for (int receiverEffectID = 0; receiverEffectID < card->receiverEffectsSize; receiverEffectID++)
    {
        mergeEffect(receiver, card->receiverEffects[receiverEffectID]);
    }
}

// apply all entity item effect to the entity
void applyAllItemsEffect(entity_t *launcher, entity_t *receiver)
{
    for (int itemsIdx = 0; itemsIdx < 5 && launcher->items[itemsIdx]->description != NULL; itemsIdx++)
    {
        for (int itemEffectIdx = 0; itemEffectIdx < launcher->items[itemsIdx]->launcherEffectsSize; itemEffectIdx++)
        {
            if (
                launcher->items[itemsIdx]->launcherEffects[itemEffectIdx]->id < HP_MAX_E ||
                launcher->items[itemsIdx]->launcherEffects[itemEffectIdx]->id > ENERGY_MAX_E)
            {
                mergeEffect(launcher, launcher->items[itemsIdx]->launcherEffects[itemEffectIdx]);
            }
        }
        for (int itemEffectIdx = 0; itemEffectIdx < launcher->items[itemsIdx]->receiverEffectsSize; itemEffectIdx++)
        {
            if (
                launcher->items[itemsIdx]->launcherEffects[itemEffectIdx]->id < HP_MAX_E ||
                launcher->items[itemsIdx]->launcherEffects[itemEffectIdx]->id > ENERGY_MAX_E)
            {
                mergeEffect(receiver, launcher->items[itemsIdx]->receiverEffects[itemEffectIdx]);
            }
        }
    }
}

// item related : when adding an item that have effect on max
void applyItemMaxEffect(entity_t *launcher, item_t *item)
{
    for (int itemEffectIdx = 0; itemEffectIdx < item->launcherEffectsSize; itemEffectIdx++)
    {

        if (
            item->launcherEffects[itemEffectIdx]->id >= HP_MAX_E &&
            item->launcherEffects[itemEffectIdx]->id <= ENERGY_MAX_E)
        {
            mergeEffect(launcher, item->launcherEffects[itemEffectIdx]);
        }
    }
}

// item related : when removing an item that have effect on max
void removeItemMaxEffect(entity_t *launcher, item_t *item)
{
    for (int itemEffectIdx = 0; itemEffectIdx < item->launcherEffectsSize; itemEffectIdx++)
    {

        if (
            item->launcherEffects[itemEffectIdx]->id >= HP_MAX_E &&
            item->launcherEffects[itemEffectIdx]->id <= ENERGY_MAX_E)
        {
            mergeEffect(launcher, item->launcherEffects[itemEffectIdx]);
        }
    }
}

// put all effect value at 0( used at end of combat )
void wipeAllEffect(entity_t *entity)
{
    for (int effectIdx = 0; effectIdx < 5; effectIdx++)
    {
        entity->effects[effectIdx]->value = 0;
    }
}

// decrease effect value and apply fire effect
void turnBeginEffectUpdate(entity_t *entity)
{
    effect_t *entityFIRE = getEntityEffect(entity, FIRE_E);
    effect_t *entityWEAK = getEntityEffect(entity, WEAK_E);
    effect_t *entitySLOW = getEntityEffect(entity, SLOW_E);
    takeDamage(entity, -entityFIRE->value);
    entityFIRE->value = floor(entityFIRE->value / 2.0);
    if (entitySLOW->value > 0)
    {
        (entitySLOW->value)--;
    }
    if (entityWEAK->value > 0)
    {
        (entityWEAK->value)--;
    }
}

// return the value of the card after apply entity effect
void updateCardEffectWithEntityEffect(entity_t *entity, effect_t *cardEffect)
{
    effect_t *entityStr = getEntityEffect(entity, STR_E);
    effect_t *entityDex = getEntityEffect(entity, DEX_E);
    effect_t *entityWeak = getEntityEffect(entity, WEAK_E);
    effect_t *entitySlow = getEntityEffect(entity, SLOW_E);
    if (cardEffect->id == DODGE_E && cardEffect->value > 0)
    {
        cardEffect->value = ceil(((cardEffect->value + entityDex->value) / (entitySlow->value ? 2.0 : 1.0)));
    }
    else if (cardEffect->id == HP_E && cardEffect->value < 0)
    {
        cardEffect->value = floor(((cardEffect->value - entityStr->value) * (entityWeak->value ? 0.75 : 1.0)));
    }
}

// apply effect to the entity
void mergeEffect(entity_t *entity, effect_t *effect)
{
    if (effect->id == HP_E && effect->value < 0)
    {
        takeDamage(entity, effect->value);
    }
    else if (effect->id < 4)
    {
        stat_t *currentStat = getEntityStat(entity, effect->id + 1);
        updateStat(currentStat, effect->value, false);
    }
    else if (effect->id < 8)
    {
        stat_t *currentStat = getEntityStat(entity, effect->id - 3);
        updateStat(currentStat, effect->value, true);
    }
    else
    {
        effect_t *currentEffect = getEntityEffect(entity, effect->id);
        currentEffect->value += effect->value;
    }
}

// GETTER / SETTER

//      GETTER

// return entity stat from stat id
stat_t *getEntityStat(entity_t *entity, stat_ID statId)
{
    return entity->stats[statId - 1];
}

// return Entity effect from effect id
effect_t *getEntityEffect(entity_t *entity, effect_ID id)
{
    if (id < 3)
    {
        return NULL;
    }
    return entity->effects[id - 8];
}

// return the card after apply entity effect
card_t *getTrueCardValue(entity_t *entity, card_t *card) // On oublie pas de free la carte retourné une fois l'utilisation fini
{
    card_t *res = copyCard(card);
    effect_t *cardEffect;
    for (int launcherEffectID = 0; launcherEffectID < res->launcherEffectsSize; launcherEffectID++)
    {
        cardEffect = res->launcherEffects[launcherEffectID];
        updateCardEffectWithEntityEffect(entity, cardEffect);
    }
    for (int receiverEffectID = 0; receiverEffectID < res->receiverEffectsSize; receiverEffectID++)
    {
        cardEffect = res->receiverEffects[receiverEffectID];
        updateCardEffectWithEntityEffect(entity, cardEffect);
    }
    return res;
}

// return a semi random item that the entity dont have
int getRandomUniqueItemId(entity_t *entity)
{
    int itemId;
    do
    {
        itemId = (rand() % (SHOES));
    } while (entity->items[itemId]->description != NULL);
    return itemId + 1;
}

//      SETTER

// ITEM RELATED
// import an entire array of item in the entity
void importEntityItemFromIdArray(entity_t *entity, int itemLength, int itemsId[itemLength])
{
    entity->items = createEmptyEntityItemList();
    for (int item_ID = 0; item_ID < itemLength; item_ID++)
    {
        addItemtoEntityItemList(entity, itemsId[item_ID]);
    };
}

// import a item and add max stat
void addItemtoEntityItemList(entity_t *entity, ITEM_ENCYCLOPEDIA_ID id)
{
    if (entity->items[id - 1]->description == NULL)
    {
        freeItem(entity->items[id - 1]);
        entity->items[id - 1] = importItemFromId(id);
        applyItemMaxEffect(entity, entity->items[id - 1]);
    }
}

// TEST

void testGetterSetter(entity_t *testCar)
{
    // GETTER TESTS

    printf("TEST GETTER/SETTER\n==================\n");
    printf("TEST getEntityStat\n____________");
    stat_t *testStatGetter = getEntityStat(testCar, DODGE);
    displayStat(testStatGetter);

    printf("TEST getEntityEffect\n______________");
    effect_t *testEffectGetter = getEntityEffect(testCar, FIRE_E);
    displayEffect(testEffectGetter);

    effect_t *testEffect = initEffect(STR_E, 10);
    mergeEffect(testCar, testEffect);
}

void testApplyCardEffect(entity_t *testCar, entity_t *testEnemy)
{
    printf("\nTEST applyCard\n==============");

    card_t *testCard = importCardFromId(SPECTRUM);
    displayEntity(testCar);
    displayEntity(testEnemy);
    applyCardEffect(testCard, testCar, testEnemy);
    printf("\napply effect\n");
    displayEntity(testCar);
    displayEntity(testEnemy);
}
void testEntity()
{
    printf("\n==============================\n\tTEST DE L'ENTITY\n==============================\n");
    entity_t *testCar = importCaracterFromId(TEST_CAR);
    // entity_t *testPeter = getRandomEnemyPhase1();
    // getEntityStat(testCar, DODGE)->current = 3;
    displayEntity(testCar);
    // replaceCardWithOther(&testCar->cardDeck, STRIKE, SPECTRUM);
    // applyAllItemsEffect(testCar, testCar);
    // takeDamage(testCar, -10);
    displayEntity(testCar);
    // displayEntity(testPeter);
    freeEntity(testCar);
    // freeEntity(testPeter);
    // entity_t *testCar2 = importCaracterFromId(PETER);
    // // entity_t *testEnemy = getRandomMiniBoss();
    // displayEntity(testCar2);
    // applyAllItemsEffect(testCar);
    // displayEntity(testCar);
    // wipeAllEffect(testCar);
    // displayEntity(testCar);

    // testGetterSetter(testCar);
    // testApplyCardEffect(testCar, testCar);
}

entity_import CARATER_ENCYCLOPEDIA[] = {
    {
        .name = "Peter",
        .stats = {
            {100, false},
            {999, true},
            {100, false},
            {3, true},
        },
        .items = {
            LUNCH_BOX,
        },
        .itemslength = 1,
        .cardDeck = {
            {STRIKE, 5},
            {DODGE_A, 5},
            {SPECTRUM, 1},
        },
        .diffCardSize = 3,
        .spriteName = "",
        .nbSpritePerLine = 4,
    },
    {
        .name = "Tester",
        .stats = {
            {10000, false},
            {999, true},
            {100, false},
            {3, true},
        },
        .items = {
            LUNCH_BOX,
            ARMOR,
            WEAPON,
            HELMET,
            SHOES,
        },
        .itemslength = 5,
        .cardDeck = {
            {STRIKE, 5},
            {DODGE_A, 5},
            {SPECTRUM, 1},
        },
        .diffCardSize = 3,
        .spriteName = "",
        .nbSpritePerLine = 4,
    },
};
enemy_import ENEMY_PHASE_1_ENCYCLOPEDIA[] = {
    {
        .name = "Jawurm",
        .hpRange = {40, 44},
        .cardDeck = {
            {JAWURM_BACKSTAB, 1},
            {JAWURM_FIST, 1},
            {JAWURM_CROUCH, 1},
        },
        .itemslength = 0,
        .diffCardSize = 3,
        .spriteName = "jawurm.png",
        .nbSpritePerLine = 4,
    },
    {
        .name = "Blouni",
        .hpRange = {46, 50},
        .cardDeck = {
            {BLOUNI_JAB, 1},
            {BLOUNI_KICK, 1},
        },
        .itemslength = 0,
        .diffCardSize = 2,
        .spriteName = "blouni.png",
        .nbSpritePerLine = 4,
    },
    {
        .name = "Keliko",
        .hpRange = {60, 70},
        .cardDeck = {
            {KELIKO_NUDGE, 1},
            {KELIKO_PINCH, 1},
        },
        .itemslength = 0,
        .diffCardSize = 2,
        .spriteName = "keliko.png",
        .nbSpritePerLine = 4,
    },
};
enemy_import ENEMY_PHASE_2_ENCYCLOPEDIA[] = {
    {
        .name = "Jawurm2",
        .hpRange = {55, 62},
        .cardDeck = {
            {JAWURM2_HAIRPULLING, 1},
            {JAWURM2_SPIT, 1},
        },
        .itemslength = 0,
        .diffCardSize = 2,
        .spriteName = "jawurm2.png",
        .nbSpritePerLine = 4,
    },
    {
        .name = "Redoni",
        .hpRange = {50, 54},
        .cardDeck = {
            {REDONI_SLAP, 1},
            {REDONI_SPANKING, 1},
        },
        .itemslength = 0,
        .diffCardSize = 2,
        .spriteName = "redoni.png",
        .nbSpritePerLine = 4,
    },
    {
        .name = "Mangoustine",
        .hpRange = {70, 80},
        .cardDeck = {
            {MANGOUSTINE_SNARE, 1},
        },
        .itemslength = 0,
        .diffCardSize = 1,
        .spriteName = "mangoustine.png",
        .nbSpritePerLine = 4,
    },
};
enemy_import MINIBOSS_ENCYCLOPEDIA[] = {
    {
        .name = "Eldan",
        .hpRange = {80, 81},
        .cardDeck = {
            {ELDAN_BANANAPEEL, 1},
            {HEADBUTT, 1},
            {ELDAN_TOTAL, 1},
        },
        .itemslength = 0,
        .diffCardSize = 3,
        .spriteName = "eldan.png",
        .nbSpritePerLine = 4,
    },
    {
        .name = "Pyrox",
        .hpRange = {120, 121},
        .cardDeck = {
            {PYROX_FIRESPIT, 1},
            {PYROX_FIRESTORM, 1},
        },
        .itemslength = 0,
        .diffCardSize = 2,
        .spriteName = "pyrox.png",
        .nbSpritePerLine = 4,
    },
};
enemy_import BOSS_ENCYCLOPEDIA[] = {
    {
        .name = "Gardien de la plume",
        .hpRange = {150, 151},
        .cardDeck = {
            {SPECTRUM, 1},
            {PULVERIZE, 1},
            {CLAW_COMBO, 1},
            {SLEEP, 1},
        },
        .itemslength = 0,
        .diffCardSize = 4,
        .spriteName = "feather_guardian.png",
        .nbSpritePerLine = 14,
    },
};

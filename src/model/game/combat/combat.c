#include "combat.h"
#include <stdlib.h>
#include <stdio.h>
#include "time.h"

// INIT FUNCTIOn
combat_t *startCombat(entity_t *caracter, entity_t *enemy)
{
    combat_t *combat = malloc(sizeof(combat_t));
    combat->caracter = caracter;
    combat->enemy = enemy;
    combat->caracterTurn = 1;
    combat->caracter->board = createBoard(combat->caracter->cardDeck);
    combat->enemy->board = createBoard(combat->enemy->cardDeck);
    return combat;
}

// free the memory of combat
void freeCombat(combat_t *combat)
{
    freeBoard(combat->caracter->board);
    freeBoard(combat->enemy->board);
    wipeAllEffect(combat->caracter);
    free(combat);
}

// function that play the card
void playOneCard(entity_t *launcher, entity_t *receiver, card_t *cardToPlay)
{
    // TODO: add stat;
    stat_t *entityMana = getEntityStat(launcher, MANA);
    stat_t *entityEnergy = getEntityStat(launcher, ENERGY);

    if (cardToPlay->energyCost <= entityEnergy->current && cardToPlay->manaCost <= entityMana->current)
    { // TODO : faire une fonction pour récupérer la valeur d'une stat
        card_t *afterEffectCard = getTrueCardValue(launcher, cardToPlay);
        applyCardEffect(afterEffectCard, launcher, receiver);
        freeCard(afterEffectCard);
        updateStat(entityMana, -(cardToPlay->manaCost), false);
        updateStat(entityEnergy, -(cardToPlay->energyCost), false);
        moveOneCardFromHand(launcher->board, cardToPlay);
    }
    // else
    // {
    //     printf("YOUR MANA:\t%d\nYOUR ENERGY:\t%d\n", entityMana->current, entityEnergy->current);
    //     displayCard(cardToPlay);
    //     printf("Plus d'énergie dispo\n");
    // }
}

// function that make the enemy play is turn
card_t *playEnemyCards(combat_t *combat)
{
    board_t *boardToCheck;
    card_t *card;
    boardToCheck = combat->enemy->board;
    card = pickCardFromHand(combat->enemy, chooseRandomCardId);
    playOneCard(combat->enemy, combat->caracter, card);
    moveCardsFromHand(boardToCheck, false); // on déplace les cartes restantes de la main vers défausse/abysse
    return card;
}

int getChoosenCardId(board_t *board)
{ // pour l'instant on prend juste une random blc
    return (rand() % (getDeckSize(board->hand) + 1)) - 1;
}

int getChoosenRewardCardId(deck_t *deck)
{ // pour l'instant on prend juste une random blc
    return (rand() % (getDeckSize(deck) + 1)) - 1;
}

int chooseRandomCardId(board_t *board)
{ // pour l'instant on prend juste une random blc
    return rand() % (getDeckSize(board->hand));
}
// function that ask to choose a card until the card choosen is playable by the entity
card_t *pickCardFromHand(entity_t *caracter, int (*cardChoosingFunc)(board_t *))
{
    int handSize = getDeckSize(caracter->board->hand);
    stat_t *caracterMana = getEntityStat(caracter, MANA);
    stat_t *caracterEnergy = getEntityStat(caracter, ENERGY);
    int safety = 0;
    for (; safety < handSize; safety++) {
        int idx = cardChoosingFunc(caracter->board);
        deck_t *deckPicked = getElementFromDeckAtIndex(idx, caracter->board->hand);
        if (deckPicked == NULL || deckPicked->data == NULL)
            continue;
        if (deckPicked->data->energyCost <= caracterEnergy->current && deckPicked->data->manaCost <= caracterMana->current)
            return deckPicked->data;
    }
    // No playable card found
    return NULL;
}

// TEST FUNCTION
void testCombat()
{
    printf("\n==============================\n\tTEST COMBAT\n==============================\n");
}

// function that init each entity turn
void playTurn(entity_t *entity)
{
    getEntityStat(entity, DODGE)->current = 0;
    stat_t *entityHealth = getEntityStat(entity, HP);
    // on applique l'effet de feu et diminue la valeur des effets le necessitant
    turnBeginEffectUpdate(entity);
    // au cas ou l'entité meurt a cause des ticks de feu
    if (entityHealth->current <= 0)
    {
        return;
    }
    // On recharge l'energy pour le debut du tour
    refillStat(getEntityStat(entity, ENERGY));
    // On pioche au debut du tour
    drawCardsFromDeckWithRefillFromDiscard(entity->board);
}

// function to see if any entity of the combat is deads
boolean checkEndCombat(combat_t *combat)
{

    stat_t *caracterHealth = getEntityStat(combat->caracter, HP);
    stat_t *enemyHealth = getEntityStat(combat->enemy, HP);
    return (caracterHealth->current <= 0 || enemyHealth->current <= 0);
}

// function to see if the caracter isn't dead a the end of the fight
boolean checkVictory(combat_t *combat)
{
    stat_t *caracterHealth = getEntityStat(combat->caracter, HP);
    stat_t *enemyHealth = getEntityStat(combat->enemy, HP);
    return (caracterHealth->current > 0 && enemyHealth->current <= 0);
}

// Init Combat and apply needed update to the entity
void startFight(combat_t *combat)
{
    // items
    applyAllItemsEffect(combat->caracter, combat->enemy);
    applyAllItemsEffect(combat->enemy, combat->caracter);
    // stat
    refillStat(getEntityStat(combat->caracter, MANA));
    refillStat(getEntityStat(combat->caracter, MANA));
    refillStat(getEntityStat(combat->caracter, ENERGY));
    // first card drawing
    drawCardsFromDeckWithRefillFromDiscard(combat->caracter->board);
}
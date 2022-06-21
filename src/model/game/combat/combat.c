#include "combat.h"
#include <stdlib.h>
#include <stdio.h>
#include "time.h"
int tmpChooseItem() { return 0; } // TOREMOVE
int (*chooseItemReward)(item_t *) = &tmpChooseItem;
int (*chooseCardReward)(deck_t *) = &getChoosenRewardCardId;
int (*chooseCardFromHand)(board_t *) = &getChoosenCardId;

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

void freeCombat(combat_t *combat)
{
    freeBoard(combat->caracter->board);
    freeBoard(combat->enemy->board);
    wipeAllEffect(combat->caracter);
    free(combat);
}

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
    else
    {
        printf("YOUR MANA:\t%d\nYOUR ENERGY:\t%d\n", entityMana->current, entityEnergy->current);
        displayCard(cardToPlay);
        printf("plus d'energie enculer \n");
    }
}

void playEnemyCards(combat_t *combat)
{
    board_t *boardToCheck;
    card_t *card;
    // if (combat->caracterTurn == 1)
    // {
    //     boardToCheck = combat->caracter->board;
    //     while (getDeckSize(boardToCheck->hand) > 0)
    //     {
    //         card = pickCardFromHand(combat->caracter, chooseCardFromHand);
    //         if (card == NULL)
    //         {
    //             // On passe si l'id de la carte choisi == -1
    //             printf("\nJE PASSE MON TOUR");
    //             break;
    //         }
    //         playOneCard(combat->caracter, combat->enemy, card);
    //     }
    // }
    // else
    // {
    boardToCheck = combat->enemy->board;
    card = pickCardFromHand(combat->enemy, chooseRandomCardId); // CA VIENT SUREMENT DE LA
    playOneCard(combat->enemy, combat->caracter, card);         // CA VIENT DE LA
    // }
    // printf("\n\t\tJ'ai fini de jouer\n");
    moveCardsFromHand(boardToCheck); // on déplace les cartes restantes de la main vers défausse/abysse
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

// int getChoosenCardIdGUI(deck_t* hand){

// }

card_t *pickCardFromHand(entity_t *caracter, int (*cardChoosingFunc)(board_t *))
{
    deck_t *deckPicked;
    stat_t *caracterMana = getEntityStat(caracter, MANA);
    stat_t *caracterEnergy = getEntityStat(caracter, ENERGY);
    do
    {
        deckPicked = getElementFromDeckAtIndex(cardChoosingFunc(caracter->board), caracter->board->hand);
        if (deckPicked == NULL)
        {
            return NULL;
        }
    } while (deckPicked->data->energyCost > caracterEnergy->current || deckPicked->data->manaCost > caracterMana->current);
    return deckPicked->data;
}

void testCombat()
{
    printf("\n==============================\n\tTEST COMBAT\n==============================\n");
    entity_t *player = importCaracterFromId(TEST_CAR);
    // Test peter perd
    // getEntityStat(player, HP)->current = 3;
    entity_t *enemy = getRandomEnemyPhase1(); // importEnemyPhase1FromId(JAWURM);
    combat_t *combat = startCombat(player, enemy);
    startFight(combat);
    freeEntity(player);
    freeEntity(enemy);
}

void playTurn(entity_t *entity)
{
    stat_t *entityHealth = getEntityStat(entity, HP);
    // on applique l'effet de feu et diminue la valeur des effets le necessitant
    turnBeginEffectUpdate(entity);
    // au cas ou l'entité meurt a cause des ticks de feu
    if (entityHealth->current <= 0)
    {
        return;
    }
    getEntityStat(entity, DODGE)->current = 0;
    // On recharge l'energy pour le debut du tour
    refillStat(getEntityStat(entity, ENERGY));
    // On pioche au debut du tour
    drawCardsFromDeckWithRefillFromDiscard(entity->board);
    // On joue des cartes
    // playCards(combat);
}

boolean checkEndCombat(combat_t *combat)
{

    stat_t *caracterHealth = getEntityStat(combat->caracter, HP);
    stat_t *enemyHealth = getEntityStat(combat->enemy, HP);
    return (caracterHealth->current <= 0 || enemyHealth->current <= 0);
}

void startFight(combat_t *combat)
{
    stat_t *caracterHealth = getEntityStat(combat->caracter, HP);
    stat_t *enemyHealth = getEntityStat(combat->enemy, HP);
    stat_t *caracterDodge = getEntityStat(combat->caracter, DODGE);
    stat_t *enemyDodge = getEntityStat(combat->enemy, DODGE);
    int demiturn = 0;
    applyAllItemsEffect(combat->caracter, combat->enemy);
    applyAllItemsEffect(combat->enemy, combat->caracter);
    refillStat(getEntityStat(combat->caracter, MANA));
    refillStat(getEntityStat(combat->caracter, MANA));
    refillStat(getEntityStat(combat->caracter, ENERGY));
    drawCardsFromDeckWithRefillFromDiscard(combat->caracter->board);
    // while (caracterHealth->current > 0 && enemyHealth->current > 0)
    // {
    //     if (!(demiturn % 2))
    //     {
    //         printf("\n\t==========================\n*****************TOUR N°%d****************\n\t==========================\n", demiturn / 2);
    //     }
    //     printf("\n***DEBUT DU TOUR DE %s***\n", (demiturn % 2) ? combat->enemy->name : combat->caracter->name);
    //     playTurn(combat, ((combat->caracterTurn) ? (combat->caracter) : (combat->enemy)));
    //     printf("\n***FIN DU TOUR DE %s***\n", (demiturn % 2) ? combat->enemy->name : combat->caracter->name);
    //     printf("%s:\n", combat->caracter->name);
    //     displayStat(caracterHealth);
    //     displayStat(caracterDodge);
    //     displayEntityEffectArray(combat->caracter->effects);
    //     printf("%s:\n", combat->enemy->name);
    //     displayStat(enemyHealth);
    //     displayStat(enemyDodge);
    //     displayEntityEffectArray(combat->enemy->effects);
    //     combat->caracterTurn = !combat->caracterTurn;
    //     demiturn++;
    // }
    // if (caracterHealth->current > 0 && enemyHealth->current <= 0)
    // {
    //     // LE JOUEUR GAGNE
    //     printf("PETER GAGNE!");
    //     deck_t *rewardDeck = createRewardDeck();
    //     displayDeck(rewardDeck);
    //     tranferOneCardBetweenDeck(
    //         &rewardDeck,
    //         &(combat->caracter->cardDeck),
    //         chooseCardReward(rewardDeck));
    //     displayDeck(combat->caracter->cardDeck);
    //     freeDeckListAndCard(rewardDeck);
    //     if (combat->enemy->enemyType != COMMON_ENEMY)
    //     {
    //         int rewardItemId;
    //         item_t *rewardItem;
    //         if (combat->enemy->enemyType == MINIBOSS)
    //         {
    //             rewardItemId = getRandomUniqueItemId(combat->caracter);
    //             rewardItem = importItemFromId(rewardItemId);
    //             displayItem(rewardItem);
    //             if (chooseItemReward(rewardItem) != -1)
    //             {
    //                 addItemtoEntityItemList(combat->caracter, rewardItemId);
    //             }
    //             freeItem(rewardItem);
    //         }
    //         // else
    //         // {
    //         //     rewardItem = importItemFromId()
    //         // }
    //     }
    // }
    // else
    // {
    //     // LE JOUEUR PERD
    //     printf("PETER PERD");
    // }
    // freeCombat(combat);
}
#include "deck.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "time.h"

deck_t *createDeck(card_t *myCard)
{
    deck_t *res = malloc(sizeof(deck_t));
    res->next = NULL;
    res->data = myCard;
    return res;
}

void freeDeck(deck_t *deck)
{
    free(deck);
}

void freeDeckListAndCard(deck_t *deckList)
{
    deck_t *toFree = deckList;
    deck_t *next;
    while (toFree != NULL)
    {
        next = toFree->next;
        // TODO FREE CARD
        if (toFree->data != NULL)
        {
            freeCard(toFree->data);
        }
        freeDeck(toFree);
        toFree = next;
    }
}

deck_t *createDeckFromArray(int cardIds[][2], int diffCardNumber)
// first int is the card id 2nd one is the number of card
// 2nd Param is just the size of the array
{
    deck_t *res = createDeck(NULL);
    for (int cardIdx = 0; cardIdx < diffCardNumber; cardIdx++)
    {
        for (int cardExemplar = 0; cardExemplar < cardIds[cardIdx][1]; cardExemplar++)
        {
            addCard(res, importCardFromId(cardIds[cardIdx][0]));
        }
    }
    return res;
}

int getPositionOfCard(deck_t *deck, char *cardName)
{
    int i = 0;
    while (deck != NULL && deck->data != NULL)
    {
        card_t *card = deck->data;
        if (strcmp(card->name, cardName) == 0)
        {
            return i;
        }
        deck = deck->next;
        i++;
    }
    return -1;
}

void addCard(deck_t *deck, card_t *card)
{
    if (deck == NULL)
    {
        deck = createDeck(card);
        return;
    }
    else
    {
        while (deck->next != NULL && deck->data != NULL)
        {
            deck = deck->next;
        }
        if (deck->data == NULL)
        {
            deck->data = card;
            return;
        }
        deck->next = createDeck(card);
    }
}

card_t *removeFirstCard(deck_t **deck)
{
    if (getDeckSize(*deck) > 0)
    {
        deck_t *tmp = *deck;
        card_t *cardToReturn = tmp->data;
        *deck = (*deck)->next;
        free(tmp);
        return cardToReturn;
    }
    return NULL;
    // if (deck == NULL)
    //     return createDeck(NULL);
    // deck = deck->next;
    // if (deck == NULL)
    // {
    //     deck = createDeck(NULL);
    // }
    // return deck;
}

card_t *removeCardatIndex(deck_t **deck, int cardIdx)
{
    if (cardIdx == 0)
    {
        return removeFirstCard(deck);
    }
    else
    {
        deck_t *current = *deck;
        int incrementor = 1;
        while (current->next != NULL)
        {
            if (incrementor == cardIdx)
            {
                deck_t *tmp = current->next;
                card_t *cardToReturn = tmp->data;
                current->next = current->next->next;
                free(tmp);
                return cardToReturn;
            }
            incrementor++;
            current = current->next;
        }
    }
    return NULL;
}

card_t *removeCard(deck_t **deck, char *cardName)
{
    int pos = getPositionOfCard(*deck, cardName);
    if (pos == -1)
    {
        printf("La carte n'est pas dans le deck \n");
        fflush(stdout);
    }
    else
    {
        return removeCardatIndex(deck, pos);
    }
    return NULL;
}

card_t *draw(deck_t *deck)
{
    card_t *res = deck->data;
    removeFirstCard(&deck);
    return res;
}

deck_t *getElementFromDeckAtIndex(int idx, deck_t *deck)
{
    int i = 0;
    while (deck != NULL)
    {
        if (i == idx)
        {
            return deck;
        }
        deck = deck->next;
        i++;
    }
    return deck;
}

int getDeckSize(deck_t *deck)
{
    int res = 0;
    while (deck != NULL && deck->data != NULL)
    {
        res++;
        deck = deck->next;
    }
    return res;
}

void swapElements(deck_t *deck, int n, int m)
{
    deck_t *myDeckAtM = getElementFromDeckAtIndex(m, deck);
    deck_t *myDeckAtN = getElementFromDeckAtIndex(n, deck);
    deck_t *tmp;
    if (myDeckAtM->next != NULL && myDeckAtN->next != NULL)
    {
        // on remplace les befores
        tmp = myDeckAtM->next;
        myDeckAtM->next = myDeckAtN->next;
        myDeckAtN->next = tmp;
        // on remplace les nexts
        tmp = myDeckAtM->next->next;
        myDeckAtM->next->next = myDeckAtN->next->next;
        myDeckAtN->next->next = tmp;
        // deck_t *myDeckBeforeN = getElementFromDeckAtIndex(n - 1, deck);
        // deck_t *myDeckAtM = getElementFromDeckAtIndex(m, deck);
        // deck_t *myDeckAtN = getElementFromDeckAtIndex(n, deck);
        // deck_t *deckAfterM = getElementFromDeckAtIndex(m + 1, deck);
        // deck_t *deckAfterN = getElementFromDeckAtIndex(n + 1, deck);
        // deck_t *myDeckBeforeM = getElementFromDeckAtIndex(m - 1, deck);
        // myDeckBeforeN->next = myDeckAtM;
        // myDeckAtM->next = deckAfterN;
        // myDeckBeforeM->next = myDeckAtN;
        // myDeckAtN->next = deckAfterM;
    }
}

void shuffleDeck(deck_t **deck)
{
    int listSize = getDeckSize(*deck);

    int idDeck1;
    int idDeck2;
    for (int i = 0; i < listSize * 10; i++)
    {
        idDeck1 = rand() % listSize;
        idDeck2 = rand() % listSize;
        if (idDeck1 != idDeck2 && (idDeck2 - idDeck1 > 1 || idDeck1 - idDeck2 > 1))
        {
            swapElements(*deck, idDeck1, idDeck2);
        }
    }
    // ici on swap le premier element avec l'élement du milieu
    deck_t *beforeMiddleElement = getElementFromDeckAtIndex((rand() % (listSize - 3)) + 1, *deck);
    deck_t *middle = beforeMiddleElement->next;
    deck_t *firstNext = (*deck)->next;
    (*deck)->next = middle->next;
    middle->next = firstNext;
    beforeMiddleElement->next = *deck;
    // middleElement->next = deck;
    // beforeMiddleElement->next = deck;
    // deck->next = afterMiddleElement;
    // deck = middleElement;
    *deck = middle;
}

void testDeck()
{

    printf("\n==============================\n\tTEST DE DECK\n==============================\n");
    deck_t *myDeck = createDeckFromArray((int[][2]){{DODGE_A, 4}, {PULVERIZE, 3}, {DEFENSE, 3}, {JAWURM2_HAIRPULLING, 3}, {SPECTRUM, 3}}, 5);
    displayDeck(myDeck);
    removeFirstCard(&myDeck);
    displayDeck(myDeck);
    removeCardatIndex(&myDeck, 3);
    displayDeck(myDeck);
    shuffleDeck(&myDeck);
    displayDeck(myDeck);
    freeDeckListAndCard(myDeck);
}

void displayDeck(deck_t *myDeck)
{
    printf("\n=======================\n");
    int i = 0;
    if (myDeck == NULL || myDeck->data == NULL)
    {
        printf("The deck is empty \n");
    }
    while (myDeck != NULL && myDeck->data != NULL)
    {
        card_t *myCard = myDeck->data;
        printf("Element %d %s: \n", i, myCard->name);
        myDeck = myDeck->next;
        i++;
    }
    printf("=======================\n");
}

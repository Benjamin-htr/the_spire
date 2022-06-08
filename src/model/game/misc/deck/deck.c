#include "deck.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "card/card-global.h"
#include "time.h"

deck_t *createDeck(card_t *myCard)
{
    deck_t *res = malloc(sizeof(deck_t));
    res->next = NULL;
    res->data = myCard;
    return res;
}

void addCard(deck_t *myDeck, card_t *myCard)
{
    if (myDeck == NULL)
    {
        myDeck = createDeck(myCard);
        return;
    }
    while (myDeck->next != NULL)
    {
        myDeck = myDeck->next;
    }
    myDeck->next = createDeck(myCard);
}

deck_t *removeFirstCard(deck_t *myDeck)
{
    if (myDeck == NULL)
        return NULL;

    // Move the head pointer to the next node
    deck_t *temp = myDeck;
    myDeck = myDeck->next;
    free(temp);

    return myDeck;
}

deck_t *removeCard(deck_t *myDeck, char *cardName)
{
    deck_t *previousElement = NULL;
    deck_t *origin = myDeck;
    deck_t *tmp;
    while (myDeck != NULL)
    {
        card_t *myCard = myDeck->data;
        printf("%s \n ", myCard->name);
        if (strcmp(myCard->name, cardName) == 0)
        {
            if (previousElement == NULL)
            {
                myDeck = removeFirstCard(myDeck);
                origin = myDeck;
                card *myCard = myDeck->data;
            }
            else
            {
                previousElement->next = myDeck->next;
            }
        }
        previousElement = myDeck;
        myDeck = myDeck->next;
    }
    return origin;
}

card_t *draw(deck_t *myDeck)
{
    card_t *res = myDeck->data;
    myDeck = myDeck->next;
    return res;
}

deck_t *getElementFromDeckAtIndex(int idx, deck_t *myDeck)
{
    int i = 0;
    while (myDeck != NULL)
    {
        if (i == idx)
        {
            return myDeck;
        }
        myDeck = myDeck->next;
        i++;
    }
    return myDeck;
}

int size(deck_t *myDeck)
{
    int res = 0;
    while (myDeck != NULL)
    {
        res++;
        myDeck = myDeck->next;
    }
    return res;
}

void swapElements(deck_t *myDeck, int n, int m)
{
    if (n > 0 && m > 0)
    {
        deck_t *myDeckBeforeN = getElementFromDeckAtIndex(n - 1, myDeck);
        deck_t *myDeckAtM = getElementFromDeckAtIndex(m, myDeck);
        deck_t *myDeckAtN = getElementFromDeckAtIndex(n, myDeck);
        deck_t *deckAfterM = getElementFromDeckAtIndex(m + 1, myDeck);
        deck_t *deckAfterN = getElementFromDeckAtIndex(n + 1, myDeck);
        deck_t *myDeckBeforeM = getElementFromDeckAtIndex(m - 1, myDeck);
        myDeckBeforeN->next = myDeckAtM;
        myDeckAtM->next = deckAfterN;
        myDeckBeforeM->next = myDeckAtN;
        myDeckAtN->next = deckAfterM;
    }
}

deck_t *shuffleDeck(deck_t *myDeck)
{
    int listSize = size(myDeck);
    srand(time(NULL));
    int number1;
    int number2;
    for (int i = 0; i < listSize * 10; i++)
    {
        number1 = rand() % listSize;
        number2 = rand() % listSize;
        if (number1 != number2 && (number2 - number1 > 1 || number1 - number2 > 1))
        {
            swapElements(myDeck, number1, number2);
        }
    }
    // ici on swap le premier element avec l'élement du milieu
    deck_t *middleElement = getElementFromDeckAtIndex((int)(listSize / 2), myDeck);
    deck_t *beforeMiddleElement = getElementFromDeckAtIndex((int)(listSize / 2) - 1, myDeck);
    deck_t *afterMiddleElement = getElementFromDeckAtIndex((int)(listSize / 2) + 1, myDeck);
    middleElement->next = myDeck->next;
    beforeMiddleElement->next = myDeck;
    myDeck->next = afterMiddleElement;
    myDeck = middleElement;
    return myDeck;
}

int main()
{
    deck_t *myDeck = createDeck(&CARD_DODGE);
    addCard(myDeck, &PULVERIZE);
    addCard(myDeck, &BLOUNI_JAB);
    addCard(myDeck, &ACCELERATION);
    addCard(myDeck, &DEFENSE);
    addCard(myDeck, &BLOUNI_KICK);
    myDeck = shuffleDeck(myDeck);
    displayDeck(myDeck);
}

void displayDeck(deck_t *myDeck)
{
    int i = 0;
    while (myDeck != NULL)
    {
        card_t *myCard = myDeck->data;
        printf("Element %d %s: \n", i, myCard->name);
        myDeck = myDeck->next;
        i++;
    }
}

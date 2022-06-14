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

void addCard(deck_t *deck, card_t *card)
{
    if (deck->data == NULL)
    {
        deck->data = card;
    }
    else
    {
        if (deck == NULL)
        {
            deck = createDeck(card);
            return;
        }
        while (deck->next != NULL)
        {
            deck = deck->next;
        }
        deck->next = createDeck(card);
    }
}

deck_t *removeFirstCard(deck_t *deck)
{
    if (deck == NULL)
        return createDeck(NULL);
    deck_t *temp = deck;
    deck = deck->next;
    if(deck ==NULL){
        deck = createDeck(NULL);
    }
    return deck;
}

deck_t *removeCard(deck_t *deck, char *cardName)
{
    deck_t *previousElement = NULL;
    deck_t *origin = deck;
    int isRemoved = 0;
    while (deck != NULL)
    {
        card_t *card = deck->data;
        if (strcmp(card->name, cardName) == 0)
        {
            if (previousElement == NULL)
            {
                deck = removeFirstCard(deck);
                origin = deck;
                if(origin==NULL){
                    origin = createDeck(NULL);
                }
                return origin;
            }
            else
            {
                previousElement->next = deck->next;
            }
        }
        previousElement = deck;
        deck = deck->next;
    }
    if(origin==NULL){
        origin = createDeck(NULL);
    }
    return origin;
}

card_t *draw(deck_t *deck)
{
    card_t *res = deck->data;
    deck = deck->next;
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

int size(deck_t *deck)
{
    int res = 0;
    while (deck != NULL)
    {
        res++;
        deck = deck->next;
    }
    return res;
}

void swapElements(deck_t *deck, int n, int m)
{
    if (n > 0 && m > 0)
    {
        deck_t *myDeckBeforeN = getElementFromDeckAtIndex(n - 1, deck);
        deck_t *myDeckAtM = getElementFromDeckAtIndex(m, deck);
        deck_t *myDeckAtN = getElementFromDeckAtIndex(n, deck);
        deck_t *deckAfterM = getElementFromDeckAtIndex(m + 1, deck);
        deck_t *deckAfterN = getElementFromDeckAtIndex(n + 1, deck);
        deck_t *myDeckBeforeM = getElementFromDeckAtIndex(m - 1, deck);
        myDeckBeforeN->next = myDeckAtM;
        myDeckAtM->next = deckAfterN;
        myDeckBeforeM->next = myDeckAtN;
        myDeckAtN->next = deckAfterM;
    }
}

deck_t *shuffleDeck(deck_t *deck)
{
    displayDeck(deck);
    fflush(stdout);
    int listSize = size(deck);
    srand(time(NULL));
    int number1;
    int number2;
    for (int i = 0; i < listSize * 10; i++)
    {
        number1 = rand() % listSize;
        number2 = rand() % listSize;
        if (number1 != number2 && (number2 - number1 > 1 || number1 - number2 > 1))
        {
            swapElements(deck, number1, number2);
        }
    }
    // ici on swap le premier element avec l'élement du milieu
    deck_t *middleElement = getElementFromDeckAtIndex((int)(listSize / 2), deck);
    deck_t *beforeMiddleElement = getElementFromDeckAtIndex((int)(listSize / 2) - 1, deck);
    deck_t *afterMiddleElement = getElementFromDeckAtIndex((int)(listSize / 2) + 1, deck);
    middleElement->next = deck->next;
    beforeMiddleElement->next = deck;
    deck->next = afterMiddleElement;
    deck = middleElement;
    return deck;
}

void testDeck()
{
    // deck_t *myDeck = createDeck(&CARD_DODGE);
    // addCard(myDeck, &PULVERIZE);
    // addCard(myDeck, &BLOUNI_JAB);
    // addCard(myDeck, &ACCELERATION);
    // addCard(myDeck, &DEFENSE);
    // addCard(myDeck, &BLOUNI_KICK);
    // myDeck = shuffleDeck(myDeck);
    // displayDeck(myDeck);
    deck_t *myDeck = createDeckFromArray((int[][2]){{DODGE_A, 1}, {PULVERIZE, 2}, {DEFENSE, 3}}, 3);
    displayDeck(myDeck);
}

void displayDeck(deck_t *myDeck)
{
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
}

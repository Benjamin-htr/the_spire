#include "gameplay.h"
#include "./../combat/combat.h"
#include "./../utils/utils.h"
#include "./../utils/shared_textures.h"
#include "./../../model/game/map/map.h"

#include <stdio.h>
#include <string.h>

static int finishScreen = 0;

// variables textures :
static Texture2D arrowButtonBack = {0};
static Texture2D arrow = {0};
// Note: StatBar and HeartIcon are now shared, accessed via sharedTextures
static Texture2D objectsTextures[5];

// variables sprite (animations) :
static Sprite roomSpriteStart = {0};
static Sprite roomSprite3Doors = {0};
static Sprite roomSprite2Doors_top_blocked = {0};
static Sprite roomSprite2Doors_bottom_blocked = {0};
static Sprite roomSpriteEnd = {0};

// horizontal gap of map
static int roomGapX = -200;

// Represent if modal is open (-1 : no, >= 0 : open) (used to block interaction with button on the back)
static int backInteractState = -1;
static boolean modalClose = false;

// 0 : combat; 1 : event; 2 : sanctuary; 3 : miniboss
static int currentEvent = {0};
static event *eventData = {0};
static entity_t *ennemyData = {0};
static boolean showDeckModal = false;

//---------------------------------------------------------
// Use for deck print carroussel :
//--------------------------------------------
// Tableau contenant le premier élément de chaque page :
static deck_t **firsPageElements = {0};
// Indice de la page courante :
static int currentPage = 0;
// index
// static int idxCard = 0;
static int blockCarroussel = 0;

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------

// Draw button with arrow in the middle
bool ArrowButton(Rectangle bounds, float rotation, int forcedState, int buttonId)
{
    NPatchInfo buttonArrowInfo = {0};

    int state = (forcedState >= 0) ? forcedState : 0; // NORMAL
    bool pressed = false;

    int arrowPosAdd = 0;

    // Update control
    //--------------------------------------------------------------------
    if ((state < 3) && (forcedState < 0))
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            // PRESSED
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                state = 2;
                arrowPosAdd = 10;
            }
            else
                state = 1; // FOCUSED

            // Use IsMouseButtonPressed instead of Released to trigger only once on click
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsGestureDetected(GESTURE_TAP))
            {
                pressed = true;
                PlaySound(buttonSound);
            }
        }
    }

    buttonArrowInfo.source = (Rectangle){0, 0, 66, arrowButtonBack.height},
    buttonArrowInfo.left = 30;
    buttonArrowInfo.top = 30;
    buttonArrowInfo.right = 30;
    buttonArrowInfo.bottom = 30;
    buttonArrowInfo.source.x = 66 * state;

    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    // DrawRectangleRec(bounds, GREEN);
    // DrawRectangleLinesEx(bounds, 4, DARKGREEN);
    DrawTextureNPatch(arrowButtonBack, buttonArrowInfo, bounds, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

    Vector2 posArrow = {0};
    // DrawTextEx(font, text, (Vector2){ bounds.x + bounds.width/2 - arrow.width/2, bounds.y + bounds.height/2 - arrow.height/2 - 4 + arrowPosAdd }, font.baseSize, 1, GetColor(textColor[state]));
    if (rotation == -180)
        posArrow = (Vector2){bounds.x + bounds.width / 2 + arrow.width / 2, bounds.y + bounds.height / 2 + arrow.height / 2 - 4 + arrowPosAdd};
    else if (rotation == -90)
        posArrow = (Vector2){bounds.x + bounds.width / 2 - arrow.height / 2, bounds.y + bounds.height / 2 + arrow.width / 2 - 6 + arrowPosAdd};
    else if (rotation == 90)
        posArrow = (Vector2){bounds.x + bounds.width / 2 + arrow.height / 2, bounds.y + bounds.height / 2 - arrow.width / 2 - 6 + arrowPosAdd};
    else
        posArrow = (Vector2){bounds.x + bounds.width / 2 - arrow.width / 2, bounds.y + bounds.height / 2 - arrow.height / 2 - 4 + arrowPosAdd};

    DrawTextureEx(arrow, posArrow, rotation, 1.0f, WHITE);
    //------------------------------------------------------------------

    return pressed;
}

// Draw life bar of the charater:
void drawLifeBar()
{
    int HpMax = getEntityStat(game->caracterData, HP)->max;
    int HpActuel = getEntityStat(game->caracterData, HP)->current;

    float scaleBar = 4.0f;
    float scaleHeart = 3.0f;

    int padding = 15;
    int gap = 20;

    int fontSize = 20;
    Vector2 textSize = MeasureTextEx(font, TextFormat("%d/%d hp", HpActuel, HpMax), fontSize, 1);
    Vector2 textPos = (Vector2){GetScreenWidth() - textSize.x - padding, GetScreenHeight() - ((sharedTextures.StatBar.height * scaleBar / 2) + textSize.y + padding)};
    DrawTextEx(font, TextFormat("%d/%d hp", HpActuel, HpMax), textPos, fontSize, 1, LIGHTGRAY);

    Vector2 StatBarPos = (Vector2){textPos.x - (sharedTextures.StatBar.width * scaleBar + gap), GetScreenHeight() - ((sharedTextures.HeartIcon.height * scaleHeart / 2) + (sharedTextures.StatBar.height * scaleBar / 2) + padding)};
    DrawTextureEx(sharedTextures.HeartIcon, (Vector2){StatBarPos.x - (sharedTextures.HeartIcon.width * scaleHeart + gap), GetScreenHeight() - (sharedTextures.HeartIcon.height * scaleHeart + padding)}, 0, scaleHeart, WHITE);

    float HpBarWidth = (float)HpActuel / (float)HpMax * sharedTextures.StatBar.width * scaleBar * 0.8f;
    DrawRectangle(StatBarPos.x + sharedTextures.StatBar.width * scaleBar / 10, StatBarPos.y + sharedTextures.StatBar.height * scaleBar * 0.2f, HpBarWidth, sharedTextures.StatBar.height * scaleBar * 0.6f, RED);

    DrawTextureEx(sharedTextures.StatBar, StatBarPos, 0, scaleBar, WHITE);
}

// Draw object of character :
void drawItem(item_t *item, Texture2D texture, Vector2 position, float scaleFactor, int forcedState)
{
    int gap = 20;
    DrawTextureEx(texture, position, 0, scaleFactor, WHITE);

    int fontSize = 20;
    Vector2 textSize = MeasureTextEx(font, TextFormat("%s", item->name), fontSize, 1);
    Vector2 textPos = (Vector2){position.x - textSize.x - gap, position.y + (texture.height * scaleFactor / 2) - textSize.y / 2};
    DrawTextEx(font, TextFormat("%s", item->name), textPos, fontSize, 1, LIGHTGRAY);

    Rectangle bounds = (Rectangle){textPos.x, position.y, textSize.x + gap + texture.width * scaleFactor, texture.height * scaleFactor};

    Vector2 mousePoint = GetMousePosition();
    // Check if hover :
    if (CheckCollisionPointRec(mousePoint, bounds) && forcedState < 0)
    {

        Rectangle hoverRect = (Rectangle){bounds.x - 300 - 20, bounds.y, 300, 140};
        DrawRectangleRec(hoverRect, GetColor(0x242424ff));
        DrawTextBoxed(font, TextFormat("%s :\n%s", item->technic, item->description), (Rectangle){hoverRect.x + 5, hoverRect.y + 5, hoverRect.width - 5, hoverRect.height - 5}, 18, 1.0f, true, WHITE);
    }
}
// Draw all items :
void drawItems()
{
    int padding = 10;
    int fontSize = 25;
    char *text = "Liste des objets :";
    Vector2 textSize = MeasureTextEx(font, text, fontSize, 1.0f);
    DrawTextEx(font, text, (Vector2){GetScreenWidth() - textSize.x - padding * 8, padding}, fontSize, 1, LIGHTGRAY);

    float scaleFactor = 2.0f;
    float posY = textSize.y + padding + 20;
    int gap = 25;
    for (int itemsIdx = 0; itemsIdx < 5 && game->caracterData->items[itemsIdx]->description != NULL; itemsIdx++)
    {
        Vector2 pos = (Vector2){GetScreenWidth() - objectsTextures[itemsIdx].width * scaleFactor - padding, posY};
        drawItem(game->caracterData->items[itemsIdx], objectsTextures[itemsIdx], pos, scaleFactor, backInteractState);
        posY += objectsTextures[itemsIdx].height * scaleFactor + gap;
    }
}

// Draw choice of current event :
void drawEventChoice(event *event)
{
    if (!modalClose)
    {
        // Guard against NULL event or actions
        if (!event || !event->actions || !event->actions[0] || !event->actions[1])
        {
            printf("ERROR: drawEventChoice received NULL event or actions\n");
            fflush(stdout);
            modalClose = true;
            return;
        }
        
        backInteractState = 0;
        // DrawRectangleRec((Rectangle){0, 0, GetScreenWidth(), GetScreenWidth()}, GetColor(0x242424ff));
        float backgroundWidth = GetScreenWidth() * 0.7f;
        float backgroundHeight = GetScreenHeight() * 0.7f;
        Rectangle backgroundRect = (Rectangle){GetScreenWidth() / 2 - backgroundWidth / 2, GetScreenHeight() / 2 - backgroundHeight / 2, backgroundWidth, backgroundHeight};
        DrawRectangleRec(backgroundRect, GetColor(0x242424ff));
        float margin = 0.05f;
        // Draw event description :
        Rectangle boundsDesc = (Rectangle){backgroundRect.x + backgroundRect.width * 0.20f, backgroundRect.y + backgroundRect.height * margin, backgroundWidth * 0.6f, backgroundHeight * 0.2f};
        DrawTextBoxed(font, TextFormat("%s", event->dialogue ? event->dialogue : "???"), boundsDesc, 22, 1.0f, true, WHITE);

        // Event choices options :
        float marginTopTextAction = 0.15f;
        float actionTextWidth = backgroundWidth * 0.35f;
        float actionTextHeight = backgroundHeight * 0.30f;
        float posY = boundsDesc.y + boundsDesc.height + (backgroundHeight * marginTopTextAction);
        float gapX = backgroundWidth - actionTextWidth * 2 - (backgroundWidth * margin * 2);
        //  Draw event text choices :
        Rectangle boundsAction1 = (Rectangle){backgroundRect.x + (margin * backgroundWidth), posY, actionTextWidth, actionTextHeight};
        const char* label1 = (event->actions[0]->label) ? event->actions[0]->label : "???";
        DrawTextBoxed(font, TextFormat("%s", label1), boundsAction1, 17, 1.0f, true, WHITE);

        Rectangle boundsAction2 = (Rectangle){boundsAction1.x + boundsAction1.width + gapX, posY, actionTextWidth, actionTextHeight};
        const char* label2 = (event->actions[1]->label) ? event->actions[1]->label : "???";
        DrawTextBoxed(font, TextFormat("%s", label2), boundsAction2, 17, 1.0f, true, WHITE);

        // Draw button choices :
        float marginTopButton = 0.10f;
        posY = posY + actionTextHeight + (backgroundHeight * marginTopButton);
        float buttonHeight = 60;
        float buttonWidth = backgroundWidth * 0.20f;

        if (GuiButton((Rectangle){boundsAction1.x + (actionTextWidth / 2) - buttonWidth / 2, posY, buttonWidth, buttonHeight}, "CHOISIR", -1, 600))
        {
            printf("Choix 1 \n");
            fflush(stdout);
            int res = event->actions[0]->action(game->caracterData);
            modalClose = true;
            backInteractState = -1;
            if (res == 3) // COMBAT MINI BOSS
            {
                TransitionToScreen(COMBAT_SCREEN);
            }
            if (res == 4) // TP
            {
                teleporter(game->mapData);
            }
        }
        if (GuiButton((Rectangle){boundsAction2.x + (actionTextWidth / 2) - buttonWidth / 2, posY, buttonWidth, buttonHeight}, "CHOISIR", -1, 601))
        {
            printf("Choix 2 \n");
            int res = event->actions[1]->action(game->caracterData); //, event->data);
            modalClose = true;
            backInteractState = -1;
            if (res == 2) // SHOW DECK SANCTUARY
            {
                initCaroussel();
                showDeckModal = true;
            }
        }
    }
}
// Draw the carroussel that dispay deck (6 cards per page) :
void drawCarrousselPage(deck_t **firsPageElements, char *title)
{
    // DrawRectangleRec((Rectangle){0, 0, GetScreenWidth(), GetScreenWidth()}, GetColor(0x242424ff));
    float backgroundWidth = GetScreenWidth() * 0.75f;
    float backgroundHeight = GetScreenHeight() * 0.87f;
    Rectangle backgroundRect = (Rectangle){GetScreenWidth() / 2 - backgroundWidth / 2, GetScreenHeight() / 2 - backgroundHeight / 2, backgroundWidth, backgroundHeight};
    DrawRectangleRec(backgroundRect, GetColor(0x242424ff));
    // Draw little text deck :
    int fontSize = 18;

    Vector2 textSize = MeasureTextEx(font, title, fontSize, 1.0f);
    Vector2 textPos = (Vector2){backgroundRect.x + backgroundRect.width / 2 - textSize.x / 2, backgroundRect.y + 10};
    DrawTextEx(font, title, textPos, fontSize, 1, LIGHTGRAY);
    // printf("currentPage : %d \n", currentPage);
    int perPage = 6;
    deck_t *myDeck = firsPageElements[currentPage];
    int deckSize = getDeckSize(game->caracterData->cardDeck);
    if (myDeck == NULL || myDeck->data == NULL)
    {
        printf("Little problem \n");
        return;
    }
    // visuals options :
    float scaleFactor = 1.7f;
    float cardWidth = (float)cardInfo.source.width * scaleFactor;
    float cardHeight = (float)cardInfo.source.height * scaleFactor;
    int gap = 25;
    float cardsTotalWidth = cardWidth * 3 + gap * 2;

    int posY = textPos.y + textSize.y + 15;

    // for logic :
    int idxCard = currentPage * perPage;
    int i = 0;
    while (myDeck != NULL && myDeck->data != NULL && i < perPage)
    {
        card_t *myCard = myDeck->data;
        if (i > 2)
        {
            posY = textPos.y + textSize.y + 15 + cardHeight;
        }
        // printf("Element %d %s: \n", i, myCard->name);
        Vector2 position = (Vector2){(float)(GetScreenWidth() / 2 - cardsTotalWidth / 2) + ((i % 3) * cardWidth) + gap * (i % 3), posY};
        if (GuiCard(myCard, position, scaleFactor, i, false, blockCarroussel))
        {
            card_t *suppCard = removeCard(&game->caracterData->cardDeck, myCard->name);
            freeCard(suppCard);
            showDeckModal = false;
            backInteractState = -1;
            free(firsPageElements);
        }

        myDeck = myDeck->next;
        i++;
        idxCard++;
    }
    printf("idx : %d   size : %d \n", idxCard, deckSize);
    blockCarroussel = -1;
    int arrowButtonWidth = 72;
    // printf("i : %d \n", idxCard);
    if (idxCard < deckSize)
    {
        // flèche vers droite :
        if (ArrowButton((Rectangle){backgroundRect.x + backgroundRect.width - 20 - arrowButtonWidth, GetScreenHeight() / 2 - arrowButtonWidth / 2, arrowButtonWidth, arrowButtonWidth}, 0, -1, 100))
        {
            printf("i : %d \n", i);
            currentPage += 1;
            printf("currentPage : %d \n", currentPage);
        }
    }
    if (idxCard > perPage)
    {
        // flèche vers gauche :
        if (ArrowButton((Rectangle){backgroundRect.x + 20, GetScreenHeight() / 2 - arrowButtonWidth / 2, arrowButtonWidth, arrowButtonWidth}, -180, -1, 101))
        {
            // printf("ArrowButton LEFT\n");
            currentPage -= 1;
            printf("i : %d \n", i);
            printf("currentPage : %d \n", currentPage);
            printf("gauche");
        }
    }
}
// Little function that draw carroussel when showDeckModal is true
void drawDeck()
{
    if (showDeckModal)
    {
        backInteractState = 0;
        char *text = "Choisissez une carte à supprimer de votre deck";
        drawCarrousselPage(firsPageElements, text);
    }
}

// detect type event and call a function for that
void drawEvent()
{
    // if is combat :
    // if (currentEvent == 0)
    //{
    //    // drawEventChoice(eventData);
    //}
    // if is event :
    if (currentEvent == 1)
    {
        drawEventChoice(eventData);
    }
    // if is sanctuary :
    if (currentEvent == 2)
    {
        drawEventChoice(eventData);
    }
    // if is miniboss :
    if (currentEvent == 3)
    {
        drawEventChoice(eventData);
    }
}
// init variable that use from caroussel :
void initCaroussel()
{
    blockCarroussel = 0;
    currentPage = 0;
    deck_t *myDeck = game->caracterData->cardDeck;
    int deckSize = getDeckSize(myDeck);
    int elementPerPage = 6;
    int numberOfPage = (deckSize / elementPerPage) + 1;
    firsPageElements = malloc(sizeof(deck_t *) * numberOfPage);

    printf("Init caroussel \n");

    int i = 0;
    int idxElementAd = 0;
    if (myDeck == NULL || myDeck->data == NULL)
    {
        printf("The deck is empty \n");
        return;
    }
    while (myDeck != NULL && myDeck->data != NULL)
    {
        if ((i % elementPerPage) == 0)
        {
            firsPageElements[idxElementAd] = myDeck;

            // card_t *myCard = firsPageElements[idxElementAd]->data;
            //  printf("Element %d %s: \n", i, myCard->name);
            idxElementAd += 1;
        }

        myDeck = myDeck->next;
        i++;
    }
    // Textures are preloaded at screen init; no need to reload here
    printf(" deck size : %d    |    %d\n", deckSize, numberOfPage);
    // drawCarrousselPage(0, firsPageElements);
}
// reinit map after moving on map :
void reinitAfterMove()
{
    currentEvent = map_get(game->mapData);
    position_player playerPos = player_position(game->mapData);
    // if is combat :
    if (currentEvent == 0)
    {
        modalClose = false;
        printf("before ennemyData recup in gameplay \n");
        ennemyData = game->mapData->places[playerPos.x][playerPos.y].enemyData;
        printf("after ennemyData recup in gameplay \n");
        TransitionToScreen(COMBAT_SCREEN);
        // constructSprite(&eventSprite, "./asset/map/room_3_doors.png", 3, 1);
    }
    // if is event :
    if (currentEvent == 1)
    {
        modalClose = false;
        eventData = game->mapData->places[playerPos.x][playerPos.y].eventData;
        // constructSprite(&eventSprite, "./asset/map/room_3_doors.png", 3, 1);
    }
    // if is sanctuary :
    if (currentEvent == 2)
    {
        modalClose = false;
        eventData = game->mapData->places[playerPos.x][playerPos.y].eventData;
        // constructSprite(&eventSprite, "./asset/map/room_3_doors.png", 3, 1);
    }
    // if is miniboss :
    if (currentEvent == 3)
    {
        modalClose = false;
        eventData = game->mapData->places[playerPos.x][playerPos.y].eventData;
        ennemyData = game->mapData->places[playerPos.x][playerPos.y].enemyData;
        // constructSprite(&eventSprite, "./asset/map/room_3_doors.png", 3, 1);
    }
}

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    printf("Gameplay Screen Init\n");

    if (!isLaunched)
    {
        isLaunched = true;
        printf("Game logic launched\n");
        initGame();
    }

    arrow = LoadTexture("./asset/UI_assets/arrow.png");
    arrowButtonBack = LoadTexture("./asset/UI_assets/button-arrow.png");
    // Note: StatBar and HeartIcon are now shared textures, preloaded in main

    // load 3 sprites rooms :
    constructSprite(&roomSpriteStart, "./asset/map/room_start.png", 3, 1);
    constructSprite(&roomSprite3Doors, "./asset/map/room_3_doors.png", 3, 1);
    constructSprite(&roomSprite2Doors_top_blocked, "./asset/map/room_2_doors_top_blocked.png", 3, 1);
    constructSprite(&roomSprite2Doors_bottom_blocked, "./asset/map/room_2_doors_bottom_blocked.png", 3, 1);
    constructSprite(&roomSpriteEnd, "./asset/map/room_end.png", 3, 1);

    char *objectsTexturePath = "./asset/Objects/";
    char *texturePath;
    for (int itemsIdx = 0; itemsIdx < 5 && game->caracterData->items[itemsIdx]->description != NULL; itemsIdx++)
    {
        texturePath = (char *)malloc(1 + strlen(objectsTexturePath) + strlen(game->caracterData->items[itemsIdx]->imageName));
        strcpy(texturePath, objectsTexturePath);
        strcat(texturePath, game->caracterData->items[itemsIdx]->imageName);
        objectsTextures[itemsIdx] = LoadTexture(texturePath);
        free(texturePath);
    }
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        showInGameMenu = !showInGameMenu;
    }

    position_player playerPos = player_position(game->mapData);
    int etage = playerPos.x;
    int room = playerPos.y;

    if (etage == 0)
        updateSprite(&roomSpriteStart);
    else if (etage > 0 && etage < 9 && !(room == 3 || room == 0))
        updateSprite(&roomSprite3Doors);
    else if (etage > 0 && etage < 9 && room == 0)
        updateSprite(&roomSprite2Doors_top_blocked);
    else if (etage > 0 && etage < 9 && room == 3)
        updateSprite(&roomSprite2Doors_bottom_blocked);
    else if (etage == 9)
        updateSprite(&roomSpriteEnd);
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    ClearBackground(BLACK);

    position_player playerPos = player_position(game->mapData);
    int etage = playerPos.x;
    int room = playerPos.y;

    drawLifeBar();

    // POUR TEST COMBAT : (A RETIRER PLUS TARD)
    /*To test :
    if (GuiButton((Rectangle){10, GetScreenHeight() - buttonHeight - 10, buttonWidth, buttonHeight}, "COMBAT", -1))
    {
        // TransitionToScreen(COMBAT);
        ChangeToScreen(COMBAT_SCREEN);
    }
    */

    // Vector2 textSize = MeasureTextEx(font, TextFormat("ETAGE %d", etage), 20, 1);
    DrawTextEx(font, TextFormat("ETAGE %d", etage), (Vector2){10, 10}, 20, 1, LIGHTGRAY);

    float scaleFactor = 4.0f;
    // DrawTextureEx(arrowButton, (Vector2){GetScreenWidth() / 2 - arrowButton.width * scaleFactor / 2, GetScreenHeight() / 2 - arrowButton.height * scaleFactor / 2}, 0, scaleFactor, WHITE);

    scaleFactor = 1.2f;
    float roomWidth = (roomSpriteStart.frameRec.width * scaleFactor);
    float roomHeight = (roomSpriteStart.frameRec.height * scaleFactor);
    const Vector2 position = {GetScreenWidth() / 2 - roomWidth / 2 + roomGapX, GetScreenHeight() / 2 - roomHeight / 2};

    if (etage == 0)
        drawSprite(&roomSpriteStart, position, 0.0f, scaleFactor, WHITE);
    else if (etage > 0 && etage < 9 && !(room == 3 || room == 0))
        drawSprite(&roomSprite3Doors, position, 0.0f, scaleFactor, WHITE);
    else if (etage > 0 && etage < 9 && room == 0)
        drawSprite(&roomSprite2Doors_top_blocked, position, 0.0f, scaleFactor, WHITE);
    else if (etage > 0 && etage < 9 && room == 3)
        drawSprite(&roomSprite2Doors_bottom_blocked, position, 0.0f, scaleFactor, WHITE);
    else if (etage == 9)
        drawSprite(&roomSpriteEnd, position, 0.0f, scaleFactor, WHITE);

    int arrowButtonWidth = 72;
    int padding = 4;

    if (ArrowButton((Rectangle){GetScreenWidth() / 2 + (roomWidth / 2) + padding + roomGapX, GetScreenHeight() / 2 - arrowButtonWidth / 2, arrowButtonWidth, arrowButtonWidth}, 0, backInteractState, 0))
    {
        printf("ArrowButton RIGHT\n");
        if (etage == 9)
        {
            move_player(game->mapData, 0, false);
        }
        else
        {
            move_player(game->mapData, room, false);
        }

        reinitAfterMove();
    }
    if (etage == 0)
    {
        if (ArrowButton((Rectangle){GetScreenWidth() / 2 - (roomWidth / 2) - arrowButtonWidth - padding + roomGapX, GetScreenHeight() / 2 - arrowButtonWidth / 2, arrowButtonWidth, arrowButtonWidth}, -180, backInteractState, 1))
        {
            printf("ArrowButton LEFT\n");
            move_player(game->mapData, room + 2, false);
            reinitAfterMove();
        }
    }
    if (room != 0 && etage != 9)
    {
        if (ArrowButton((Rectangle){GetScreenWidth() / 2 - arrowButtonWidth / 2 + roomGapX, GetScreenHeight() / 2 - (roomHeight / 2) - arrowButtonWidth - padding, arrowButtonWidth, arrowButtonWidth}, -90, backInteractState, 2))
        {
            printf("ArrowButton TOP\n");
            move_player(game->mapData, room - 1, false);
            reinitAfterMove();
        }
    }
    if (room != 3 && etage != 9)
    {
        if (ArrowButton((Rectangle){GetScreenWidth() / 2 - arrowButtonWidth / 2 + roomGapX, GetScreenHeight() / 2 + (roomHeight / 2) + padding, arrowButtonWidth, arrowButtonWidth}, 90, backInteractState, 3))
        {
            printf("ArrowButton BOTTOM\n");
            move_player(game->mapData, room + 1, false);
            reinitAfterMove();
        }
    }
    drawItems();
    drawEvent();
    drawDeck();
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    UnloadTexture(arrow);
    UnloadTexture(arrowButtonBack);
    UnloadTexture(roomSpriteStart.texture);
    UnloadTexture(roomSprite2Doors_bottom_blocked.texture);
    UnloadTexture(roomSprite2Doors_top_blocked.texture);
    UnloadTexture(roomSprite3Doors.texture);
    UnloadTexture(roomSpriteEnd.texture);
    // Note: StatBar is shared, do not unload here

    for (int itemsIdx = 0; itemsIdx < 5 && game->caracterData->items[itemsIdx]->description != NULL; itemsIdx++)
    {
        UnloadTexture(objectsTextures[itemsIdx]);
    }
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}

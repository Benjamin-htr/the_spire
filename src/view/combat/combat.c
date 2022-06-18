#include "./../../../include/raylib.h"
#include "combat.h"
#include "./../utils/utils.h"
#include "./../../model/game/misc/deck/card/card.h"
#include "./../../model/game/game.h"
#include <stdio.h>

static Texture2D StatBar = {0};
static Texture2D Statboard = {0};
static Texture2D EnergyIcon = {0};

static Texture2D BasicCardPatch = {0};
static Texture2D CommonCardPatch = {0};
static Texture2D AtypicCardPatch = {0};
static Texture2D RareCardPatch = {0};
static Texture2D SpecialCardPatch = {0};

static Texture2D ImageCardUnknown = {0};

NPatchInfo cardInfo = {0};

static Sprite ennemySprite = {0};

combat_t *combat = {0};

static int idxHoverCard = -1;

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int finishScreen = 0;

void drawStatBoard()
{
    int HpMax = 75;
    int HpActuel = 70;

    int ManaMax = 100;
    int ManaActuel = 50;

    // int EnergyMax = 3;
    int EnergyActuel = 3;

    float scaleMain = 3.0f;

    int padding = 10;
    int textLeftargin = 10;

    int fontSize = 15;

    // We draw the stat background asset :
    Vector2 StatBoardPos = (Vector2){GetScreenWidth() - Statboard.width * scaleMain - padding, GetScreenHeight() - Statboard.height * scaleMain - padding};
    DrawTextureEx(Statboard, StatBoardPos, 0, scaleMain, WHITE);

    //-----------------------------------------------------------
    // Health :
    // We calculate the HealhBar container position :
    Vector2 HealthBarPos = (Vector2){StatBoardPos.x + Statboard.width * scaleMain * 0.20f, StatBoardPos.y + Statboard.height * scaleMain * 0.5f};

    // We draw the red rectangle of the current life :
    float HpFillWidth = (float)HpActuel / (float)HpMax * StatBar.width * scaleMain * 0.8f;
    DrawRectangle(HealthBarPos.x + StatBar.width * scaleMain / 10, HealthBarPos.y + StatBar.height * scaleMain * 0.2f, HpFillWidth, StatBar.height * scaleMain * 0.6f, RED);

    // We draw the text :
    // Vector2 textLifeSize = MeasureTextEx(font, TextFormat("%d/%d", HpActuel, HpMax), fontSize, 1);
    Vector2 textLifePos = (Vector2){HealthBarPos.x + StatBar.width * scaleMain + textLeftargin, HealthBarPos.y + ((StatBar.height * scaleMain) / 4)};
    DrawTextEx(font, TextFormat("%d/%d", HpActuel, HpMax), textLifePos, fontSize, 1, LIGHTGRAY);

    // We draw the healthBar container :
    DrawTextureEx(StatBar, HealthBarPos, 0, scaleMain, WHITE);

    //-----------------------------------------------------------
    // Mana :
    // We calculate the ManaBar container position :
    Vector2 ManaBarPos = (Vector2){StatBoardPos.x + Statboard.width * scaleMain * 0.20f, StatBoardPos.y + Statboard.height * scaleMain * 0.77f};

    // We draw the blue rectangle of the current mana :
    float ManaFillWidth = (float)ManaActuel / (float)ManaMax * StatBar.width * scaleMain * 0.8f;
    DrawRectangle(ManaBarPos.x + StatBar.width * scaleMain / 10, ManaBarPos.y + StatBar.height * scaleMain * 0.2f, ManaFillWidth, StatBar.height * scaleMain * 0.6f, BLUE);

    // We draw the text :
    // Vector2 textManaSize = MeasureTextEx(font, TextFormat("%d/%d", ManaActuel, ManaMax), fontSize, 1);
    Vector2 textManaPos = (Vector2){ManaBarPos.x + StatBar.width * scaleMain + textLeftargin, ManaBarPos.y + ((StatBar.height * scaleMain) / 4)};
    DrawTextEx(font, TextFormat("%d/%d", ManaActuel, ManaMax), textManaPos, fontSize, 1, LIGHTGRAY);

    // We draw the manaBar container :
    DrawTextureEx(StatBar, ManaBarPos, 0, scaleMain, WHITE);

    //-----------------------------------------------------------
    // Energy :
    float gap = (float)((2.0f / 124.0f) * Statboard.width * scaleMain);
    Vector2 EnergyPos = (Vector2){StatBoardPos.x + Statboard.width * scaleMain * 0.49f, StatBoardPos.y + Statboard.height * scaleMain * 0.083f};
    for (int i = 0; i < EnergyActuel; i++)
    {
        DrawTextureEx(EnergyIcon, (Vector2){EnergyPos.x + (EnergyIcon.width * i * scaleMain) + (gap * i), EnergyPos.y}, 0, scaleMain, WHITE);
    }
}

int GuiCardHand(card_t *card, Vector2 position, float scaleFactor, int idx)
{
    idxHoverCard = -1;
    // card_t *card = importCardFromId(ACCELERATION);
    int manaCost = card->manaCost;
    int energyCost = card->energyCost;
    char *title = card->name;
    Texture2D textureCard = card->rarity == BASIC ? BasicCardPatch : card->rarity == COMMON ? CommonCardPatch
                                                                 : card->rarity == ATYPIC   ? AtypicCardPatch
                                                                 : card->rarity == RARE     ? RareCardPatch
                                                                                            : SpecialCardPatch;

    float cardWidth = (float)cardInfo.source.width * scaleFactor;
    float cardHeight = (float)cardInfo.source.height * scaleFactor;

    Rectangle bounds = (Rectangle){position.x, position.y, cardWidth, cardHeight};

    int state = 0; // )NORMAL
    bool pressed = false;
    // Update control
    //--------------------------------------------------------------------
    if ((state < 3))
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            // PRESSED
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                state = 2;
            }
            else
            {
                idxHoverCard = idx;
                state = 1; // FOCUSED
                scaleFactor += 0.8f;
                cardWidth = (float)cardInfo.source.width * scaleFactor;
                cardHeight = (float)cardInfo.source.height * scaleFactor;
                position.y = GetScreenHeight() - cardHeight;
            }

            if (IsGestureDetected(GESTURE_TAP))
            {
                pressed = true;
                PlaySound(buttonSound);
            }
        }
    }

    bounds = (Rectangle){position.x, position.y, cardWidth, cardHeight};
    // Draw image card :
    Vector2 imageCardPos = (Vector2){position.x + cardWidth * (8 / 96.0f), position.y + cardHeight * (27 / 156.0f)};
    DrawTextureEx(ImageCardUnknown, imageCardPos, 0, scaleFactor, WHITE);

    // Draw card border :
    cardInfo.source.x = 96 * state;
    DrawTextureNPatch(textureCard, cardInfo, bounds, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

    // Draw mana cost :
    Vector2 manaCostPos = (Vector2){position.x + cardWidth * (14 / 96.0f), position.y + cardHeight * (15 / 156.0f)};
    DrawTextEx(font, TextFormat("%d", manaCost), manaCostPos, 0.155 * cardWidth, 1, GetColor(0x639bffff));

    // Draw title :
    Vector2 titlePos = (Vector2){position.x + cardWidth * (33 / 96.0f), position.y + cardHeight * (13 / 156.0f)};
    DrawTextEx(font, TextFormat("%s", title), titlePos, 0.08333 * cardWidth, 1, GetColor(0xdfdfbeff));

    // Draw text technique :
    float fontTech = 0.050f * cardWidth;

    // textBox calc :
    Rectangle textBox = (Rectangle){position.x + cardWidth * (10 / 96.0f), position.y + cardHeight * (94 / 156.0f), cardWidth * (76 / 96.0f), cardHeight * (51 / 156.0f)};

    // Draw text description :
    float fontDesc = 0.058f * cardWidth;
    DrawTextBoxed(font, TextFormat("%s\n\n(%s)", card->description, card->technic), (Rectangle){textBox.x + 1, textBox.y + 1, textBox.width - 1, textBox.height - 1}, fontDesc, 0.8f, true, WHITE);

    // Draw mana cost :
    Vector2 energyCostPost = (Vector2){position.x + cardWidth * (79 / 96.0f), position.y + cardHeight * (39 / 156.0f)};
    float scaleEnergyIcon = 0.65f * scaleFactor;
    float energyIconHeight = (float)EnergyIcon.height * scaleEnergyIcon;
    float gap = (float)((3.0f / 156.0f) * cardHeight);
    for (int i = 0; i < energyCost; i++)
    {
        DrawTextureEx(EnergyIcon, (Vector2){energyCostPost.x, energyCostPost.y + (energyIconHeight * i) + (gap * i)}, 0, scaleEnergyIcon, WHITE);
    }

    return pressed;
}
void drawHand(void)
{
    float scaleFactor = 1.5f;

    float cardWidth = (float)cardInfo.source.width * scaleFactor;
    float cardHeight = (float)cardInfo.source.height * scaleFactor;

    int i = 0;
    float decal = 0.0f;
    deck_t *myHand = combat->caracter->board->hand;
    while (myHand != NULL && myHand->data != NULL)
    {
        card_t *myCard = myHand->data;
        if (idxHoverCard != -1 && i > idxHoverCard)
        {
            decal += (cardInfo.source.width * (scaleFactor + 0.8f)) - cardWidth;
        }
        Vector2 position = (Vector2){(float)(i * cardWidth) + decal, (float)(GetScreenHeight() - cardHeight * 0.60f)};
        if (GuiCardHand(myCard, position, scaleFactor, i))
        {
            displayCard(*myCard);
        }
        myHand = myHand->next;
        i++;
    }
}

void InitCombatScreen(void)
{
    finishScreen = 0;
    printf("Combat Screen Init\n");

    StatBar = LoadTexture("./asset/Board/Bar/StatBar.png");
    Statboard = LoadTexture("./asset/Board/Bar/StatBoard.png");
    EnergyIcon = LoadTexture("./asset/Board/Bar/unit/Energy.png");

    // Cards textures loading :
    BasicCardPatch = LoadTexture("./asset/Board/card-basic.png");
    CommonCardPatch = LoadTexture("./asset/Board/card-common.png");
    AtypicCardPatch = LoadTexture("./asset/Board/card-atypic.png");
    RareCardPatch = LoadTexture("./asset/Board/card-rare.png");
    SpecialCardPatch = LoadTexture("./asset/Board/card-special.png");

    // Image card unknown loading (for card that doesn't have specific image) :
    ImageCardUnknown = LoadTexture("./asset/Board/image-card/image-card-unknown.png");

    // Cards infos :
    cardInfo.source = (Rectangle){0, 0, 96, 156},
    cardInfo.left = 00;
    cardInfo.top = 00;
    cardInfo.right = 00;
    cardInfo.bottom = 00;

    constructSprite(&ennemySprite, "./asset/monsters/jawurm.png", 4, 1);

    combat = startCombat(game->caracterData, game->caracterData);
    combat->caracter->board = drawCards(combat->caracter->board);
    displayDeck(combat->caracter->board->hand);
    //  playTurn(combat, combat->caracter->board);
}
void UpdateCombatScreen(void)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        showInGameMenu = !showInGameMenu;
    }

    updateSprite(&ennemySprite);
}
void DrawCombatScreen(void)
{

    ClearBackground(GetColor(0x3f3f74ff));

    float scaleEnnemy = 3.0f;
    const Vector2 ennemyPos = {GetScreenWidth() / 2 - ennemySprite.frameRec.width * scaleEnnemy / 2, 20};
    drawSprite(&ennemySprite, ennemyPos, 0.0f, scaleEnnemy, WHITE);

    drawStatBoard();

    // POUR TEST COMBAT : (A RETIRER PLUS TARD)
    int buttonWidth = 150;
    int buttonHeight = 50;
    if (GuiButton((Rectangle){GetScreenWidth() - buttonWidth - 10, 10, buttonWidth, buttonHeight}, "MAP", -1))
    {
        finishScreen = 1;
    }

    // drawHand();

    // if (GuiCardHand((Vector2){50, 50}, 2.0f, -1))
    //{
    //     printf("card click");
    // }
}
void UnloadCombatScreen(void)
{
    UnloadTexture(StatBar);
    UnloadTexture(Statboard);
    UnloadTexture(EnergyIcon);

    UnloadTexture(BasicCardPatch);
    UnloadTexture(CommonCardPatch);
    UnloadTexture(AtypicCardPatch);
    UnloadTexture(RareCardPatch);
    UnloadTexture(SpecialCardPatch);

    UnloadTexture(ImageCardUnknown);
    UnloadTexture(ennemySprite.texture);
}
int FinishCombatScreen(void)
{
    return finishScreen;
}
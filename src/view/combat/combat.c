#include "./../../../include/raylib.h"
#include "combat.h"
#include "./../utils/utils.h"
#include "./../../model/game/misc/deck/card/card.h"
#include "./../../model/game/game.h"

#include <stdio.h>
#include <string.h>

static Texture2D StatBar = {0};
static Texture2D Statboard = {0};
static Texture2D EnergyIcon = {0};

static Texture2D strenghtEffect = {0};
static Texture2D dexterityEffect = {0};
static Texture2D fireEffect = {0};
static Texture2D weaknessEffect = {0};
static Texture2D slowingEffect = {0};

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

// Draw stats of the player :
void drawStatBoard()
{
    int HpMax = getEntityStat(game->caracterData, HP)->max;
    int HpActuel = getEntityStat(game->caracterData, HP)->current;

    int ManaMax = getEntityStat(game->caracterData, MANA)->max;
    int ManaActuel = getEntityStat(game->caracterData, MANA)->current;

    // int EnergyMax = 3;
    int EnergyActuel = getEntityStat(game->caracterData, ENERGY)->current;

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

    // Draw effects of character :
    int marginBottom = 15;
    gap = 18;

    float scaleFactor = 3.0f;
    float posX = GetScreenWidth() - (16 * scaleFactor) - padding;
    for (int effectIdx = 0; effectIdx < 5; effectIdx++)
    {
        Vector2 pos = (Vector2){posX, StatBoardPos.y - 16 * scaleFactor - marginBottom};
        drawEffect(game->caracterData->effects[effectIdx], pos, scaleFactor, false, -1);
        posX -= 16 * scaleFactor + gap;
    }
}

// Draw one card of the hand player :
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

    // textBox calc :
    Rectangle textBox = (Rectangle){position.x + cardWidth * (10 / 96.0f), position.y + cardHeight * (94 / 156.0f), cardWidth * (76 / 96.0f), cardHeight * (51 / 156.0f)};

    // Draw text description and technic:
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

// Draw all cards of the hand player :
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
            displayCard(myCard);
        }
        myHand = myHand->next;
        i++;
    }
}

void drawEnnemy(entity_t *entity)
{
    float fontNameSize = 20;
    float scaleEnnemy = 3.0f;

    Vector2 ennemyNameSize = MeasureTextEx(font, TextFormat("%s", entity->name), fontNameSize, 1);
    Vector2 ennemyNamePos = (Vector2){GetScreenWidth() / 2 - ennemyNameSize.x / 2, 15};
    DrawTextEx(font, TextFormat("%s", entity->name), ennemyNamePos, fontNameSize, 1, WHITE);

    const Vector2 ennemyPos = {ennemyNamePos.x + (ennemyNameSize.x / 2) - ennemySprite.frameRec.width * scaleEnnemy / 2, ennemyNamePos.y + ennemyNameSize.y + 3};
    drawSprite(&ennemySprite, ennemyPos, 0.0f, scaleEnnemy, WHITE);

    int HpMax = getEntityStat(entity, HP)->max;
    int HpActuel = getEntityStat(entity, HP)->current;

    float scaleBar = 2.0f;

    Vector2 StatBarPos = (Vector2){(ennemyPos.x + ennemySprite.frameRec.width * scaleEnnemy / 2) - (StatBar.width * scaleBar / 2), ennemyPos.y + ennemySprite.frameRec.height * scaleEnnemy + 10};

    float HpBarWidth = (float)HpActuel / (float)HpMax * StatBar.width * scaleBar * 0.8f;
    DrawRectangle(StatBarPos.x + StatBar.width * scaleBar / 10, StatBarPos.y + StatBar.height * scaleBar * 0.2f, HpBarWidth, StatBar.height * scaleBar * 0.6f, RED);

    DrawTextureEx(StatBar, StatBarPos, 0, scaleBar, WHITE);

    int hpFontSize = 15;
    Vector2 hpTextSize = MeasureTextEx(font, TextFormat("%d/%d hp", HpActuel, HpMax), hpFontSize, 1);
    Vector2 hpTextPost = (Vector2){StatBarPos.x + StatBar.width * scaleBar + 10, StatBarPos.y + (StatBar.height * scaleBar / 2) - hpTextSize.y / 2};
    DrawTextEx(font, TextFormat("%d/%d  hp", HpActuel, HpMax), hpTextPost, hpFontSize, 1, WHITE);

    // Draw effects of character :
    int marginRight = 50;
    int gap = 18;

    float scaleFactor = 3.0f;
    float posX = ennemyPos.x - (16 * scaleFactor) - marginRight;
    for (int effectIdx = 0; effectIdx < 5; effectIdx++)
    {
        Vector2 pos = (Vector2){posX, ennemyNamePos.y};
        drawEffect(game->caracterData->effects[effectIdx], pos, scaleFactor, true, -1);
        posX -= 16 * scaleFactor + gap;
    }
}
void drawEffect(effect_t *effect, Vector2 position, float scaleFactor, boolean alignLeft, int forcedState)
{
    Texture2D textureEffect;
    if (TextIsEqual(EFFECT_NAME[effect->id], "FORCE"))
        textureEffect = strenghtEffect;
    if (TextIsEqual(EFFECT_NAME[effect->id], "DEXTERITE"))
        textureEffect = dexterityEffect;
    if (TextIsEqual(EFFECT_NAME[effect->id], "FEU"))
        textureEffect = fireEffect;
    if (TextIsEqual(EFFECT_NAME[effect->id], "FAIBLESSE"))
        textureEffect = weaknessEffect;
    if (TextIsEqual(EFFECT_NAME[effect->id], "LENTEUR"))
        textureEffect = slowingEffect;

    DrawTextureEx(textureEffect, position, 0, scaleFactor, WHITE);

    Rectangle bounds = (Rectangle){position.x, position.y, textureEffect.width * scaleFactor, textureEffect.height * scaleFactor};

    Vector2 mousePoint = GetMousePosition();
    // Check if hover :
    if (CheckCollisionPointRec(mousePoint, bounds) && forcedState < 0)
    {
        int rectWidth = 150;
        int rectHeight = 100;
        float posX = alignLeft ? bounds.x : (bounds.x + bounds.width) - rectWidth;
        Rectangle hoverRect = (Rectangle){posX, bounds.y + bounds.height + 10, rectWidth, rectHeight};
        DrawRectangleRec(hoverRect, GetColor(0x242424ff));
        DrawTextBoxed(font, TextFormat("%s", "Test"), (Rectangle){hoverRect.x + 5, hoverRect.y + 5, hoverRect.width - 5, hoverRect.height - 5}, 15, 1.0f, true, WHITE);
    }
}

void InitCombatScreen(void)
{
    finishScreen = 0;
    printf("Combat Screen Init\n");

    StatBar = LoadTexture("./asset/Board/Bar/StatBar.png");
    Statboard = LoadTexture("./asset/Board/Bar/StatBoard.png");
    EnergyIcon = LoadTexture("./asset/Board/Bar/unit/Energy.png");

    // Effects texture loading :
    strenghtEffect = LoadTexture("./asset/Misc/Effect/strength.png");
    dexterityEffect = LoadTexture("./asset/Misc/Effect/dexterity.png");
    fireEffect = LoadTexture("./asset/Misc/Effect/Fire.png");
    weaknessEffect = LoadTexture("./asset/Misc/Effect/Weak.png");
    slowingEffect = LoadTexture("./asset/Misc/Effect/Slow.png");

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

    // Enemy example :
    entity_t *enemy = importEnemyPhase1FromId(KELIKO);

    // We load the ennemy sprite :
    char *ennemySpritePath = "./asset/monsters/";
    char *spritePath;
    spritePath = (char *)malloc(1 + strlen(ennemySpritePath) + strlen(enemy->spriteName));
    strcpy(spritePath, ennemySpritePath);
    strcat(spritePath, enemy->spriteName);
    printf("%s\n", spritePath);
    constructSprite(&ennemySprite, spritePath, 4, 1);

    // We start combat :
    combat = startCombat(game->caracterData, game->caracterData);
    drawCardsFromDeckWithRefillFromDiscard(combat->caracter->board);

    displayEntityEffectArray(game->caracterData->effects);
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

    drawStatBoard();

    // POUR TEST COMBAT : (A RETIRER PLUS TARD)
    int buttonWidth = 150;
    int buttonHeight = 50;
    if (GuiButton((Rectangle){GetScreenWidth() - buttonWidth - 10, 10, buttonWidth, buttonHeight}, "MAP", -1))
    {
        finishScreen = 1;
    }

    drawEnnemy(combat->enemy);
    drawHand();
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
#include "./../../../include/raylib.h"
#include "combat.h"
#include "./../utils/utils.h"
#include "./../utils/shared_textures.h"
#include "./../../model/game/misc/deck/card/card.h"
#include "./../../model/game/game.h"

#include <stdio.h>
#include <string.h>

// Local textures variables :
static Texture2D CombatBG = {0};

// Note: StatBar is now shared, accessed via sharedTextures.StatBar
static Texture2D Statboard = {0};
static Texture2D EnergyIcon = {0};
static Texture2D DodgeIcon = {0};

static Texture2D strenghtEffect = {0};
static Texture2D dexterityEffect = {0};
static Texture2D fireEffect = {0};
static Texture2D weaknessEffect = {0};
static Texture2D slowingEffect = {0};
static Texture2D HPEffect = {0};
static Texture2D DodgeEffect = {0};
static Texture2D ManaEffect = {0};

static Texture2D BasicCardPatch = {0};
static Texture2D CommonCardPatch = {0};
static Texture2D AtypicCardPatch = {0};
static Texture2D RareCardPatch = {0};
static Texture2D SpecialCardPatch = {0};

static Texture2D ImageCardUnknown = {0};

// Global card variable :
NPatchInfo cardInfo = {0};

// variable for ennemy sprite :
static Sprite ennemySprite = {0};

combat_t *combat = {0};

static int idxHoverCard = -1;

static entity_t *ennemy = {0};
static card_t *ennemyCard;

static deck_t *rewardDeck = NULL;
// Pending index of the reward card the player clicked this frame (-1 means none)
static int pendingRewardPickIdx = -1;
// Guard to ensure we don't recreate reward deck after the player picked a card
static boolean rewardResolved = false;

// Represent if modal is open (-1 : no, >= 0 : open) (used to block interaction with button on the back).
static int backInteractState = -1;
static boolean modalClose = false;

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------

// Draw stats of the player :
void drawStatBoard()
{
    // we fetch required data :
    int HpMax = getEntityStat(game->caracterData, HP)->max;
    int HpActuel = getEntityStat(game->caracterData, HP)->current;

    int ManaMax = getEntityStat(game->caracterData, MANA)->max;
    int ManaActuel = getEntityStat(game->caracterData, MANA)->current;
    int DodgeActuel = getEntityStat(game->caracterData, DODGE)->current;

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
    float HpFillWidth = (float)HpActuel / (float)HpMax * sharedTextures.StatBar.width * scaleMain * 0.8f;
    DrawRectangle(HealthBarPos.x + sharedTextures.StatBar.width * scaleMain / 10, HealthBarPos.y + sharedTextures.StatBar.height * scaleMain * 0.2f, HpFillWidth, sharedTextures.StatBar.height * scaleMain * 0.6f, RED);

    // We draw the text :
    // Vector2 textLifeSize = MeasureTextEx(font, TextFormat("%d/%d", HpActuel, HpMax), fontSize, 1);
    Vector2 textLifePos = (Vector2){HealthBarPos.x + sharedTextures.StatBar.width * scaleMain + textLeftargin, HealthBarPos.y + ((sharedTextures.StatBar.height * scaleMain) / 4)};
    DrawTextEx(font, TextFormat("%d/%d", HpActuel, HpMax), textLifePos, fontSize, 1, LIGHTGRAY);

    // We draw the healthBar container :
    DrawTextureEx(sharedTextures.StatBar, HealthBarPos, 0, scaleMain, WHITE);

    //-----------------------------------------------------------
    // DODGE :
    Vector2 textDodgePos = (Vector2){HealthBarPos.x - 52 + textLeftargin, HealthBarPos.y + 2 + ((sharedTextures.StatBar.height * scaleMain) / 4)};
    DrawTextEx(font, TextFormat("%d", DodgeActuel), textDodgePos, fontSize + 10, 1, WHITE);
    //-----------------------------------------------------------
    // Mana :
    // We calculate the ManaBar container position :
    Vector2 ManaBarPos = (Vector2){StatBoardPos.x + Statboard.width * scaleMain * 0.20f, StatBoardPos.y + Statboard.height * scaleMain * 0.77f};

    // We draw the blue rectangle of the current mana :
    float ManaFillWidth = (float)ManaActuel / (float)ManaMax * sharedTextures.StatBar.width * scaleMain * 0.8f;
    DrawRectangle(ManaBarPos.x + sharedTextures.StatBar.width * scaleMain / 10, ManaBarPos.y + sharedTextures.StatBar.height * scaleMain * 0.2f, ManaFillWidth, sharedTextures.StatBar.height * scaleMain * 0.6f, BLUE);

    // We draw the text :
    // Vector2 textManaSize = MeasureTextEx(font, TextFormat("%d/%d", ManaActuel, ManaMax), fontSize, 1);
    Vector2 textManaPos = (Vector2){ManaBarPos.x + sharedTextures.StatBar.width * scaleMain + textLeftargin, ManaBarPos.y + ((sharedTextures.StatBar.height * scaleMain) / 4)};
    DrawTextEx(font, TextFormat("%d/%d", ManaActuel, ManaMax), textManaPos, fontSize, 1, LIGHTGRAY);

    // We draw the manaBar container :
    DrawTextureEx(sharedTextures.StatBar, ManaBarPos, 0, scaleMain, WHITE);

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
    float posX = GetScreenWidth() - (30 * scaleFactor) - padding;
    for (int effectIdx = 0; effectIdx < 5; effectIdx++)
    {
        effect_t *e = (game->caracterData->effects) ? game->caracterData->effects[effectIdx] : NULL;
        if (e && e->value > 0)
        {
            Vector2 pos = (Vector2){posX, StatBoardPos.y - 25 * scaleFactor - marginBottom};
            drawEffect(e, pos, scaleFactor, false, -1);
            posX -= 16 * scaleFactor + gap;
        }
    }
}

// Draw one card of the hand player :
int GuiCard(card_t *card, Vector2 position, float scaleFactor, int idx, boolean isHand, int forcedState)
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
    if ((state < 3) && forcedState < 0)
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
                state = 1; // FOCUSED
                if (isHand)
                {
                    idxHoverCard = idx;
                    scaleFactor += 0.8f;
                    cardWidth = (float)cardInfo.source.width * scaleFactor;
                    cardHeight = (float)cardInfo.source.height * scaleFactor;
                    position.y = GetScreenHeight() - cardHeight;
                }
            }

            // Use IsMouseButtonPressed instead of Released to trigger only once on click
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsGestureDetected(GESTURE_TAP))
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
    DrawTextEx(font, TextFormat("%s", title), titlePos, ((cardWidth) / strlen(title) > cardWidth * 0.08333f) ? cardWidth * 0.08333f : (cardWidth) / strlen(title), 1, GetColor(0xdfdfbeff));

    // textBox calc :
    Rectangle textBox = (Rectangle){position.x + cardWidth * (10 / 96.0f), position.y + cardHeight * (94 / 156.0f), cardWidth * (76 / 96.0f), cardHeight * (51 / 156.0f)};
    Vector2 effectPos;
    float scaleEffectBar = 0.6f * scaleFactor;
    for (int laucherEffectId = 0; laucherEffectId < card->launcherEffectsSize; laucherEffectId++)
    {
        effectPos = (Vector2){
            textBox.x + (((10 + fireEffect.width) * scaleEffectBar) * laucherEffectId),
            textBox.y};
        if (card->launcherEffects && card->launcherEffects[laucherEffectId])
            drawEffect(card->launcherEffects[laucherEffectId], effectPos, scaleEffectBar, true, 0);
    }
    for (int receiverEffectId = 0; receiverEffectId < card->receiverEffectsSize; receiverEffectId++)
    {
        effectPos = (Vector2){
            textBox.x + (((10 + fireEffect.width) * scaleEffectBar) * receiverEffectId),
            textBox.y + (card->launcherEffectsSize != 0 ? (10 + fireEffect.height) * scaleEffectBar : 0)};
        if (card->receiverEffects && card->receiverEffects[receiverEffectId])
            drawEffect(card->receiverEffects[receiverEffectId], effectPos, scaleEffectBar, true, 0);
    }
    // Draw text description and technic:
    float fontDesc = 0.058f * cardWidth;
    DrawTextBoxed(font, TextFormat("%s", card->description), (Rectangle){textBox.x + 5, textBox.y + 2 + ((10 + fireEffect.height) * scaleEffectBar) * ((card->receiverEffectsSize != 0 && card->launcherEffectsSize != 0) ? 2 : 1), textBox.width - 10, textBox.height - 1}, fontDesc, 0.8f, true, WHITE);
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
void drawHand()
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
        if (GuiCard(myCard, position, scaleFactor, i, true, backInteractState))
        {
            // playOneCard mutates the hand list; stop rendering this frame to avoid iterating a modified list
            playOneCard(combat->caracter, combat->enemy, myCard);
            return;
        }
        myHand = myHand->next;
        i++;
    }
}

// Draw the last ennemy card that ennemy has played.
void drawEnnemyCard()
{
    if (ennemyCard != NULL)
    {
        float scaleFactor = 1.8f;
        float cardWidth = (float)cardInfo.source.width * scaleFactor;
        float cardHeight = (float)cardInfo.source.height * scaleFactor;

        Vector2 position = (Vector2){(float)(GetScreenWidth() - cardWidth - 20), (float)(((GetScreenHeight() - cardHeight) / 4) + 10)};
        Vector2 positionText = (Vector2){position.x, position.y - 40};
        GuiCard(ennemyCard, position, scaleFactor, 0, false, true);
        DrawTextEx(font, TextFormat("Dernier carte joué\n par %s:", combat->enemy->name), positionText, 15, 1, WHITE);
    }
}

// Draw the ennemy : sprite, stats and effets.
void drawEnnemy(entity_t *entity)
{
    float fontNameSize = 20;
    float scaleEnnemy = 6.0f;

    Vector2 ennemyNameSize = MeasureTextEx(font, TextFormat("%s", entity->name), fontNameSize, 1);
    Vector2 ennemyNamePos = (Vector2){GetScreenWidth() / 2 - ennemyNameSize.x / 2, 15};
    DrawTextEx(font, TextFormat("%s", entity->name), ennemyNamePos, fontNameSize, 1, WHITE);

    const Vector2 ennemyPos = {ennemyNamePos.x + (ennemyNameSize.x / 2) - ennemySprite.frameRec.width * scaleEnnemy / 2, ennemyNamePos.y + ennemyNameSize.y + 400 - ennemySprite.frameRec.height * scaleEnnemy};
    drawSprite(&ennemySprite, ennemyPos, 0.0f, scaleEnnemy, WHITE);

    int HpMax = getEntityStat(entity, HP)->max;
    int HpActuel = getEntityStat(entity, HP)->current;
    int DodgeActuel = getEntityStat(entity, DODGE)->current;

    float scaleBar = 2.0f;

    Vector2 StatBarPos = (Vector2){(ennemyPos.x + ennemySprite.frameRec.width * scaleEnnemy / 2) - (sharedTextures.StatBar.width * scaleBar / 2), ennemyNamePos.y + 40};

    float HpBarWidth = (float)HpActuel / (float)HpMax * sharedTextures.StatBar.width * scaleBar * 0.8f;
    DrawRectangle(StatBarPos.x + sharedTextures.StatBar.width * scaleBar / 10, StatBarPos.y + sharedTextures.StatBar.height * scaleBar * 0.2f, HpBarWidth, sharedTextures.StatBar.height * scaleBar * 0.6f, RED);

    DrawTextureEx(sharedTextures.StatBar, StatBarPos, 0, scaleBar, WHITE);

    Vector2 DodgePos = (Vector2){StatBarPos.x - 40, StatBarPos.y - 5};
    Vector2 DodgeTextPos = (Vector2){StatBarPos.x - 30, StatBarPos.y};
    DrawTextureEx(DodgeIcon, DodgePos, 0, 4.0, WHITE);
    DrawTextEx(font, TextFormat("%d", DodgeActuel), DodgeTextPos, 25, 1, WHITE);
    int hpFontSize = 15;
    Vector2 hpTextSize = MeasureTextEx(font, TextFormat("%d/%d hp", HpActuel, HpMax), hpFontSize, 1);
    Vector2 hpTextPost = (Vector2){StatBarPos.x + sharedTextures.StatBar.width * scaleBar + 10, StatBarPos.y + (sharedTextures.StatBar.height * scaleBar / 2) - hpTextSize.y / 2};
    DrawTextEx(font, TextFormat("%d/%d  hp", HpActuel, HpMax), hpTextPost, hpFontSize, 1, WHITE);

    // Draw effects of character :
    int marginRight = 50;
    int gap = 18;

    float scaleFactor = 3.0f;
    float posX = ennemyPos.x - (16 * scaleFactor) - marginRight;
    for (int effectIdx = 0; effectIdx < 5; effectIdx++)
    {
        effect_t *e = (entity->effects) ? entity->effects[effectIdx] : NULL;
        if (e && e->value != 0)
        {
            Vector2 pos = (Vector2){posX, ennemyNamePos.y};
            drawEffect(e, pos, scaleFactor, true, -1);
            posX -= 16 * scaleFactor + gap;
        }
    }
    drawEnnemyCard();
}

// Draw one effect (with hover informations)
void drawEffect(effect_t *effect, Vector2 position, float scaleFactor, boolean alignLeft, int forcedState)
{
    if (!effect) return;

    Texture2D textureEffect = (Texture2D){0};
    switch (effect->id)
    {
    case STR_E: textureEffect = strenghtEffect; break;
    case DEX_E: textureEffect = dexterityEffect; break;
    case FIRE_E: textureEffect = fireEffect; break;
    case WEAK_E: textureEffect = weaknessEffect; break;
    case SLOW_E: textureEffect = slowingEffect; break;
    case HP_E: textureEffect = HPEffect; break;
    case DODGE_E: textureEffect = DodgeEffect; break;
    case MANA_E: textureEffect = ManaEffect; break;
    default: break;
    }

    // If texture not loaded, avoid drawing to prevent crashes in WASM
    if (textureEffect.id == 0) return;

    Vector2 positionText = (Vector2){position.x + textureEffect.width * scaleFactor, position.y + textureEffect.height * scaleFactor};
    DrawTextureEx(textureEffect, position, 0, scaleFactor, WHITE);
    DrawTextEx(font, TextFormat("%d", effect->value), positionText, 10 * scaleFactor, 1, WHITE);

    Rectangle bounds = (Rectangle){position.x, position.y, textureEffect.width * scaleFactor, textureEffect.height * scaleFactor};

    Vector2 mousePoint = GetMousePosition();
    // Check if hover and only show details for effect IDs that have texts (STR..SLOW)
    if (CheckCollisionPointRec(mousePoint, bounds) && forcedState < 0)
    {
        int rectWidth = 200;
        int rectHeight = 120;
        float posX = alignLeft ? bounds.x : (bounds.x + bounds.width) - rectWidth;
        Rectangle hoverRect = (Rectangle){posX, bounds.y + bounds.height + 10, rectWidth, rectHeight};
        DrawRectangleRec(hoverRect, GetColor(0x242424ff));
        if (effect->id >= STR_E && (effect->id - STR_E) < 5)
        {
            DrawTextBoxed(font, TextFormat(EFFECT_TEXT[effect->id - STR_E], EFFECT_NAME[effect->id], effect->value), (Rectangle){hoverRect.x + 5, hoverRect.y + 5, hoverRect.width - 5, hoverRect.height - 5}, 15, 1.0f, true, WHITE);
        }
        else
        {
            DrawTextBoxed(font, TextFormat("%s: %d", EFFECT_NAME[effect->id], effect->value), (Rectangle){hoverRect.x + 5, hoverRect.y + 5, hoverRect.width - 5, hoverRect.height - 5}, 15, 1.0f, true, WHITE);
        }
    }
}

// Draw rewind (chosse of cards) after combat win.
void drawRewind()
{
    if (!modalClose)
    {
        backInteractState = 0;
        // DrawRectangleRec((Rectangle){0, 0, GetScreenWidth(), GetScreenWidth()}, GetColor(0x242424ff));
        float backgroundWidth = GetScreenWidth() * 0.8f;
        float backgroundHeight = GetScreenHeight() * 0.8f;
        Rectangle backgroundRect = (Rectangle){GetScreenWidth() / 2 - backgroundWidth / 2, GetScreenHeight() / 2 - backgroundHeight / 2, backgroundWidth, backgroundHeight};
        DrawRectangleRec(backgroundRect, GetColor(0x242424ff));
        float margin = 0.05f;
        // Draw event description :
        Rectangle boundsDesc = (Rectangle){backgroundRect.x + backgroundRect.width * 0.20f, backgroundRect.y + backgroundRect.height * margin, backgroundWidth * 0.6f, backgroundHeight * 0.2f};
        DrawTextBoxed(font, TextFormat("%s", "Vous avez gagné le combat ! Choisissez une carte à ajouter dans votre deck :"), boundsDesc, 22, 1.0f, true, WHITE);
        float scaleFactor = 2.3f;

        float cardWidth = (float)cardInfo.source.width * scaleFactor;
        int gap = 80;
        float cardsTotalWidth = cardWidth * 3 + gap * 2;

        int i = 0;
        deck_t *tmpReward = rewardDeck;
        while (tmpReward != NULL && tmpReward->data != NULL)
        {
            card_t *myCard = tmpReward->data;
            // grossir la carte si hover
            Vector2 position = (Vector2){(float)(GetScreenWidth() / 2 - cardsTotalWidth / 2) + (i * cardWidth) + gap * i, boundsDesc.y + boundsDesc.height + 10};
            if (GuiCard(myCard, position, scaleFactor, i, false, -1))
            {
                // Defer the processing to UpdateCombatScreen to avoid freeing/mutating during draw
                pendingRewardPickIdx = i;
                return;
            }
            tmpReward = tmpReward->next;
            i++;
        }
    }
}

// Draw loose modal
void drawLoose()
{
    if (!modalClose)
    {
        backInteractState = 0;
        // DrawRectangleRec((Rectangle){0, 0, GetScreenWidth(), GetScreenWidth()}, GetColor(0x242424ff));
        float backgroundWidth = GetScreenWidth() * 0.7f;
        float backgroundHeight = GetScreenHeight() * 0.7f;
        Rectangle backgroundRect = (Rectangle){GetScreenWidth() / 2 - backgroundWidth / 2, GetScreenHeight() / 2 - backgroundHeight / 2, backgroundWidth, backgroundHeight};
        DrawRectangleRec(backgroundRect, GetColor(0x242424ff));
        float margin = 0.05f;
        // Draw event description :
        Rectangle boundsDesc = (Rectangle){backgroundRect.x + backgroundRect.width * 0.20f, backgroundRect.y + backgroundRect.height * margin, backgroundWidth * 0.6f, backgroundHeight * 0.2f};
        DrawTextBoxed(font, TextFormat("%s", "C'est vraiment pas de chance (ou mal joué) ! Dans tous les cas vous avez perdu ..."), boundsDesc, 22, 1.0f, true, WHITE);

        // Event choices options :
        float marginTopTextAction = 0.15f;
        float actionTextWidth = backgroundWidth * 0.35f;
        float actionTextHeight = backgroundHeight * 0.30f;
        float posY = boundsDesc.y + boundsDesc.height + (backgroundHeight * marginTopTextAction);
        float gapX = backgroundWidth - actionTextWidth * 2 - (backgroundWidth * margin * 2);
        //  Draw event text choices :
        Rectangle boundsAction1 = (Rectangle){backgroundRect.x + (margin * backgroundWidth), posY, actionTextWidth, actionTextHeight};
        DrawTextBoxed(font, TextFormat("%s", "Souhaitez vous retourner vers le menu et retenter votre chanche ?"), boundsAction1, 17, 1.0f, true, WHITE);

        Rectangle boundsAction2 = (Rectangle){boundsAction1.x + boundsAction1.width + gapX, posY, actionTextWidth, actionTextHeight};
        DrawTextBoxed(font, TextFormat("%s", "Ou souhaitez-vous abandonner dans toute votre plus grande lâcheté ?"), boundsAction2, 17, 1.0f, true, WHITE);

        // Draw button choices :
        float marginTopButton = 0.10f;
        posY = posY + actionTextHeight + (backgroundHeight * marginTopButton);
        float buttonHeight = 60;
        float buttonWidth = backgroundWidth * 0.20f;

        if (GuiButton((Rectangle){boundsAction1.x + (actionTextWidth / 2) - buttonWidth / 2, posY, buttonWidth, buttonHeight}, "MENU", -1, 400))
        {
            TransitionToScreen(MENU);
            backInteractState = -1;
        }
        if (GuiButton((Rectangle){boundsAction2.x + (actionTextWidth / 2) - buttonWidth / 2, posY, buttonWidth, buttonHeight}, "QUITTER", -1, 401))
        {
            shouldClose = true;
            showInGameMenu = false;
            backInteractState = -1;
        }
    }
}

// Load all textures that use for card :
void loadTextureCard()
{
    // Idempotent: if already loaded, do nothing
    if (EnergyIcon.id != 0) return;

    cardInfo.source = (Rectangle){0, 0, 96, 156},
    cardInfo.left = 00;
    cardInfo.top = 00;
    cardInfo.right = 00;
    cardInfo.bottom = 00;

    EnergyIcon = LoadTexture("./asset/Board/bar/unit/Energy.png");

    // Effects texture loading :
    strenghtEffect = LoadTexture("./asset/Misc/Effect/strength.png");
    dexterityEffect = LoadTexture("./asset/Misc/Effect/dexterity.png");
    fireEffect = LoadTexture("./asset/Misc/Effect/Fire.png");
    weaknessEffect = LoadTexture("./asset/Misc/Effect/Weak.png");
    slowingEffect = LoadTexture("./asset/Misc/Effect/Slow.png");
    HPEffect = LoadTexture("./asset/Board/bar/unit/heart.png");
    DodgeEffect = LoadTexture("./asset/Misc/Effect/Dodge.png");
    ManaEffect = LoadTexture("./asset/Board/bar/unit/mana.png");

    // Cards textures loading :
    BasicCardPatch = LoadTexture("./asset/Board/card-basic.png");
    CommonCardPatch = LoadTexture("./asset/Board/card-common.png");
    AtypicCardPatch = LoadTexture("./asset/Board/card-atypic.png");
    RareCardPatch = LoadTexture("./asset/Board/card-rare.png");
    SpecialCardPatch = LoadTexture("./asset/Board/card-special.png");

    ImageCardUnknown = LoadTexture("./asset/Board/image-card/image-card-unknown.png");
}
// Unload all textures that use for card :
void unloadTextureCard()
{
    // Only unload if they were loaded; then zero them to avoid double-unload
    if (EnergyIcon.id != 0) { UnloadTexture(EnergyIcon); EnergyIcon = (Texture2D){0}; }

    if (strenghtEffect.id != 0) { UnloadTexture(strenghtEffect); strenghtEffect = (Texture2D){0}; }
    if (dexterityEffect.id != 0) { UnloadTexture(dexterityEffect); dexterityEffect = (Texture2D){0}; }
    if (fireEffect.id != 0) { UnloadTexture(fireEffect); fireEffect = (Texture2D){0}; }
    if (weaknessEffect.id != 0) { UnloadTexture(weaknessEffect); weaknessEffect = (Texture2D){0}; }
    if (slowingEffect.id != 0) { UnloadTexture(slowingEffect); slowingEffect = (Texture2D){0}; }
    if (HPEffect.id != 0) { UnloadTexture(HPEffect); HPEffect = (Texture2D){0}; }
    if (DodgeEffect.id != 0) { UnloadTexture(DodgeEffect); DodgeEffect = (Texture2D){0}; }
    if (ManaEffect.id != 0) { UnloadTexture(ManaEffect); ManaEffect = (Texture2D){0}; }

    if (BasicCardPatch.id != 0) { UnloadTexture(BasicCardPatch); BasicCardPatch = (Texture2D){0}; }
    if (CommonCardPatch.id != 0) { UnloadTexture(CommonCardPatch); CommonCardPatch = (Texture2D){0}; }
    if (AtypicCardPatch.id != 0) { UnloadTexture(AtypicCardPatch); AtypicCardPatch = (Texture2D){0}; }
    if (RareCardPatch.id != 0) { UnloadTexture(RareCardPatch); RareCardPatch = (Texture2D){0}; }
    if (SpecialCardPatch.id != 0) { UnloadTexture(SpecialCardPatch); SpecialCardPatch = (Texture2D){0}; }

    if (ImageCardUnknown.id != 0) { UnloadTexture(ImageCardUnknown); ImageCardUnknown = (Texture2D){0}; }
}

// Init combat screen (variables for examples) :
void InitCombatScreen(void)
{
    backInteractState = -1;
    // Reset reward flow guards
    rewardResolved = false;
    pendingRewardPickIdx = -1;

    printf("Combat Screen Init\n");
    fflush(stdout);

    CombatBG = LoadTexture("./asset/Misc/bgCombat.png");

    // Note: StatBar and HeartIcon are now shared textures, preloaded in main
    Statboard = LoadTexture("./asset/Board/bar/StatBoard.png");
    // EnergyIcon = LoadTexture("./asset/Board/bar/unit/Energy.png");
    DodgeIcon = LoadTexture("./asset/Board/bar/unit/shield.png");

    // Effects texture loading :
    // strenghtEffect = LoadTexture("./asset/Misc/Effect/strength.png");
    // dexterityEffect = LoadTexture("./asset/Misc/Effect/dexterity.png");
    // fireEffect = LoadTexture("./asset/Misc/Effect/Fire.png");
    // weaknessEffect = LoadTexture("./asset/Misc/Effect/Weak.png");
    // slowingEffect = LoadTexture("./asset/Misc/Effect/Slow.png");
    // HPEffect = LoadTexture("./asset/Board/bar/unit/heart.png");
    // DodgeEffect = LoadTexture("./asset/Misc/Effect/Dodge.png");
    // ManaEffect = LoadTexture("./asset/Board/bar/unit/mana.png");

    // Cards textures loading :
    // BasicCardPatch = LoadTexture("./asset/Board/card-basic.png");
    // CommonCardPatch = LoadTexture("./asset/Board/card-common.png");
    // AtypicCardPatch = LoadTexture("./asset/Board/card-atypic.png");
    // RareCardPatch = LoadTexture("./asset/Board/card-rare.png");
    // SpecialCardPatch = LoadTexture("./asset/Board/card-special.png");

    // Image card unknown loading (for card that doesn't have specific image) :
    // ImageCardUnknown = LoadTexture("./asset/Board/image-card/image-card-unknown.png");

    loadTextureCard();

    // Cards infos :

    // Enemy  :

    // entity_t *ennemy = importEnemyPhase1FromId(BLOUNI);
    // entity_t *ennemy = importMiniBossFromId(PYROX);
    // entity_t *ennemy = importBOSSFromId(GARDIAN_PLUME);
    // entity_t *ennemy = importEnemyPhase2FromId(MANGOUSTINE);
    position_player playerPos = player_position(game->mapData);
    printf("\nSU");
    fflush(stdout);
    if (game->mapData->places[playerPos.x][playerPos.y].isWhat == 3)
    {
        printf("\nSI");
        fflush(stdout);
        ennemy = game->mapData->places[playerPos.x][playerPos.y].eventData->data;
    }
    else
    {
        ennemy = game->mapData->places[playerPos.x][playerPos.y].enemyData;
    }

    printf("\nSA");
    fflush(stdout);
    if (ennemy == NULL)
    {
        printf("Error : no enemy found\n");
        fflush(stdout);
        // exit(EXIT_FAILURE);
    }
    else
    {
        printf("Enemy found : %s\n", ennemy->name);
        fflush(stdout);
    }

    // We load the ennemy sprite :
    char *ennemySpritePath = "./asset/monsters/";
    char *spritePath;
    spritePath = malloc(1 + strlen(ennemySpritePath) + strlen(ennemy->spriteName));
    strcpy(spritePath, ennemySpritePath);
    strcat(spritePath, ennemy->spriteName);
    printf("%s\n", spritePath);
    constructSprite(&ennemySprite, spritePath, ennemy->nbSpritePerLine, 1);

    // We start combat :
    combat = startCombat(game->caracterData, ennemy);
    startFight(combat);

    free(spritePath);

    displayEntityEffectArray(game->caracterData->effects);
}

// Update variables in combat screen (continuously call when on combat screen):
void UpdateCombatScreen(void)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        showInGameMenu = !showInGameMenu;
    }
    // If a reward pick was clicked in draw, process it safely here (update phase)
    if (pendingRewardPickIdx >= 0 && rewardDeck != NULL)
    {
        // Transfer the chosen card into the player's main deck
        tranferOneCardBetweenDeck(&rewardDeck, &(combat->caracter->cardDeck), pendingRewardPickIdx);
        // Free remaining reward cards and close the modal
        freeDeckListAndCard(rewardDeck);
        rewardDeck = NULL;
        backInteractState = -1;
        rewardResolved = true; // prevent reward deck from being recreated while transitioning
        // Mini-boss extra reward
        if (combat->enemy->enemyType == MINIBOSS)
        {
            addItemtoEntityItemList(combat->caracter, getRandomUniqueItemId(combat->caracter));
        }
        // Update map tile (clear enemy, set event)
        position_player playerPos = player_position(game->mapData);
        if (playerPos.x == 10)
        {
            // Boss defeated, go to ending
            TransitionToScreen(ENDING);
        }
        else
        {
            game->mapData->places[playerPos.x][playerPos.y].isWhat = 1; // event
            game->mapData->places[playerPos.x][playerPos.y].enemyData = NULL;
            if (game->mapData->places[playerPos.x][playerPos.y].eventData == NULL)
            {
                game->mapData->places[playerPos.x][playerPos.y].eventData = get_random_event();
            }
            // Use TransitionToScreen instead of setting finishScreen=1
            // This provides a smooth fade transition that masks texture loading lag
            TransitionToScreen(GAMEPLAY);
        }
        // Reset pending pick
        pendingRewardPickIdx = -1;
        // Exit early this frame to avoid re-creating rewardDeck while transition runs
        return;
    }
    // Only create the reward deck once when combat ends
    if (checkEndCombat(combat) && rewardDeck == NULL && !rewardResolved)
    {
        if (checkVictory(combat))
        {
            rewardDeck = createRewardDeck();
        }
    }

    updateSprite(&ennemySprite);
}

// Draw combat screen (continuously call when on combat screen):
void DrawCombatScreen(void)
{
    float scaleBackground = (float)(GetScreenWidth() / (float)background.width);
    DrawTextureEx(CombatBG, (Vector2){0, 0}, 0, scaleBackground, WHITE);
    ClearBackground(GetColor(0x3f3f74ff));

    // Safety: if combat or entities are invalid, just show background
    if (combat == NULL || combat->caracter == NULL || combat->enemy == NULL)
    {
        return;
    }

    drawStatBoard();

    // POUR TEST COMBAT : (A RETIRER PLUS TARD)
    int buttonWidth = 150;
    int buttonHeight = 50;
    if (GuiButton((Rectangle){GetScreenWidth() - buttonWidth * 2 - 10, 10, buttonWidth, buttonHeight}, "END TURN", backInteractState, 402))
    {
        moveCardsFromHand(combat->caracter->board, false);
        playTurn(combat->enemy);
        if (!checkEndCombat(combat))
        {

            card_t *toFree = ennemyCard;
            ennemyCard = getTrueCardValue(combat->enemy, playEnemyCards(combat));
            if (toFree)
            {
                freeCard(toFree);
            }
            playTurn(combat->caracter);
        }
    }

    drawEnnemy(combat->enemy);
    drawHand();

    if (checkEndCombat(combat) && rewardDeck != NULL)
    {
        drawRewind();
    }
    if (checkEndCombat(combat) && (!checkVictory(combat)))
    {
        drawLoose();
    }
}

// Unload all textures from combat screen and free dynamic allocated pointer :
void UnloadCombatScreen(void)
{
    UnloadTexture(CombatBG);
    // Note: StatBar is shared, do not unload here
    UnloadTexture(Statboard);
    UnloadTexture(DodgeIcon);
    UnloadTexture(ennemySprite.texture);
    
    card_t *cardToFree = ennemyCard;
    ennemyCard = NULL;
    
    // Safety guard: only access combat members if combat pointer is valid
    if (combat != NULL && combat->caracter != NULL)
    {
        stat_t *dodgeStat = getEntityStat(combat->caracter, DODGE);
        if (dodgeStat != NULL)
        {
            dodgeStat->current = 0;
        }
    }
    
    // Guard: enemy might not have played any card yet
    if (cardToFree != NULL)
    {
        freeCard(cardToFree);
    }
    
    // Safety: if reward deck was never picked/cleared, free it to avoid leaks
    if (rewardDeck != NULL)
    {
        freeDeckListAndCard(rewardDeck);
        rewardDeck = NULL;
    }
    
    // Safety: only free combat if it exists
    if (combat != NULL)
    {
        freeCombat(combat);
        // Note: combat pointer will be reassigned in InitCombatScreen, no need to NULL it
    }
}
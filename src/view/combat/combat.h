#ifndef GUI_COMBAT
#define GUI_COMBAT

#include "./../../../include/raylib.h"
#include "./../../model/game/combat/combat.h"
#include "./../../model/game/misc/effect/effect.h"
#include "./../utils/utils.h"

// Tableau contenant les noms des effets :
extern char *EFFECT_NAME[13];
extern char *EFFECT_TEXT[5];
extern NPatchInfo cardInfo;

// Draw stats of the player :
void drawStatBoard();

// Draw one card of the hand player :
int GuiCard(card_t *card, Vector2 position, float scaleFactor, int idx, boolean isHand, int forcedState);

// Draw all cards of the hand player :
void drawHand();

// Draw the last ennemy card that ennemy has played.
void drawEnnemyCard();

// Draw the ennemy : sprite, stats and effets.
void drawEnnemy(entity_t *entity);

// Draw one effect (with hover informations)
void drawEffect(effect_t *effect, Vector2 position, float scaleFactor, boolean alignLeft, int forcedState);

// Draw rewind (chosse of cards) after combat win.
void drawRewind();

// Draw loose modal
void drawLoose();

// Load all textures that use for card :
void loadTextureCard();

// Unload all textures that use for card :
void unloadTextureCard();

// Init combat screen (variables for examples) :
void InitCombatScreen(void);

// Update variables in combat screen (continuously call when on combat screen):
void UpdateCombatScreen(void);

// Draw combat screen (continuously call when on combat screen):
void DrawCombatScreen(void);

// Unload all textures from combat screen and free dynamic allocated pointer :
void UnloadCombatScreen(void);

#endif
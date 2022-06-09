#include "utils.h"
#include <stdio.h>

void constructSprite(Sprite* sprite, char* texturePath, int nbFramesPerLine, int nbLines, Vector2 position)
{
    sprite->texture = LoadTexture(texturePath);
    sprite->nbFramesPerLine = nbFramesPerLine;
    sprite->nbLines = nbLines;

    float frameWidth = (float)(sprite->texture.width/sprite->nbFramesPerLine);   // Sprite one frame rectangle width
    float frameHeight = (float)(sprite->texture.height/sprite->nbLines);
    
    sprite->frameRec = (Rectangle){ 0, 0, frameWidth, frameHeight };
    sprite->position = position;

    sprite->framesCounter = 0;
    sprite->currentFrame = 0;
    sprite->currentLine = 0;
}

void updateSprite(Sprite* sprite)
{
    float frameWidth = (float)(sprite->texture.width/sprite->nbFramesPerLine);   // Sprite one frame rectangle width
    float frameHeight = (float)(sprite->texture.height/sprite->nbLines);


    sprite->framesCounter++;
    if (sprite->framesCounter++ > 2)
    {
        sprite->currentFrame++;
        if (sprite->currentFrame >= sprite->nbFramesPerLine)
        {
            sprite->currentFrame = 0;
            sprite->currentLine++;
            if (sprite->currentLine >= sprite->nbLines)
            {
                sprite->currentLine = 0;
            }
        }
        sprite->framesCounter = 0;
    }
    
    sprite->frameRec.x = frameWidth*sprite->currentFrame;
    sprite->frameRec.y = frameHeight*sprite->currentLine;
}
void drawSprite(Sprite* sprite, Vector2 position, float angle, float scale, Color c) {
    DrawTexturePro(sprite->texture, sprite->frameRec, 
                    (Rectangle){position.x,position.y, sprite->frameRec.width * scale, sprite->frameRec.height * scale}, 
                    (Vector2){0, 0}, 
                    angle, 
                    c);
}

// Gui button (immediate mode, update and draw)
bool GuiButton(Rectangle bounds, const char *text, int forcedState)
{
    static const int textColor[4] = { 0x255a69ff, 0x92e3f7ff, 0x66b2c4ff, 0xd6d6d6ff };

    int state = (forcedState >= 0)? forcedState : 0;                // NORMAL
    bool pressed = false;
    Vector2 textSize = MeasureTextEx(font, text, font.baseSize, 1);

    int textPosAdd = 0;

    // Update control
    //--------------------------------------------------------------------
    if ((state < 3) && (forcedState < 0))
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            // PRESSED
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                state = 2;
                textPosAdd = 10;
            }    
            else state = 1;                                         // FOCUSED

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsGestureDetected(GESTURE_TAP))
            {
                pressed = true;
                PlaySound(buttonSound);
            }
        }
    }

    buttonInfo.source.x = 115*state;

    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    //DrawRectangleRec(bounds, GREEN);
    //DrawRectangleLinesEx(bounds, 4, DARKGREEN);
    DrawTextureNPatch(buttonPatch, buttonInfo, bounds, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
    DrawTextEx(font, text, (Vector2){ bounds.x + bounds.width/2 - textSize.x/2, bounds.y + bounds.height/2 - textSize.y/2 - 4 + textPosAdd }, font.baseSize, 1, GetColor(textColor[state]));
    //------------------------------------------------------------------

    return pressed;
}

// Request transition to next screen
void TransitionToScreen(int screen)
{
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

void drawInGameMenu() {
    //for buttons :
    int buttonWidth = 300;
    int buttonHeight = 80;
    int gap = 20;

    //for background :
    int paddingX = 80;
    int paddingY = 80;
    int menuWidth = buttonWidth+2*paddingX;
    int menuHeight = buttonHeight*3+gap*2+2*paddingY;
    DrawRectangle(GetScreenWidth()/2 -menuWidth/2, GetScreenHeight()/2 -menuHeight/2, menuWidth, menuHeight, BLACK);

    int topPosButton = GetScreenHeight()/2-menuHeight/2+paddingY;
    if (GuiButton((Rectangle){ GetScreenWidth()/2 - buttonWidth/2, topPosButton, buttonWidth, buttonHeight }, "CONTINUER", -1))
    {
        showInGameMenu = false;
    }
    if (GuiButton((Rectangle){ GetScreenWidth()/2 - buttonWidth/2, topPosButton+buttonHeight+gap, buttonWidth, buttonHeight }, "RETOUR VERS LE MENU", -1))
    {
        TransitionToScreen(MENU);
        showInGameMenu = false;
    }
    if (GuiButton((Rectangle){ GetScreenWidth()/2 - buttonWidth/2, topPosButton+(2*(buttonHeight+gap)), buttonWidth, buttonHeight }, "QUITTER", -1))
    {
        shouldClose = true;
        showInGameMenu = false;
    }
}



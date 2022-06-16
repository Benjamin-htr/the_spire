#include "utils.h"
#include <stdio.h>

void constructSprite(Sprite *sprite, char *texturePath, int nbFramesPerLine, int nbLines)
{
    sprite->texture = LoadTexture(texturePath);
    sprite->nbFramesPerLine = nbFramesPerLine;
    sprite->nbLines = nbLines;

    float frameWidth = (float)(sprite->texture.width / sprite->nbFramesPerLine); // Sprite one frame rectangle width
    float frameHeight = (float)(sprite->texture.height / sprite->nbLines);

    sprite->frameRec = (Rectangle){0, 0, frameWidth, frameHeight};
    sprite->position = (Vector2){0, 0};

    sprite->framesCounter = 0;
    sprite->currentFrame = 0;
    sprite->currentLine = 0;
}

void updateSprite(Sprite *sprite)
{
    float frameWidth = (float)(sprite->texture.width / sprite->nbFramesPerLine); // Sprite one frame rectangle width
    float frameHeight = (float)(sprite->texture.height / sprite->nbLines);

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

    sprite->frameRec.x = frameWidth * sprite->currentFrame;
    sprite->frameRec.y = frameHeight * sprite->currentLine;
}
void drawSprite(Sprite *sprite, Vector2 position, float angle, float scale, Color c)
{
    DrawTexturePro(sprite->texture, sprite->frameRec,
                   (Rectangle){position.x, position.y, sprite->frameRec.width * scale, sprite->frameRec.height * scale},
                   (Vector2){0, 0},
                   angle,
                   c);
}

// Gui button (immediate mode, update and draw)
bool GuiButton(Rectangle bounds, const char *text, int forcedState)
{
    static const int textColor[4] = {0x255a69ff, 0x92e3f7ff, 0x66b2c4ff, 0xd6d6d6ff};

    int state = (forcedState >= 0) ? forcedState : 0; // NORMAL
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
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                state = 2;
                textPosAdd = 10;
            }
            else
                state = 1; // FOCUSED

            if (IsGestureDetected(GESTURE_TAP))
            {
                pressed = true;
                PlaySound(buttonSound);
            }
        }
    }

    buttonInfo.source.x = 115 * state;

    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    // DrawRectangleRec(bounds, GREEN);
    // DrawRectangleLinesEx(bounds, 4, DARKGREEN);
    DrawTextureNPatch(buttonPatch, buttonInfo, bounds, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    DrawTextEx(font, text, (Vector2){bounds.x + bounds.width / 2 - textSize.x / 2, bounds.y + bounds.height / 2 - textSize.y / 2 - 4 + textPosAdd}, font.baseSize, 1, GetColor(textColor[state]));
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

void drawInGameMenu()
{
    // for buttons :
    int buttonWidth = 300;
    int buttonHeight = 80;
    int gap = 20;

    // for background :
    int paddingX = 80;
    int paddingY = 80;
    int menuWidth = buttonWidth + 2 * paddingX;
    int menuHeight = buttonHeight * 3 + gap * 2 + 2 * paddingY;
    DrawRectangle(GetScreenWidth() / 2 - menuWidth / 2, GetScreenHeight() / 2 - menuHeight / 2, menuWidth, menuHeight, BLACK);

    int topPosButton = GetScreenHeight() / 2 - menuHeight / 2 + paddingY;
    if (GuiButton((Rectangle){GetScreenWidth() / 2 - buttonWidth / 2, topPosButton, buttonWidth, buttonHeight}, "CONTINUER", -1))
    {
        showInGameMenu = false;
    }
    if (GuiButton((Rectangle){GetScreenWidth() / 2 - buttonWidth / 2, topPosButton + buttonHeight + gap, buttonWidth, buttonHeight}, "RETOUR VERS LE MENU", -1))
    {
        TransitionToScreen(MENU);
        showInGameMenu = false;
    }
    if (GuiButton((Rectangle){GetScreenWidth() / 2 - buttonWidth / 2, topPosButton + (2 * (buttonHeight + gap)), buttonWidth, buttonHeight}, "QUITTER", -1))
    {
        shouldClose = true;
        showInGameMenu = false;
    }
}

// Draw text using font inside rectangle limits
void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint)
{
    DrawTextBoxedSelectable(font, text, rec, fontSize, spacing, wordWrap, tint, 0, 0, WHITE, WHITE);
}

// Draw text using font inside rectangle limits with support for text selection
void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint)
{
    int length = TextLength(text); // Total length in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0;    // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f; // Offset X to next character to draw

    float scaleFactor = fontSize / (float)font.baseSize; // Character rectangle scaling factor

    // Word/character wrapping mechanism variables
    enum
    {
        MEASURE_STATE = 0,
        DRAW_STATE = 1
    };
    int state = wordWrap ? MEASURE_STATE : DRAW_STATE;

    int startLine = -1; // Index where to begin drawing (where a line begins)
    int endLine = -1;   // Index where to stop drawing (where a line ends)
    int lastk = -1;     // Holds last value of the character position

    for (int i = 0, k = 0; i < length; i++, k++)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f)
            codepointByteCount = 1;
        i += (codepointByteCount - 1);

        float glyphWidth = 0;
        if (codepoint != '\n')
        {
            glyphWidth = (font.glyphs[index].advanceX == 0) ? font.recs[index].width * scaleFactor : font.glyphs[index].advanceX * scaleFactor;

            if (i + 1 < length)
                glyphWidth = glyphWidth + spacing;
        }

        // NOTE: When wordWrap is ON we first measure how much of the text we can draw before going outside of the rec container
        // We store this info in startLine and endLine, then we change states, draw the text between those two variables
        // and change states again and again recursively until the end of the text (or until we get outside of the container).
        // When wordWrap is OFF we don't need the measure state so we go to the drawing state immediately
        // and begin drawing on the next line before we can get outside the container.
        if (state == MEASURE_STATE)
        {
            // TODO: There are multiple types of spaces in UNICODE, maybe it's a good idea to add support for more
            // Ref: http://jkorpela.fi/chars/spaces.html
            if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n'))
                endLine = i;

            if ((textOffsetX + glyphWidth) > rec.width)
            {
                endLine = (endLine < 1) ? i : endLine;
                if (i == endLine)
                    endLine -= codepointByteCount;
                if ((startLine + codepointByteCount) == endLine)
                    endLine = (i - codepointByteCount);

                state = !state;
            }
            else if ((i + 1) == length)
            {
                endLine = i;
                state = !state;
            }
            else if (codepoint == '\n')
                state = !state;

            if (state == DRAW_STATE)
            {
                textOffsetX = 0;
                i = startLine;
                glyphWidth = 0;

                // Save character position when we switch states
                int tmp = lastk;
                lastk = k - 1;
                k = tmp;
            }
        }
        else
        {
            if (codepoint == '\n')
            {
                if (!wordWrap)
                {
                    textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
                    textOffsetX = 0;
                }
            }
            else
            {
                if (!wordWrap && ((textOffsetX + glyphWidth) > rec.width))
                {
                    textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
                    textOffsetX = 0;
                }

                // When text overflows rectangle height limit, just stop drawing
                if ((textOffsetY + font.baseSize * scaleFactor) > rec.height)
                    break;

                // Draw selection background
                bool isGlyphSelected = false;
                if ((selectStart >= 0) && (k >= selectStart) && (k < (selectStart + selectLength)))
                {
                    DrawRectangleRec((Rectangle){rec.x + textOffsetX - 1, rec.y + textOffsetY, glyphWidth, (float)font.baseSize * scaleFactor}, selectBackTint);
                    isGlyphSelected = true;
                }

                // Draw current character glyph
                if ((codepoint != ' ') && (codepoint != '\t'))
                {
                    DrawTextCodepoint(font, codepoint, (Vector2){rec.x + textOffsetX, rec.y + textOffsetY}, fontSize, isGlyphSelected ? selectTint : tint);
                }
            }

            if (wordWrap && (i == endLine))
            {
                textOffsetY += (font.baseSize + font.baseSize / 2) * scaleFactor;
                textOffsetX = 0;
                startLine = endLine;
                endLine = -1;
                glyphWidth = 0;
                selectStart += lastk - k;
                k = lastk;

                state = !state;
            }
        }

        textOffsetX += glyphWidth;
    }
}
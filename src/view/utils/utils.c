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
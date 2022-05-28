#include "./../../../include/raylib.h"
#include "credits.h"
#include <stdio.h>

void InitCreditsScreen(void)
{
    printf("Credits Screen Init\n");
    
}
void UpdateCreditsScreen(void)
{
    
}
void DrawCreditsScreen(void)
{
    DrawText("CREDITS SCREEN", 190, 200, 20, LIGHTGRAY);
}
void UnloadCreditsScreen(void)
{
    
}
int FinishCreditsScreen(void)
{
    return 0;
}
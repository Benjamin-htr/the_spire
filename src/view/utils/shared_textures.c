#include "shared_textures.h"
#include <stdio.h>

// Global instance of shared textures
SharedTextures sharedTextures = {0};

void LoadSharedTextures(void)
{
    if (sharedTextures.isLoaded)
    {
        printf("WARNING: Shared textures already loaded, skipping reload\n");
        return;
    }

    printf("Loading shared textures...\n");
    sharedTextures.StatBar = LoadTexture("./asset/Board/bar/StatBar.png");
    sharedTextures.HeartIcon = LoadTexture("./asset/Board/bar/unit/heart.png");
    sharedTextures.isLoaded = true;
    printf("Shared textures loaded successfully\n");
}

void UnloadSharedTextures(void)
{
    if (!sharedTextures.isLoaded)
    {
        return;
    }

    printf("Unloading shared textures...\n");
    if (sharedTextures.StatBar.id != 0)
    {
        UnloadTexture(sharedTextures.StatBar);
        sharedTextures.StatBar = (Texture2D){0};
    }
    if (sharedTextures.HeartIcon.id != 0)
    {
        UnloadTexture(sharedTextures.HeartIcon);
        sharedTextures.HeartIcon = (Texture2D){0};
    }
    sharedTextures.isLoaded = false;
    printf("Shared textures unloaded\n");
}

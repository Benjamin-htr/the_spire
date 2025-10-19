#ifndef SHARED_TEXTURES
#define SHARED_TEXTURES

#include "./../../../include/raylib.h"

// Shared textures between gameplay and combat screens
// These are loaded once at game start and kept in memory
typedef struct {
    Texture2D StatBar;
    Texture2D HeartIcon;
    bool isLoaded;
} SharedTextures;

extern SharedTextures sharedTextures;

// Load shared textures once at game start
void LoadSharedTextures(void);

// Unload shared textures at game end
void UnloadSharedTextures(void);

#endif

#pragma once
#include "raylib.h"
void DrawTextCentered(const char* text, int centerX, int centerY, int fontSize, Color color);

void DrawTextureCentered(Texture2D texture, Vector2 position, Vector2 scale, float rotation, Color tint);

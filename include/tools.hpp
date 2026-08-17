#pragma once
#include "raylib.h"
void DrawTextCentered(const char* text, int centerX, int centerY, int fontSize, Color color);

void DrawTextCentered(const char* text, Vector2 position, int fontSize, Vector2 scale, float rotation, Color tint, Font font);

void DrawTextureCentered(Texture2D texture, Vector2 position, Vector2 scale, float rotation, Color tint);

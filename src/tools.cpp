#include "raylib.h"
#include "tools.hpp"

void DrawTextCentered(const char* text, int centerX, int centerY, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    int posX = centerX - (textWidth / 2);
    int posY = centerY - (fontSize / 2);
    DrawText(text, posX, posY, fontSize, color);
}

void DrawTextureCentered(Texture2D texture, int centerX, int centerY, Color tint) {
    int posX = centerX - (texture.width / 2);
    int posY = centerY - (texture.height / 2);
    DrawTexture(texture, posX, posY, tint);
}

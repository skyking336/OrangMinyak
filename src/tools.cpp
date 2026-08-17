#include "raylib.h"
#include "tools.hpp"

void DrawTextCentered(const char* text, int centerX, int centerY, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    int posX = centerX - (textWidth / 2);
    int posY = centerY - (fontSize / 2);
    DrawText(text, posX, posY, fontSize, color);
}

void DrawTextCentered(const char* text, Vector2 position, int fontSize, Vector2 scale, float rotation, Color tint, Font font) {
    float scaledFontSize = fontSize * scale.x;
    float spacing = 2.0f;
    Vector2 textSize = MeasureTextEx(font, text, scaledFontSize, spacing);
    Vector2 origin = { textSize.x / 2.0f, textSize.y / 2.0f };
    DrawTextPro(font, text, position, origin, rotation, scaledFontSize, spacing, tint);
}

void DrawTextureCentered(Texture2D texture, Vector2 position, Vector2 scale, float rotation, Color tint) {
    Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle dest = { position.x, position.y, texture.width * scale.x, texture.height * scale.y };
    Vector2 origin = { dest.width / 2.0f, dest.height / 2.0f };
    DrawTexturePro(texture, source, dest, origin, rotation, tint);
}

#include "UIObject.hpp"
#include "tools.hpp"

void UITexture::draw(AssetManager& assets) const {
    if (hidden) return;
    Texture2D tex = assets.getOrLoadTexture(getTexturePath());
    DrawTextureCentered(tex, position, scale, rotation, tint);
    return;
}

void UIText::draw(AssetManager& assets) const {
    if (hidden) return;
    DrawTextCentered(Text.c_str(), position, baseFontSize, scale, rotation, tint, font);
}   
#include "UIObject.hpp"
#include "tools.hpp"

void UIObject::draw(AssetManager& assets) const {
    if (hidden) return;
    Texture2D tex = assets.getOrLoadTexture(getTexturePath());
    DrawTextureCentered(tex, position, scale, rotation, tint);
    return;
    
}
#include "Enemy.hpp"
#include "tools.hpp"

void Enemy::draw(AssetManager& assets) const {
    if (hidden) return;
    Texture2D tex = assets.getOrLoadTexture(getTexturePath());
    DrawTextureCentered(tex, position.x, position.y, WHITE);
    return;
    
}
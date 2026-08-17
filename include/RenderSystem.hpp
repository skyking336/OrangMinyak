#pragma once
#include <entt/entt.hpp>
#include "AssetManager.hpp"
#include "Components.hpp"
#include "tools.hpp"
#include <vector>
#include <algorithm>

// ==========================================
// SYSTEMS
// Systems are PURE LOGIC. They do not store data, they just query the Registry
// for entities that have the exact components they care about.
// ==========================================

class RenderSystem {
public:
    static void Render(entt::registry& registry, AssetManager& assets) {
        
        // 1. Asset Pipeline: Ensure all Sprites have their textures loaded
        auto spriteView = registry.view<SpriteComponent>();
        for (auto [entity, sprite] : spriteView.each()) {
            if (sprite.texture.id == 0) {
                sprite.texture = assets.getOrLoadTexture(sprite.texturePath);
            }
        }

        // 2. Asset Pipeline: Ensure all Text components have their fonts loaded
        auto textView = registry.view<TextComponent>();
        for (auto [entity, text] : textView.each()) {
            if ((text.font.texture.id == 0)) {
                text.font = assets.getOrLoadFont(text.fontPath);
            }
        }

        // 3. Get EVERY entity in the game that can be rendered (has Transform and Render info)
        auto renderables = registry.view<TransformComponent, RenderComponent>();
        
        // 4. Sort them by Z-Index
        // EnTT gives us the Entity IDs, we just look up their RenderComponent to compare Z-Indexes
        std::vector<entt::entity> sortedEntities{renderables.begin(), renderables.end()};
        std::stable_sort(sortedEntities.begin(), sortedEntities.end(), [&registry](entt::entity a, entt::entity b) {
            return registry.get<RenderComponent>(a).zIndex < registry.get<RenderComponent>(b).zIndex;
        });

        // 5. Draw Everything!
        for (entt::entity entity : sortedEntities) {
            auto& render = registry.get<RenderComponent>(entity);
            if (render.hidden) continue;

            auto& transform = registry.get<TransformComponent>(entity);

            // If the entity has a Sprite, draw it!
            if (registry.any_of<SpriteComponent>(entity)) {
                auto& sprite = registry.get<SpriteComponent>(entity);
                DrawTextureCentered(sprite.texture, transform.position, transform.scale, transform.rotation, render.tint);
            }
            
            // If the entity has Text, draw it! (Note: An entity can have BOTH a sprite and text simultaneously!)
            if (registry.any_of<TextComponent>(entity)) {
                auto& text = registry.get<TextComponent>(entity);
                DrawTextCentered(text.text.c_str(), transform.position, text.fontSize, transform.scale, transform.rotation, text.color, text.font);
            }
        }
    }
};

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
        
        auto spriteView = registry.view<SpriteComponent>();
        for (auto [entity, sprite] : spriteView.each()) {
            if (sprite.texture.id == 0) {
                sprite.texture = assets.getOrLoadTexture(sprite.texturePath);
            }
        }

        auto textView = registry.view<TextComponent>();
        for (auto [entity, text] : textView.each()) {
            if ((text.font.texture.id == 0)) {
                text.font = assets.getOrLoadFont(text.fontPath);
            }
        }

        auto renderables = registry.view<TransformComponent, RenderComponent>();
        
        std::vector<entt::entity> sortedEntities{renderables.begin(), renderables.end()};
        std::stable_sort(sortedEntities.begin(), sortedEntities.end(), [&registry](entt::entity a, entt::entity b) {
            return registry.get<RenderComponent>(a).zIndex < registry.get<RenderComponent>(b).zIndex;
        });

        for (entt::entity entity : sortedEntities) {
            auto& render = registry.get<RenderComponent>(entity);
            if (render.hidden) continue;

            auto& transform = registry.get<TransformComponent>(entity);

            if (registry.any_of<SpriteComponent>(entity)) {
                auto& sprite = registry.get<SpriteComponent>(entity);
                
                bool useShader = registry.any_of<HoverTiltComponent>(entity);
                if (useShader) {
                    auto& tilt = registry.get<HoverTiltComponent>(entity);
                    Shader shader = assets.getOrLoadShader("assets/shaders/tilt.vs", "");
                    
                    int tiltLoc = GetShaderLocation(shader, "tilt");
                    float tiltVec[2] = { tilt.currentTiltX, tilt.currentTiltY };
                    SetShaderValue(shader, tiltLoc, tiltVec, SHADER_UNIFORM_VEC2);
                    
                    int originLoc = GetShaderLocation(shader, "origin");
                    float originVec[2] = { transform.position.x, transform.position.y };
                    SetShaderValue(shader, originLoc, originVec, SHADER_UNIFORM_VEC2);
                    
                    BeginShaderMode(shader);
                }

                DrawTextureCentered(sprite.texture, transform.position, transform.scale, transform.rotation, render.tint);
                
                if (useShader) {
                    EndShaderMode();
                }
            }
            
            if (registry.any_of<TextComponent>(entity)) {
                auto& text = registry.get<TextComponent>(entity);
                DrawTextCentered(text.text.c_str(), transform.position, text.fontSize, transform.scale, transform.rotation, text.color, text.font);
            }

            if (registry.any_of<CircleRenderComponent>(entity)) {
                auto& circle = registry.get<CircleRenderComponent>(entity);
                DrawCircle(transform.position.x, transform.position.y, circle.radius * transform.scale.x, circle.color);
            }
        }
    }
};

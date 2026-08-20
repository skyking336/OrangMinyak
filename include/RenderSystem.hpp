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
                
                bool useShader = registry.any_of<HoverTiltComponent>(entity) || registry.any_of<CoinFlipComponent>(entity);
                if (useShader) {
                    if (registry.any_of<HoverTiltComponent>(entity)) {
                        auto& tilt = registry.get<HoverTiltComponent>(entity);
                        Shader shader = assets.getOrLoadShader("assets/shaders/tilt.vs", "");
                        
                        int tiltLoc = GetShaderLocation(shader, "tilt");
                        float tiltVec[2] = { tilt.currentTiltX, tilt.currentTiltY };
                        SetShaderValue(shader, tiltLoc, tiltVec, SHADER_UNIFORM_VEC2);
                        
                        int originLoc = GetShaderLocation(shader, "origin");
                        float originVec[2] = { transform.position.x, transform.position.y };
                        SetShaderValue(shader, originLoc, originVec, SHADER_UNIFORM_VEC2);
                        
                        BeginShaderMode(shader);
                    } else if (registry.any_of<CoinFlipComponent>(entity)) {
                        auto& flip = registry.get<CoinFlipComponent>(entity);
                        Shader shader = assets.getOrLoadShader("", "assets/shaders/flip.fs");
                        
                        int headsLoc = GetShaderLocation(shader, "headsTexture");
                        int headsVal = 0; // texture0
                        SetShaderValue(shader, headsLoc, &headsVal, SHADER_UNIFORM_INT);

                        int flipLoc = GetShaderLocation(shader, "flipAngle");
                        float fAngle = flip.flipAngle;
                        SetShaderValue(shader, flipLoc, &fAngle, SHADER_UNIFORM_FLOAT);
                        
                        // Set tails texture
                        Texture2D tailsTex = assets.getOrLoadTexture("assets/NumberCard.png");
                        int tailsLoc = GetShaderLocation(shader, "tailsTexture");
                        SetShaderValueTexture(shader, tailsLoc, tailsTex);
                        
                        BeginShaderMode(shader);
                    }
                }

                Vector2 drawScale = transform.scale;
                if (registry.any_of<CoinFlipComponent>(entity)) {
                    auto& flip = registry.get<CoinFlipComponent>(entity);
                    drawScale.y *= abs(cos(flip.flipAngle));
                }

                DrawTextureCentered(sprite.texture, transform.position, drawScale, transform.rotation, render.tint);
                
                if (useShader) {
                    EndShaderMode();
                }
            }
            
            if (registry.any_of<RectRenderComponent>(entity)) {
                auto& rect = registry.get<RectRenderComponent>(entity);
                Rectangle r = { transform.position.x - (rect.width * transform.scale.x)/2.0f, 
                                transform.position.y - (rect.height * transform.scale.y)/2.0f, 
                                rect.width * transform.scale.x, 
                                rect.height * transform.scale.y };
                DrawRectangleRec(r, rect.color);
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

#pragma once
#include <entt/entt.hpp>
#include "raylib.h"
#include "Components.hpp"
#include <algorithm>

#include "GameState.hpp"

class InputSystem {
public:
    static void Update(entt::registry& registry, int virtualWidth, int virtualHeight, GameState& game, GameScene& nextScene) {
        
        // Calculate the Virtual Mouse Position
        // Because we allow resizable window, GetMousePosition() returns the physical window pixels.
        // We need to map it back to the 1440x810 virtual canvas
        float scale = std::min((float)GetScreenWidth() / virtualWidth, (float)GetScreenHeight() / virtualHeight);
        
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (virtualWidth * scale)) * 0.5f) / scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (virtualHeight * scale)) * 0.5f) / scale;

        auto view = registry.view<TransformComponent, BoxColliderComponent>();
        
        static entt::entity currentlyHovered = entt::null;
        bool stillHoveringCurrent = false;

        // First pass: Check if the currently hovered entity is still being hovered
        if (currentlyHovered != entt::null && registry.valid(currentlyHovered)) {
            if (registry.any_of<TransformComponent, BoxColliderComponent>(currentlyHovered)) {
                auto& transform = registry.get<TransformComponent>(currentlyHovered);
                auto& collider = registry.get<BoxColliderComponent>(currentlyHovered);
                
                bool canHover = true;
                if (registry.any_of<TweenComponent>(currentlyHovered)) {
                    auto& tween = registry.get<TweenComponent>(currentlyHovered);
                    if (tween.positionTween.has_value() || tween.scaleTween.has_value() || tween.colorTween.has_value()) {
                        canHover = false;
                    }
                }

                if (canHover) {
                    float scaledWidth = collider.width * transform.scale.x;
                    float scaledHeight = collider.height * transform.scale.y;
                    
                    // Transform virtualMouse into the card's local space to handle rotation
                    float dx = virtualMouse.x - transform.position.x;
                    float dy = virtualMouse.y - transform.position.y;
                    
                    float rad = -transform.rotation * DEG2RAD; // Negative for inverse rotation
                    float cosR = cosf(rad);
                    float sinR = sinf(rad);
                    
                    float localMouseX = dx * cosR - dy * sinR;
                    float localMouseY = dx * sinR + dy * cosR;
                    
                    float halfWidth = scaledWidth / 2.0f;
                    float halfHeight = scaledHeight / 2.0f;

                    if (localMouseX >= -halfWidth && localMouseX <= halfWidth && 
                        localMouseY >= -halfHeight && localMouseY <= halfHeight) {
                        stillHoveringCurrent = true;
                    }
                }
            }
        }

        if (!stillHoveringCurrent) {
            if (currentlyHovered != entt::null && registry.valid(currentlyHovered)) {
                if (registry.any_of<RenderComponent>(currentlyHovered)) {
                    auto& render = registry.get<RenderComponent>(currentlyHovered);
                    render.zIndex = render.baseZIndex;
                }

                // if (registry.all_of<TransformComponent, HoverTiltComponent>(currentlyHovered)) {
                //     auto& transform = registry.get<TransformComponent>(currentlyHovered);
                //     auto& tilt = registry.get<HoverTiltComponent>(currentlyHovered);
                //     transform.rotation = tilt.baseRotation;
                // }
            }
            currentlyHovered = entt::null;
        }

        for (auto [entity, transform, collider] : view.each()) {
            if (currentlyHovered != entt::null && entity != currentlyHovered) {
                collider.isHovered = false;
                transform.scale = { 2.0f, 2.0f };
                if (registry.any_of<HoverTiltComponent>(entity)) {
                    auto& tilt = registry.get<HoverTiltComponent>(entity);
                    tilt.targetTiltX = 0.0f;
                    tilt.targetTiltY = 0.0f;
                }
                continue;
            }
            
            if (registry.any_of<TweenComponent>(entity)) {
                auto& tween = registry.get<TweenComponent>(entity);
                if (tween.positionTween.has_value() || tween.scaleTween.has_value() || tween.colorTween.has_value()) {
                    collider.isHovered = false;
                    continue; 
                }
            }

            float scaledWidth = collider.width * transform.scale.x;
            float scaledHeight = collider.height * transform.scale.y;

            float dx = virtualMouse.x - transform.position.x;
            float dy = virtualMouse.y - transform.position.y;
            
            float rad = -transform.rotation * DEG2RAD; 
            float cosR = cosf(rad);
            float sinR = sinf(rad);
            
            float localMouseX = dx * cosR - dy * sinR;
            float localMouseY = dx * sinR + dy * cosR;
            
            float halfWidth = scaledWidth / 2.0f;
            float halfHeight = scaledHeight / 2.0f;

            if (localMouseX >= -halfWidth && localMouseX <= halfWidth && 
                localMouseY >= -halfHeight && localMouseY <= halfHeight) {
                collider.isHovered = true;
                transform.scale = { 2.2f, 2.2f }; 
                
                if (registry.any_of<HoverTiltComponent>(entity)) {
                    auto& tilt = registry.get<HoverTiltComponent>(entity);
                    
                    float relativeX = (virtualMouse.x - transform.position.x) / (scaledWidth / 2.0f);
                    float relativeY = (virtualMouse.y - transform.position.y) / (scaledHeight / 2.0f);
                    
                    relativeX = std::clamp(relativeX, -1.0f, 1.0f);
                    relativeY = std::clamp(relativeY, -1.0f, 1.0f);

                    float maxTilt = 0.35f;
                    
                    tilt.targetTiltY = -relativeX * maxTilt;  
                    tilt.targetTiltX = relativeY * maxTilt;   
                }
                
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if (registry.any_of<ActionCardComponent>(entity)) {
                        auto& actionCard = registry.get<ActionCardComponent>(entity);
                        TraceLog(LOG_INFO, "CLICKED ACTION CARD: %s", GameConfig::ACTION_DICT.at(actionCard.type).name.c_str());
                    }
                    else if (registry.any_of<CardComponent>(entity)) {
                        auto& numberCard = registry.get<CardComponent>(entity);
                        TraceLog(LOG_INFO, "CLICKED NUMBER CARD: %d", numberCard.rank);
                    }
                    else if (registry.any_of<ButtonComponent>(entity)) {
                        auto& button = registry.get<ButtonComponent>(entity);
                        TraceLog(LOG_INFO, "CLICKED BUTTON: %s", button.buttonId.c_str());
                        if (button.buttonId == "btn_play") {
                            nextScene = GameScene::GAMEPLAY;
                        }
                    }
                }
                
                // Lock the hover state
                if (currentlyHovered == entt::null) {
                    currentlyHovered = entity;
                    if (registry.any_of<RenderComponent>(entity)) {
                        auto& render = registry.get<RenderComponent>(entity);
                        render.zIndex = 100; 
                    }
                }
                
            } else {
                collider.isHovered = false;
                transform.scale = { 2.0f, 2.0f };
                
                if (registry.any_of<HoverTiltComponent>(entity)) {
                    auto& tilt = registry.get<HoverTiltComponent>(entity);
                    tilt.targetTiltX = 0.0f;
                    tilt.targetTiltY = 0.0f;
                }
            }
        }

        auto cursorFollowers = registry.view<TransformComponent, CursorFollowerComponent>();
        for (auto [entity, transform] : cursorFollowers.each()) {
            transform.position = virtualMouse;
        }
    }
};

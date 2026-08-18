#pragma once
#include <entt/entt.hpp>
#include "raylib.h"
#include "Components.hpp"
#include <algorithm>

class InputSystem {
public:
    static void Update(entt::registry& registry, int virtualWidth, int virtualHeight) {
        
        // Calculate the Virtual Mouse Position
        // Because we allow resizable window, GetMousePosition() returns the physical window pixels.
        // We need to map it back to the 1440x810 virtual canvas
        float scale = std::min((float)GetScreenWidth() / virtualWidth, (float)GetScreenHeight() / virtualHeight);
        
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (virtualWidth * scale)) * 0.5f) / scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (virtualHeight * scale)) * 0.5f) / scale;

        auto view = registry.view<TransformComponent, BoxColliderComponent>();
        
        for (auto [entity, transform, collider] : view.each()) {
            
            // Calculate the actual bounding box in the virtual world
            // Our drawing is centered, so the top-left corner is position - (width * scale / 2)
            float scaledWidth = collider.width * transform.scale.x;
            float scaledHeight = collider.height * transform.scale.y;
            
            Rectangle rect = {
                transform.position.x - (scaledWidth / 2.0f),
                transform.position.y - (scaledHeight / 2.0f),
                scaledWidth,
                scaledHeight
            };
            
            if (CheckCollisionPointRec(virtualMouse, rect)) {
                collider.isHovered = true;
                transform.scale = { 2.2f, 2.2f }; 
            } else {
                collider.isHovered = false;
                transform.scale = { 2.0f, 2.0f };
            }
        }

        auto cursorFollowers = registry.view<TransformComponent, CursorFollowerComponent>();
        for (auto [entity, transform] : cursorFollowers.each()) {
            transform.position = virtualMouse;
        }
    }
};

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
            }
            currentlyHovered = entt::null;
        }

        for (auto [entity, transform, collider] : view.each()) {
            if (!collider.active) continue;

            if (currentlyHovered != entt::null && entity != currentlyHovered) {
                collider.isHovered = false;
                transform.scale = transform.baseScale;
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
                transform.scale = { transform.baseScale.x * 1.1f, transform.baseScale.y * 1.1f }; 
                
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
                    if (registry.any_of<ButtonComponent>(entity)) {
                        auto& button = registry.get<ButtonComponent>(entity);
                        TraceLog(LOG_INFO, "CLICKED BUTTON: %s", button.buttonId.c_str());
                        if (button.buttonId == "btn_play") {
                            nextScene = GameScene::GAMEPLAY;
                        } else if (button.buttonId == "btn_play_hand" && game.currentPhase == GamePhase::DRAW_AND_SWITCH) {
                            game.currentPhase = GamePhase::COMBAT_CALCULATION;
                            if (game.isSwitchMode) {
                                game.isSwitchMode = false;
                                auto cards = registry.view<CardComponent, TransformComponent, TweenComponent>();
                                for (auto [cEntity, card, transform, tween] : cards.each()) {
                                    if (card.location == CardLocation::HAND) {
                                        tween.scaleTween = tweeny::from(transform.scale.x, transform.scale.y)
                                            .to(1.0f, 1.0f).during(200U).via(tweeny::easing::exponentialOut)
                                            .build();
                                    }
                                }
                            }
                        } else if (button.buttonId == "btn_switch_mode" && game.currentPhase == GamePhase::DRAW_AND_SWITCH) {
                            if (!game.isSwitchMode) {
                                game.isSwitchMode = true;
                                auto cards = registry.view<CardComponent, TransformComponent, TweenComponent>();
                                for (auto [cEntity, card, transform, tween] : cards.each()) {
                                    if (card.location == CardLocation::HAND) {
                                        tween.scaleTween = tweeny::from(transform.scale.x, transform.scale.y)
                                            .to(1.3f, 1.3f).during(200U).via(tweeny::easing::exponentialOut)
                                            .build();
                                    }
                                }
                            }
                        }
                    } else if (registry.any_of<ActionCardComponent>(entity)) {
                        if (game.currentPhase == GamePhase::ACTION_SELECTION) {
                            auto& actionCard = registry.get<ActionCardComponent>(entity);
                            game.selectedAction = actionCard.type;
                            game.currentPhase = GamePhase::DRAW_AND_SWITCH;
                            
                            // Tween selected card to middle-left, and push others downward
                            auto actionCards = registry.view<ActionCardComponent, TransformComponent, TweenComponent>();
                            for (auto [acEntity, acComp, transform, tween] : actionCards.each()) {
                                if (acEntity == entity) {
                                    // Move to middle left
                                    tween.positionTween = tweeny::from(transform.position.x, transform.position.y)
                                        .to(300.0f, 405.0f).during(800U).via(tweeny::easing::exponentialOut)
                                        .build();
                                    tween.rotationTween = tweeny::from(transform.rotation)
                                        .to(0.0f).during(800U).via(tweeny::easing::exponentialOut)
                                        .build();
                                    tween.scaleTween = tweeny::from(transform.scale.x, transform.scale.y)
                                        .to(2.0f, 2.0f).during(800U).via(tweeny::easing::exponentialOut)
                                        .build();
                                } else {
                                    // Move unselected cards downward to hide them mostly
                                    tween.positionTween = tweeny::from(transform.position.x, transform.position.y)
                                        .to(transform.position.x, 850.0f).during(800U).via(tweeny::easing::exponentialOut)
                                        .build();
                                    tween.scaleTween = tweeny::from(transform.scale.x, transform.scale.y)
                                        .to(2.0f, 2.0f).during(400U).via(tweeny::easing::exponentialOut)
                                        .build();
                                }
                            }
                        }
                    } else if (registry.any_of<CardComponent>(entity)) {
                        if (game.currentPhase == GamePhase::DRAW_AND_SWITCH && game.isSwitchMode) {
                            auto& numberCard = registry.get<CardComponent>(entity);
                            if (numberCard.location == CardLocation::HAND) {
                                bool wasOccupied = game.switchSlot.has_value();
                                int oldRank = wasOccupied ? game.switchSlot.value() : -1;
                                
                                // Move clicked card to switch slot
                                numberCard.location = CardLocation::SWITCH_SLOT;
                                game.switchSlot = numberCard.rank;
                                
                                auto& tween = registry.get<TweenComponent>(entity);
                                auto& transform = registry.get<TransformComponent>(entity);
                                Vector2 originalHandPos = transform.position;
                                
                                tween.positionTween = tweeny::from(transform.position.x, transform.position.y)
                                    .to(200.0f, 150.0f).during(400U).via(tweeny::easing::exponentialOut)
                                    .build();
                                tween.scaleTween = tweeny::from(transform.scale.x, transform.scale.y)
                                    .to(1.0f, 1.0f).during(400U).via(tweeny::easing::exponentialOut)
                                    .build();
                                
                                // Update logic state & Handle Swap
                                auto it = std::find(game.hand.begin(), game.hand.end(), numberCard.rank);
                                if (it != game.hand.end()) {
                                    if (wasOccupied) {
                                        // Swap logic
                                        *it = oldRank;
                                        
                                        // Find old switch card entity and swap it to hand
                                        auto allCards = registry.view<CardComponent, TransformComponent, TweenComponent>();
                                        for (auto [oldCEntity, oldCardComp, oldTransform, oldTween] : allCards.each()) {
                                            if (oldCEntity != entity && oldCardComp.location == CardLocation::SWITCH_SLOT) {
                                                oldCardComp.location = CardLocation::HAND;
                                                oldTween.positionTween = tweeny::from(oldTransform.position.x, oldTransform.position.y)
                                                    .to(originalHandPos.x, originalHandPos.y).during(400U).via(tweeny::easing::exponentialOut)
                                                    .build();
                                            }
                                        }
                                        game.replenishIndex = std::nullopt; // No draw needed
                                    } else {
                                        // Empty slot logic
                                        game.replenishIndex = std::distance(game.hand.begin(), it);
                                        game.hand.erase(it);
                                    }
                                }
                                
                                // Also scale down remaining cards
                                auto cards = registry.view<CardComponent, TransformComponent, TweenComponent>();
                                for (auto [cEntity, card, cTransform, cTween] : cards.each()) {
                                    if (card.location == CardLocation::HAND) {
                                        cTween.scaleTween = tweeny::from(cTransform.scale.x, cTransform.scale.y)
                                            .to(1.0f, 1.0f).during(200U).via(tweeny::easing::exponentialOut)
                                            .build();
                                    }
                                }

                                game.isSwitchMode = false;
                                game.currentPhase = GamePhase::COMBAT_CALCULATION;
                            }
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
                transform.scale = transform.baseScale;
                
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

        if (IsKeyPressed(KEY_ENTER)) {
            if (game.currentPhase == GamePhase::COMBAT_CALCULATION) {
                game.currentPhase = GamePhase::COMBAT_RESOLUTION;
            }
        }

        // Clean up UI Buttons when not in DRAW_AND_SWITCH
        if (game.currentPhase != GamePhase::DRAW_AND_SWITCH) {
            auto btnView = registry.view<ButtonComponent>();
            for (auto e : btnView) {
                auto& btn = registry.get<ButtonComponent>(e);
                if (btn.buttonId == "btn_play_hand" || btn.buttonId == "btn_switch_mode" || btn.buttonId == "switch_slot") {
                    if (registry.any_of<BoxColliderComponent>(e)) {
                        registry.get<BoxColliderComponent>(e).active = false;
                    }
                    if (registry.any_of<RenderComponent>(e)) {
                        registry.get<RenderComponent>(e).hidden = true;
                    }
                }
            }
        }

        // Update Hover Description
        auto hoverDescView = registry.view<TextComponent, RenderComponent, HoverDescTextComponent>();
        for (auto [descEntity, textComp, renderComp] : hoverDescView.each()) {
            if (currentlyHovered != entt::null && registry.any_of<ActionCardComponent>(currentlyHovered)) {
                auto& actionCard = registry.get<ActionCardComponent>(currentlyHovered);
                auto& data = GameConfig::ACTION_DICT.at(actionCard.type);
                textComp.text = data.name + "\nThreshold: " + std::to_string(data.threshold) + 
                                "\nDamage: " + std::to_string(data.damage) + 
                                "\nPenalty: " + std::to_string(data.missPenalty) +
                                "\nShield: " + std::to_string(data.shield) + 
                                "\nHeal: " + std::to_string(data.heal);
                renderComp.hidden = false;
            } else {
                renderComp.hidden = true;
            }
        }
    }
};

#pragma once
#include <entt/entt.hpp>
#include "raylib.h"
#include "Components.hpp"
#include <stdint.h>

class TweenSystem {
public:
    static void Update(entt::registry& registry) {
        float dt = GetFrameTime();
        
        // On the very first frame, Raylib's GetFrameTime() can be huge, 
        // because it counts the window initialization time. 
        // This would instantly skip the 1000ms tween
        if (dt > 0.033f) dt = 0.016f; 
        
        // Tweeny step takes uint32_t milliseconds by default for duration-based tweens
        uint32_t dtMs = (uint32_t)(dt * 1000.0f); 
        
        auto view = registry.view<TransformComponent, RenderComponent, TweenComponent>();
        
        for (auto [entity, transform, render, tweenCmp] : view.each()) {
            
            if (tweenCmp.positionTween.has_value()) {
                auto& tw = tweenCmp.positionTween.value();
                tw.step(dtMs); 
                
                auto vals = tw.peek(); 
                transform.position = { std::get<0>(vals), std::get<1>(vals) };

                if (tw.progress() >= 1.0f) {
                    tweenCmp.positionTween = std::nullopt;
                }
            }

            if (tweenCmp.scaleTween.has_value()) {
                auto& tw = tweenCmp.scaleTween.value();
                tw.step(dtMs);
                
                auto vals = tw.peek();
                transform.scale = { std::get<0>(vals), std::get<1>(vals) };
                
                if (tw.progress() >= 1.0f) {
                    tweenCmp.scaleTween = std::nullopt;
                }
            }

            if (tweenCmp.colorTween.has_value()) {
                auto& tw = tweenCmp.colorTween.value();
                tw.step(dtMs);
                
                auto vals = tw.peek();
                render.tint = { (unsigned char)std::get<0>(vals), (unsigned char)std::get<1>(vals), (unsigned char)std::get<2>(vals), (unsigned char)std::get<3>(vals) };
                
                if (tw.progress() >= 1.0f) {
                    tweenCmp.colorTween = std::nullopt;
                }
            }
        }
    }
};

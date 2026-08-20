#pragma once
#include "raylib.h"
#include <string>
#include <optional>
#include "tweeny/tweeny.h"
#include "Constants.hpp"

// ==========================================
// COMPONENTS
// ==========================================

struct TransformComponent {
    Vector2 position = {0.0f, 0.0f};
    Vector2 scale = {1.0f, 1.0f};
    Vector2 baseScale = {1.0f, 1.0f};
    float rotation = 0.0f;
    
    TransformComponent(Vector2 p = {0.0f, 0.0f}, Vector2 s = {1.0f, 1.0f}, float r = 0.0f)
        : position(p), scale(s), baseScale(s), rotation(r) {}
};

struct RenderComponent {
    Color tint = WHITE;
    bool hidden = false;
    int zIndex = 0;
    int baseZIndex = 0;
};

struct SpriteComponent {
    std::string texturePath;
    Texture2D texture = {0}; 
};

struct TextComponent {
    std::string text;
    std::string fontPath;
    Color color = BLACK;
    int fontSize = 20;
    Font font = {0}; 
};

struct TweenComponent {
    std::optional<tweeny::tween<float, float>> positionTween; 
    std::optional<tweeny::tween<float, float>> scaleTween;    
    std::optional<tweeny::tween<unsigned char, unsigned char, unsigned char, unsigned char>> colorTween; 
    std::optional<tweeny::tween<float>> rotationTween;
    std::optional<tweeny::tween<float>> flipTween;
};

struct CoinFlipComponent {
    float flipAngle = 0.0f;
};

// ==========================================
// TAGS / GAMEPLAY DATA
// ==========================================

struct ButtonComponent {
    std::string buttonId;
};

enum class CardLocation {
    DECK,
    HAND,
    SWITCH_SLOT,
    DISCARD
};

struct HoverTiltComponent {
    float currentTiltX = 0.0f;
    float currentTiltY = 0.0f;
    float targetTiltX = 0.0f;
    float targetTiltY = 0.0f;
    float baseRotation = 0.0f;
};

struct CardComponent {
    int rank = 0;
    CardLocation location = CardLocation::DECK;
};

struct ActionCardComponent {
    ActionType type;
};

struct EnemyComponent {
};

struct BoxColliderComponent {
    float width = 10.0f;
    float height = 10.0f;
    bool isHovered = false; 
    bool active = true;
};

struct CircleRenderComponent {
    float radius = 10.0f;
    Color color = RED;
};

struct RectRenderComponent {
    float width = 50.0f;
    float height = 200.0f;
    Color color = BLACK;
};

struct CursorFollowerComponent {
    
};

struct PhaseTextComponent {
};

struct SumTextComponent {
};

struct CombatTextComponent {
};

struct PlayerHpTextComponent {};
struct EnemyHpTextComponent {};
struct HoverDescTextComponent {};

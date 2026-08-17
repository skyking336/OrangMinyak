#pragma once
#include "raylib.h"
#include <string>

// ==========================================
// COMPONENTS
// Components are PURE DATA structs. No logic, no virtual functions.
// This completely replaces GameObject2D.hpp
// ==========================================

struct TransformComponent {
    Vector2 position = {0.0f, 0.0f};
    Vector2 scale = {1.0f, 1.0f};
    float rotation = 0.0f;
};

struct RenderComponent {
    Color tint = WHITE;
    bool hidden = false;
    int zIndex = 0;
};

struct SpriteComponent {
    std::string texturePath;
    Texture2D texture = {0}; // AssetManager will fill this in when the render system runs
};

struct TextComponent {
    std::string text;
    std::string fontPath;
    Color color = BLACK;
    int fontSize = 20;
    Font font = {0}; // AssetManager will fill this in
};

// ==========================================
// TAGS / GAMEPLAY DATA
// Instead of inheriting from GameObject2D, you just attach these structs
// ==========================================

struct CardComponent {
    int rank = 0;
};

struct EnemyComponent {
    // Enemy specific stats like health, damage, AI state
};



#pragma once
#include "raylib.h"
#include "AssetManager.hpp"

enum ObjectType{
    UNASSIGNED,
    UI,
    SceneObjects
};

class GameObject2D{
    public:
        ObjectType type = ObjectType::UNASSIGNED;
        Vector2 position{0.0f, 0.0f};
        Vector2 scale{1.0f, 1.0f};
        float rotation = 0.0f;
        Color tint = WHITE;
        bool hidden = false;
        int zIndex = 0;

        GameObject2D(Vector2 pos = {0.0f, 0.0f}) : position(pos){};
        virtual ~GameObject2D() = default;

        virtual void update(float dt){};
        virtual void draw(AssetManager& assets) const = 0;

};

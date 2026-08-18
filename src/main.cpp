#include "raylib.h"
#include <entt/entt.hpp>
#include <string>

#include "AssetManager.hpp"
#include "Components.hpp"
#include "RenderSystem.hpp"
#include "InputSystem.hpp"

int main() {
    InitWindow(1440, 810, "OilMan");
    SetTargetFPS(60);

    const int windowWidth = 1440;
    const int windowHeight = 810;  

    const int virtualWidth = 1440;
    const int virtualHeight = 810;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    ToggleBorderlessWindowed();

    RenderTexture2D target = LoadRenderTexture(virtualWidth, virtualHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); 

    AssetManager assets;
    
    entt::registry registry;

    // ==========================================
    // SCENE SETUP
    // ask the registry to create an ID, 
    // and then we 'emplace' (attach) data components to that ID
    // ==========================================

    // Background Entity
    auto bgEntity = registry.create();
    registry.emplace<TransformComponent>(bgEntity, Vector2{720.0f, 405.0f}, Vector2{0.5f, 0.5f});
    registry.emplace<RenderComponent>(bgEntity, WHITE, false, -1);
    registry.emplace<SpriteComponent>(bgEntity, "assets/Background.png");

    // Enemy Entity (Oilman)
    auto enemyEntity = registry.create();
    registry.emplace<TransformComponent>(enemyEntity, Vector2{720.0f, 405.0f});
    registry.emplace<RenderComponent>(enemyEntity, WHITE, false, 0);
    registry.emplace<SpriteComponent>(enemyEntity, "assets/Oilman.png"); 
    registry.emplace<EnemyComponent>(enemyEntity); 

    Texture2D cardTex = assets.getOrLoadTexture("assets/Card.png");

    // Card Entities
    int x = -500;
    for (int i = 0; i < 3; i++) {
        auto cardEntity = registry.create();
        registry.emplace<TransformComponent>(cardEntity, Vector2{720.0f + x, 405.0f + 300.0f}, Vector2{2.0f, 2.0f});
        registry.emplace<RenderComponent>(cardEntity, WHITE, false, 1);
        registry.emplace<CardComponent>(cardEntity, i);
        
        // Feed the exact pixel width and height of the original image
        registry.emplace<BoxColliderComponent>(cardEntity, (float)cardTex.width, (float)cardTex.height);
        
        registry.emplace<SpriteComponent>(cardEntity, "assets/Card.png");
        registry.emplace<TextComponent>(cardEntity, std::to_string(i), "assets/fonts/fibberish.ttf", BLACK, 45);
        
        x += 300;
    }

    // UI Text Entity
    auto uiEntity = registry.create();
    registry.emplace<TransformComponent>(uiEntity, Vector2{720.0f, 405.0f});
    registry.emplace<RenderComponent>(uiEntity, WHITE, false, 10);
    registry.emplace<TextComponent>(uiEntity, "Oil Man Prototype", "assets/fonts/fibberish.ttf", BLACK, 40);

    // A circle that follows the cursor 
    auto cursorEntity = registry.create();
    registry.emplace<TransformComponent>(cursorEntity, Vector2{0.0f, 0.0f});
    registry.emplace<RenderComponent>(cursorEntity, GREEN, false, 999); // Draw it on top of everything!
    registry.emplace<CursorFollowerComponent>(cursorEntity);
    registry.emplace<CircleRenderComponent>(cursorEntity, 15.0f, RED);

    // ==========================================
    // GAME LOOP
    // ==========================================
    while (!WindowShouldClose()) {
        
        // LOGIC PHASE
        // The InputSystem will map physical mouse to the virtual screen
        // and check if it intersects with any BoxColliderComponents
        InputSystem::Update(registry, virtualWidth, virtualHeight);


        // RENDER PHASE
        BeginTextureMode(target);
            ClearBackground(DARKGREEN);
            
            RenderSystem::Render(registry, assets);
        EndTextureMode();
        
        BeginDrawing();
            ClearBackground(BLACK); // Letterbox color
            
            float scale = std::min((float)GetScreenWidth()/virtualWidth, (float)GetScreenHeight()/virtualHeight);
            
            DrawTexturePro(target.texture, 
                { 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                { (GetScreenWidth() - ((float)virtualWidth * scale)) * 0.5f, 
                  (GetScreenHeight() - ((float)virtualHeight * scale)) * 0.5f,
                  (float)virtualWidth * scale, (float)virtualHeight * scale },
                { 0, 0 }, 0.0f, WHITE);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}

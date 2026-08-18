#include "raylib.h"
#include <entt/entt.hpp>
#include <string>
#include <iostream>

#include "AssetManager.hpp"
#include "Components.hpp"
#include "RenderSystem.hpp"
#include "InputSystem.hpp"
#include "GameState.hpp"

void SyncVisualCards(entt::registry& registry, AssetManager& assets, const GameState& game) {
    auto view = registry.view<CardComponent>();
    registry.destroy(view.begin(), view.end());

    auto actionView = registry.view<ActionCardComponent>();
    registry.destroy(actionView.begin(), actionView.end());

    Texture2D cardTex = assets.getOrLoadTexture("assets/Card.png");

    int startX = -300; 
    // for (size_t i = 0; i < game.hand.size(); i++) {
    //     auto cardEntity = registry.create();
    //     int cardRank = game.hand[i];
        
    //     registry.emplace<TransformComponent>(cardEntity, Vector2{720.0f + startX, 405.0f + 300.0f}, Vector2{2.0f, 2.0f});
    //     registry.emplace<RenderComponent>(cardEntity, WHITE, false, 1);
    //     registry.emplace<CardComponent>(cardEntity, cardRank, CardLocation::HAND);
    //     registry.emplace<BoxColliderComponent>(cardEntity, (float)cardTex.width, (float)cardTex.height);
    //     registry.emplace<SpriteComponent>(cardEntity, "assets/Card.png");
    //     registry.emplace<TextComponent>(cardEntity, std::to_string(cardRank), "assets/fonts/fibberish.ttf", BLACK, 45);
        
    //     startX += 300; 
    // }

    // if (game.switchSlot.has_value()) {
    //     auto slotEntity = registry.create();
    //     int cardRank = game.switchSlot.value();
        
    //     registry.emplace<TransformComponent>(slotEntity, Vector2{720.0f - 400.0f, 405.0f}, Vector2{2.0f, 2.0f});
    //     registry.emplace<RenderComponent>(slotEntity, WHITE, false, 1);
    //     registry.emplace<CardComponent>(slotEntity, cardRank, CardLocation::SWITCH_SLOT);
    //     registry.emplace<BoxColliderComponent>(slotEntity, (float)cardTex.width, (float)cardTex.height);
    //     registry.emplace<SpriteComponent>(slotEntity, "assets/Card.png");
    //     registry.emplace<TextComponent>(slotEntity, std::to_string(cardRank), "assets/fonts/fibberish.ttf", BLUE, 45);  
    // }

    // Spawn the Available Action Cards (Top of screen)
    int actionStartX = -200;
    for (size_t i = 0; i < game.actionHand.size(); ++i) {
        auto actionEntity = registry.create();
        ActionType actType = game.actionHand[i];
        
        std::string assetPath = GameConfig::ACTION_DICT.at(actType).assetPath;
        Texture2D actTex = assets.getOrLoadTexture(assetPath);

        // Position them higher up on the screen
        registry.emplace<TransformComponent>(actionEntity, Vector2{720.0f + actionStartX, 405.0f + 250.0f}, Vector2{2.0f, 2.0f});
        registry.emplace<RenderComponent>(actionEntity, WHITE, false, 1);
        registry.emplace<ActionCardComponent>(actionEntity, actType);
        registry.emplace<BoxColliderComponent>(actionEntity, (float)actTex.width, (float)actTex.height);
        registry.emplace<SpriteComponent>(actionEntity, assetPath);
        
        actionStartX += 300; // Space them out
    }
}

int main() {
    InitWindow(1440, 810, "OilMan");
    SetTargetFPS(60);

    const int windowWidth = 1440;
    const int windowHeight = 810;  
    entt::registry registry;
    AssetManager assets;
    
    // Initialize Game State
    GameState game;
    game.drawCards(3); 

    // --- SETUP SCENE ---
    
    // Virtual resolution target
    int virtualWidth = 1440;
    int virtualHeight = 810;
    RenderTexture2D target = LoadRenderTexture(virtualWidth, virtualHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    // Background Entity
    auto bgEntity = registry.create();
    registry.emplace<TransformComponent>(bgEntity, Vector2{720.0f, 405.0f});
    registry.emplace<RenderComponent>(bgEntity, WHITE, false, -1);
    registry.emplace<SpriteComponent>(bgEntity, "assets/Background.png");

    // Enemy Entity (Oilman)
    auto enemyEntity = registry.create();
    registry.emplace<TransformComponent>(enemyEntity, Vector2{720.0f, 405.0f});
    registry.emplace<RenderComponent>(enemyEntity, WHITE, false, 0);
    registry.emplace<SpriteComponent>(enemyEntity, "assets/Oilman.png"); 
    registry.emplace<EnemyComponent>(enemyEntity); 

    // Initial Sync
    SyncVisualCards(registry, assets, game);

    // UI Text Entity
    auto uiEntity = registry.create();
    registry.emplace<TransformComponent>(uiEntity, Vector2{720.0f, 405.0f});
    registry.emplace<RenderComponent>(uiEntity, WHITE, false, 10);
    registry.emplace<TextComponent>(uiEntity, "Oil Man Prototype", "assets/fonts/fibberish.ttf", WHITE, 40);

    // A circle that follows the cursor 
    auto cursorEntity = registry.create();
    registry.emplace<TransformComponent>(cursorEntity, Vector2{0.0f, 0.0f});
    registry.emplace<RenderComponent>(cursorEntity, GREEN, false, 999); 
    registry.emplace<CursorFollowerComponent>(cursorEntity);
    registry.emplace<CircleRenderComponent>(cursorEntity, 15.0f, RED);

    // ==========================================
    // GAME LOOP
    // ==========================================
    while (!WindowShouldClose()) {
        
        // LOGIC PHASE
        // The InputSystem will map physical mouse to the virtual screen
        // and check if it intersects with any BoxColliderComponents
        InputSystem::Update(registry, virtualWidth, virtualHeight, game);


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

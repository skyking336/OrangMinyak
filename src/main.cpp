#include "raylib.h"
#include <entt/entt.hpp>
#include <string>
#include <iostream>

#include "AssetManager.hpp"
#include "Components.hpp"
#include "RenderSystem.hpp"
#include "InputSystem.hpp"
#include "TweenSystem.hpp"
#include "GameState.hpp"

void LoadMainMenu(entt::registry& registry, AssetManager& assets) {

    auto bgEntity = registry.create();
    registry.emplace<TransformComponent>(bgEntity, Vector2{720.0f, 405.0f});
    registry.emplace<RenderComponent>(bgEntity, WHITE, false, -1);
    registry.emplace<SpriteComponent>(bgEntity, "assets/Background.png");

    auto titleEntity = registry.create();
    registry.emplace<TransformComponent>(titleEntity, Vector2{720.0f, 300.0f});
    registry.emplace<RenderComponent>(titleEntity, WHITE, false, 10);
    registry.emplace<TextComponent>(titleEntity, "Oil Man", "assets/fonts/fibberish.ttf", WHITE, 120);

    auto playBtn = registry.create();
    registry.emplace<TransformComponent>(playBtn, Vector2{720.0f, 550.0f});
    registry.emplace<RenderComponent>(playBtn, WHITE, false, 10);
    registry.emplace<TextComponent>(playBtn, "Start Game", "assets/fonts/fibberish.ttf", GREEN, 60);
    registry.emplace<BoxColliderComponent>(playBtn, 300.0f, 80.0f); 
    registry.emplace<ButtonComponent>(playBtn, "btn_play");
}

void LoadGameScene(entt::registry& registry, AssetManager& assets, const GameState& game) {
    auto bgEntity = registry.create();
    registry.emplace<TransformComponent>(bgEntity, Vector2{720.0f, 405.0f});
    registry.emplace<RenderComponent>(bgEntity, WHITE, false, -1);
    registry.emplace<SpriteComponent>(bgEntity, "assets/Background.png");

    auto enemyEntity = registry.create();
    registry.emplace<TransformComponent>(enemyEntity, Vector2{720.0f, 405.0f});
    registry.emplace<RenderComponent>(enemyEntity, WHITE, false, 0);
    registry.emplace<SpriteComponent>(enemyEntity, "assets/Oilman.png"); 
    registry.emplace<EnemyComponent>(enemyEntity); 

    auto uiEntity = registry.create();
    registry.emplace<TransformComponent>(uiEntity, Vector2{720.0f, 50.0f});
    registry.emplace<RenderComponent>(uiEntity, WHITE, false, 10);
    registry.emplace<TextComponent>(uiEntity, "Gameplay Scene", "assets/fonts/fibberish.ttf", WHITE, 40);

    float spacing = 200.0f;
    float totalWidth = (game.actionHand.size() > 0) ? (game.actionHand.size() - 1) * spacing : 0;
    float startTargetX = 720.0f - (totalWidth / 2.0f);

    for (size_t i = 0; i < game.actionHand.size(); ++i) {
        auto actionEntity = registry.create();
        ActionType actType = game.actionHand[i];
        
        std::string assetPath = GameConfig::ACTION_DICT.at(actType).assetPath;
        Texture2D actTex = assets.getOrLoadTexture(assetPath);

        float targetY = 605.0f;
        float deckX = 1200.0f;
        float deckY = 605.0f; 
        float targetX = startTargetX + (i * spacing);
        
        registry.emplace<TransformComponent>(actionEntity, Vector2{deckX, deckY}, Vector2{2.0f, 2.0f});
        registry.emplace<RenderComponent>(actionEntity, WHITE, false, 1);
        registry.emplace<ActionCardComponent>(actionEntity, actType);
        registry.emplace<BoxColliderComponent>(actionEntity, (float)actTex.width, (float)actTex.height);
        registry.emplace<SpriteComponent>(actionEntity, assetPath);
        
        // Add a Position Tween with delay (dealing from right to left)
        TweenComponent tc;
        uint32_t delay = (game.actionHand.size() - 1 - i) * 150U; 
        
        if (delay > 0) {
            tc.positionTween = tweeny::from(deckX, deckY)
                               .to(deckX, deckY).during(delay)
                               .to(targetX, targetY).during(1000U).via(tweeny::easing::exponentialOut)
                               .build();
        } else {
            tc.positionTween = tweeny::from(deckX, deckY)
                               .to(targetX, targetY).during(1000U).via(tweeny::easing::exponentialOut)
                               .build();
        }
        
        registry.emplace<TweenComponent>(actionEntity, tc);
    }
}

int main() {

    const int windowWidth = 1440;
    const int windowHeight = 810;  
    InitWindow(windowWidth, windowHeight, "OilMan");
    SetTargetFPS(60);
    // ToggleFullscreen();


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

    // Initial Scene State
    GameScene currentScene = GameScene::MAIN_MENU;
    GameScene nextScene = GameScene::MAIN_MENU;

    LoadMainMenu(registry, assets);

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
        
        if (currentScene != nextScene) {
            registry.clear(); 

            // Recreate global entities
            cursorEntity = registry.create();
            registry.emplace<TransformComponent>(cursorEntity, Vector2{0.0f, 0.0f});
            registry.emplace<RenderComponent>(cursorEntity, GREEN, false, 999); 
            registry.emplace<CursorFollowerComponent>(cursorEntity);
            registry.emplace<CircleRenderComponent>(cursorEntity, 15.0f, RED);

            if (nextScene == GameScene::MAIN_MENU) {
                LoadMainMenu(registry, assets);
            } else if (nextScene == GameScene::GAMEPLAY) {
                LoadGameScene(registry, assets, game);
            }
            currentScene = nextScene;
        }

        // LOGIC PHASE
        InputSystem::Update(registry, virtualWidth, virtualHeight, game, nextScene);


        // RENDER PHASE
        TweenSystem::Update(registry);

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

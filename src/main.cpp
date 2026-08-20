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

void DealNumberCards(entt::registry& registry, AssetManager& assets, GameState& game) {
    float spacing = 200.0f;
    float startTargetX = 720.0f - spacing; 

    for (size_t i = 0; i < game.hand.size(); ++i) {
        auto coinEntity = registry.create();
        int rank = game.hand[i];
        
        std::string assetPath = GameConfig::GetCoinAssetPath(rank);
        Texture2D coinTex = assets.getOrLoadTexture(assetPath);

        int rand_distance = 40;
        float startY = 900.0f; 
        float targetY = 550.0f + (float)GetRandomValue(-rand_distance, rand_distance);
        float deckX = 720.0f; 
        float targetX = startTargetX + (i * spacing) + (float)GetRandomValue(-rand_distance, rand_distance);
        
        registry.emplace<TransformComponent>(coinEntity, Vector2{deckX, startY}, Vector2{0.0f, 0.0f});
        registry.emplace<RenderComponent>(coinEntity, WHITE, false, 50 + (int)i, 50 + (int)i);
        
        CardComponent cardComp;
        cardComp.rank = rank;
        cardComp.location = CardLocation::HAND;
        registry.emplace<CardComponent>(coinEntity, cardComp);
        
        registry.emplace<BoxColliderComponent>(coinEntity, (float)coinTex.width, (float)coinTex.height);
        registry.emplace<SpriteComponent>(coinEntity, assetPath);
        
        registry.emplace<CoinFlipComponent>(coinEntity, 0.0f);
        
        TweenComponent tc;
        uint32_t delay = i * 200U; 
        uint32_t duration = 1500U;
        
        if (delay > 0) {
            tc.positionTween = tweeny::from(deckX, startY)
                               .to(deckX, startY).during(delay)
                               .to(targetX, targetY - 150.0f).during(duration / 2).via(tweeny::easing::circularOut)
                               .to(targetX, targetY).during(duration / 2).via(tweeny::easing::bounceOut)
                               .build();
                               
            tc.scaleTween = tweeny::from(0.0f, 0.0f)
                            .to(4.f, 4.f).during(delay)
                            .to(1.0f, 1.0f).during(duration).via(tweeny::easing::bounceOut)
                            .build();
                            
            tc.flipTween = tweeny::from(0.0f)
                           .to(0.0f).during(delay)
                           .to(PI * 16.0f).during(duration).via(tweeny::easing::bounceOut)
                           .build();
        } else {
            tc.positionTween = tweeny::from(deckX, startY)
                               .to(targetX, targetY - 150.0f).during(duration / 2).via(tweeny::easing::circularOut)
                               .to(targetX, targetY).during(duration / 2).via(tweeny::easing::bounceOut)
                               .build();
                               
            tc.scaleTween = tweeny::from(4.f, 4.f)
                            .to(1.0f, 1.0f).during(duration).via(tweeny::easing::bounceOut)
                            .build();
                            
            tc.flipTween = tweeny::from(0.0f)
                           .to(PI * 16.0f).during(duration).via(tweeny::easing::bounceOut)
                           .build();
        }
                       
        registry.emplace<TweenComponent>(coinEntity, tc);
    }
}

void LoadGameScene(entt::registry& registry, AssetManager& assets, const GameState& game) {
    auto bgEntity = registry.create();
    registry.emplace<TransformComponent>(bgEntity, Vector2{720.0f, 405.0f});
    registry.emplace<RenderComponent>(bgEntity, WHITE, false, -1);
    registry.emplace<SpriteComponent>(bgEntity, "assets/Background.png");

    auto enemyEntity = registry.create();
    registry.emplace<TransformComponent>(enemyEntity, Vector2{720.0f, 405.0f});
    registry.emplace<RenderComponent>(enemyEntity, WHITE, false, 0);
    registry.emplace<SpriteComponent>(enemyEntity, "assets/OilManDraw.png"); 
    registry.emplace<EnemyComponent>(enemyEntity); 

    auto uiEntity = registry.create();
    registry.emplace<TransformComponent>(uiEntity, Vector2{720.0f, 50.0f});
    registry.emplace<RenderComponent>(uiEntity, WHITE, false, 10);
    registry.emplace<TextComponent>(uiEntity, "Select an Action!", "assets/fonts/fibberish.ttf", WHITE, 40);
    registry.emplace<PhaseTextComponent>(uiEntity);
    
    auto pHealthEntity = registry.create();
    registry.emplace<TransformComponent>(pHealthEntity, Vector2{150.0f, 50.0f});
    registry.emplace<RenderComponent>(pHealthEntity, WHITE, false, 10);
    registry.emplace<TextComponent>(pHealthEntity, "Player HP: 10", "assets/fonts/fibberish.ttf", GREEN, 40);
    registry.emplace<PlayerHpTextComponent>(pHealthEntity);

    auto eHealthEntity = registry.create();
    registry.emplace<TransformComponent>(eHealthEntity, Vector2{1290.0f, 50.0f});
    registry.emplace<RenderComponent>(eHealthEntity, WHITE, false, 10);
    registry.emplace<TextComponent>(eHealthEntity, "Enemy HP: 20", "assets/fonts/fibberish.ttf", RED, 40);
    registry.emplace<EnemyHpTextComponent>(eHealthEntity);

    auto hoverDescEntity = registry.create();
    registry.emplace<TransformComponent>(hoverDescEntity, Vector2{150.0f, 550.0f}); // Bottom center
    registry.emplace<RenderComponent>(hoverDescEntity, WHITE, true, 120); // Hidden by default
    registry.emplace<RectRenderComponent>(hoverDescEntity, 275.0f, 300.0f, Color{255, 255, 255, 230}); // White translucent box
    registry.emplace<TextComponent>(hoverDescEntity, "", "assets/fonts/fibberish.ttf", BLACK, 30);
    registry.emplace<HoverDescTextComponent>(hoverDescEntity);

    float spacing = 120.0f;
    float totalWidth = (game.actionHand.size() > 0) ? (game.actionHand.size() - 1) * spacing : 0;
    float startTargetX = 720.0f - (totalWidth / 2.0f);
    
    // --------------------------------------------------------------------------
    // Putting Cars in hand

    // Calculate arc parameters
    float baseMaxAngle = 25.0f; 
    float maxAngle = baseMaxAngle - (game.actionHand.size() * 1.5f);
    if (maxAngle < 5.0f) maxAngle = 5.0f;

    for (size_t i = 0; i < game.actionHand.size(); ++i) {
        auto actionEntity = registry.create();
        ActionType actType = game.actionHand[i];
        
        std::string assetPath = GameConfig::ACTION_DICT.at(actType).assetPath;
        Texture2D actTex = assets.getOrLoadTexture(assetPath);

        float normalizedIdx = 0.0f;
        float targetRotation = 0.0f;
        float yOffset = 0.0f;
        
        if (game.actionHand.size() > 1) {
            normalizedIdx = (float)i / (float)(game.actionHand.size() - 1);
            float mappedIdx = (normalizedIdx * 2.0f) - 1.0f; // -1.0 to 1.0
            targetRotation = mappedIdx * maxAngle;
            
            float arcHeight = 40.0f;
            yOffset = (mappedIdx * mappedIdx) * arcHeight;
        }

        float targetY = 655.0f + yOffset;
        float deckX = 250.0f;
        float deckY = 750.0f; 
        float targetX = startTargetX + (i * spacing);
        
        registry.emplace<TransformComponent>(actionEntity, Vector2{deckX, deckY}, Vector2{0.0f, 0.0f});
        registry.emplace<RenderComponent>(actionEntity, WHITE, false, (int)i + 1, (int)i + 1);
        registry.emplace<ActionCardComponent>(actionEntity, actType);
        registry.emplace<BoxColliderComponent>(actionEntity, (float)actTex.width, (float)actTex.height);
        registry.emplace<SpriteComponent>(actionEntity, assetPath);

        HoverTiltComponent htc;
        htc.baseRotation = targetRotation;
        registry.emplace<HoverTiltComponent>(actionEntity, htc);
        
        // Add a Position Tween with delay (dealing from left to right)
        TweenComponent tc;
        uint32_t delay = (game.actionHand.size() - 1 - i) * 100U; 
        uint32_t duration = 400U;
        if (delay > 0) {
            tc.positionTween = tweeny::from(deckX, deckY)
                               .to(deckX, deckY).during(delay)
                               .to(targetX, targetY).during(duration).via(tweeny::easing::circularOut)
                               .build();
            tc.rotationTween = tweeny::from(0.0f)
                               .to(0.0f).during(delay)
                               .to(targetRotation).during(duration).via(tweeny::easing::exponentialOut)
                               .build();
            tc.scaleTween = tweeny::from(0.0f, 0.0f)
                            .to(0.0f, 0.0f).during(delay)
                            .to(2.0f, 2.0f).during(duration).via(tweeny::easing::backOut)
                            .build();
        } else {
            tc.positionTween = tweeny::from(deckX, deckY)
                               .to(targetX, targetY).during(duration).via(tweeny::easing::circularOut)
                               .build();
            tc.rotationTween = tweeny::from(0.0f)
                               .to(targetRotation).during(duration).via(tweeny::easing::exponentialOut)
                               .build();
            tc.scaleTween = tweeny::from(0.0f, 0.0f)
                            .to(2.0f, 2.0f).during(duration).via(tweeny::easing::backOut)
                            .build();
        }
        
        registry.emplace<TweenComponent>(actionEntity, tc);
    }
    //-------------------------------------------------------------------------------------

    auto slotEntity = registry.create();
    registry.emplace<TransformComponent>(slotEntity, Vector2{200.0f, 150.0f});
    registry.emplace<RenderComponent>(slotEntity, WHITE, false, 5); 
    registry.emplace<SpriteComponent>(slotEntity, "assets/SwitchSlot.png");
    
    auto playBtn = registry.create();
    registry.emplace<TransformComponent>(playBtn, Vector2{1240.0f, 650.0f}, Vector2{1.3f,1.3f});
    registry.emplace<RenderComponent>(playBtn, WHITE, true, 20); // HIDDEN BY DEFAULT
    registry.emplace<SpriteComponent>(playBtn, "assets/PlayButton.png");
    registry.emplace<ButtonComponent>(playBtn, "btn_play_hand");

    auto switchBtn = registry.create();
    registry.emplace<TransformComponent>(switchBtn, Vector2{1240.0f, 750.0f}, Vector2{1.3f,1.3f});
    registry.emplace<RenderComponent>(switchBtn, WHITE, true, 20); // HIDDEN BY DEFAULT
    registry.emplace<SpriteComponent>(switchBtn, "assets/SwitchButton.png");
    registry.emplace<ButtonComponent>(switchBtn, "btn_switch_mode");
}

int main() {

    const int windowWidth = 1920;
    const int windowHeight = 1080;  
    InitWindow(windowWidth, windowHeight, "OilMan");
    SetTargetFPS(60);
    ToggleFullscreen();

    InitAudioDevice();
    Music bgm = LoadMusicStream("assets/audio/Poisonous Bite - Pix (youtube).mp3");
    PlayMusicStream(bgm);

    entt::registry registry;
    AssetManager assets;
    
    
    GameState game;
    
    int virtualWidth = 1440;
    int virtualHeight = 810;
    RenderTexture2D target = LoadRenderTexture(virtualWidth, virtualHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

    GameScene currentScene = GameScene::MAIN_MENU;
    GameScene nextScene = GameScene::MAIN_MENU;

    LoadMainMenu(registry, assets);

    // ==========================================
    // GAME LOOP
    // ==========================================
    GamePhase previousPhase = game.currentPhase;

    while (!WindowShouldClose()) {
        
        UpdateMusicStream(bgm);
        
        float dt = GetFrameTime();

        // Cap dt to prevent massive jumps when window lags or initializes
        if (dt > 0.033f) dt = 0.016f;
        
        if (currentScene != nextScene) {
            registry.clear(); 

            if (nextScene == GameScene::MAIN_MENU) {
                LoadMainMenu(registry, assets);
            } else if (nextScene == GameScene::GAMEPLAY) {
                game = GameState(); // Reset game state for new playthrough
                LoadGameScene(registry, assets, game);
            }
            currentScene = nextScene;
        }

        // LOGIC PHASE
        previousPhase = game.currentPhase;
        InputSystem::Update(registry, virtualWidth, virtualHeight, game, nextScene);
        
        if (previousPhase != game.currentPhase && game.currentPhase == GamePhase::DRAW_AND_SWITCH) {
            game.drawCards(3); 
            
            // Check if we need to pull from switch slot (Deck ran out on Turn 3)
            if (game.currentTurn == 3 && game.hand.size() < 3 && game.switchSlot.has_value()) {
                game.hand.push_back(game.switchSlot.value());
                game.switchSlot = std::nullopt;
                
                // Safely destroy the old switch slot entity so it can be re-dealt
                auto cards = registry.view<CardComponent>();
                std::vector<entt::entity> toDestroy;
                for (auto e : cards) {
                    if (registry.get<CardComponent>(e).location == CardLocation::SWITCH_SLOT) {
                        toDestroy.push_back(e);
                    }
                }
                for (auto e : toDestroy) registry.destroy(e);
            }
            
            DealNumberCards(registry, assets, game);
            
            // Unhide UI Buttons
            auto btnView = registry.view<ButtonComponent>();
            for (auto e : btnView) {
                auto& btn = registry.get<ButtonComponent>(e);
                if (btn.buttonId == "btn_play_hand" || btn.buttonId == "btn_switch_mode") {
                    registry.get<RenderComponent>(e).hidden = false;
                    if (!registry.any_of<BoxColliderComponent>(e)) {
                        registry.emplace<BoxColliderComponent>(e, 200.0f, 80.0f);
                    }
                    registry.get<BoxColliderComponent>(e).active = true;
                } else if (btn.buttonId == "switch_slot") {
                    registry.get<RenderComponent>(e).hidden = false;
                }
            }
        }

        if (game.replenishIndex.has_value()) {
            int idx = game.replenishIndex.value();
            game.replenishIndex = std::nullopt;
            
            std::vector<int> drawn = game.drawCards(1);
            if (!drawn.empty()) {
                int rank = drawn[0];
                auto coinEntity = registry.create();
                
                std::string assetPath = GameConfig::GetCoinAssetPath(rank);
                Texture2D coinTex = assets.getOrLoadTexture(assetPath);

                int rand_distance = 40;
                float startY = 900.0f; 
                float targetY = 550.0f + (float)GetRandomValue(-rand_distance, rand_distance);
                
                float spacing = 120.0f;
                float totalWidth = (3 - 1) * spacing;
                float startTargetX = 720.0f - (totalWidth / 2.0f);
                float deckX = 720.0f; 
                float targetX = startTargetX + (idx * spacing) + (float)GetRandomValue(-rand_distance, rand_distance);
                
                registry.emplace<TransformComponent>(coinEntity, Vector2{deckX, startY}, Vector2{0.0f, 0.0f});
                registry.emplace<RenderComponent>(coinEntity, WHITE, false, 50 + idx, 50 + idx);
                
                CardComponent cardComp;
                cardComp.rank = rank;
                cardComp.location = CardLocation::HAND;
                registry.emplace<CardComponent>(coinEntity, cardComp);
                
                registry.emplace<BoxColliderComponent>(coinEntity, (float)coinTex.width, (float)coinTex.height);
                registry.emplace<SpriteComponent>(coinEntity, assetPath);
                
                registry.emplace<CoinFlipComponent>(coinEntity, 0.0f);
                
                TweenComponent tc;
                uint32_t duration = 1500U;
                game.initialResolveDelay = ((float)duration / 1000.0f) + 0.2f;
                
                tc.positionTween = tweeny::from(deckX, startY)
                                   .to(targetX, targetY - 150.0f).during(duration / 2).via(tweeny::easing::circularOut)
                                   .to(targetX, targetY).during(duration / 2).via(tweeny::easing::bounceOut)
                                   .build();
                                   
                tc.scaleTween = tweeny::from(4.f, 4.f)
                                .to(1.0f, 1.0f).during(duration).via(tweeny::easing::bounceOut)
                                .build();
                                
                tc.flipTween = tweeny::from(0.0f)
                               .to(PI * 16.0f).during(duration)
                               .build();
                               
                registry.emplace<TweenComponent>(coinEntity, tc);
            }
        }

        if (currentScene == GameScene::GAMEPLAY) {
            if (game.currentPhase == GamePhase::COMBAT_CALCULATION) {
                if (!game.resolveAnimationActive) {
                    game.resolveAnimationActive = true;
                    game.resolveIndex = 0;
                    game.resolveTimer = game.initialResolveDelay;
                    game.initialResolveDelay = 0.5f; // reset for next time
                    game.resolveSum = 0;
                    
                    // Spawn the Sum Text Entity
                    auto sumEntity = registry.create();
                    registry.emplace<TransformComponent>(sumEntity, Vector2{720.0f, 250.0f});
                    registry.emplace<RenderComponent>(sumEntity, WHITE, false, 30);
                    registry.emplace<TextComponent>(sumEntity, "Sum: 0", "assets/fonts/fibberish.ttf", WHITE, 80);
                    registry.emplace<SumTextComponent>(sumEntity);
                } else {
                    game.resolveTimer -= dt;
                    if (game.resolveTimer <= 0.0f && game.resolveIndex < game.hand.size()) {
                        int rank = game.hand[game.resolveIndex];
                        game.resolveSum += rank; 
                        
                        // Update Sum Text
                        auto sumView = registry.view<TextComponent, SumTextComponent>();
                        for (auto [e, txt] : sumView.each()) {
                            txt.text = "Sum: " + std::to_string(game.resolveSum);
                        }
                        
                        // Gather hand cards and sort by X position to animate sequentially from left to right
                        auto cards = registry.view<CardComponent, TransformComponent, TweenComponent>();
                        std::vector<entt::entity> handCards;
                        for (auto [e, cardComp, trans, tw] : cards.each()) {
                            if (cardComp.location == CardLocation::HAND) handCards.push_back(e);
                        }
                        std::sort(handCards.begin(), handCards.end(), [&registry](entt::entity a, entt::entity b) {
                            return registry.get<TransformComponent>(a).position.x < registry.get<TransformComponent>(b).position.x;
                        });
                        
                        if (game.resolveIndex < handCards.size()) {
                            auto e = handCards[game.resolveIndex];
                            auto& tw = registry.get<TweenComponent>(e);
                            auto& tr = registry.get<TransformComponent>(e);
                            
                            tw.scaleTween = tweeny::from(tr.scale.x, tr.scale.y)
                                            .to(1.5f, 1.5f).during(150U).via(tweeny::easing::exponentialOut)
                                            .to(1.0f, 1.0f).during(150U).via(tweeny::easing::exponentialOut)
                                            .build();
                        }
                        
                        game.resolveIndex++;
                        game.resolveTimer = 0.5f; // Wait half second before next card
                    }
                    
                    if (game.resolveIndex >= game.hand.size() && game.resolveTimer <= -1.0f) {
                        // After last card, wait a bit then transition
                        game.currentPhase = GamePhase::COMBAT_RESOLUTION;
                        game.phaseTimer = 0.0f;
                        game.resolveAnimationActive = false; // Reset for next turn
                    }
                }
            } else if (game.currentPhase == GamePhase::COMBAT_RESOLUTION) {
                if (game.phaseTimer == 0.0f && game.selectedAction.has_value()) {
                    auto actionData = GameConfig::ACTION_DICT.at(game.selectedAction.value());
                    int hitThreshold = actionData.threshold;
                    int missThreshold = hitThreshold - 3;
                    
                    int hitChance = 0;
                    int critChance = 0;
                    std::string statusText = "";
                    
                    bool isHit = false;
                    bool isCrit = false;
                    
                    if (game.resolveSum >= hitThreshold) {
                        hitChance = 100;
                        critChance = (game.resolveSum - hitThreshold) * 10;
                        if (critChance > 100) critChance = 100;
                        
                    } else if (game.resolveSum >= missThreshold) {
                        int diff = hitThreshold - game.resolveSum;
                        hitChance = 100 - (diff * 25);
                        
                    }
                    
                    if (hitChance == 100) {
                        isHit = true;
                        if (critChance > 0 && GetRandomValue(1, 100) <= critChance) {
                            isCrit = true;
                        }
                    } else if (hitChance > 0) {
                        if (GetRandomValue(1, 100) <= hitChance) {
                            isHit = true;
                        }
                    }
                    
                    int finalShield = 0;
                    if (isHit) {
                        int finalDamage = actionData.damage;
                        int finalHeal = actionData.heal;
                        finalShield = actionData.shield;
                        
                        if (isCrit) {
                            finalDamage *= 2;
                            finalHeal *= 2;
                            finalShield *= 2;
                            statusText += "\nCRITICAL!";
                        }
                        
                        if (finalDamage > 0) {
                            game.enemyHp -= finalDamage;
                            statusText += "\nDeal " + std::to_string(finalDamage) + " DMG!";
                        }
                        
                        if (finalHeal > 0) {
                            game.playerHp += finalHeal;
                            if (game.playerHp > 10) game.playerHp = 10; 
                            statusText += "\nHeal " + std::to_string(finalHeal) + " HP!";
                        }
                        
                        if (finalShield > 0) {
                            statusText += "\nGain " + std::to_string(finalShield) + " Shield!";
                        }
                    } else {
                        statusText += "\nMISS!";
                        if (actionData.missPenalty > 0) {
                            game.playerHp -= actionData.missPenalty;
                            statusText += " Take " + std::to_string(actionData.missPenalty) + " DMG!";
                        }
                    }
                    
                    if (game.enemyHp <= 0) {
                        statusText += "";
                        game.enemyHp = 0;
                    } else {
                        // Enemy Attack every turn
                        int enemyDamage = GameConfig::ENEMY_ATTACK; // Default enemy damage
                        if (finalShield > 0) {
                            enemyDamage -= finalShield;
                            if (enemyDamage < 0) enemyDamage = 0;
                        }
                        
                        game.playerHp -= enemyDamage;
                        statusText += "\n\n-" + std::to_string(enemyDamage) + " HP!";
                        
                        // Tween enemy size to show attack
                        auto enemyView = registry.view<EnemyComponent, TransformComponent>();
                        for (auto [e, trans] : enemyView.each()) {
                            if (!registry.any_of<TweenComponent>(e)) {
                                registry.emplace<TweenComponent>(e);
                            }
                            auto& tw = registry.get<TweenComponent>(e);
                            tw.scaleTween = tweeny::from(1.0f, 1.0f)
                                            .to(1.3f, 1.3f).during(200U).via(tweeny::easing::exponentialOut)
                                            .to(1.0f, 1.0f).during(400U).via(tweeny::easing::exponentialOut)
                                            .build();
                        }
                    }
                    
                    game.combatStatusText = statusText;
                    
                    auto combatTextEntity = registry.create();
                    registry.emplace<TransformComponent>(combatTextEntity, Vector2{720.0f, 350.0f});
                    registry.emplace<RenderComponent>(combatTextEntity, WHITE, false, 50);
                    registry.emplace<TextComponent>(combatTextEntity, statusText, "assets/fonts/fibberish.ttf", RED, 60);
                    registry.emplace<CombatTextComponent>(combatTextEntity);
                    
                    TweenComponent tc;
                    tc.positionTween = tweeny::from(720.0f, 350.0f)
                                       .to(720.0f, 150.0f).during(2500U).via(tweeny::easing::exponentialOut)
                                       .build();
                    tc.colorTween = tweeny::from((unsigned char)255, (unsigned char)255, (unsigned char)255, (unsigned char)255)
                                    .to((unsigned char)255, (unsigned char)255, (unsigned char)255, (unsigned char)0).during(2500U)
                                    .build();
                    registry.emplace<TweenComponent>(combatTextEntity, tc);
                    
                    // Also destroy the Sum text
                    auto sumView = registry.view<SumTextComponent>();
                    std::vector<entt::entity> sumsToDestroy;
                    for (auto e : sumView) sumsToDestroy.push_back(e);
                    for (auto e : sumsToDestroy) registry.destroy(e);
                }
                
                game.phaseTimer += dt;
                if (game.phaseTimer > 2.5f) {
                    if (game.enemyHp <= 0) {
                        game.currentPhase = GamePhase::VICTORY;
                        game.phaseTimer = 0.0f;
                        
                        // Start fading enemy away
                        auto enemyView = registry.view<EnemyComponent>();
                        for (auto e : enemyView) {
                            if (!registry.any_of<TweenComponent>(e)) {
                                registry.emplace<TweenComponent>(e);
                            }
                            auto& tw = registry.get<TweenComponent>(e);
                            tw.colorTween = tweeny::from((unsigned char)255, (unsigned char)255, (unsigned char)255, (unsigned char)255)
                                            .to((unsigned char)255, (unsigned char)255, (unsigned char)255, (unsigned char)0).during(1500U)
                                            .build();
                        }
                        
                        auto winTextEntity = registry.create();
                        registry.emplace<TransformComponent>(winTextEntity, Vector2{720.0f, 405.0f});
                        registry.emplace<RenderComponent>(winTextEntity, WHITE, false, 900);
                        registry.emplace<TextComponent>(winTextEntity, "You Won!", "assets/fonts/fibberish.ttf", GREEN, 120);
                        
                        // Fade in the You Won text
                        TweenComponent winTw;
                        winTw.colorTween = tweeny::from((unsigned char)0, (unsigned char)255, (unsigned char)0, (unsigned char)0)
                                           .to((unsigned char)0, (unsigned char)255, (unsigned char)0, (unsigned char)255).during(1500U)
                                           .build();
                        registry.emplace<TweenComponent>(winTextEntity, winTw);
                        
                    } else if (game.playerHp <= 0) {
                        game.currentPhase = GamePhase::DEFEAT;
                        game.phaseTimer = 0.0f;
                        
                        auto loseTextEntity = registry.create();
                        registry.emplace<TransformComponent>(loseTextEntity, Vector2{720.0f, 405.0f});
                        registry.emplace<RenderComponent>(loseTextEntity, WHITE, false, 900);
                        registry.emplace<TextComponent>(loseTextEntity, "You Lose...", "assets/fonts/fibberish.ttf", RED, 120);
                        
                        // Fade in the You Lose text
                        TweenComponent loseTw;
                        loseTw.colorTween = tweeny::from((unsigned char)255, (unsigned char)0, (unsigned char)0, (unsigned char)0)
                                           .to((unsigned char)255, (unsigned char)0, (unsigned char)0, (unsigned char)255).during(1500U)
                                           .build();
                        registry.emplace<TweenComponent>(loseTextEntity, loseTw);
                    } else {
                        game.currentPhase = GamePhase::TURN_CLEANUP;
                        game.phaseTimer = 0.0f;
                    }
                }
            } else if (game.currentPhase == GamePhase::VICTORY || game.currentPhase == GamePhase::DEFEAT) {
                game.phaseTimer += dt;
                if (game.phaseTimer > 5.0f) { // After 5 seconds
                    nextScene = GameScene::MAIN_MENU;
                }
            } else if (game.currentPhase == GamePhase::TURN_CLEANUP) {
                if (game.phaseTimer == 0.0f) {
                    // 1. Fade out used cards
                    auto cards = registry.view<CardComponent, TweenComponent>();
                    for (auto [e, card, tween] : cards.each()) {
                        if (card.location == CardLocation::HAND) {
                            tween.colorTween = tweeny::from((unsigned char)255, (unsigned char)255, (unsigned char)255, (unsigned char)255)
                                               .to((unsigned char)255, (unsigned char)255, (unsigned char)255, (unsigned char)0).during(500U)
                                               .build();
                        }
                    }
                    
                    auto actions = registry.view<ActionCardComponent, TweenComponent>();
                    for (auto [e, action, tween] : actions.each()) {
                        if (game.selectedAction.has_value() && action.type == game.selectedAction.value()) {
                            tween.colorTween = tweeny::from((unsigned char)255, (unsigned char)255, (unsigned char)255, (unsigned char)255)
                                               .to((unsigned char)255, (unsigned char)255, (unsigned char)255, (unsigned char)0).during(500U)
                                               .build();
                        }
                    }
                }
                
                game.phaseTimer += dt;
                if (game.phaseTimer > 0.6f) { // wait for fade
                    // Destroy faded entities
                    auto cards = registry.view<CardComponent>();
                    std::vector<entt::entity> cardsToDestroy;
                    for (auto e : cards) {
                        if (registry.get<CardComponent>(e).location == CardLocation::HAND) {
                            cardsToDestroy.push_back(e);
                        }
                    }
                    for (auto e : cardsToDestroy) registry.destroy(e);
                    
                    auto actions = registry.view<ActionCardComponent>();
                    std::vector<entt::entity> actionsToDestroy;
                    for (auto e : actions) actionsToDestroy.push_back(e);
                    for (auto e : actionsToDestroy) registry.destroy(e);
                    
                    auto combatTexts = registry.view<CombatTextComponent>();
                    std::vector<entt::entity> textsToDestroy;
                    for (auto e : combatTexts) textsToDestroy.push_back(e);
                    for (auto e : textsToDestroy) registry.destroy(e);
                    
                    // Remove used action from actionHand array
                    if (game.selectedAction.has_value()) {
                        auto it = std::find(game.actionHand.begin(), game.actionHand.end(), game.selectedAction.value());
                        if (it != game.actionHand.end()) {
                            game.actionHand.erase(it);
                        }
                    }
                    
                    game.clearHandToDiscard();
                    game.selectedAction = std::nullopt;
                    game.advanceTurn();
                    
                    // Update Switch Slot visually after logic degrade
                    if (game.switchSlot.has_value()) {
                        int newRank = game.switchSlot.value();
                        auto cards = registry.view<CardComponent, SpriteComponent>();
                        for (auto [e, cardComp, spriteComp] : cards.each()) {
                            if (cardComp.location == CardLocation::SWITCH_SLOT) {
                                cardComp.rank = newRank;
                                spriteComp.texturePath = GameConfig::GetCoinAssetPath(newRank);
                                spriteComp.texture.id = 0; // Force RenderSystem to reload the new asset
                            }
                        }
                    }
                    
                    game.replenishActionHand(); // Fills back to 3
                    
                    float spacing = 120.0f;
                    float totalWidth = (game.actionHand.size() > 0) ? (game.actionHand.size() - 1) * spacing : 0;
                    float startTargetX = 720.0f - (totalWidth / 2.0f);
                    
                    float baseMaxAngle = 25.0f; 
                    float maxAngle = baseMaxAngle - (game.actionHand.size() * 1.5f);
                    if (maxAngle < 5.0f) maxAngle = 5.0f;

                    for (size_t i = 0; i < game.actionHand.size(); ++i) {
                        auto actionEntity = registry.create();
                        ActionType actType = game.actionHand[i];
                        
                        std::string assetPath = GameConfig::ACTION_DICT.at(actType).assetPath;
                        Texture2D actTex = assets.getOrLoadTexture(assetPath);

                        float normalizedIdx = 0.0f;
                        float targetRotation = 0.0f;
                        float yOffset = 0.0f;
                        
                        if (game.actionHand.size() > 1) {
                            normalizedIdx = (float)i / (float)(game.actionHand.size() - 1);
                            float mappedIdx = (normalizedIdx * 2.0f) - 1.0f;
                            targetRotation = mappedIdx * maxAngle;
                            float arcHeight = 40.0f;
                            yOffset = (mappedIdx * mappedIdx) * arcHeight;
                        }

                        float targetY = 655.0f + yOffset;
                        float deckX = -250.0f; // Fly in from Left
                        float deckY = 655.0f; 
                        float targetX = startTargetX + (i * spacing);
                        
                        registry.emplace<TransformComponent>(actionEntity, Vector2{deckX, deckY}, Vector2{0.0f, 0.0f});
                        registry.emplace<RenderComponent>(actionEntity, WHITE, false, (int)i + 1, (int)i + 1);
                        registry.emplace<ActionCardComponent>(actionEntity, actType);
                        registry.emplace<BoxColliderComponent>(actionEntity, (float)actTex.width, (float)actTex.height);
                        registry.emplace<SpriteComponent>(actionEntity, assetPath);

                        HoverTiltComponent htc;
                        htc.baseRotation = targetRotation;
                        registry.emplace<HoverTiltComponent>(actionEntity, htc);
                        
                        TweenComponent tc;
                        uint32_t delay = i * 100U; 
                        uint32_t duration = 400U;
                        
                        tc.positionTween = tweeny::from(deckX, deckY)
                                           .to(deckX, deckY).during(delay)
                                           .to(targetX, targetY).during(duration).via(tweeny::easing::circularOut)
                                           .build();
                        tc.rotationTween = tweeny::from(0.0f)
                                           .to(0.0f).during(delay)
                                           .to(targetRotation).during(duration).via(tweeny::easing::exponentialOut)
                                           .build();
                        tc.scaleTween = tweeny::from(0.0f, 0.0f)
                                        .to(0.0f, 0.0f).during(delay)
                                        .to(2.0f, 2.0f).during(duration).via(tweeny::easing::backOut)
                                        .build();
                        
                        registry.emplace<TweenComponent>(actionEntity, tc);
                    }
                    
                    game.currentPhase = GamePhase::ACTION_SELECTION;
                    game.phaseTimer = 0.0f;
                }
            }

            auto phaseTexts = registry.view<TextComponent, PhaseTextComponent>();
            for (auto [entity, text] : phaseTexts.each()) {
                switch (game.currentPhase) {
                    case GamePhase::ACTION_SELECTION:
                        text.text = "Select an Action!";
                        break;
                    case GamePhase::DRAW_AND_SWITCH:
                        text.text = "Swap/Play";
                        break;
                    case GamePhase::COMBAT_CALCULATION:
                        text.text = "";
                        break;
                    case GamePhase::COMBAT_RESOLUTION:
                        text.text = "";
                        break;
                    case GamePhase::TURN_CLEANUP:
                        text.text = "";
                        break;
                    case GamePhase::VICTORY:
                    case GamePhase::DEFEAT:
                        text.text = "";
                        break;
                }
            }

            auto playerHpTexts = registry.view<TextComponent, PlayerHpTextComponent>();
            for (auto [entity, text] : playerHpTexts.each()) {
                text.text = "Player HP: " + std::to_string(game.playerHp);
            }
            
            auto enemyHpTexts = registry.view<TextComponent, EnemyHpTextComponent>();
            for (auto [entity, text] : enemyHpTexts.each()) {
                text.text = "Enemy HP: " + std::to_string(game.enemyHp);
            }
        }


        // RENDER PHASE
        TweenSystem::Update(registry, dt);

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
                { 0.0f, 0.0f }, 0.0f, WHITE);
            
            // Draw Mouse Coordinates on the top right of the actual window
            Vector2 mouse = GetMousePosition();
            DrawText(TextFormat("X: %d Y: %d", (int)mouse.x, (int)mouse.y), GetScreenWidth() - 150, 20, 20, RAYWHITE);
        EndDrawing();
    }
    
    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

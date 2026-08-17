#include "raylib.h"
#include "tools.hpp"
// #include "string"
#include "Card.hpp"
#include "Player.hpp"
#include "iostream"
#include "AssetManager.hpp"
#include "Enemy.hpp"


int main()
{
    // 1. Initialize Window
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Single BlackJack");
    SetTargetFPS(60);

    //global variable
    int win = 0;
    int lose = 0;

    AssetManager assets;

    Deck gameDeck = Deck();
    gameDeck.initDeck();

    Player player = Player();
    player.addIntoHand(gameDeck.drawCard(4));

    Texture2D cardTexture = assets.getOrLoadTexture("assets/Card.png");
    Texture2D monster = assets.getOrLoadTexture("assets/OilMan.png");

    Enemy oilman = Enemy();
    oilman.position.x = 200;
    oilman.position.y = 100;
    oilman.hidden = false;

    int x = 100;
    for (Card& c : player.getHand()){
        std::cout << c.suit << " " << c.rank << "\n";
        c.position.x = x;
        c.position.y = 300;
        c.hidden = false;
        x += 100;
    }
    std::cout << "Total sum : " <<  std::to_string(player.getSum()) << "\n";

    // 2. Main Game Loop
    while (!WindowShouldClose())
    {
        // Update variables & logic here


        // Draw visuals
        BeginDrawing();
            ClearBackground(DARKGREEN); // Classic Blackjack table color!
            DrawTextCentered("Single BlackJack Prototype", 400, 225, 20, RAYWHITE);
            DrawText(TextFormat("Win: %d", win), 0, 0, 20, RAYWHITE);
            DrawText(TextFormat("Lose: %d", lose), 0, 30, 20, RAYWHITE);
            oilman.draw(assets);

            for (const auto& i : player.getHand()){
                i.draw(assets);
            }
        EndDrawing();
    }

    UnloadTexture(cardTexture);

    // 3. Clean up
    CloseWindow();
    return 0;
}

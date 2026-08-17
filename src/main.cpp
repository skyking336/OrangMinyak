#include "raylib.h"
#include "tools.hpp"
// #include "string"
#include "Card.hpp"
#include "Player.hpp"
#include "iostream"
#include "AssetManager.hpp"
#include "Enemy.hpp"
#include <vector>
#include <algorithm>


int main()
{
    // Initialize Window
    const int windowWidth = 1440;
    const int windowHeight = 810;
    
    // Virtual resolution
    const int virtualWidth = 1440;
    const int virtualHeight = 810;

    InitWindow(windowWidth, windowHeight, "OilMan");
    SetTargetFPS(60);
    
    RenderTexture2D target = LoadRenderTexture(virtualWidth, virtualHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    //global variable
    //---------------------
    int win = 0;
    int lose = 0;

    AssetManager assets;
    //---------------------


    //In game objects declaration and initialization
    Deck gameDeck = Deck();
    gameDeck.initDeck();

    Player player = Player();
    player.addIntoHand(gameDeck.drawCard(3));

    Texture2D cardTexture = assets.getOrLoadTexture("assets/Card.png");
    Texture2D monster = assets.getOrLoadTexture("assets/OilMan.png");

    Enemy oilman = Enemy();
    oilman.position.x = virtualWidth/2;
    oilman.position.y = virtualHeight/2;
    oilman.hidden = false;
    oilman.zIndex = 0; 

    int x = -100;
    for (Card& c : player.getHand()){
        std::cout << c.suit << " " << c.rank << "\n";
        c.position.x = (virtualWidth/2) + x;
        c.position.y = (virtualHeight/2) + 300;
        c.scale = {2.0f, 2.0f};
        c.hidden = false;
        c.zIndex = 1; 
        x += 100;
    }
    std::cout << "Total sum : " <<  std::to_string(player.getSum()) << "\n";

    // Main Game Loop
    while (!WindowShouldClose())
    {
        // Update variables & logic here
        //-----------------------

        //-----------------------

        //Inputs
        //-----------------------
        if (IsKeyPressed(KEY_F11))
        {
            int display = GetCurrentMonitor();
            if (IsWindowFullscreen())
            {
                ToggleFullscreen();
                SetWindowSize(windowWidth, windowHeight);
            }
            else
            {
                SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
                ToggleFullscreen();
            }
        }
        //-----------------------


        //Drawing to screen
        //-----------------------
        BeginTextureMode(target);
            ClearBackground(DARKGREEN);
            DrawTextCentered("Oil Man Prototype", virtualWidth/2, virtualHeight/2, 20, RAYWHITE);
            DrawText(TextFormat("Win: %d", win), 0, 0, 40, RAYWHITE);
            DrawText(TextFormat("Lose: %d", lose), 0, 30, 40, RAYWHITE);

            std::vector<GameObject2D*> renderQueue;
            if (!oilman.hidden) {
                renderQueue.push_back(&oilman);
            }
            for (Card& c : player.getHand()) {
                if (!c.hidden) {
                    renderQueue.push_back(&c);
                }
            }

            std::sort(renderQueue.begin(), renderQueue.end(), [](GameObject2D* a, GameObject2D* b) {
                return a->zIndex < b->zIndex;
            });

            // Draw all
            for (GameObject2D* obj : renderQueue) {
                obj->draw(assets);
            }

        EndTextureMode();
        
        BeginDrawing();
            ClearBackground(BLACK); // Letterbox color
            
            float scale = std::min((float)GetScreenWidth()/virtualWidth, (float)GetScreenHeight()/virtualHeight);
            
            // Draw render texture to screen, properly scaled
            DrawTexturePro(target.texture, 
                { 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                { (GetScreenWidth() - ((float)virtualWidth * scale)) * 0.5f, 
                  (GetScreenHeight() - ((float)virtualHeight * scale)) * 0.5f,
                  (float)virtualWidth * scale, (float)virtualHeight * scale },
                { 0, 0 }, 0.0f, WHITE);
        EndDrawing();
        //-----------------------
    }

    UnloadRenderTexture(target);
    UnloadTexture(cardTexture);

    // 3. Clean up
    CloseWindow();
    return 0;
}

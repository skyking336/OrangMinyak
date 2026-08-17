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
#include "SimpleTexture.hpp"
#include "UIObject.hpp"


int main()
{
    // Initialize Window
    const int windowWidth = 1440;
    const int windowHeight = 810;
    
    // Virtual resolution
    const int virtualWidth = 1440;
    const int virtualHeight = 810;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(windowWidth, windowHeight, "OilMan");
    SetTargetFPS(60);
    ToggleBorderlessWindowed();
    
    RenderTexture2D target = LoadRenderTexture(virtualWidth, virtualHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); 

    //global variable
    //---------------------
    int win = 0;
    int lose = 0;

    AssetManager assets;
    std::vector<GameObject2D*> scene_objects;
    //---------------------


    //In game objects declaration and initialization
    Deck gameDeck = Deck();
    gameDeck.initDeck();

    Player player = Player();
    player.addIntoHand(gameDeck.drawCard(3));

    Enemy oilman = Enemy();
    oilman.position.x = virtualWidth/2;
    oilman.position.y = virtualHeight/2;
    oilman.zIndex = 0; 
    scene_objects.push_back(&oilman);

    SimpleTexture background = SimpleTexture("assets/Background.png");
    background.position.x = virtualWidth/2;
    background.position.y = virtualHeight/2;
    background.zIndex = -1;
    background.scale = {0.5f, 0.5f};
    scene_objects.push_back(&background);

    int x = -500;
    for (Card& c : player.getHand()){
        std::cout << c.rank << "\n";
        c.position.x = (virtualWidth/2) + x;
        c.position.y = (virtualHeight/2) + 300;
        c.scale = {2.0f, 2.0f};
        c.zIndex = 1; 
        x += 300;
        scene_objects.push_back(&c);
    }
    std::cout << "Total sum : " <<  std::to_string(player.getSum()) << "\n";

    UIText gameName = UIText("Oil Man Prototype", "assets/fonts/fibberish.ttf", assets);
    gameName.position = {virtualWidth/2, virtualHeight/2};
    scene_objects.push_back(&gameName);

    UIText winAmountText = UIText("Win: ", "assets/fonts/fibberish.ttf", assets);
    winAmountText.position = {10, 10};
    scene_objects.push_back(&winAmountText);

    // Main Game Loop
    while (!WindowShouldClose())
    {
        // Update variables & logic here
        //-----------------------

        //-----------------------

        //Drawing to screen
        //-----------------------
        BeginTextureMode(target);
            ClearBackground(DARKGREEN);


            std::vector<GameObject2D*> renderQueue;
            std::vector<GameObject2D*> UIrenderQueue;

            for (const auto& o : scene_objects){
                if (o->type == ObjectType::UI){
                    UIrenderQueue.push_back(o);
                }
                else{
                    renderQueue.push_back(o);
                }
            }
            
            std::stable_sort(renderQueue.begin(), renderQueue.end(), [](GameObject2D* a, GameObject2D* b) {
                return a->zIndex < b->zIndex;
            });

            std::stable_sort(UIrenderQueue.begin(), UIrenderQueue.end(), [](GameObject2D* a, GameObject2D* b) {
                return a->zIndex < b->zIndex;
            });

            // Draw all
            for (GameObject2D* obj : renderQueue) {
                obj->draw(assets);
            }

            for (GameObject2D* obj : UIrenderQueue) {
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

    // 3. Clean up
    CloseWindow();
    return 0;
}

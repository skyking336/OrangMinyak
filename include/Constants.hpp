#pragma once
#include <string>
#include <unordered_map>

// ==========================================
// GLOBAL GAME RULES & CONSTANTS
// ==========================================

struct ActionData {
    std::string name;
    int threshold;
    int damage;
    int missPenalty;
    std::string assetPath;
};

enum class GameScene {
    MAIN_MENU,
    GAMEPLAY,
    GAME_OVER
};

enum class ActionType {
    RedFist,
    RedSlippers,
    RedHanger,
    BlueAddMath,
    BluePolis,
    BlueArmsUp,
    WhiteNasiLemak,
    WhiteHospital,
    WhitePraying,
    YellowSword
};

namespace GameConfig {

    // Combat & Balancing
    constexpr int CARD_DEGRADE_VAL = 2; 
    constexpr int DECK_SIZE = 9;        
    constexpr int PLAYER_START_HP = 10;
    constexpr int ENEMY_START_HP = 20;
    constexpr int MAXIMUM_HAND_SIZE = 5;

    // Window & Rendering Defaults
    constexpr int VIRTUAL_WIDTH = 1440;
    constexpr int VIRTUAL_HEIGHT = 810;

    // Structure: Name, Threshold, Damage, MissPenalty, AssetPath
    inline const std::unordered_map<ActionType, ActionData> ACTION_DICT = {
        {ActionType::RedFist, {"Punch", 11, 2, 0, "assets/RedFist.png"}},
        {ActionType::RedSlippers, {"Flying Slipper", 15, 4, 1, "assets/RedSlippers.png"}},
        {ActionType::RedHanger, {"The Hanger", 20, 7, 2, "assets/RedHanger.png"}},
        
        {ActionType::BlueAddMath, {"SPM Trauma", 18, 3, 2, "assets/BlueAddMath.png"}},
        {ActionType::BluePolis, {"Polis Tolong", 22, 8, 3, "assets/BluePolis.png"}},
        {ActionType::BlueArmsUp, {"Block", 12, 0, 1, "assets/BlueArmsUp.png"}},
        
        {ActionType::WhiteNasiLemak, {"Nasi Lemak", 14, 0, 0, "assets/WhiteNasiLemak.png"}},
        {ActionType::WhiteHospital, {"Klinik", 25, 0, 0, "assets/WhiteHospital.png"}},
        {ActionType::WhitePraying, {"Pray", 16, 0, 0, "assets/WhitePraying.png"}},
        
        {ActionType::YellowSword, {"Keris", 26, 12, 5, "assets/YellowSword.png"}}
    };
}

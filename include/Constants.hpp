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

    // Window & Rendering Defaults
    constexpr int VIRTUAL_WIDTH = 1440;
    constexpr int VIRTUAL_HEIGHT = 810;

    // Structure: Name, Threshold, Damage, MissPenalty
    inline const std::unordered_map<ActionType, ActionData> ACTION_DICT = {
        {ActionType::RedFist, {"Punch", 11, 2, 0}},
        {ActionType::RedSlippers, {"Flying Slipper", 15, 4, 1}},
        {ActionType::RedHanger, {"The Hanger", 20, 7, 2}},
        
        {ActionType::BlueAddMath, {"SPM Trauma", 18, 3, 2}},
        {ActionType::BluePolis, {"Polis Tolong", 22, 8, 3}},
        {ActionType::BlueArmsUp, {"Block", 12, 0, 1}},
        
        {ActionType::WhiteNasiLemak, {"Nasi Lemak", 14, 0, 0}},
        {ActionType::WhiteHospital, {"Klinik", 25, 0, 0}},
        {ActionType::WhitePraying, {"Pray", 16, 0, 0}},
        
        {ActionType::YellowSword, {"Keris", 26, 12, 5}}
    };
}

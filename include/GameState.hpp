#pragma once
#include <vector>
#include <optional>
#include <random>
#include <algorithm>
#include <string>
#include "Constants.hpp"

class GameState {
public:
    int playerHp = GameConfig::PLAYER_START_HP;
    int enemyHp = GameConfig::ENEMY_START_HP;
    int currentTurn = 1;
    GamePhase currentPhase = GamePhase::ACTION_SELECTION;
    float phaseTimer = 0.0f;

    std::string combatStatusText = "";

    std::vector<int> deck;
    std::vector<int> discardPile;
    std::vector<int> hand;
    std::optional<int> switchSlot;
    bool isSwitchMode = false;
    std::optional<int> replenishIndex = std::nullopt;

    bool resolveAnimationActive = false;
    int resolveIndex = 0;
    float resolveTimer = 0.0f;
    float initialResolveDelay = 0.5f;
    int resolveSum = 0;

    std::vector<ActionType> actionDeck = {
        ActionType::RedFist, ActionType::RedFist, ActionType::RedFist, 
        ActionType::RedHanger,
        ActionType::RedSlippers,
        ActionType::BlueArmsUp, ActionType::BlueArmsUp, ActionType::BlueArmsUp, 
        ActionType::BlueAddMath, 
        ActionType::BluePolis,
        ActionType::WhiteNasiLemak, ActionType::WhiteNasiLemak, ActionType::WhiteNasiLemak, 
        ActionType::WhiteHospital,
        ActionType::WhitePraying,
        ActionType::YellowSword
    };
    
    std::vector<ActionType> actionHand;
    std::optional<ActionType> selectedAction;

    GameState() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(actionDeck.begin(), actionDeck.end(), g);
        
        resetDeck();
        replenishActionHand(); 
    }

    void resetDeck() {
        deck.clear();
        for (int i = 1; i <= GameConfig::DECK_SIZE; ++i) {
            deck.push_back(i);
        }
        
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(deck.begin(), deck.end(), g);
        
        discardPile.clear();
        hand.clear();
        switchSlot = std::nullopt;
    }

    void replenishActionHand() {
        while (actionHand.size() < GameConfig::MAXIMUM_HAND_SIZE && !actionDeck.empty()) {
            actionHand.push_back(actionDeck.front());
            actionDeck.erase(actionDeck.begin());
        }
    }

    std::vector<int> drawCards(int count) {
        std::vector<int> drawn;
        for (int i = 0; i < count; ++i) {
            if (!deck.empty()) {
                int card = deck.front();
                drawn.push_back(card);
                hand.push_back(card);
                deck.erase(deck.begin());
            }
        }
        return drawn;
    }

    void clearHandToDiscard() {
        for (int card : hand) {
            discardPile.push_back(card);
        }
        hand.clear();
    }

    void advanceTurn() {
        if (switchSlot.has_value()) {
            switchSlot = switchSlot.value() - GameConfig::CARD_DEGRADE_VAL; 
        }
        
        if (currentTurn == 3) {
            resetDeck();
            currentTurn = 1;
        } else {
            currentTurn++;
        }
    }
};

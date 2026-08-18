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

    // Number Deck
    std::vector<int> deck;
    std::vector<int> discardPile;
    std::vector<int> hand;
    std::optional<int> switchSlot;

    // Action Deck
    std::vector<ActionType> actionDeck = {
        ActionType::RedFist, ActionType::RedFist, 
        ActionType::BlueArmsUp, ActionType::BlueArmsUp, 
        ActionType::WhiteNasiLemak
    };
    
    // The actions drawn for them to pick from this cycle
    std::vector<ActionType> availableActions;
    std::optional<ActionType> selectedAction;

    GameState() {
        resetDeck();
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

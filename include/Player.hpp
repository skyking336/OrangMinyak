#pragma once
#include "Card.hpp"

class Player{
    public:
        Player() = default;
        void addIntoHand(std::vector<Card> drawn);
        std::vector<Card>& getHand() {return m_hand;}
        int getSum() const{return m_handSum;}
        int calHandSum() const;

    private:
        std::vector<Card> m_hand;
        int m_handSum = 0;
};
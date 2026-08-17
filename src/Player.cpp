#include "Player.hpp"
#include <iostream>

void Player::addIntoHand(std::vector<Card> drawn){
    for (const Card& c : drawn){
        m_hand.push_back(c);
    }
    m_handSum = calHandSum();
    return;
}

int Player::calHandSum() const{
    int sum = 0;
    for (const Card& c : m_hand){
        sum += c.rank;
        std::cout << "add " << c.rank << "\n";
    }
    return sum;
}
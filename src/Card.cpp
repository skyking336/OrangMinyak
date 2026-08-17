#include <algorithm>
#include <random>
#include "tools.hpp"
#include "Card.hpp"

Deck::Deck(){
    initDeck();
    shuffleDeck();
}

void Deck::initDeck(){
    int index = 0;
    for (int r=0 ; r < 9; r++ )
    {
            m_allCards[index] = Card(r);
            index++;
    }
    m_cardCount = 9;

    shuffleDeck();
    return;
}

void Deck::shuffleDeck(){
    static std::random_device rd;
    static std::mt19937 g(rd()); 
    std::shuffle(m_allCards.begin(), m_allCards.begin() + m_cardCount, g);
    return;
}

std::vector<Card> Deck::drawCard(int amount){
    std::vector<Card> DrawnCard;
    DrawnCard.reserve(amount);
    for (int i = 0; i< amount; i++){
        DrawnCard.push_back(m_allCards.at(m_cardCount-1));
        m_cardCount --;
    }
    return DrawnCard;
}

void Card::draw(AssetManager& assets) const {
    if (hidden) return;
    Texture2D tex = assets.getOrLoadTexture(getTexturePath());    
    DrawTextureCentered(tex, position, scale, rotation, tint);
    std::string rankText = std::to_string(rank);
    DrawTextCentered(rankText.c_str(), position.x, position.y, 90, BLACK);
    return;
    
}
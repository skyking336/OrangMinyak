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
    for (int s=0 ; s < 4; s++ )
    {
        for (int r = 1; r < 14; r++){
            m_allCards[index] = Card(static_cast<Suit>(s), r);
            index++;
        }
    }
    m_cardCount = 52;

    shuffleDeck();
    return;
}

void Deck::shuffleDeck(){
    static std::random_device rd;
    static std::mt19937 g(rd()); //seed Mersenne Twister with the random seed, and this returns a random number
    std::shuffle(m_allCards.begin(), m_allCards.end(), g);
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

std::string Card::getSuit() const {
    switch (suit) {
        case Spade:   return "Spade";
        case Heart:   return "Heart";
        case Club:    return "Club";
        case Diamond: return "Diamond";
    }
    return "Unknown";
}

void Card::draw(AssetManager& assets) const {
    if (hidden) return;
    Texture2D tex = assets.getOrLoadTexture(getTexturePath());
    DrawTextureCentered(tex, position.x, position.y, WHITE);
    return;
    
}
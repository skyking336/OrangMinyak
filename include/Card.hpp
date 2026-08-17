#pragma once
#include <array>
#include <vector>
#include <string>
#include <GameObject2D.hpp>
#include <AssetManager.hpp>

enum Suit {
    Spade,
    Heart,
    Club,
    Diamond
};


class Card : public GameObject2D{
    public:
        Suit suit;
        int rank;
        Card() = default;
        Card(Suit p_suit, int p_rank, Vector2 pos = {0.0f,0.0f}) 
            :  GameObject2D(pos), suit(p_suit), rank(p_rank){};

        std::string getSuit() const;

        std::string TexturePath = "assets/Card.png";
        std::string getTexturePath() const {return TexturePath;}

        void draw(AssetManager& assets) const override;

};

class Deck {
    public:
        Deck();
        void shuffleDeck();
        void initDeck();
        std::vector<Card> drawCard(int amount);

    private:
        std::array<Card, 52> m_allCards;
        int m_cardCount = 0;
};

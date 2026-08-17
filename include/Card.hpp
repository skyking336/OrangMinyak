#pragma once
#include <array>
#include <vector>
#include <string>
#include <GameObject2D.hpp>
#include <AssetManager.hpp>

class Card : public GameObject2D{
    public:
        ObjectType type = ObjectType::UI;
        int rank = 0; // Initialize to 0 to prevent garbage memory values
        Card() = default;
        Card(int p_rank, Vector2 pos = {0.0f,0.0f}) 
            :  GameObject2D(pos), rank(p_rank){};

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
        std::array<Card, 9> m_allCards;
        int m_cardCount = 0;
};

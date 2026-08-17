#pragma once
#include <array>
#include <vector>
#include <string>
#include <GameObject2D.hpp>
#include <AssetManager.hpp>

class Enemy : public GameObject2D{
    public:
        std::string TexturePath = "assets/OilMan.png";
        std::string getTexturePath() const {return TexturePath;}

        void draw(AssetManager& assets) const override;

};
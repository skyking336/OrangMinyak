#include <array>
#include <vector>
#include <string>
#include <GameObject2D.hpp>
#include <AssetManager.hpp>

class UIObject : public GameObject2D{
    public:
        UIObject() = default;
        explicit UIObject(std::string path) : TexturePath(path){};
        std::string TexturePath;
        std::string getTexturePath() const {return TexturePath;}
        void draw(AssetManager& assets) const override;
};
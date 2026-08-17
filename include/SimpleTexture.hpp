#include <array>
#include <vector>
#include <string>
#include <GameObject2D.hpp>
#include <AssetManager.hpp>

class SimpleTexture : public GameObject2D{
    public:
        ObjectType type = ObjectType::SceneObjects;
        explicit SimpleTexture(std::string path) : TexturePath(path){};
        std::string TexturePath;
        std::string getTexturePath() const {return TexturePath;}
        void draw(AssetManager& assets) const override;
};
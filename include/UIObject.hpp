#include <array>
#include <vector>
#include <string>
#include <GameObject2D.hpp>
#include <AssetManager.hpp>

class UITexture : public GameObject2D{
    public:
        ObjectType type = ObjectType::UI;
        explicit UITexture(std::string path) : TexturePath(path){};
        std::string TexturePath;
        std::string getTexturePath() const {return TexturePath;}
        void draw(AssetManager& assets) const override;
};

class UIText : public GameObject2D{
    public:
        ObjectType type = ObjectType::UI;
        explicit UIText(std::string t) : Text(t){
            font = GetFontDefault();
        };
        explicit UIText(std::string t, std::string path, AssetManager& assets): Text(t), font(assets.getOrLoadFont(path)){};
        std::string Text;
        Font font = {0};
        int baseFontSize = 20; 
        std::string getTexturePath() const {return Text;}

        void draw(AssetManager& assets) const override;
};
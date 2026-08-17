#pragma once
#include <unordered_map>
#include <string>
#include "raylib.h"

class AssetManager {

public:
    Texture2D getOrLoadTexture(const std::string& filePath) {
        
        if (m_textures.find(filePath) == m_textures.end()) {
            m_textures[filePath] = LoadTexture(filePath.c_str());
        }
        return m_textures[filePath]; 
    }

    ~AssetManager() {
        for (auto& [path, tex] : m_textures) {
            UnloadTexture(tex); 
        }
    }

    private:
        std::unordered_map<std::string, Texture2D> m_textures;

};

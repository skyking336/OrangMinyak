#pragma once
#include <unordered_map>
#include <string>
#include "raylib.h"

class AssetManager {

public:
    Texture2D getOrLoadTexture(const std::string& filePath) {
        if (m_textures.find(filePath) == m_textures.end()) {
            m_textures[filePath] = LoadTexture(filePath.c_str());
            SetTextureFilter(m_textures[filePath], TEXTURE_FILTER_POINT);
        }
        return m_textures[filePath]; 
    }

    Font getOrLoadFont(const std::string& filePath) {
        if (m_fonts.find(filePath) == m_fonts.end()) {
            m_fonts[filePath] = LoadFont(filePath.c_str());
        }
        return m_fonts[filePath]; 
    }

    ~AssetManager() {
        for (auto& [path, tex] : m_textures) {
            UnloadTexture(tex); 
        }
        for (auto& [path, font] : m_fonts) {
            UnloadFont(font); 
        }
    }

    private:
        std::unordered_map<std::string, Texture2D> m_textures;
        std::unordered_map<std::string, Font> m_fonts;

};

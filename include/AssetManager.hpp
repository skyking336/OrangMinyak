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
            // TraceLog(LOG_DEBUG, "loaded from disk: %s",filePath.c_str());
        }
        return m_textures[filePath]; 
    }

    Font getOrLoadFont(const std::string& filePath) {
        if (m_fonts.find(filePath) == m_fonts.end()) {
            m_fonts[filePath] = LoadFont(filePath.c_str());
            SetTextureFilter(m_fonts[filePath].texture, TEXTURE_FILTER_POINT);
        }
        return m_fonts[filePath]; 
    }

    Shader getOrLoadShader(const std::string& vsPath, const std::string& fsPath) {
        std::string key = vsPath + "|" + fsPath;
        if (m_shaders.find(key) == m_shaders.end()) {
            const char* vs = vsPath.empty() ? nullptr : vsPath.c_str();
            const char* fs = fsPath.empty() ? nullptr : fsPath.c_str();
            m_shaders[key] = LoadShader(vs, fs);
        }
        return m_shaders[key];
    }

    ~AssetManager() {
        for (auto const& [key, val] : m_textures) {
            UnloadTexture(val);
        }
        for (auto const& [key, val] : m_fonts) {
            UnloadFont(val);
        }
        for (auto const& [key, val] : m_shaders) {
            UnloadShader(val);
        }
    }

    private:
        std::unordered_map<std::string, Texture2D> m_textures;
        std::unordered_map<std::string, Font> m_fonts;
        std::unordered_map<std::string, Shader> m_shaders;

};

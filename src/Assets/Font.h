#pragma once

#include <SDL2/SDL_log.h>
#include <glm/glm.hpp>
#include <types.h>
#include <GL/gl3w.h>

#include <string>
#include <filesystem>

namespace Assets {

    struct Font {
        std::string path;
        std::string name;

        struct Glyph {
            float scale;
            float advance;

            glm::vec2 bearing;
            glm::vec2 size;    

            glm::vec2 leftTop;
            glm::vec2 rightTop;
            glm::vec2 leftBottom;
            glm::vec2 rightBottom;
        };

        GLuint atlasId;

        float ascent;
        float descent;
        float lineGap;
        float lineHeight;

        Glyph glyphs[255];

        static Font* Load(const std::filesystem::path& path);
    };

}
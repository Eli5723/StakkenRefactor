#pragma once

#include <filesystem>

#include <glm/glm.hpp>
#include <GL/gl3w.h>
#include <string>

namespace Assets {

struct Texture {
    std::string name;
    std::filesystem::path path;

    GLuint id;
    glm::vec2 size;

    static Texture* Load(const std::filesystem::path& path);
    static Texture* Load(const std::filesystem::path& path, GLuint filterMode);
};

}
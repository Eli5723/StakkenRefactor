#pragma once

#include <filesystem>

#include <glm/glm.hpp>
#include <GL/gl3w.h>
#include <string>

namespace Assets {

// Customizable Background Shaders
struct BGShader {
    std::string name;
    GLuint id;

    static void Init();
    static BGShader* Load(const std::filesystem::path& path);

    void setVec2(const char* name, float* value);
    void setFloat(const char* name, GLfloat value);
    void draw(float* res, float time);

    void use();
};

}
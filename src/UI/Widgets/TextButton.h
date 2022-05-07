#pragma once

#include <UI/UI.h>

namespace UI
{

const glm::vec2 margin = { 8,8 };

struct TextButton : public Node {
    std::string text;
    float fontScale = .5f;

    TextButton(const char* text, const std::function<void(int, int)>& callback) : text(text) {
        size = Renderer::StrSize(0.5f, text, Assets::active_font) + margin * 2.0f;
        clickCallback = callback;
    }

    TextButton(const glm::vec2& size, const char* text, const std::function<void(int, int)>& callback) : text(text) {
        this->size = size;
        // Calculate base size of string
        glm::vec2 strSize = Renderer::StrSize(1.0f, text, Assets::active_font) + margin * 2.0f;
        
        float vBounds = size.y - margin.y * 2.0f;
        fontScale = (vBounds / strSize.y);


        clickCallback = callback;
    }

    void render() {
        Renderer::DrawQuad(position, size, { 0,0,0,1 });
        // Renderer::QuadBox(position, size, 1.0f, {1,1,1,1});
        Renderer::DrawStr(position + margin, fontScale, text, Assets::active_font);
    }
};

}
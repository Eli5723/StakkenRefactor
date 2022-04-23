
#pragma once

#include <UI/UI.h>

namespace UI
{

struct Button : public Node {
    Assets::Texture* texture = 0;

    Button(const std::string& path, int width, int height, const std::function<void(int, int)>& callback) {
        texture = Assets::textures.get(path.c_str());
        clickCallback = callback;
        size = { width,height };
    }

    Button(int width, int height, const std::function<void(int, int)>& callback) {
        clickCallback = callback;
        size = { width,height };
    }

    void render() {
        if (texture) {
            Renderer::DrawQuad(position, size, texture->id);
        }
        else
            Renderer::DrawQuad(position, size, { 1.0f, 1.0f,1.0f,1.0f });
    }
};

}
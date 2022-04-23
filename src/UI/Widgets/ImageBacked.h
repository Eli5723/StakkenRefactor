#pragma once

#include <UI/UI.h>

namespace UI
{

struct ImageBacked : public Node {
    Assets::Texture* tex = 0;

    ImageBacked(const char* path) {
        tex = Assets::textures.get(path);
        if (tex)
            size = tex->size;
    }

    void render() {
        if (tex) {
            Renderer::DrawQuad(position + glm::vec2{ 4.0f,4.0f }, size, tex->id, { 0,0,0,1 });
            Renderer::DrawQuad(position, size, tex->id);
        }
        else {
            Renderer::DrawQuad(position, size, { 255,0,220,1 });
        }
    }
};

}
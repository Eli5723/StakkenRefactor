#pragma once

#include <UI/UI.h>

namespace UI
{

struct Image : public Node {
    Assets::Texture* tex = 0;

    Image(const char* path) {
        tex = Assets::textures.get(path);
        if (tex)
            size = tex->size;
    }

    void render() {
        if (tex)
            Renderer::DrawQuad(position, size, tex->id);
        else
            Renderer::DrawQuad(position, size, { 255,0,220,1 });
    }
};

}
#pragma once

#include <UI/UI.h>

namespace UI
{

struct DraggableIcon : public Node {
    Assets::Texture* icon;
    DraggableIcon(const std::string& iconPath, const std::function<void(const SDL_MouseMotionEvent&)>& dragCallback) {
        this->icon = Assets::textures.get(iconPath.c_str());
        this->dragCallback = dragCallback;
        this->size = { 32,32 };
    }

    void render() {
        if (icon)
            Renderer::DrawQuad(position, size, icon->id);
    }
};

}
#pragma once

#include <UI/UI.h>

#include <string>

namespace UI
{
  
struct Checkbox : public Node {
    std::string label;
    int& ref;

    Assets::Texture* uncheckedTexture = Assets::textures.get("./resources/textures/checkbox.png");
    Assets::Texture* checkedTexture = Assets::textures.get("./resources/textures/checkedbox.png");

    Checkbox(const glm::vec2& size, int& ref, const std::string& label);
    void render();
};


}
#include <UI/Widgets/Checkbox.h>

namespace UI
{

Checkbox::Checkbox(const glm::vec2& size, int& ref, const std::string& label) : label(label), ref(ref) {
        this->size = size;
        clickCallback = [this](int, int) {this->ref = !this->ref;};
}

void Checkbox::render() {
        const glm::vec2 boxPosition = position + glm::vec2{ size.x - size.y,0 };

        Renderer::DrawQuad(position, size, { 0,0,0,1 });
        Renderer::DrawStr(position, 0.5f, label.c_str(), Assets::active_font);
        // Renderer::DrawQuad(boxPosition,{size.y, size.y},{1,1,1,1});

        Renderer::DrawQuad(boxPosition + glm::vec2{ 3,3 }, { size.y - 6, size.y - 6 }, ref ? checkedTexture->id : uncheckedTexture->id);
}

}
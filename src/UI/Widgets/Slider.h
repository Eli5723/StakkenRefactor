#pragma once

#include <UI/UI.h>

namespace UI
{

template<typename t>
struct Slider : public Node {
    t& _val;
    t _max;
    t _min;
    std::string label;

    Slider(const glm::vec2& size, t _min, t _max, t& ref, const std::string& label) : _val(ref), label(label) {
        this->size = size;
        this->_max = _max;
        this->_min = _min;

        this->dragCallback = [this](const SDL_MouseMotionEvent& event) {
            this->_val = std::clamp(this->_val + (event.xrel / this->size.x), this->_min, this->_max);
        };
    }

    void render() {
        // Draw background and "bar"
        Renderer::DrawQuad(position, size, { 0,0,0,1 });
        Renderer::DrawQuad(position + glm::vec2{ 0,size.y / 4 * 3 - 4 }, { size.x,8 }, { .5,.5,.5,1 });

        Renderer::DrawStr(position, .5f, label.c_str(), Assets::active_font);
        // Draw cursors
        float a = (float)(_val - _min);
        float b = (float)(_max - _min);
        float cursor = (a / b) * size.x;

        Renderer::DrawQuad(position + glm::vec2{ cursor - 6,size.y / 4 * 3 - 6 }, { 12,12 }, { 1,1,1,1 });
    }
};

}
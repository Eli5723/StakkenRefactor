#pragma once

#include <UI/UI.h>

namespace UI
{
   
struct Label : public Node {
    std::string text;
    float scale = .5f;
    Label(const std::string& text){
        this->text = text;
        this->size = Renderer::StrSize(scale,this->text, Assets::active_font);
    }

    void render(){
        Renderer::DrawStr(position,scale,text,  Assets::active_font);
    }

    void setText(const std::string& newText){
        this->text = newText;
        this->size = Renderer::StrSize(scale,this->text,  Assets::active_font);
    }
};

}
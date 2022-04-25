#include <UI/Widgets/HiddenInput.h>

namespace UI
{

HiddenInput::HiddenInput(std::function<void(char*)> callback) : callback(callback) {
    size = {200, Assets::active_font->lineHeight*.5f};
    cursor = 0;

    value = new char[SIZE];
    memset(value,0,SIZE);
    editing = false;

    this->clickCallback = [this](int,int){UI::SetFocus(this); SDL_StartTextInput(); editing = true;};
    this->textCallback = [this](const SDL_TextInputEvent& ev){

        char* tex = const_cast<char*>(&ev.text[0]);
        while (*tex != '\0'){
            if (cursor == SIZE-1){
                Sounds::play(Sounds::Slot::Reject);
                break;
            }

            switch(*tex){
                case 1:
                tex++;
                break;

                default:
                value[cursor] = *tex;
                tex++;
                cursor++;
                break;
            }
        }
    };

    this->keyCallback = [this](const SDL_KeyboardEvent& ev){
        switch (ev.keysym.sym){
            case '\b':
                if (cursor == 0)
                    break;

                value[cursor-1] = 0;
                cursor--;
            break;
            case '\n':
            case '\r':
                UI::ClearFocus();
                this->callback(this->value);
                editing = false;
            break;
        }

    };
}
HiddenInput::~HiddenInput(){
    delete [] value;
}

void HiddenInput::render(){
    Renderer::DrawQuad(position, size, {0,0,0,1});
    Renderer::DrawStrHide(position, .5f, value, Assets::active_font, {1,1,1,1});
    
    if (editing){
        const glm::vec2 strSize = Renderer::StrSizeHidden(.5f, value, Assets::active_font);
        Renderer::DrawQuad(position + glm::vec2{strSize.x,0.0f}, {2.0f,Assets::active_font->lineHeight},{1,1,1,1});
    } 
}

}
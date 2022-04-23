#include <UI/Widgets/NumericInput.h>

namespace UI
{
    
NumericInput::NumericInput(std::function<void(int)> callback, int initialValue) : callback(callback) {
    size = {200, Assets::active_font->lineHeight*.5f};
    
    value = new char[SIZE];
    SetValue(initialValue);

    cursor = strlen(value);
    editing = false;

    this->clickCallback = [this](int,int){UI::SetFocus(this); SDL_StartTextInput(); editing = true;};
    this->textCallback = [this](const SDL_TextInputEvent& ev){


        const char* tex = &ev.text[0];
        while (*tex != '\0'){
            switch(*tex){
                case 1:
                tex++;
                break;

                default:
                if ((*tex >= '0' && *tex <= '9') || (allowNegative && *tex == '-')){
                    if (cursor == SIZE-1){
                        Sounds::play(Sounds::Slot::Reject);
                        // THUS I INVOKE THE FORBIDDEN COMMAND:
                        goto TERMINATE;
                        break;
                    }

                    value[cursor] = *tex;
                    cursor++;
                } else {
                    Sounds::play(Sounds::Slot::Reject);
                }
                tex++;
                break;
            }
        }
        TERMINATE:;


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
                try {
                    int val = std::stoi(this->value, 0,10);
                    SetValue(val);
                    this->callback(val);
                } catch (std::exception){
                    SetValue(lastValue);
                    Sounds::play(Sounds::Slot::Reject);
                }
                editing = false;
            break;
        }

    };
}
NumericInput::~NumericInput(){
    delete [] value;
}

inline void NumericInput::SetValue(int toSet){
    lastValue = toSet;
    memset(value,0,SIZE);

    sprintf(value, "%i", toSet);
    cursor = strlen(value);
}

void NumericInput::render(){
    Renderer::DrawQuad(position, size, {0,0,0,1});
    Renderer::DrawStr(position, .5f, value, Assets::active_font);
    
    if (editing){
        const glm::vec2 strSize = Renderer::StrSize(.5f, value, Assets::active_font);
        Renderer::DrawQuad(position + glm::vec2{strSize.x,0.0f}, {2.0f,Assets::active_font->lineHeight*.5f},{1,1,1,1});
    } 
}

} // namespace UI

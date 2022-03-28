#include <UI/NodeTypes.h>

namespace UI {
// LobbyViewer::LobbyViewer(Lobby* l, Player* p) : lobby(l), localPlayer(p), viewers() {
//     lobby->viewer = this;
//     localPlayerViewer = new GameViewer(localPlayer->game, localPlayer->identity);
//     this->addChild(localPlayerViewer);

//     for (auto player : lobby->players){

//     }
// }
// LobbyViewer::LobbyViewer(Lobby* l, Player* p) : lobby(l), localPlayer(p), viewers() {
//     lobby->viewer = this;
//     localPlayerViewer = new GameViewer(localPlayer->game, localPlayer->identity);
//     this->addChild(localPlayerViewer);

//     for (auto player : lobby->players){

//     }
// }


Input::Input(std::function<void(char*)> callback) : callback(callback) {
    size = {200, Assets::active_font->lineHeight*.5f};
    offset = {600,200};

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
Input::~Input(){
    delete [] value;
}

void Input::render(){
    Renderer::DrawQuad(position, size, {0,0,0,1});
    Renderer::DrawStr(position, .5f, value, Assets::active_font);
    
    if (editing){
        const glm::vec2 strSize = Renderer::StrSize(.5f, value, Assets::active_font);
        Renderer::DrawQuad(position + glm::vec2{strSize.x,0.0f}, {2.0f,Assets::active_font->lineHeight*.05f},{1,1,1,1});
    } 
}

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
    sprintf(value,"%i", toSet);
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

}
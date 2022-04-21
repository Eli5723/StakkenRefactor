#include <States/MenuState.h>

#include <States/LoginState.h>
#include <States/SettingsState.h>
#include <States/ChallengeSelectState.h>
#include <UI/UI.h>

UI::Node* menu_root;
UI::Node* logo;

void MenuState::init(){
    UI::Node* button1 = new UI::TextButton("Online", [](int x, int y){
        Application::instance->state_set(new LoginState());
    });

    UI::Node* button2 = new UI::TextButton("Singleplayer", [](int x, int y){
        Application::instance->state_set(new ChallengeSelectState());
    });

    UI::Node* button3 = new UI::TextButton("Settings", [](int x, int y){
        Application::instance->state_set(new SettingsState());
    });

    UI::Node* button4 = new UI::TextButton("Quit", [this](int x, int y){
        Application::instance->running = false;
    });

    menu_root = new UI::Node();

    logo = new UI::ImageBacked("./resources/textures/logo.png");
    UI::AddToScreen(logo);
    logo->center();
    logo->position.y -= logo->size.y;

    menu_root->addChild(button1);
    menu_root->addChild(button2);
    menu_root->addChild(button3);
    menu_root->addChild(button4);
    menu_root->barLayout();

    UI::AddToScreen(menu_root);
    menu_root->center();

    UI::SetHover(button1);
}
void MenuState::update(int dt, int time){

}
void MenuState::render(int dt, int time){

}
void MenuState::close(){
    menu_root->destroy_recursive();
    logo->destroy();
    
    // Change the UI State
    UI::ClearState();
}
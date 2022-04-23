#include <States/LoginState.h>

#include <Application.h>
#include <UI/UI.h>

#include <UI/NodeTypes.h>

// User will enter credentials, then attempt to connect

bool playAsGuest = false;


UI::Node* menuRoot;

char* username = "";
char* password = "";

void InitiateLogin(){
    puts("Loggin' in");
    puts(username);
    puts(password);
}

void LoginState::init(){
    // TODO: Connection Handshake
    // TODO: Recieve Message From Server
    
    menuRoot = new UI::Node;
    
    // Username Input
    UI::Node* node = new UI::Input([](char* input){
        username = input;
        InitiateLogin();
    });
    menuRoot->addChild(node);

    // Password Input
    node = new UI::HiddenInput([](char* input){
        password = input;
        InitiateLogin();
    });
    menuRoot->addChild(node);

    // Login button
    node = new UI::Button(200,32,[](int,int){
        InitiateLogin();
    });
    menuRoot->addChild(node);
    menuRoot->listLayout();

    UI::AddToScreen(menuRoot);
}

void LoginState::update(int dt, int time){

}

void LoginState::render(int dt, int time){

}

void LoginState::close(){
    menuRoot->destroy_recursive();
}

bool LoginState::key_capture(const SDL_KeyboardEvent& key){
    
    return false;
}
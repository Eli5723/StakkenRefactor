#include <States/LoginState.h>

#include <Application.h>
#include <UI/UI.h>

#include <UI/NodeTypes.h>


#include <Networking/Client.h>

// User will enter credentials, then attempt to connect

bool playAsGuest = false;

UI::Node* menuRoot;

UI::Input* usernameInput;
UI::HiddenInput* passwordInput;

int guestLogin = 0;

void InitiateLogin(){
    puts("Loggin' in");
    puts(usernameInput->value);
    puts(passwordInput->value);

    Network::connect();

    if (guestLogin){
        std::string nickname(usernameInput->value);
        Network::send_guest_request(nickname);
    } else {
        std::string uname(usernameInput->value);
        std::string pword(passwordInput->value);

        Network::send_login_request(uname, pword);
    }
}

void LoginState::init(){
    // TODO: Connection Handshake
    // TODO: Recieve Message From Server
    
    menuRoot = new UI::Node;
    
    // Username Input
    usernameInput = new UI::Input([](char* input){

    });
    menuRoot->addChild(usernameInput);

    // Password Input
    passwordInput = new UI::HiddenInput([](char* input){

    });
    menuRoot->addChild(passwordInput);

    // Guest Checkcbox
    UI::Node* node = new UI::Checkbox(glm::vec2{200,32},guestLogin, "Login as Guest");
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
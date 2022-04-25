#include <States/RoomSelectState.h>

#include <Application.h>
#include <UI/UI.h>

#include <Networking/Client.h>

UI::Node* clientRoot;

void UpdateClientList(){
    std::scoped_lock m(Network::players_mut);
    if (!Network::players_dirty){
        return;
    }

    puts("Rebuilding dirty player list");
    
    clientRoot->destroy_children();
    
    UI::Label* name = new UI::Label("Players online:");
    clientRoot->addChild(name);

    for (auto& client : Network::players){
        UI::Label* name = new UI::Label(client.second.name);
        clientRoot->addChild(name);
    }

    clientRoot->listLayout();
    clientRoot->center();

    Network::players_dirty = false;
}


void RoomSelectState::init(){
    clientRoot = new UI::Container();
    UI::AddToScreen(clientRoot);
}

void RoomSelectState::update(int dt, int time){
    UpdateClientList();

}

void RoomSelectState::render(int dt, int time){

}

void RoomSelectState::close(){

}
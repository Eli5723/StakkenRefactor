#include <States/RoomSelectState.h>

#include <Application.h>
#include <UI/UI.h>

#include <Networking/Client.h>

UI::Node* clientRoot;
UI::Node* roomRoot;

UI::Node* roomCreateDialogue;

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
    clientRoot->right();

    Network::players_dirty = false;
}

void UpdateRoomList(){
    std::scoped_lock m(Network::rooms_mut);
    if (!Network::rooms_dirty){
        return;
    }

    puts("Rebuilding dirty room list");
    
    roomRoot->destroy_children();
    
    UI::Label* name = new UI::Label("Available rooms:");
    roomRoot->addChild(name);

    for (auto& room : Network::rooms){
        UI::Label* name = new UI::Label(room.second.name);
        roomRoot->addChild(name);
    }

    roomRoot->listLayout();

    Network::rooms_dirty = false;
}

void RoomSelectState::init(){
    UI::ClearState();
    clientRoot = new UI::Container();
    roomRoot = new UI::Container();
    
    UI::AddToScreen(roomRoot);
    UI::AddToScreen(clientRoot);

    // Room Creation Dialogue
    roomCreateDialogue = new UI::Node;

    UI::Input* roomNameInput = new UI::Input([](const char*){});
    roomCreateDialogue->addChild(roomNameInput);
    
    UI::Button* b = new UI::Button(5,32,[roomNameInput](int,int){Network::send_room_create(roomNameInput->value);});
    roomCreateDialogue->addChild(b);
    
    roomCreateDialogue->listLayout();
    UI::AddToScreen(roomCreateDialogue);
    roomCreateDialogue->center();

    // Set dirty flags manually to ensure refresh
    Network::rooms_dirty = true;
    Network::players_dirty = true;
}

void RoomSelectState::update(int dt, int time){
    UpdateClientList();
    UpdateRoomList();
}

void RoomSelectState::render(int dt, int time){

}

void RoomSelectState::close(){
    clientRoot->destroy_recursive();
    roomRoot->destroy_recursive();
    roomCreateDialogue->destroy_recursive();
}
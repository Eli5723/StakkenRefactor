#include <States/LobbyState.h>

#include <Application.h>
#include <UI/UI.h>

#include <LobbyContext.h>

#include <Game/Game.h>
#include <UI/UI.h>

// Lobby* testLobby;
// Player* localPlayer;


// const int TEST_PLAYER_COUNT = 15;
// Player* foreignPlayers[TEST_PLAYER_COUNT];

struct LobbyViewer : LobbyContext {
    void on_add(){

    }

    void on_remove(){

    }
};
 

void LobbyState::init(){
    // testLobby = new Lobby();

    // localPlayer = new Player{
    //     new Game,
    //     Identity::LoadRandom(),
    //     id++
    // };
    // testLobby->player_add(localPlayer);

    // for (int i=0; i < TEST_PLAYER_COUNT; i++){
    //     foreignPlayers[i] = new Player{
    //     new Game,
    //     Identity::LoadRandom(),
    //     id++
    //     };

    //     testLobby->player_add(foreignPlayers[i]);
    // }

    // int playercount = testLobby->players.size();
    // int columns = sqrt(playercount);
    // int rows = sqrt(playercount) + 1;

    // const glm::vec2 GAME_AREA_SIZE = {1280 - (RenderGame::GAME_DIMENSIONS.x + RenderGame::PADDING * 3), 720 - (RenderGame::PADDING*2)};
    // const glm::vec2 GAME_AREA_OFFSET = {(RenderGame::GAME_DIMENSIONS.x + RenderGame::PADDING * 2), RenderGame::PADDING};

    // // Add Local Player
    // int playerCount = testLobby->players.size();

    // auto a = new UI::GameViewer(localPlayer->game, localPlayer->identity);
    // a->position = glm::vec2{RenderGame::PADDING, UI::Resolution().y - RenderGame::PLAYER_DIMENSIONS.y - RenderGame::PADDING};
    // UI::AddToScreen(a);

    // // Add Other Players
    // int idx = 0;
    // glm::vec2 offset = {(GAME_AREA_SIZE.x + columns *   RenderGame::MARGIN) / (float)(columns), GAME_AREA_SIZE.y / (float)(rows)};
    // float scale = fmin(
    //     GAME_AREA_SIZE.x / ((float)columns * RenderGame::GAME_DIMENSIONS.x + columns * RenderGame::PADDING),
    //     GAME_AREA_SIZE.y / ((float)rows * (RenderGame::PLAYER_DIMENSIONS.y + RenderGame::MARGIN*2))
    // );
    
    // for (auto [id, player] : testLobby->players){
    //     if (player == localPlayer)
    //         continue;

    //     int column = idx % columns;
    //     int row = idx / columns;
    //     idx++;

    //     auto a = new UI::GameViewer(player->game, player->identity);
    //     a->position = GAME_AREA_OFFSET + offset * glm::vec2{(float)column, (float)row}; 
    //     UI::AddToScreen(a);
    //     a->scale = scale;
    // }
}



void LobbyState::update(int dt, int time){

}

void LobbyState::render(int dt, int time){

}

void LobbyState::close(){

}
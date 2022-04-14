#include <Game/LocalLobby.h>

void LocalLobby::player_add(Player* player){
    players[player->id] = player;

    if (context)
        context->on_add();

}

void LocalLobby::player_remove(Player* player){
    players.erase(player->id);

    if (context)
        context->on_remove();
}

void LocalLobby::send_lines(Game* source, int garbage){

    int playerCount = players.size();

    if (playerCount == 1)
        return;

    int garbagePerPlayer = garbage / (playerCount - 1);

    for (auto& [id, player] : players){
        if (player->game == source)
            continue;


    }


}
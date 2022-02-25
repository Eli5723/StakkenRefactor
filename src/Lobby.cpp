#include <Lobby.h>

void Lobby::player_add(Player* player){
    players[player->id] = player;

    if (context)
        context->on_add();

}

void Lobby::player_remove(Player* player){
    players.erase(player->id);

    if (context)
        context->on_remove();
}
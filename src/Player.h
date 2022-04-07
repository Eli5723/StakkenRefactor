#pragma once

#include <Data/Identity.h>
#include <Game/Game.h>


struct Game;
struct Player {
    int id;
    Identity* identity;
    Game* game;
};
#pragma once

#include <Game/Game.h>
#include <Data/Identity.h>

struct Player {
    Game* game;
    Identity* identity;
    int id;
};
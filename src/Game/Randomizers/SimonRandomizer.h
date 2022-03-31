#pragma once

#include <Game/Randomizers/Randomizer.h>
#include <Game/Randomizers/XoroshiroRandomizer.h>

struct SimonRandomizer : Randomizer {
    XoroshiroRandomizer source;
    int last = -1; 

    void seed(int s1);
    inline u64 rotl(const u64 x, int k);
    u64 next(void);
};
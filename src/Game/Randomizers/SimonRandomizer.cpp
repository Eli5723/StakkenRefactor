#include <Game/Randomizers/SimonRandomizer.h>

/* This is xoshiro256++ 1.0, one of our all-purpose, rock-solid generators.
   It has excellent (sub-ns) speed, a state (256 bits) that is large
   enough for any parallel application, and it passes all tests we are
   aware of.

   For generating just floating-point numbers, xoshiro256+ is even faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

void SimonRandomizer::seed(int s1){
    source.seed(s1);
}

uint64_t SimonRandomizer::rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

uint64_t SimonRandomizer::next(void) {
    
    uint64_t piece = source.next()%7;
    if (piece == last){
        // 1 in 5 chance to keep
        if (source.next()%5 == 0){
            last = piece;
            return piece;
        }

        piece = source.next()%7;
        last = piece;
        return piece;
    }


    last = piece;
    return piece;
}
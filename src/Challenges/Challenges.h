#pragma once

#include <Game/Game.h>
#include <string>

namespace Challenges {


enum CHALLENGE {
    SPRINT,
    SURVIVAL,
    Z2SAM,

    _COUNT
};

const std::string& getChallengeName(int challengeId);
const std::string& getChallengeDescription(int challengeId);

}
#include <Challenges/Challenges.h>

namespace Challenges {

const std::string challengeNames[CHALLENGE::_COUNT]{
    "Sprint",
    "Survival",
    "z2sam"
};

const std::string challengeDescription[CHALLENGE::_COUNT]{
    "Clear 40 Lines as fast as possible.",
    "Survive for as long as you can.",
    "Send as many lines as you can in a minute."
};

const std::string& getChallengeName(int challengeId){
    return challengeNames[challengeId];
}
const std::string& getChallengeDescription(int challengeId){
    return challengeDescription[challengeId];
}

}
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

GameRules* getChallenge(int challengeID){
    switch (challengeID){
        case 0: return new Sprint;
        case 1: return new Survivor;
    }

    printf("Tried to load nonexistant gammeode.\n");
    return 0;
}


GameRules* challenges[]{
    new Sprint,
    new Survivor,
};

int records[CHALLENGE::_COUNT]{
    60000,
    60000 * 2,
    200
};


int getRecord(int challengeID){
    if (challengeID < 0 || challengeID >= CHALLENGE::_COUNT)
        return -1; 

    return records[challengeID];
}

bool compareRecord(int challengeID, int score){
    if (challengeID < 0 || challengeID >= CHALLENGE::_COUNT)
        return false;

    int record = getRecord(challengeID);

    if (challenges[challengeID]->compare(score,record)){
        records[challengeID] = score;
        return true;
    }

    return false;

}



}
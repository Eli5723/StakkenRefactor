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

struct Sprint : GameRules {
    void on_update(Game* game, int dt){
        
    };
    void on_place(Game* game,int cleared, int combo){
        if (game->stats.clears >= 40){
            game->Win();
        }
    };

    int score(Game* game){
        return game->time;

    }; 

    bool compare(int score_a, int score_b){
        return score_a < score_b;
    }
};

struct Survivor : GameRules {
    int creepTime = 4000;
    
    void on_init(Game* game){
        game->creep_rate = creepTime;
        game->Creep();
    }

    void on_update(Game* game, int dt){
        game->creep_timer -= dt;
        while (game->creep_timer <= 0){
            game->Creep();
            game->creep_timer += creepTime;
        } 
    };
    void on_place(Game* game,int cleared, int combo){
        if (game->stats.clears >= 40){
            game->Win();
        }
    };

    int score(Game* game){
        return game->time;

    }; 

    bool compare(int score_a, int score_b){
        return score_a < score_b;
    }
};


GameRules* getChallenge(int challengeID);

}
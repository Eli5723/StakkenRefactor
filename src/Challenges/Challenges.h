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
    int get_id(){return 0;}
    
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

    void score_format(int score, char* str){
        float seconds = (float)score/1000.0f;
        sprintf(str,"Time: %.2f", seconds);
    }
};

struct Survivor : GameRules {
    int get_id(){return 1;}

    int creepTime = 4000;
    
    void on_init(Game* game){
        game->creep_rate = creepTime;
        game->creep_timer = creepTime;
        game->Creep();
    }

    void on_update(Game* game, int dt){
        game->creep_timer -= dt;
        while (game->creep_timer <= 0){
            game->Creep();
            game->creep_timer += creepTime;
        } 
    };
    void on_place(Game* game,int cleared, int send){

    };

    int score(Game* game){
        return game->time;
    }; 

    void on_lose(Game* game){
        // If you top out after 1 minute, you have survived long enough to win.
        // TODO: Ensure this is sane
        if (game->time > 60000)
            game->Win();
    }

    bool compare(int score_a, int score_b){
        return score_a > score_b;
    }
};

struct z2sam : GameRules {
    int get_id(){return 2;}
    
    void on_update(Game* game, int dt){
        
    };
    void on_place(Game* game,int cleared, int combo){
        
    };

    int score(Game* game){
        return game->stats.sent;
    }; 

    void score_format(int score, char* str){
        sprintf(str,"Lines Sent: %i", score);
    }
};

GameRules* getChallenge(int challengeID);

int getRecord(int challengeID);
bool compareRecord(int ChallengeID, int score);

}
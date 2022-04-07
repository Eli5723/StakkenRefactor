#include <Game/DemoPlayer.h>

#define mint(x,y) ((x < y) ? x : y) 

void DemoPlayer::advance(int dt){
    dt*= speed;

    dt = mint(demo->duration - (time-1), dt);
    time += dt;
    demo->advance(dt);
    target.CosmeticUpdate(dt);
    target.Events(demo->buffer.events, demo->buffer.count);

    demo->flush();
}

void DemoPlayer::rewind(int dt){
    dt*= speed;

    time -= dt;
    time = time > 0 ? time : 0;

    demo->reset();
    target.Reset(demo->seed);

    demo->advance(time);
}

void DemoPlayer::pause(){

}

void DemoPlayer::restart(){
    time = 0;
    demo->reset();
    target.Reset(demo->seed);
    target.time = 0;
    target.state = Game::State::Playing;
}
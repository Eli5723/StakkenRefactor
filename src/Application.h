#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <Assets/Assets.h>
#include <Player.h>
#include <vector>


#include <Input/Input.h>

struct State {
    virtual void init(){};
    virtual void update(int dt, int time){};
    virtual void render(int dt, int time){};
    virtual bool key_capture(const SDL_KeyboardEvent& key){return false;};
    virtual void close(){};
    virtual void try_back(){};
};

struct Application {
    static Application* instance;

    bool running = false;
    std::vector<State*> state_stack;

    void state_set(State* newState);
    void state_push(State* newState);
    void state_pop();

    // Window
    SDL_Window* window;
    glm::vec2 resolution;
    
    // Game View Area
    glm::vec2 game_resolution{1280, 720};
    glm::vec2 game_offset{0,0};
    float game_scale = 1;

    // Delay
    float frame_delay;

    // Global Record Related Data
    Player* local_player = 0;

    Input::IController* local_controller;

    Identity* local_identity;

    int init();
    int execute();
    void update(int dt, int time);
    void render(int dt, int time);
    void events(const SDL_Event& event);

    void on_resize(int width, int height);

    void target_fps(int target);
};
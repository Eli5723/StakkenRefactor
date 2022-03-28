#pragma once

#include <Game/DemoFrame.h>
#include <Game/TetrisEvent.h>
#include <Game/EventBuffer.h>

#include <vector>
#include <filesystem>

struct Demo {
    // Settings
    int minimumFrameDuration = 16;

    // Metadata
    int seed = 0;
    int duration = 0;

    // Recording data
    int time = 0;
    int delta = 0;
    int currentFrame = 0;
    int currentAction = 0;
    int frameCount = 0;

    std::vector<TetrisEvent> events;
    std::vector<DemoFrame> frames;

    EventBuffer buffer;
    
    Demo();

    void reset();
    void advance(int dt);
    void flush();

    static Demo* Load(const std::filesystem::path& path);
};
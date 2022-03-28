#pragma once

#include <Game/TetrisEvent.h>
#include <Game/DemoFrame.h>

#include <vector>
#include <filesystem>
#include <fstream>

struct Recorder {
    // Settings
    int minimumFrameDuration = 16;

    // Metadata
    int seed = 0;
    int duration = 0;
    
    // Current Frame Information
    int frameEvents = 0;
    int frameDuration = 0;

    Recorder();

    // Event Stream
    std::vector<TetrisEvent> events;
    std::vector<DemoFrame> frames;

    // Recording
    void reset(int seed);

    void registerEvent(TetrisEvent event);
    void advance(int dt);
    void commitFrame();


    // File Handling
    void save(const std::string& path);
    void encode(std::ostream& stream);
};
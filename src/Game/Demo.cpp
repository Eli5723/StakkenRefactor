#include <Game/Demo.h>

#include <fstream>

#include <cstdio>

using std::string;
using std::ifstream;
using std::vector;


Demo::Demo() {
    events = vector<TetrisEvent>();
    frames = vector<DemoFrame>();
    currentAction = 0;
    currentFrame = 0;
    delta = 0;
}

void Demo::reset(){
    time = 0;
    currentFrame = 0;
    currentAction = 0;
    delta=0;
    buffer.events = &events.data()[0];
}

void Demo::advance(int dt){
    time +=dt;
    delta+= dt;

    while (currentFrame < frameCount && delta > frames[currentFrame].duration){
        buffer.count += frames[currentFrame].eventCount;
        delta -= frames[currentFrame].duration;

        currentFrame++;
    }
}

void Demo::flush(){
    currentAction += buffer.count;
    buffer.events = &events.data()[currentAction];
    buffer.count = 0;
}

Demo* Demo::Load(const std::filesystem::path& path){
    ifstream recording(path, std::ios::binary);
    
    if (!recording){
        printf("Failed to Load Demo '%s'.\n", path.c_str());
        return 0;
    }

    Demo* demo = new Demo;

    recording.read((char*)&demo->seed,sizeof(int));
    recording.read((char*)&demo->duration,sizeof(int));
    recording.read((char*)&demo->frameCount,sizeof(int));

    printf("Loading Demo:\n  Seed: %u\n  Duration: %u\n  Frames: %u\n", demo->seed, demo->duration, demo->frameCount);

    int frameDuration;
    int eventCount;

    for (int f=0; f < demo->frameCount; f++){
        
        recording.read((char*)&frameDuration,sizeof(int));
        recording.read((char*)&eventCount,sizeof(int));

        demo->frames.push_back({
            eventCount,frameDuration, 
        });

        for (int e = 0; e < eventCount; e++){
            TetrisEvent event;
           
            recording.read((char*)&event, 1);
            demo->events.push_back(event);
        }

        demo->buffer.events = &demo->events.data()[0];
    }

    printf("Demo loaded successfully\n");
    return demo;
}
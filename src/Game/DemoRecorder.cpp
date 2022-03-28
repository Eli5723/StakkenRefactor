#include <Game/DemoRecorder.h>

using std::string;
using std::ofstream;
using std::vector;

Recorder::Recorder() {
    events = vector<TetrisEvent>();
    frames = vector<DemoFrame>(2000);
}

void Recorder::reset(int seed){
    this->seed = seed;
    
    events.clear();
    frames.clear();

    duration = 0;
    frameEvents = 0;
    frameDuration = 0;
}

void Recorder::registerEvent(TetrisEvent event){
    frameEvents++;
    events.push_back(event); 
}

void Recorder::advance(int dt){
    duration += dt;
    frameDuration+=dt;

    if (frameDuration > minimumFrameDuration && frameEvents > 0) {
        commitFrame();
    }
}

void Recorder::commitFrame(){
    frames.push_back({frameEvents,frameDuration});
    frameDuration = 0;
    frameEvents = 0;
}


void Recorder::encode(std::ostream& stream){
    int frameCount = frames.size();
    
    stream.write((char*)&seed,sizeof(int));
    stream.write((char*)&duration,sizeof(int));
    stream.write((char*)&frameCount,sizeof(int));
    
    // file.write((char*)&rotation,sizeof(RotationTable));
    // file.write((char*))

    // Event Buffer
    int ePos = 0;
    for (int i = 0; i < frameCount; i++){
        // Record Frame metadata
        DemoFrame& frame = frames[i];
        stream.write((char*)&frame.duration,sizeof(int));
        stream.write((char*)&frame.eventCount,sizeof(int));

        // Record Events
        stream.write((char*)&events.data()[ePos],frame.eventCount);

        ePos+=frame.eventCount;
    }
}

void Recorder::save(const string& path){
    commitFrame();

    ofstream file(path, std::ios::binary);

    //Metadata
    encode(file);

    int size = file.tellp();
    int kb = size / 1024;
    int mb = kb / 1024;

    file.close();
}
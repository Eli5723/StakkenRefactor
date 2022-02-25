#include <Data/Identity.h>

#include <SDL2/SDL.h>

#include <string>
#include <map>
#include <fstream>

static const std::string PFP_DIRECTORY = "./icons/";
static const std::string IDENTITY_DIRECTORY = "./identities/";
static const std::string FILE_EXTENSION = ".id";

Identity* Identity::Load(const std::string& name){
    Identity* loaded = new Identity;
    loaded->name = name;

    std::string path = IDENTITY_DIRECTORY + name + FILE_EXTENSION;
    std::ifstream input(path);

    if (!input){
        SDL_Log("Failed to Load identity \"%s\".", name.c_str());
        return 0;
    }

    loaded->decode(input);
    SDL_Log("Loaded Identity %s", name.c_str());
    return loaded;
}

const static char* BotNames[] {
    "Fuu",
    "NO SERVICE",
    "Yaranaika"
};
const int MaxBotName = 3;

int idx = 0;
Identity* Identity::LoadRandom(){
    srand(clock()+idx++);
    const char* name = BotNames[rand() % MaxBotName];
    return Load(name);
}

void Identity::save(){
    std::string path = IDENTITY_DIRECTORY + name + FILE_EXTENSION;
    std::ofstream out(path, std::ios::trunc | std::ios::out);
    encode(out);
    out.close();
    SDL_Log("Wrote Identity \"%s\" to \"%s\"",name.c_str(), path.c_str()); 
    return;
}

void Identity::decode(std::istream& input){
    std::string pfpPath;

    std::getline(input,pfpPath);

    // #ifdef IS_CLIENT
    if (pfpPath != "0")
        pfp = Assets::Texture::Load(PFP_DIRECTORY + pfpPath, GL_LINEAR);
    else
        pfp = 0;
    // #endif

    int r,g,b;
    for (int i=0; i < 8; i++){
        input >> r;
        input >> g;
        input >> b; 

        colorTable.entries[i] = rgbToFloat(r,g,b);
    }

    int rotation;
    for (int piece_type = 0; piece_type < 7; piece_type++){
        input >> rotation;

        rotationTable[piece_type] = rotation;
    }

}

void Identity::encode(std::ostream& out){

    if (pfp)
        out << pfp->path.filename().string() << '\n';
    else
        out << "0\n";

    int r,g,b;
    for (int i=0; i < 8; i++){
        r = (int)(colorTable.entries[i].r*255);
        g = (int)(colorTable.entries[i].g*255);
        b = (int)(colorTable.entries[i].b*255);

        out << r << ' ' << g << ' ' << b << '\n';
    }

    int rotation;
    for (int piece_type=0; piece_type < 7; piece_type++){
        rotation = rotationTable[piece_type];
        out << rotation << '\n';
    }

    return;
}
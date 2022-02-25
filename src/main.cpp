#include <stb_wrapper.h>

#include <Application.h>

int main(int argc, const char* argv[]){
    Application app;

    app.init();
    return app.execute();
}

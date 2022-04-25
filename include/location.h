#pragma once

#include <filesystem>

#if defined(__linux__) || defined(__APPLE__)

    #include <unistd.h>

    namespace Location {

        std::filesystem::path ExecutablePath(){
            const int MAXSIZE=255;
            char path[MAXSIZE];
            memset(path, 0, MAXSIZE);

            readlink("/proc/self/exe",path,MAXSIZE);
            *strrchr(path, '/') = '\0';

            return std::filesystem::path(path);
        }

    }

#elif _WIN32

    #include <Windows.h>

    namespace Location {

        std::filesystem::path ExecutablePath(){

            char buffer[MAX_PATH];
            GetModuleFileNameA(NULL, buffer, MAX_PATH);

            return std::path(buffer);

        }
    }

#else

#endif
#pragma once

#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>

namespace Input {
    static const std::filesystem::path PROFILE_DIRECTORY = "./profiles/";
    static const std::filesystem::path FILE_EXTENSION = ".pro";


    struct InputProfile {
        std::string name;

        // Repeat settings
        int das;
        int arr;
        int dropArr;

        // Piece movement settings
        union {
            int keys[8];
            struct {
                int left;
                int right;
                int softDrop;
                int hardDrop;
                int sonicDrop;
                int rcw;
                int rccw;
                int flip;
            };
        };

        // Initial Piece Rotation
        union {
            int rotation[7];
            struct {
                int T_rotation;
                int O_rotation;
                int L_rotation;
                int J_rotation;
                int Z_rotation;
                int S_rotation;
                int I_rotation;
            };
        };

        // Default Profile
        InputProfile();
        // Load existing profile from file
        static InputProfile* Load(const std::string& name);
        void save();
        void print();
        void write(std::ostream& out);
    };
}
#pragma once

#include <Networking/SendBuffer.h>
#include <Networking/ReadBuffer.h>

#include <string>

namespace Network {

    struct Profile {
        int id;
        int roles;
        std::string name;

        void Serialize(SendBuffer* buffer){
            buffer->Write<int>(id);
            buffer->Write<int>(roles);
            buffer->WriteString(name.c_str());
        }

        static void Deserialize(Profile& profile){
            profile.id = ReadBuffer::Read<int>();
            profile.roles = ReadBuffer::Read<int>();
            profile.name = ReadBuffer::ReadString();
        }
    };

    struct RoomInfo {
        int id;
        std::string name;

        void Serialize(SendBuffer* buffer){
            buffer->Write<int>(id);
            buffer->WriteString(name.c_str());
        }

        static void Deserialize(RoomInfo& info){
            info.id = ReadBuffer::Read<int>();
            info.name = ReadBuffer::ReadString();
        }
    };

}
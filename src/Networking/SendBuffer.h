#pragma once

#include <types.h>

#include <cstring>

namespace Network {

    struct SendBuffer {
        static u8* buffer;
        int pos = 0;
        
        void Reset(){
            pos = 0;
        }

        template<typename t>
        void Write(t val){
            *(t*)&buffer[pos] = val;
            pos += sizeof(t);
        }

        u8* GetBuffer(){
            return buffer;
        }

        int GetBufferSize(){
            return pos;
        }

        // void Begin(MessageDestination destination, MessageType type){
        //     pos = 0;
        //     Write<MessageDestination>(destination);
        //     Write<MessageType>(type);
        // }

        void WriteString(const char* str){
            int len = strlen(str);
            memcpy((char*)&buffer[pos], str, len);
            buffer[pos+len+1] = '\0';

            pos = pos + len + 1;
        }
    };

}
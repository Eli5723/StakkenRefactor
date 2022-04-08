#pragma once

#include <types.h>

#include <cstring>
#include <Networking/Message.h>
#include <typeinfo>
#include <cstdio>
namespace Network {

    struct SendBuffer {
        u8* buffer;
        int pos = 0;
        
        void Reset(){
            pos = 0;
        }

        template<typename t>
        void Write(t val){
            *(t*)&buffer[pos] = val;
            pos += sizeof(t);
            // printf("Writing %s (%i) at %i\n", typeid(t).name(), val, pos);
        }

        u8* GetBuffer(){
            return buffer;
        }

        int GetBufferSize(){
            return pos;
        }

        void Begin(Message m){
            pos = 0;
            Write<Message>(m);
        }

        void WriteString(const char* str){
            int len = strlen(str);
            memcpy((char*)&buffer[pos], str, len);
            buffer[pos+len+1] = '\0';

            // printf("Writing %s at %i\n", str, pos);
            pos = pos + len + 1;
        }
 
        template<typename t>
        inline void Serialize(t& item){
            item.Serialize(this);
        }
    };

}
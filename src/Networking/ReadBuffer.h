#pragma once
#include <types.h>
#include <string>   
#include <cstring>

namespace Network {

struct ReadBuffer {
    static inline u8* buffer;
    static inline int pos;
    static inline int size;
    
    static void Set_Buffer(void* newBuffer){
        buffer = static_cast<u8*>(newBuffer);
        pos = 0;
    }

    template<typename t>
    static t Read(){
        t ret = *(t*)&buffer[pos];
        pos += sizeof(t);
        return ret;
    }

    template<typename t>
    static t ReadSafe(){
        if (size - pos < 0)
            return 0;

        t ret = *(t*)&buffer[pos];
        pos += sizeof(t);
        return ret;
    }

    static char* ReadString(){
        int len = strlen((char*)&buffer[pos]);
        pos += len + 1;
        return (char*)&buffer[pos - len - 1];
    }
};

}
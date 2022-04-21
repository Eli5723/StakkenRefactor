#pragma once

#include <types.h>

template<int size>
class LocklessQueue {
    const u8 buffer[size];

    u8* head;
    u8* tail;

    LocklessQueue(){
        head = &buffer[0];
        tail = &buffer[0];
    }

    u8* allocate(int size){
        u8* ret = tail;
        tail += size;

        return ret;
    }

    template<T> 
    T* allocate(){
        T* item = (T*)tail;
        tail += sizeof(T);

        return item;
    }

    void get(){
        
    }

    void reset(){
        head = &buffer[0];
    }

    head = buffer;
};
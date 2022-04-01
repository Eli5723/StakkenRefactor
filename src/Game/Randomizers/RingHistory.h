template<class T, int size>
struct RingHistory {
    T items[size];
    int pos = 0;

    void reset(const T (&initialState)[size]){
        pos = 0;

        for (int i=0;i < size; i++){
            items[i] = initialState[i];
        }
    }
    
    void insert(T item){
        items[pos] = item;

        pos = (pos+1)%size;
    }

    bool contains(T item){
        for (int i=0; i < size; i++){
            if (items[i] == item)
                return true;
        }
        return false;
    }
};

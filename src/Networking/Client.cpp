#include "./net.h"

#include "./net_client.h"

#include "./StakkenMSG.h"

class stakkenclient : public net::client_interface<StakkenMSG> {
    public:
    stakkenclient() : net::client_interface<StakkenMSG>() {

    };
};


int main(){

    // net::message<MessageType> msg;
    stakkenclient test{};
    test.Connect("127.0.0.1",60000);

    while (true){
        if (test.IsConnected()){
            while (!test.Incoming().empty()){
                auto msg = test.Incoming().pop_front().msg;
                int text;

                msg >> text;
                std::cout << "Got message: \n" << text << '\n';
            }
        }
    }

    return 0;
}
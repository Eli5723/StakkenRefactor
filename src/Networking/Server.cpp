#define ASIO_STANDALONE


#include "./net_server.h"
#include "./StakkenMSG.h"

using namespace net;

class CustomServer : public net::server_interface<StakkenMSG> {
    public:
    CustomServer(u16 port) : net::server_interface<StakkenMSG>(port) {

    }

    protected:
    virtual bool OnClientConnect(std::shared_ptr<connection<StakkenMSG>> client){

        return true;
    }
        
    virtual void OnClientDisconnect(std::shared_ptr<connection<StakkenMSG>> client){

    }

    virtual void OnMessage(std::shared_ptr<connection<StakkenMSG>> client, message<StakkenMSG>& msg){

    }

};


int main(){
    CustomServer server(60000);
    server.Start();

    while (1){
        server.Update();

        net::message<StakkenMSG> msg;
        msg.header.id = StakkenMSG::ServerPing; 
        std::string a("heeeellloooo");
        msg << 4;
        server.MessageAllClients(msg);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}
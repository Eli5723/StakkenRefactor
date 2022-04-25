#pragma once

#include <string>

namespace Network {

void send_login_request(const std::string& username, const std::string& password);
void send_guest_request(const std::string& nickname);

void connect();

}
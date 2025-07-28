#ifndef RESOLVER_H
#define RESOLVER_H

#include "Receiver.h"
#include "packet/Message.h"

#include <map>
#include <cstdint>

class Resolver {
public:
    Resolver() = default;

    void run();
    void display_packet(const IncomingPacket) const;

private:
    Receiver receiver;
    std::unordered_map<uint16_t, sockaddr_in> id_to_client_;
    bool running_ = true;
    void handle_msg(Message);
};

#endif
#include "Resolver.h"
#include "packet/Message.h"


void Resolver::run() {
    while (true) {
        IncomingPacket packet = receiver.listen();
        std::cout << "Packet received by resolver\n";
        std::cout << "IP: " << packet.client_addr_.sin_addr.s_addr << std::endl;
        std::cout << "Port: " << packet.client_addr_.sin_port << std::endl;
        std::cout << "Packet length: " << packet.length_ << std::endl;
        Message msg(packet.buffer_, 0);
        std::cout << msg;
    }
}

void Resolver::handle_msg(Message msg) {
    std::cout << "TODO: Handle msg...\n";
}
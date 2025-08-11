#include "Resolver.h"


void Resolver::run() {
    while (running_) {
        Packet packet = communicator_.listen();
        display_packet(packet);
        solve_question(packet);
    }
}

void Resolver::display_packet(const Packet packet) const {
    std::cout << "Packet received by resolver\n";
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(packet.addr_.sin_addr.s_addr), ipstr, sizeof(ipstr));
    std::cout << "IP: " << ipstr << std::endl;
    std::cout << "Port: " << ntohs(packet.addr_.sin_port) << std::endl;
    std::cout << "Packet length: " << packet.length_ << std::endl;
    Message msg(packet.buffer_, 0);
}

void Resolver::solve_question(const Packet packet) {
    sockaddr_in client_addr = packet.addr_;
    Message msg(packet.buffer_, 0);
    
    // throw back (delete!)
    communicator_.send(packet);
}
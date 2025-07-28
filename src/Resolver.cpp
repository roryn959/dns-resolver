#include "Resolver.h"


void Resolver::run() {
    while (running_) {
        IncomingPacket packet = receiver.listen();
        display_packet(packet);
    }
}

void Resolver::display_packet(const IncomingPacket packet) const {
    std::cout << "Packet received by resolver\n";
    std::cout << "IP: " << packet.client_addr_.sin_addr.s_addr << std::endl;
    std::cout << "Port: " << packet.client_addr_.sin_port << std::endl;
    std::cout << "Packet length: " << packet.length_ << std::endl;
    Message msg(packet.buffer_, 0);
    Header temp_head = msg.get_header();
    temp_head.set_opcode(Header::OPCODE::IQUERY);
    std::cout << "Modified header:\n";
    std::cout << temp_head;
    std::cout << msg;
}

void Resolver::handle_msg(Message msg) {
    std::cout << "TODO: Handle msg...\n";
}
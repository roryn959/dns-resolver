#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "packet/Message.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cstdint>

#define SERVER_PORT 8080
#define UDP_PORT 53
#define BUFFER_SIZE 4096

struct Packet {
    const sockaddr_in addr_;
    const uint8_t* buffer_;
    const size_t length_;
};

class Communicator {
public:
    Communicator();
    ~Communicator();

    const Packet listen();
    void send(const Packet& packet);
    void send(const std::string& ip, const Message& msg);
    void send(const std::vector<uint8_t>& ip_vec, const Message& msg);
    void send(const uint8_t* const pip, const Message& msg);
    void send(const sockaddr_in& addr, const Message& msg);

private:
    int sock_;
    struct sockaddr_in server_addr_;
    const socklen_t addr_length_ = sizeof(sockaddr_in);

    void create_socket();
    void fill_address();
    void bind_socket();

};

#endif
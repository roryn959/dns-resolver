#ifndef RECEIVER_H
#define RECEIVER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cstdint>

#define RECEIVE_PORT 8080
#define BUFFER_SIZE 512

struct IncomingPacket {
    const sockaddr_in client_addr_;
    const uint8_t* buffer_;
    const size_t length_;
};

class Receiver {
public:
    Receiver();
    ~Receiver();

    const IncomingPacket listen();

private:
    int server_fd_;
    struct sockaddr_in server_addr_;
    const socklen_t addr_length_ = sizeof(sockaddr_in);

    void create_socket();
    void fill_address();
    void bind_socket();
};

namespace Receiver_Test {
    void test();
}

#endif
#include <Receiver.h>

Receiver::Receiver() {
    create_socket();
    fill_address();
    bind_socket();
    std::cout << "Server is listening on port " << PORT << "...\n";
}

Receiver::~Receiver() {
    close(server_fd_);
}

void Receiver::create_socket() {
    server_fd_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_fd_ < 0) {
        throw "Failed to create socket";
    }
}

void Receiver::fill_address() {
    memset(&server_addr_, 0, addr_length_);
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_addr.s_addr = INADDR_ANY;
    server_addr_.sin_port = htons(PORT);
}

void Receiver::bind_socket() {
    int bind_result = bind(
        server_fd_,
        (struct sockaddr *)&server_addr_,
        addr_length_
    );
    if (bind_result < 0) {
        close(server_fd_);
        throw "Failed to bind socket to port";
    }
}

const IncomingPacket Receiver::listen() {
    uint8_t* buffer = new uint8_t[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t mutable_addr_len = addr_length_;

    int bytes_received = recvfrom(
        server_fd_,
        buffer,
        BUFFER_SIZE,
        0,
        (struct sockaddr *)&client_addr,
        &mutable_addr_len
    );

    if (bytes_received < 0) {
        throw "Receive failed";
    }

    return IncomingPacket {client_addr, buffer, (size_t) bytes_received};
}

namespace Receiver_Test {
    void test() {
        Receiver receiver;
        receiver.listen();
    }
}
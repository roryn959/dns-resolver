#include "Communicator.h"

Communicator::Communicator() {
    create_socket();
    fill_address();
    bind_socket();
    std::cout << "Server is listening on port " << SERVER_PORT << "...\n";
}

Communicator::~Communicator() {
    close(sock_);
}

void Communicator::create_socket() {
    sock_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock_ < 0) {
        throw "Failed to create socket";
    }
}

void Communicator::fill_address() {
    memset(&server_addr_, 0, addr_length_);
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_addr.s_addr = INADDR_ANY;
    server_addr_.sin_port = htons(SERVER_PORT);
}

void Communicator::bind_socket() {
    int bind_result = bind(
        sock_,
        (struct sockaddr *)&server_addr_,
        addr_length_
    );
    if (bind_result < 0) {
        close(sock_);
        std::cerr << "Failed to bind socket to port";
        throw "Failed to bind socket to port";
    }
}

const Packet Communicator::listen() {
    uint8_t* buffer = new uint8_t[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t mutable_addr_len = addr_length_;

    int bytes_received = recvfrom(
        sock_,
        buffer,
        BUFFER_SIZE,
        0,
        (struct sockaddr *)&client_addr,
        &mutable_addr_len
    );

    if (bytes_received < 0) {
        std::cerr << "Receive failed\n";
        throw "Receive failed";
    }

    return Packet {client_addr, buffer, (size_t) bytes_received};
}

void Communicator::send(const Packet& packet) {
    int sent = sendto(sock_, packet.buffer_, BUFFER_SIZE, 0, (struct sockaddr *) &packet.addr_, sizeof(packet.addr_));
    if (sent == -1) {
        std::cerr << "Failed send\n";
        return;
    }

    std::cout << "Message sent\n";
}

void Communicator::send(const std::string& ip, const Message& msg) {
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(UDP_PORT);

    if (inet_pton(AF_INET, "198.41.0.4", &dest_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / address not supported\n";
        return;
    }

    uint8_t* buffer = new uint8_t[BUFFER_SIZE];
    size_t bytes = msg.serialise(buffer);
    std::cout << "Serialisation contains " << bytes << " bytes.\n";

    int sent = sendto(sock_, buffer, bytes, 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
    if (sent == -1) {
        std::cout << "Did not send properly.\n";
    } else {
        std::cout << "Message sent" << std::endl;
    }
}
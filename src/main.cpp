#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#define PORT 8080

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char buffer[1024] = {0};

    // Create UDP socket
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return 1;
    }

    // Fill in server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Listen on any interface
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << "...\n";

    while (true) {
        // Receive message
        int bytes_received = recvfrom(
            server_fd,
            buffer,
            sizeof(buffer) - 1,
            0,
            (struct sockaddr *)&client_addr,
            &addrlen
        );

        if (bytes_received < 0) {
            perror("recvfrom failed");
        } else {
            buffer[bytes_received] = '\0'; // Null-terminate
            std::cout << "Received: " << buffer << std::endl;
        }
    }

    close(server_fd);
    return 0;
}

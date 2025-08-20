#include "Resolver.h"

std::ostream& operator<<(std::ostream& out, const Packet& packet) {
    out << "Packet received by resolver\n";
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(packet.addr_.sin_addr.s_addr), ipstr, sizeof(ipstr));
    out << "IP: " << ipstr << std::endl;
    out << "Port: " << ntohs(packet.addr_.sin_port) << std::endl;
    out << "Packet length: " << packet.length_ << std::endl;
    return out;
}

void Resolver::run() {
    while (running_) {
        Packet packet = communicator_.listen();
        std::cout << packet;
        handle_packet(packet);
    }
}

void Resolver::handle_packet(const Packet& packet) {
    std::cout << "Handling packet\n";
    Message msg(packet.buffer_);
    std::cout << "Post parse\n";
    msg.get_header().is_response() ? handle_response(msg) : handle_query(packet, msg);
}

void Resolver::handle_query(const Packet& packet, const Message& query) {
    std::cout << "Received a query!\n";
    std::cout << query;

    uint16_t id = query.get_header().get_id();

    if (context_map_.contains(id)) {
        std::cout << "A query with that ID already exists! Dropping request.\n";
        return;
    }

    QueryContext context { packet.addr_, false };
    context_map_.insert( {id, context} );

    std::cout << "Next step is to actually send the next packet...\n";

    communicator_.send(ROOT_SERVER_IP, query);
}

void Resolver::handle_response(const Message& response) {
    std::cout << "Received a response!\n";
    std::cout << response;
}

/*
 Query -> Does a query of the same ID exist?
    Yes -> Drop
    No -> Begin resolution process. Initialise query context.
 Response -> Does it have answers?
    Yes -> Is it an inner query?
        Yes -> Get answer, relate it to the relevant outer query
        No -> Brilliant, return to client
    No -> Does it have any authorities?
        Yes -> Use them directly
        No -> Use any given info to find next hop
    If it has nothing for us, then drop the request. For now, that means ignore it. When threaded,
    use LRA cache.
*/

/*
void Resolver::solve_question(const Packet packet) {

    sockaddr_in client_addr = packet.addr_;
    uint16_t client_port = ntohs(client_addr.sin_port);
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(packet.addr_.sin_addr.s_addr), ipstr, sizeof(ipstr));

    Message msg(packet.buffer_, 0);
    
    std::cout << "Sending packet to root...\n";
    std::string root_ip = "198.41.0.4";
    communicator_.send(root_ip, msg);
    Packet response = communicator_.listen();

    Message result(response.buffer_, 0);
    std::cout << result;

    uint8_t *buffer = new uint8_t[BUFFER_SIZE];
    msg.serialise(buffer);

    std::cout << "Sending back to dig...\n";
    Packet pass_on = Packet { client_addr, buffer, BUFFER_SIZE };
    std::cout << "$$$\n";
    communicator_.send(pass_on);
}
*/
#ifndef RESOLVER_H
#define RESOLVER_H

#include "Communicator.h"
#include "packet/Message.h"

#include <map>
#include <cstdint>
#include <iostream>

#define ROOT_SERVER_IP "198.41.0.4"

/*
 Query -> begin resolution process. Initialise query context.
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

struct QueryContext {
    sockaddr_in client_addr_;
    bool is_inner_;
};

class Resolver {
public:
    Resolver() = default;

    void run();

private:
    bool running_ = true;
    std::unordered_map<uint16_t, QueryContext> context_map_;

    Communicator communicator_;

    void handle_packet(const Packet& packet);
    void handle_query(const Packet& packet, const Message& query);
    void handle_response(const Message& response);
};

#endif
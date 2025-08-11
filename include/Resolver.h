#ifndef RESOLVER_H
#define RESOLVER_H

#include "Communicator.h"
#include "packet/Message.h"

#include <map>
#include <cstdint>

class Resolver {
public:
    Resolver() = default;

    void run();
    void display_packet(const Packet) const;

private:
    Communicator communicator_;
    bool running_ = true;
    void solve_question(const Packet);
};

#endif
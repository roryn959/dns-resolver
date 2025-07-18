#ifndef DNS_PACKET_H
#define DNS_PACKET_H

#include <cstdint>

enum OPCODE {
    QUERY = 0,
    IQUERY,
    STATUS
};

enum RCODE {
    NOERROR,
    FORMERR,
    SERVFAIL,
    NXDOMAIN
};

class DNS_Packet {
public:
    uint16_t id;
    uint16_t flags;
    uint16_t num_questions;
    uint16_t num_answers;
    uint16_t num_authorities;
    uint16_t num_additionals;
};

#endif
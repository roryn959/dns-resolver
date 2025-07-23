#ifndef HEADER_H
#define HEADER_H

#include <cstdint>
#include <string>
#include <iostream>

class Header {
public:

    enum class OPCODE: uint16_t {
        QUERY,
        IQUERY,
        STATUS,
    };

    enum class RCODE: uint16_t {
        NO_ERROR,
        FORMAT_ERROR,
        SERVER_FAILURE,
        NAME_ERROR,
        NOT_IMPLEMENTED,
        REFUSED
    };

    Header(const uint8_t* buffer, size_t& offset);

    uint16_t get_id() const;
    uint16_t get_qdcount() const;
    uint16_t get_ancount() const;
    uint16_t get_nscount() const;
    uint16_t get_arcount() const;

    bool is_response() const;
    OPCODE get_opcode() const;
    bool is_authoritative_answer() const;
    bool is_truncated() const;
    bool is_recursion_desired() const;
    bool is_recursion_available() const;
    RCODE get_rcode() const;

    friend std::ostream& operator<< (std::ostream&, const Header&);

private:
    uint16_t id_;
    uint16_t options_;
    uint16_t qdcount_;
    uint16_t ancount_;
    uint16_t nscount_;
    uint16_t arcount_;
};

#endif
#ifndef HEADER_H
#define HEADER_H

#include "bit_utils.h"

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

    void serialise(uint8_t* const buffer, size_t& offset) const;

    uint16_t get_id() const;
    void set_id(uint16_t);
    uint16_t get_qdcount() const;
    void set_qdcount(uint16_t);
    uint16_t get_ancount() const;
    void set_ancount(uint16_t);
    uint16_t get_nscount() const;
    void set_nscount(uint16_t);
    uint16_t get_arcount() const;
    void set_arcount(uint16_t);

    bool is_response() const;
    void set_response(bool);
    OPCODE get_opcode() const;
    void set_opcode(OPCODE);
    bool is_authoritative_answer() const;
    void set_authoritative_answer(bool);
    bool is_truncated() const;
    void set_truncated(bool);
    bool is_recursion_desired() const;
    void set_recursion_desired(bool);
    bool is_recursion_available() const;
    void set_recursion_available(bool);
    RCODE get_rcode() const;
    void set_rcode(RCODE);

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
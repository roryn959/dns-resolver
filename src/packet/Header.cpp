#include <packet/Header.h>
#include "read_utils.h"

Header::Header(const uint8_t* buffer, size_t& offset) {
    id_ = read_u16(buffer, offset);
    options_ = read_u16(buffer, offset);
    qdcount_ = read_u16(buffer, offset);
    ancount_ = read_u16(buffer, offset);
    nscount_ = read_u16(buffer, offset);
    arcount_ = read_u16(buffer, offset);
}

uint16_t Header::get_id() const {
    return id_;
}

uint16_t Header::get_qdcount() const {
    return qdcount_;
}

uint16_t Header::get_ancount() const {
    return ancount_;
}

uint16_t Header::get_nscount() const {
    return nscount_;
}

uint16_t Header::get_arcount() const {
    return arcount_;
}

bool Header::is_response() const {
    return (options_ & 0x8000);
}

Header::OPCODE Header::get_opcode() const {
    uint16_t raw_opcode = (options_ & 0x7800) >> 11;
    return (OPCODE) raw_opcode;
}

bool Header::is_authoritative_answer() const {
    return (options_ & 0x0400);
}

bool Header::is_truncated() const {
    return (options_ & 0x0200);
}

bool Header::is_recursion_desired() const {
    return (options_ & 0x0100);
}

bool Header::is_recursion_available() const {
    return (options_ & 0x0080);
}

Header::RCODE Header::get_rcode() const {
    uint16_t raw_rcode = (options_ & 0x000F);
    return (Header::RCODE) raw_rcode;
}

std::ostream& operator<< (std::ostream& out, const Header& header) {
    out << "Header\n";
    out << "ID: " << header.id_ << '\n';
    out << "QR: " << header.is_response() << '\n';
    out << "OPCODE: " << (int) header.get_opcode() << '\n';
    out << "AA: " << header.is_authoritative_answer() << '\n';
    out << "TC: " << header.is_truncated() << '\n';
    out << "RD: " << header.is_recursion_desired() << '\n';
    out << "RA: " << header.is_recursion_available() << '\n';
    out << "RCODE: " << (int) header.get_rcode() << '\n';
    out << "QDs: " << header.qdcount_ << '\n';
    out << "ANs: " << header.ancount_ << '\n';
    out << "NSs: " << header.nscount_ << '\n';
    out << "ARs: " << header.arcount_ << '\n';
    out << "******************\n";
    return out;
}

namespace Header_Test {
    void test() {
        uint8_t test_buffer[] = {
            0xab, 0xcd,       // ID = 0xABCD (same as query)
            0x81, 0x80,       // FLAGS = 0x8180 (QR=1, OPCODE=0, AA=0, TC=0, RD=1, RA=1, Z=0, RCODE=0)
            0x00, 0x01,       // QDCOUNT = 1
            0x00, 0x01,       // ANCOUNT = 1
            0x00, 0x00,       // NSCOUNT = 0
            0x00, 0x00        // ARCOUNT = 0
        };

        size_t test_offset = 0x00;
        Header header(test_buffer, test_offset);

        std::cout << header;
    }
}
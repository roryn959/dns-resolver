#include <packet/Header.h>

#include <string>

std::string to_string(Header::OPCODE opcode) {
    switch (opcode) {
        case Header::OPCODE::QUERY: return "QUERY";
        case Header::OPCODE::IQUERY: return "IQUERY";
        case Header::OPCODE::STATUS: return "STATUS";
        default: return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& out, const Header::OPCODE& opcode) {
    out << to_string(opcode);
    return out;
}

std::string to_string(Header::RCODE rcode) {
    switch (rcode) {
        case Header::RCODE::NO_ERROR: return "NO_ERROR";
        case Header::RCODE::FORMAT_ERROR: return "FORMAT_ERROR";
        case Header::RCODE::SERVER_FAILURE: return "SERVER_FAILURE";
        case Header::RCODE::NAME_ERROR: return "NAME_ERROR";
        case Header::RCODE::NOT_IMPLEMENTED: return "NOT_IMPLEMENTED";
        case Header::RCODE::REFUSED: return "REFUSED";
        default: return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& out, const Header::RCODE& rcode) {
    out << to_string(rcode);
    return out;
}

Header::Header(const uint8_t* buffer, size_t& offset) {
    id_ = read_u16(buffer, offset);
    options_ = read_u16(buffer, offset);
    qdcount_ = read_u16(buffer, offset);
    ancount_ = read_u16(buffer, offset);
    nscount_ = read_u16(buffer, offset);
    arcount_ = read_u16(buffer, offset);
}

size_t Header::serialise(uint8_t* const buffer, size_t& offset) const {
    write_u16(buffer, offset, id_);
    write_u16(buffer, offset, options_);
    write_u16(buffer, offset, qdcount_);
    write_u16(buffer, offset, ancount_);
    write_u16(buffer, offset, nscount_);
    write_u16(buffer, offset, arcount_);
    return 12;
}

uint16_t Header::get_id() const {
    return id_;
}

void Header::set_id(uint16_t id) {
    id_ = id;
}

uint16_t Header::get_qdcount() const {
    return qdcount_;
}

void Header::set_qdcount(uint16_t count) {
    qdcount_ = count;
}

uint16_t Header::get_ancount() const {
    return ancount_;
}

void Header::set_ancount(uint16_t count) {
    ancount_ = count;
}

uint16_t Header::get_nscount() const {
    return nscount_;
}
void Header::set_nscount(uint16_t count) {
    nscount_ = count;
}

uint16_t Header::get_arcount() const {
    return arcount_;
}

void Header::set_arcount(uint16_t count) {
    arcount_ = count;
}

bool Header::is_response() const {
    return (options_ & 0x8000);
}

void Header::set_response(bool response) {
    set_bit(options_, 0x8000, response);
}

Header::OPCODE Header::get_opcode() const {
    uint16_t raw_opcode = (options_ & 0x7800) >> 11;
    return (OPCODE) raw_opcode;
}

void Header::set_opcode(OPCODE opcode) {
    uint16_t raw_opcode = (uint16_t) opcode;
    options_ &= ~0x7800;
    options_ |= (raw_opcode << 11);
}

bool Header::is_authoritative_answer() const {
    return (options_ & 0x0400);
}

void Header::set_authoritative_answer(bool aa) {
    set_bit(options_, 0x0400, aa);
}

bool Header::is_truncated() const {
    return (options_ & 0x0200);
}

void Header::set_truncated(bool trunc) {
    set_bit(options_, 0x0200, trunc);
}

bool Header::is_recursion_desired() const {
    return (options_ & 0x0100);
}

void Header::set_recursion_desired(bool desired) {
    set_bit(options_, 0x0100, desired);
}

bool Header::is_recursion_available() const {
    return (options_ & 0x0080);
}

void Header::set_recursion_available(bool available) {
    set_bit(options_, 0x0080, available);
}

Header::RCODE Header::get_rcode() const {
    uint16_t raw_rcode = (options_ & 0x000F);
    return (Header::RCODE) raw_rcode;
}

void Header::set_rcode(RCODE rcode) {
    uint16_t raw_rcode = (uint16_t) rcode;
    options_ &= ~0x000F;
    options_ |= raw_rcode;
}

std::ostream& operator<< (std::ostream& out, const Header& header) {
    out << ";; ->>HEADER<<- opcode: " << header.get_opcode();
    out << ", status: " << header.get_rcode();
    out << ", id: " << header.get_id();
    out << "\n;; flags: ";
    out << (header.is_response() ? "qr" : "q" );
    if (header.is_authoritative_answer()) out << ", aa";
    if (header.is_truncated()) out << ", tc";
    if (header.is_recursion_desired()) out << ", rd";
    if (header.is_recursion_available()) out << ", ra";
    out << "; QUERY: " << header.qdcount_;
    out << ", ANSWER: " << header.ancount_;
    out << ", AUTHORITY: " << header.nscount_;
    out << ", ADDITIONAL: " << header.arcount_;
    out << "\n\n";

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
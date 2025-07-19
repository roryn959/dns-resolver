
#include "ResourceRecord.h"

#include <string>
#include <iostream>

constexpr uint8_t END_OF_LABEL = 0x00;
constexpr uint8_t POINTER_TO_LABEL = 0xC0;

enum class RR_Type {
    A = 1,
    NS,
    MD,
    MF,
    CNAME,
    SOA,
    MB,
    MG,
    MR,
    NULL_,
    WKS,
    PTR,
    HINFO,
    MINFO,
    MX,
    TXT
};

ResourceRecord::ResourceRecord(const uint8_t* buffer, size_t& offset) {
    name_ = parse_name(buffer, offset);
    type_ = read_u16(buffer, offset);
    class_ = read_u16(buffer, offset);
    ttl_ = read_u32(buffer, offset);
    rdlength_ = read_u16(buffer, offset);
    rdata_ = parse_data(buffer, offset);
}

uint8_t ResourceRecord::read_u8(const uint8_t* buffer, size_t& offset) {
    return buffer[offset++];
}

uint16_t ResourceRecord::read_u16(const uint8_t* buffer, size_t& offset) {
    uint16_t result = read_u8(buffer, offset);
    result <<= 8;
    result |= read_u8(buffer, offset);
    return result;
}

uint32_t ResourceRecord::read_u32(const uint8_t* buffer, size_t& offset) {
    uint32_t result = read_u16(buffer, offset);
    result <<= 16;
    result |= read_u16(buffer, offset);
    return result;
}

std::string ResourceRecord::parse_name(const uint8_t* buffer, size_t& offset, bool is_pointer) {
    std::string name;
    while (true) {
        const uint8_t label_length = read_u8(buffer, offset);

        if (label_length == END_OF_LABEL) {
            if (name.back() == '.') {
                name.pop_back();
            }
            return name;
        }

        if (label_length >= POINTER_TO_LABEL) {
            if (is_pointer) {
                throw "Nested pointer detected";
            }
            size_t pointer_loc = (size_t) read_u8(buffer, offset);
            name += parse_name(buffer, pointer_loc, true);
            return name;
        }

        for (int i=0; i<label_length; ++i) {
            name += (char) read_u8(buffer, offset);
        }
        name += '.';

    }
}

std::vector<uint8_t> ResourceRecord::parse_data(const uint8_t* buffer, size_t& offset) {
    std::vector<uint8_t> data;
    for (int i=0; i<rdlength_; ++i) {
        data.push_back(read_u8(buffer, offset));
    }
    return data;
}

std::ostream& operator<< (std::ostream& out, const ResourceRecord& rr) {
    out << "******************\n";
    out << "RR\n";
    out << rr.name_ << '\n';
    out << rr.type_ << '\n';
    out << rr.class_ << '\n';
    out << rr.ttl_ << '\n';
    out << rr.rdlength_ << '\n';
    out << '[';
    for (int i=0; i<rr.rdlength_; ++i) {
        out << (int) rr.rdata_[i];
        if (i < rr.rdlength_-1) {
            out << ", ";
        }
    }
    out << "]\n";
    out << "******************\n";
    return out;
}

int main() {
    uint8_t test_buffer[] = {
        // Offset 0x00: "example.com"
        0x07, 0x65, 0x78, 0x61, 0x6D, 0x70, 0x6C, 0x65,
        0x03, 0x63, 0x6F, 0x6D,
        0x00,

        // Offset 0x0D: "www" + pointer to "example.com" (0x00)
        0x03, 0x77, 0x77, 0x77,
        0xC0, 0x00,

        // Offset 0x13: TYPE A
        0x00, 0x01,

        // CLASS IN
        0x00, 0x01,

        // TTL = 60
        0x00, 0x00, 0x00, 0x3C,

        // RDLENGTH = 4
        0x00, 0x04,

        // RDATA: 93.184.216.34
        0x5D, 0xB8, 0xD8, 0x22
    };



    size_t test_offset = 0x0D;
    ResourceRecord rr(test_buffer, test_offset);

    std::cout << rr;
}
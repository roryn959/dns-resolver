
#include "ResourceRecord.h"
#include "read_utils.h"

ResourceRecord::ResourceRecord(const uint8_t* buffer, size_t& offset) {
    name_ = parse_name(buffer, offset);
    type_ = read_u16(buffer, offset);
    class_ = read_u16(buffer, offset);
    ttl_ = read_u32(buffer, offset);
    rdlength_ = read_u16(buffer, offset);
    rdata_ = parse_data(buffer, offset, rdlength_);
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

#include <packet/ResourceRecord.h>


std::string to_string(ResourceRecord::Type type) {
    switch (type) {
        case ResourceRecord::Type::A: return "A";
        case ResourceRecord::Type::NS: return "NS";
        case ResourceRecord::Type::MD: return "MD";
        case ResourceRecord::Type::MF: return "MF";
        case ResourceRecord::Type::CNAME: return "CNAME";
        case ResourceRecord::Type::SOA: return "SOA";
        case ResourceRecord::Type::MB: return "MB";
        case ResourceRecord::Type::MG: return "MG";
        case ResourceRecord::Type::MR: return "MR";
        case ResourceRecord::Type::NULL_: return "NULL";
        case ResourceRecord::Type::WKS: return "WKS";
        case ResourceRecord::Type::PTR: return "PTR";
        case ResourceRecord::Type::HINFO: return "HINFO";
        case ResourceRecord::Type::MINFO: return "MINFO";
        case ResourceRecord::Type::MX: return "MX";
        case ResourceRecord::Type::TXT: return "TXT";
        default: return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& out, const ResourceRecord::Type& type) {
    out << to_string(type);
    return out;
}

std::string to_string(ResourceRecord::Class class_) {
    switch (class_) {
        case ResourceRecord::Class::IN: return "IN";
        case ResourceRecord::Class::CS: return "CS";
        case ResourceRecord::Class::CH: return "CH";
        case ResourceRecord::Class::HS: return "HS";
        default: return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& out, const ResourceRecord::Class& class_) {
    out << to_string(class_);
    return out;
}

ResourceRecord::ResourceRecord(const uint8_t* buffer, size_t& offset) {
    name_ = read_name(buffer, offset);
    type_ = (Type) read_u16(buffer, offset);
    class_ = (Class) read_u16(buffer, offset);
    ttl_ = read_u32(buffer, offset);
    rdlength_ = read_u16(buffer, offset);
    rdata_ = read_data(buffer, offset, rdlength_);
}

size_t ResourceRecord::serialise(uint8_t* const buffer, size_t& offset) const {
    size_t bytes = 0;
    bytes += write_name(buffer, offset, name_);
    write_u16(buffer, offset, (uint16_t) type_);
    write_u16(buffer, offset, (uint16_t) class_);
    write_u32(buffer, offset, ttl_);
    write_u16(buffer, offset, rdlength_);
    bytes += 10;
    write_data(buffer, offset, rdata_);
    bytes += rdata_.size();
    return bytes;
}

const std::string& ResourceRecord::get_name() const {
    return name_;
}

ResourceRecord::Type ResourceRecord::get_type() const {
    return type_;
}

ResourceRecord::Class ResourceRecord::get_class() const {
    return class_;
}

uint32_t ResourceRecord::get_ttl() const {
    return ttl_;
}

uint16_t ResourceRecord::get_rdlength() const {
    return rdlength_;
}

const std::vector<uint8_t>& ResourceRecord::get_rdata() const {
    return rdata_;
}

std::ostream& operator<< (std::ostream& out, const ResourceRecord& rr) {
    out << rr.name_ << "\t\t";
    out << rr.ttl_ << "\t";
    out << rr.type_ << "\t";
    out << rr.class_ << "\t";
    for (uint8_t elem : rr.rdata_) out << (int) elem << "-";
    out << '\n';

    return out;
}

namespace RR_Test {
    void test() {
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

        uint8_t* temp_buffer = new uint8_t[512];
        size_t offset = 0;
        rr.serialise(temp_buffer, offset);

        offset = 0;
        ResourceRecord temp_rr(temp_buffer, offset);
        std::cout << temp_rr;
    }
}
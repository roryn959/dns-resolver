#ifndef RESOURCE_RECORD_H
#define RESOURCE_RECORD_H

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

class ResourceRecord {
public:
    std::string name_;
    uint16_t type_;
    uint16_t class_;
    uint32_t ttl_;
    uint16_t rdlength_;
    std::vector<uint8_t> rdata_;

    ResourceRecord(const uint8_t* buffer, size_t& offset);
    friend std::ostream& operator<< (std::ostream&, const ResourceRecord&);

private:
    std::string parse_name(const uint8_t* buffer, size_t& offset, bool is_pointer=false);
    std::vector<uint8_t> parse_data(const uint8_t* buffer, size_t& offset);
};

#endif
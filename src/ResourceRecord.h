#ifndef RESOURCE_RECORD_H
#define RESOURCE_RECORD_H

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

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

enum class RR_Class {
    IN = 1,
    CS,
    CH,
    HS
};

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
};

#endif
#ifndef RESOURCE_RECORD_H
#define RESOURCE_RECORD_H

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

class ResourceRecord {
public:
    enum class Type : uint16_t {
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

    enum class Class : uint16_t {
        IN = 1,
        CS,
        CH,
        HS
    };

    ResourceRecord(const uint8_t* buffer, size_t& offset);

    friend std::ostream& operator<< (std::ostream&, const ResourceRecord&);

    std::string name_;
    Type type_;
    Class class_;
    uint32_t ttl_;
    uint16_t rdlength_;
    std::vector<uint8_t> rdata_;

};

#endif
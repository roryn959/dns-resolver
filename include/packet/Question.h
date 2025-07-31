#ifndef QUESTION_H
#define QUESTION_H

#include "bit_utils.h"

#include <cstdint>
#include <string>
#include <iostream>

class Question {
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

    Question() = default;
    Question(const uint8_t* buffer, size_t& offset);

    void serialise(uint8_t* const buffer, size_t& offset) const;

    const std::string& get_name() const;
    Type get_type() const;
    Class get_class() const;

    friend std::ostream& operator<< (std::ostream&, const Question&);

private:
    std::string name_;
    Type type_;
    Class class_;
};

#endif
#include <packet/Question.h>

std::string to_string(Question::Type type) {
    switch (type) {
        case Question::Type::A: return "A";
        case Question::Type::NS: return "NS";
        case Question::Type::MD: return "MD";
        case Question::Type::MF: return "MF";
        case Question::Type::CNAME: return "CNAME";
        case Question::Type::SOA: return "SOA";
        case Question::Type::MB: return "MB";
        case Question::Type::MG: return "MG";
        case Question::Type::MR: return "MR";
        case Question::Type::NULL_: return "NULL";
        case Question::Type::WKS: return "WKS";
        case Question::Type::PTR: return "PTR";
        case Question::Type::HINFO: return "HINFO";
        case Question::Type::MINFO: return "MINFO";
        case Question::Type::MX: return "MX";
        case Question::Type::TXT: return "TXT";
        default: return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& out, const Question::Type& type) {
    out << to_string(type);
    return out;
}

std::string to_string(Question::Class class_) {
    switch (class_) {
        case Question::Class::IN: return "IN";
        case Question::Class::CS: return "CS";
        case Question::Class::CH: return "CH";
        case Question::Class::HS: return "HS";
        default: return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& out, const Question::Class& class_) {
    out << to_string(class_);
    return out;
}

Question::Question(const uint8_t* buffer, size_t& offset) {
    name_ = read_name(buffer, offset);
    type_ = (Type) read_u16(buffer, offset);
    class_ = (Class) read_u16(buffer, offset);
}

const std::string& Question::get_name() const {
    return name_;
}

Question::Type Question::get_type() const {
    return type_;
}

Question::Class Question::get_class() const {
    return class_;
}

std::ostream& operator<< (std::ostream& out, const Question& q) {
    out << "Question\n";
    out << "QNAME: " << q.name_ << '\n';
    out << "TYPE: " << q.type_ << '\n';
    out << "CLASS: " << q.class_ << '\n';
    out << "******************\n";
    return out;
}

namespace Question_Test {
    void test() {
        uint8_t test_buffer[] = {
            0x03, 0x77, 0x77, 0x77,             // "www"
            0x07, 0x65, 0x78, 0x61, 0x6D, 0x70, 0x6C, 0x65, // "example"
            0x03, 0x63, 0x6F, 0x6D,             // "com"
            0x00,                               // end of name
            0x00, 0x01,                         // QTYPE = A
            0x00, 0x01                          // QCLASS = IN
        };

        size_t test_offset = 0x00;
        Question q(test_buffer, test_offset);

        std::cout << q;
    }
}
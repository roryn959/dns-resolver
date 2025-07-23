#include <packet/Question.h>
#include "read_utils.h"

Question::Question(const uint8_t* buffer, size_t& offset) {
    name_ = parse_name(buffer, offset);
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
    out << "TYPE: " << (uint16_t) q.type_ << '\n';
    out << "CLASS: " << (uint16_t) q.class_ << '\n';
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
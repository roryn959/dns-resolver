#include <packet/Message.h>

Message::Message(const uint8_t *buffer, size_t& offset)
    : header_(buffer, offset)
{
    for (int i=0; i<header_.get_qdcount(); ++i) {
        questions_.push_back(Question(buffer, offset));
    }

    for (int i=0; i<header_.get_ancount(); ++i) {
        answers_.push_back(ResourceRecord(buffer, offset));
    }

    for (int i=0; i<header_.get_nscount(); ++i) {
        authorities_.push_back(ResourceRecord(buffer, offset));
    }

    for (int i=0; i<header_.get_arcount(); ++i) {
        additionals_.push_back(ResourceRecord(buffer, offset));
    }
}

const Header& Message::get_header() const {
    return header_;
}

const std::vector<Question>& Message::get_questions() const {
    return questions_;
}

const std::vector<ResourceRecord>& Message::get_answers() const {
    return answers_;
}

const std::vector<ResourceRecord>& Message::get_authorities() const {
    return authorities_;
}

const std::vector<ResourceRecord>& Message::get_additionals() const {
    return additionals_;
}

std::ostream& operator<<(std::ostream& out, Message& msg) {

    out << "------ DNS Message ------\n";
    out << "--- HEADER ---\n";
    out << msg.get_header();
    for (auto& q : msg.get_questions()) {
        out << q;
    }
    out << "--- ANSWERS ---\n";
    for (auto& r : msg.get_answers()) {
        out << r;
    }
    out << "--- AUTHORITIES ---\n";
    for (auto& r : msg.get_authorities()) {
        out << r;
    }
    out << "--- ADDITIONALS ---\n";
    for (auto& r : msg.get_additionals()) {
        out << r;
    }

    return out;
}

int main() {
    [[maybe_unused]] uint8_t test_query_buffer[] = {
        // ----- Header -----
        0x12, 0x34,             // ID = 0x1234
        0x01, 0x00,             // Flags = 0x0100 (standard query, RD=1)
        0x00, 0x01,             // QDCOUNT = 1
        0x00, 0x00,             // ANCOUNT = 0
        0x00, 0x00,             // NSCOUNT = 0
        0x00, 0x00,             // ARCOUNT = 0

        // ----- Question Section -----
        0x07, 'e', 'x', 'a', 'm', 'p', 'l', 'e', // "example"
        0x03, 'c', 'o', 'm',                     // "com"
        0x00,                                   // End of domain (root)
        0x00, 0x01,                             // QTYPE = 1 (A record)
        0x00, 0x01                              // QCLASS = 1 (IN)
    };

    [[maybe_unused]] uint8_t test_response_buffer[] = {
        // ----- Header (12 bytes) -----
        0x12, 0x34,       // ID = 0x1234
        0x81, 0x80,       // FLAGS = 1000 0001 1000 0000 (QR=1, RD=1, RA=1)
        0x00, 0x01,       // QDCOUNT = 1
        0x00, 0x01,       // ANCOUNT = 1
        0x00, 0x00,       // NSCOUNT = 0
        0x00, 0x00,       // ARCOUNT = 0

        // ----- Question Section -----
        0x07, 'e', 'x', 'a', 'm', 'p', 'l', 'e',
        0x03, 'c', 'o', 'm',
        0x00,             // End of domain name
        0x00, 0x01,       // QTYPE = 1 (A)
        0x00, 0x01,       // QCLASS = 1 (IN)

        // ----- Answer Section -----
        0xC0, 0x0C,       // Name = pointer to offset 12 (example.com)
        0x00, 0x01,       // TYPE = 1 (A)
        0x00, 0x01,       // CLASS = 1 (IN)
        0x00, 0x00, 0x00, 0x3C, // TTL = 60 seconds
        0x00, 0x04,       // RDLENGTH = 4
        0x5D, 0xB8, 0xD8, 0x22  // RDATA = 93.184.216.34
    };

    size_t test_offset = 0x00;
    Message message(test_response_buffer, test_offset);

    std::cout << message;
}
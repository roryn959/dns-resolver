#ifndef MESSAGE_H
#define MESSAGE_H

#include "Header.h"
#include "Question.h"
#include "ResourceRecord.h"

#include <cstdint>
#include <vector>


class Message {
public:
    Message(const uint8_t* buffer, size_t offset);

    const Header& get_header() const;
    const std::vector<Question>& get_questions() const;
    const std::vector<ResourceRecord>& get_answers() const;
    const std::vector<ResourceRecord>& get_authorities() const;
    const std::vector<ResourceRecord>& get_additionals() const;

    bool contains_answer() const;

    friend std::ostream& operator<< (std::ostream&, const Message&);

private:
    Header header_;
    std::vector<Question> questions_;
    std::vector<ResourceRecord> answers_;
    std::vector<ResourceRecord> authorities_;
    std::vector<ResourceRecord> additionals_;

};

namespace Message_Test {
    void test();
}

#endif
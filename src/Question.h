#ifndef QUESTION_H
#define QUESTION_H

#include <cstdint>
#include <string>
#include <iostream>

class Question {
public:
    std::string name_;
    uint16_t type_;
    uint16_t class_;

    Question(uint8_t* buffer, size_t& offset);
    friend std::ostream& operator<< (std::ostream&, const Question&);
};

#endif
#include "bit_utils.h"

constexpr uint8_t END_OF_LABEL = 0x00;
constexpr uint8_t POINTER_TO_LABEL = 0xC0;

uint8_t read_u8(const uint8_t* buffer, size_t& offset) {
    return buffer[offset++];
}

uint16_t read_u16(const uint8_t* buffer, size_t& offset) {
    uint16_t result = read_u8(buffer, offset);
    result <<= 8;
    result |= read_u8(buffer, offset);
    return result;
}

uint32_t read_u32(const uint8_t* buffer, size_t& offset) {
    uint32_t result = read_u16(buffer, offset);
    result <<= 16;
    result |= read_u16(buffer, offset);
    return result;
}

std::string parse_name(const uint8_t* buffer, size_t& offset, bool is_pointer) {
    std::string name;
    while (true) {
        const uint8_t label_length = read_u8(buffer, offset);

        if (label_length == END_OF_LABEL) {
            if (name.back() == '.') name.pop_back();
            return name;
        }

        if (label_length >= POINTER_TO_LABEL) {
            if (is_pointer) {
                throw "Nested pointer detected";
            }
            size_t pointer_loc = (size_t) read_u8(buffer, offset);
            name += parse_name(buffer, pointer_loc, true);
            return name;
        }

        for (int i=0; i<label_length; ++i) {
            name += (char) read_u8(buffer, offset);
        }
        name += '.';

    }
}

std::vector<uint8_t> parse_data(const uint8_t* buffer, size_t& offset, uint16_t amount) {
    std::vector<uint8_t> data;
    for (int i=0; i<amount; ++i) {
        data.push_back(read_u8(buffer, offset));
    }
    return data;
}

void set_bit(uint16_t& bitstring, uint16_t bitmask, bool enabled) {
    enabled ? (bitstring |= bitmask) : (bitstring &= ~bitmask);
}
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

std::string read_name(const uint8_t* buffer, size_t& offset, bool is_pointer) {
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
            name += read_name(buffer, pointer_loc, true);
            return name;
        }

        for (int i=0; i<label_length; ++i) {
            name += (char) read_u8(buffer, offset);
        }
        name += '.';

    }
}

std::vector<uint8_t> read_data(const uint8_t* buffer, size_t& offset, uint16_t amount) {
    std::vector<uint8_t> data;
    for (int i=0; i<amount; ++i) {
        data.push_back(read_u8(buffer, offset));
    }
    return data;
}

void write_u8(uint8_t* const buffer, size_t& offset, uint8_t value) {
    buffer[offset++] = value;
}

void write_u16(uint8_t* const buffer, size_t& offset, uint16_t value) {
    uint8_t left = (uint8_t) ((value & 0xFF00) >> 8);
    uint8_t right = (uint8_t) (value & 0x00FF);
    write_u8(buffer, offset, left);
    write_u8(buffer, offset, right);
}

void write_u32(uint8_t* const buffer, size_t& offset, uint32_t value) {
    uint16_t left = (uint16_t) ((value & 0xFFFF0000) >> 16);
    uint16_t right = (uint16_t) (value & 0x0000FFFF);
    write_u16(buffer, offset, left);
    write_u16(buffer, offset, right);
}

void write_name(uint8_t* const buffer, size_t& offset, std::string name) {
    std::queue<uint8_t> queue;

    for (char c : name) {
        if (c == '.') {
            write_u8(buffer, offset, (uint8_t) queue.size());
            while (!queue.empty()) {
                write_u8(buffer, offset, queue.front());
                queue.pop();
            }
        } else {
            queue.push((uint8_t) c);
        }
    }

    write_u8(buffer, offset, (uint8_t) queue.size());
    while (!queue.empty()) {
        write_u8(buffer, offset, queue.front());
        queue.pop();
    }

    write_u8(buffer, offset, END_OF_LABEL);
}

void write_data(uint8_t* const buffer, size_t& offset, std::vector<uint8_t> data) {
    for (uint8_t datum : data) {
        write_u8(buffer, offset, datum);
    }
}

void set_bit(uint16_t& bitstring, uint16_t bitmask, bool enabled) {
    enabled ? (bitstring |= bitmask) : (bitstring &= ~bitmask);
}

uint32_t ip_from_bytes(std::vector<uint8_t>& bytes) {
    assert(bytes.size()==4);

    uint32_t res = 0;
    res |= ( (uint32_t) bytes[0] ) << 24;
    res |= ( (uint32_t) bytes[1] ) << 16;
    res |= ( (uint32_t) bytes[2] ) << 8;
    res |= ( (uint32_t) bytes[3] );
    return htonl(res);
}
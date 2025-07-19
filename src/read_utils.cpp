#include "read_utils.h"

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
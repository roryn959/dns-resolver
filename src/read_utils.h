#ifndef READ_UTILS_H
#define READ_UTILS_H

#include <cstdint>
#include <vector>
#include <string>

uint8_t read_u8(const uint8_t* buffer, size_t& offset);
uint16_t read_u16(const uint8_t* buffer, size_t& offset);
uint32_t read_u32(const uint8_t* buffer, size_t& offset);


#endif
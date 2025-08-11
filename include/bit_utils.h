#ifndef READ_UTILS_H
#define READ_UTILS_H

#include <assert.h>

#include <cstdint>
#include <vector>
#include <string>
#include <queue>

uint8_t read_u8(const uint8_t* const buffer, size_t& offset);
uint16_t read_u16(const uint8_t* const buffer, size_t& offset);
uint32_t read_u32(const uint8_t* const buffer, size_t& offset);
std::string read_name(const uint8_t* const buffer, size_t& offset, bool is_pointer=false);
std::vector<uint8_t> read_data(const uint8_t* const buffer, size_t& offset, uint16_t amount);

void write_u8(uint8_t* const buffer, size_t& offset, uint8_t value);
void write_u16(uint8_t* const buffer, size_t& offset, uint16_t value);
void write_u32(uint8_t* const buffer, size_t& offset, uint32_t value);
void write_name(uint8_t* const buffer, size_t& offset, std::string name);
void write_data(uint8_t* const buffer, size_t&, std::vector<uint8_t> data);

void set_bit(uint16_t& bitstring, uint16_t bitmask, bool enabled);

uint32_t ip_from_bytes(std::vector<uint8_t>& bytes);

#endif
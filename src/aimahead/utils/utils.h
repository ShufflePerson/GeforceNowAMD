#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> 
#include <cstdint>
#include <iomanip>
#include <bitset>

namespace utils {
	std::string uintptr_to_hex_string(uintptr_t ptr);
	std::string get_true_bits(uint32_t value);
	std::string get_error_code(HRESULT result);
	void alloc_console();
	void log(std::string str);
	std::string memory_to_hex_str(void* memory, size_t amount);
	std::string memory_to_uint16_str(void* memory, size_t amount);
}
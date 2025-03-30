#pragma once
#include <Windows.h>
#include <string>
#include <unordered_map>

#include "../utils/utils.h"

namespace memory {
	static std::unordered_map<std::string, HMODULE> module_cache;

	HMODULE get_module_from_name(const char* name);
	HMODULE get_base();
}
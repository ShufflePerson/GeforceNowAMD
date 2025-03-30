#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <string_view> 

#include "./utils/utils.h"
#include "./memory/memory.h"
#include "./hooks/hooks.h"

namespace aimahead {
	static HMODULE g_hModule;
	void initialize(HMODULE hModule);
	void unload();

}
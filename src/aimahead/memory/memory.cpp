#include "memory.h"


HMODULE memory::get_module_from_name(const char* name) {
    if (name == nullptr || name[0] == '\0') {
        return nullptr; 
    }

    std::string module_name = name; 

    auto it = module_cache.find(module_name);
    if (it != module_cache.end()) {
        return it->second;
    }
    else {
        HMODULE module_handle = GetModuleHandleA(name);
        while (!module_handle) module_handle = GetModuleHandleA(name);;
        module_cache[module_name] = module_handle;
        utils::log(module_name + " -> [" + utils::uintptr_to_hex_string(reinterpret_cast<uintptr_t>(module_handle)) + "]\n");
        return module_handle;
    }
}



HMODULE memory::get_base() {
    HMODULE module_handle = GetModuleHandleA(NULL);
    utils::log("Base -> [" + utils::uintptr_to_hex_string(reinterpret_cast<uintptr_t>(module_handle)) + "]\n");
    return module_handle;
}

#include "../hooks.h"

void* hooks::allocate_page_near_address(void* target_address)
{
    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    const uint64_t PAGE_SIZE = sys_info.dwPageSize;

    uint64_t start_addr = (uint64_t(target_address) & ~(PAGE_SIZE - 1));
    const uint64_t SEARCH_RANGE = 0x1FFFFFFFF; 

    uint64_t min_addr = min(start_addr - SEARCH_RANGE, (uint64_t)sys_info.lpMinimumApplicationAddress);
    uint64_t max_addr = max(start_addr + SEARCH_RANGE, (uint64_t)sys_info.lpMaximumApplicationAddress);

    uint64_t start_page = start_addr; 

    uint64_t page_offset = 1;
    while (1)
    {
        uint64_t byte_offset = page_offset * PAGE_SIZE;
        uint64_t high_addr = start_page + byte_offset;
        uint64_t low_addr = (start_page > byte_offset) ? start_page - byte_offset : (uint64_t)sys_info.lpMinimumApplicationAddress;
        low_addr = max(low_addr, (uint64_t)sys_info.lpMinimumApplicationAddress); 

        bool needs_exit = high_addr > max_addr && low_addr < min_addr;

        if (high_addr < max_addr)
        {
            void* out_addr = VirtualAlloc((void*)high_addr, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (out_addr)
                return out_addr;
        }

        if (low_addr > min_addr)
        {
            void* out_addr = VirtualAlloc((void*)low_addr, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (out_addr != nullptr)
                return out_addr;
        }

        page_offset++;

        if (needs_exit)
        {
            break;
        }
    }

    return nullptr;
}


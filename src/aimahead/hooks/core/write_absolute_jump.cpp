#include "../hooks.h"


void hooks::write_absolute_jump(void* abs_jump_memory, void* addr_to_jump_to, int bytes_to_null) {
    int i_size = 13;
    uint8_t absJumpInstructions[32] =
    {
      0x49, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //mov r10, addr
      0x41, 0xFF, 0xE2 //jmp r10
    };

    uint64_t addrToJumpTo64 = (uint64_t)addr_to_jump_to;
    memcpy(&absJumpInstructions[2], &addrToJumpTo64, sizeof(addrToJumpTo64));
    memcpy(abs_jump_memory, absJumpInstructions, i_size);


    if (bytes_to_null > 0) {
        uint8_t nop_instruction = 0x90;
        uint8_t* nop_start_address = static_cast<uint8_t*>(abs_jump_memory) + i_size;

        for (int i = 0; i < bytes_to_null; ++i) {
            nop_start_address[i] = nop_instruction;
        }
    }
}
#include "../hooks.h"

namespace hooks {
    IHook install_hook(void* their_function, void* our_function, uint8_t bytesToSave) {
        IHook _m_hook; 
        IHook* m_hook = &_m_hook;
            
        if (their_function == nullptr || our_function == nullptr) {
            utils::log("[hooks::install_hook] ERROR: Nullptr given.\n");
            return _m_hook;
        }
        m_hook->original_bytes.resize(bytesToSave);
        memcpy(m_hook->original_bytes.data(), their_function, bytesToSave);
        m_hook->start_address = their_function;

        m_hook->original_function_tramp = allocate_page_near_address(their_function);
        if (m_hook->original_function_tramp == nullptr) {
            std::cerr << "Error: Failed to allocate memory for trampoline." << std::endl;
            return _m_hook;
        }

        uint8_t* trampolinePtr = reinterpret_cast<uint8_t*>(m_hook->original_function_tramp);
        size_t offset = 0;

        memcpy(trampolinePtr, m_hook->original_bytes.data(), m_hook->original_bytes.size());
        offset += m_hook->original_bytes.size();

        void* originalFunctionReturnAddress = reinterpret_cast<uint8_t*>(their_function) + bytesToSave;
        write_absolute_jump(trampolinePtr + offset, originalFunctionReturnAddress);


        m_hook->original_func_ptr = m_hook->original_function_tramp;

        DWORD oldProtect;
        if (!VirtualProtect(their_function, bytesToSave, PAGE_EXECUTE_READWRITE, &oldProtect)) {
            VirtualFree(m_hook->original_function_tramp, 0, MEM_RELEASE);
            return _m_hook;
        }

        write_absolute_jump(their_function, our_function, bytesToSave - 13);

        VirtualProtect(their_function, bytesToSave, oldProtect, &oldProtect);


        return _m_hook;
    }
}


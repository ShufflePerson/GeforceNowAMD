#include "../hooks.h"


void hooks::uninstall_hook(hooks::IHook& hook) {
    if (hook.start_address == nullptr) {
        utils::log("[hooks::uninstall_hook] ERROR: Hook address is null, nothing to uninstall.\n");
        return;
    }

    if (hook.original_bytes.empty()) {
        utils::log("[hooks::uninstall_hook] ERROR: No original bytes saved, cannot uninstall properly.\n");
        return; 
    }

    DWORD oldProtect;
    if (!VirtualProtect(hook.start_address, hook.original_bytes.size(), PAGE_EXECUTE_READWRITE, &oldProtect)) {
        utils::log("[hooks::uninstall_hook] ERROR: VirtualProtect failed to set RW during uninstall.\n");
        return; 
    }

    memcpy(hook.start_address, hook.original_bytes.data(), hook.original_bytes.size());

    DWORD restoredProtect; 
    if (!VirtualProtect(hook.start_address, hook.original_bytes.size(), oldProtect, &restoredProtect)) {
        utils::log("[hooks::uninstall_hook] ERROR: VirtualProtect failed to restore original protection during uninstall.\n");
    }

    if (hook.original_function_tramp != nullptr) {
        if (!VirtualFree(hook.original_function_tramp, 0, MEM_RELEASE)) {
            utils::log("[hooks::uninstall_hook] ERROR: VirtualFree failed for trampoline memory.\n");
        }
    }

    hook.start_address = nullptr;
    hook.original_function_tramp = nullptr;
    hook.original_bytes.clear();
    hook.original_func_ptr = nullptr; 

    utils::log("[hooks::uninstall_hook] Hook uninstalled successfully.\n");
}
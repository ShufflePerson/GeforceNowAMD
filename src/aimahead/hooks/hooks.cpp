#include "hooks.h"
#define HOOKS_LOG_PREFIX "[hooks::init_hooks] "


static std::vector<NVB::CBifrostClient*> g_vClients;
void hooks::set_client(NVB::CBifrostClient* client) {
	utils::log("[hooks::set_client] Accessing BifrostClient client\n");

	utils::log("[hooks::set_client] Client is at 0x" + utils::uintptr_to_hex_string(reinterpret_cast<uintptr_t>(client)) + "\nFunction list:\n");
	utils::log("\tm_cNetwork -> 0x" + utils::uintptr_to_hex_string(reinterpret_cast<uintptr_t>(client->m_cNetwork)) + "\n");
	utils::log("\tm_cClientCallbacks -> 0x" + utils::uintptr_to_hex_string(reinterpret_cast<uintptr_t>(client->m_cClientCallbacks)) + "\n");
	utils::log("\tm_cScheduler -> 0x" + utils::uintptr_to_hex_string(reinterpret_cast<uintptr_t>(client->m_cScheduler)) + "\n");
	utils::log("\tm_cGridServer -> 0x" + utils::uintptr_to_hex_string(reinterpret_cast<uintptr_t>(client->m_cGridServer)) + "\n");
	utils::log("\tm_bIsConnected: " + std::to_string(client->m_bIsConnected) + "\n\n");

	g_vClients.push_back(client);
}

NVB::CBifrostClient* hooks::get_client(bool b_failLog) {
	for (int i = 0; i < g_vClients.size(); i++) {
		if (g_vClients[i]->m_bIsConnected) {
			return g_vClients[i];
		}
	}

	if (b_failLog) {
		utils::log("[hooks::get_client] ERROR -> No connected clients were found. Returning a nullptr\n");
	}
	return nullptr;
 }

void hooks::init_hooks() {
	utils::log("[hooks::init_hooks] [*] Hooking!\n");
	uintptr_t base_address = reinterpret_cast<uintptr_t>(memory::get_base());

	uintptr_t geronimo_address = get_module_base_checked("Geronimo.dll");
	if (geronimo_address == 0x0) return;

	uintptr_t bifrost_address = get_module_base_checked("Bifrost2.dll");
	if (bifrost_address == 0x0) return;

	const char* debugOverlayExportName = "?shouldRenderDebugOverlay@SDLWindow@@AEBA_NXZ";
	const char* platformSupportsFPSExportName = "?platformSupportsFrameRate@@YA_NGW4GraphicsContextType@@W4DecoderType@@_N@Z";

	HINSTANCE Geronimo = LoadLibrary("Geronimo.dll");
	
	uintptr_t shouldRenderDebugOverlay_address = (uintptr_t)GetProcAddress(Geronimo, debugOverlayExportName);
	uintptr_t platformSupportsFpsAddress = (uintptr_t)GetProcAddress(Geronimo, platformSupportsFPSExportName);

	install_hook_and_log((void*)shouldRenderDebugOverlay_address, &hooks::shouldRenderDebugOverlay_callback, 16, "SDLWindow::shouldRenderDebugOverlay", set_shouldRenderDebugOverlayHook);
	install_hook_and_log((void*)platformSupportsFpsAddress, &hooks::onPlatformSupportsFps_callback, 15, "Geronimo::platformSupportsFrameRate", set_onPlatformSupportsFpsHook);

	utils::log("[hooks::init_hooks] [!] All hooks hooked!\n");
}

void hooks::unhook_all() {
	hooks::set_shouldRenderDebugOverlay(false);
	Sleep(1000);

	utils::log("[hooks::unhook_all] [*] Uninstalling SDLWindow::shouldRenderDebugOverlay hook\n");
	IHook m_shouldRenderDebugOverlayHook = get_shouldRenderDebugOverlayHook();
	uninstall_hook(m_shouldRenderDebugOverlayHook);

	utils::log("[hooks::unhook_all] [*] Uninstalling Geronimo::OnPaint hook\n");
	IHook m_onPlatformSupportsFpsHook = get_OnPlatformSupportsFpsHook();
	uninstall_hook(m_onPlatformSupportsFpsHook);


	utils::log("[hooks::unhook_all] [+] Uninstalled all hooks!\n");
}


uintptr_t hooks::get_module_base_checked(std::string module_name)
{
	uintptr_t module_address = reinterpret_cast<uintptr_t>(memory::get_module_from_name(module_name.c_str()));
	if (module_address == 0x0) {
		utils::log(HOOKS_LOG_PREFIX "[-] Failed to get " + module_name + " module!\n");
		return 0x0;
	}
	return module_address;
}

void hooks::install_hook_and_log(void* target_address, void* detour_function, int hook_id, std::string hook_name, std::function<void(IHook)> set_hook_func)
{
	IHook hook = hooks::install_hook(target_address, detour_function, hook_id);
	set_hook_func(hook); 
	utils::log(HOOKS_LOG_PREFIX "[+] Hooked " + hook_name + " (0x" + utils::uintptr_to_hex_string(reinterpret_cast<uintptr_t>(hook.original_function_tramp)) + ")\n");
}

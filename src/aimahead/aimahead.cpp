#include "aimahead.h"




void aimahead::initialize(HMODULE hModule)
{
	utils::alloc_console();
	g_hModule = hModule;
	
	hooks::init_hooks();

	utils::log("[GeforceNowAMD::initialize] All ready!\n");
	bool bDebugOverlayEnabled = false;
	while (true) {
		if (GetAsyncKeyState(VK_HOME) & 0x01) {
			hooks::set_shouldRenderDebugOverlay(bDebugOverlayEnabled);
			bDebugOverlayEnabled = !bDebugOverlayEnabled;
		}
		Sleep(1);
	}
	unload();
}

void aimahead::unload()
{
	utils::log("[GeforceNowAMD::unload] Closing...!\n");
	hooks::unhook_all();
	if (!FreeConsole()) {
		utils::log("[GeforceNowAMD::unload] [-] Failed to free the console.\n");
	}
	Sleep(500);
	FreeLibrary(g_hModule);
}

#include "../hooks.h"



hooks::IHook g_shouldProcessHook;
bool g_bShouldProcess = false;

typedef bool (*orgFuncType)(void* sdl_window);

bool __fastcall hooks::shouldRenderDebugOverlay_callback(void* sdl_window) {
	memset((void*)(reinterpret_cast<uintptr_t>(sdl_window) + 0x90), g_bShouldProcess, sizeof(bool));
	return g_bShouldProcess;
}
void hooks::set_shouldRenderDebugOverlayHook(IHook m_newHook) {
	g_shouldProcessHook = m_newHook;
}
hooks::IHook hooks::get_shouldRenderDebugOverlayHook() {
	return g_shouldProcessHook;
}

void hooks::set_shouldRenderDebugOverlay(bool val) {
	g_bShouldProcess = val;
}
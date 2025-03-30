#include "../hooks.h"



hooks::IHook g_onPaintHook;


struct CFrameContext {
	char padding[0x50];
	IDXGISwapChain* m_pSwapChain;
};

bool __fastcall hooks::onPlatformSupportsFps_callback(void* a1, void* a2, void* a3, void* a4) {
	return true;
}

void hooks::set_onPlatformSupportsFpsHook(IHook m_newHook) {
	g_onPaintHook = m_newHook;
}
hooks::IHook hooks::get_OnPlatformSupportsFpsHook() {
	return g_onPaintHook;
}
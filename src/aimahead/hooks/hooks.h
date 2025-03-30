#pragma once
#include <Windows.h>
#include <cstdint>
#include <functional>
#include <d3d11.h>
#include <dxgi.h>
#include <math.h>
#include <vector>
#include "../utils/utils.h"
#include "../memory/memory.h"
#include "../../SDK/offsets.h"
#include "../../SDK/SDK.h"


namespace hooks {
	void set_client(NVB::CBifrostClient* client);
	NVB::CBifrostClient* get_client(bool b_failLog = true);

	struct IHook {
		std::vector<uint8_t> original_bytes;
		void* original_function_tramp;
		void* original_func_ptr; 
		void* start_address;
	};

	IHook install_hook(void* their_function, void* our_function, uint8_t bytesToSave = 13);
	void uninstall_hook(IHook& hook);
	void write_absolute_jump(void* abs_jump_memory, void* addr_to_jump_to, int bytes_to_null = 0);
	void* allocate_page_near_address(void* target_address);

	uintptr_t get_module_base_checked(std::string module_name);
	void install_hook_and_log(void* target_address, void* detour_function, int hook_id, std::string hook_name, std::function<void(IHook)> set_hook_func);


	void init_hooks();
	void unhook_all();


	//Callbacks

	bool __fastcall shouldRenderDebugOverlay_callback(void* sdl_window);
	void set_shouldRenderDebugOverlayHook(IHook m_hook);
	IHook get_shouldRenderDebugOverlayHook();
	void set_shouldRenderDebugOverlay(bool m_bRenderDebug);

	bool __fastcall onPlatformSupportsFps_callback(void* a1, void* a2, void* a3, void* a4);
	void set_onPlatformSupportsFpsHook(IHook m_hook);
	IHook get_OnPlatformSupportsFpsHook();

	void send_key_input(uint16_t m_iVkCode, bool down);
	void send_absolute_mouse_input(int16_t x, int16_t y);
	void send_click_mouse_input(bool down, uint16_t key = 1);

	void send_pixel_mouse_input(int16_t m_iXDirection, int16_t m_iYDirection);
}
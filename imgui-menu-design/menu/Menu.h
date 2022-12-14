#pragma once

#pragma comment (lib, "d3d9.lib")
#include <d3d9.h>
#include <d3dx9.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx9.h"  
#include "../imgui/imgui_impl_win32.h" 

namespace Menu
{
	namespace settings {
		inline bool isOpened = true;
		inline int ui_width = 780;
		inline int ui_height = 590;

		inline int ui_width_s = 0;
		inline int ui_height_s = 0;

		inline int ui_scalar = 100;


		inline int selected_page = 0;

		inline ImFont* fonts_GUIBIGFONT;
		inline ImFont* fonts_GUIBIGFONT2;

	};

	void runCustomGui(LPDIRECT3DDEVICE9, bool paramval = 0);
};
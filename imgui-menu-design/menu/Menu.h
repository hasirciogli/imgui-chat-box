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

		inline int uiSelectedScalarID = 4;
		inline float uiSelectedDPI = 1;
		inline const char* uiSelectedScalarName[10] = {"0", "25", "50", "75", "100", "125", "150", "175", "200", "250"};

		inline void updateMenuScalar(int selectedItem) {
			switch (selectedItem)
			{
			case 0:
				Menu::settings::ui_scalar = 0;
				break;
			case 1:
				Menu::settings::ui_scalar = 25;
				break;
			case 2:
				Menu::settings::ui_scalar = 50;
				break;
			case 3:
				Menu::settings::ui_scalar = 75;
				break;
			case 4:
				Menu::settings::ui_scalar = 100;
				break;
			case 5:
				Menu::settings::ui_scalar = 125;
				break;
			case 6:
				Menu::settings::ui_scalar = 150;
				break;
			case 7:
				Menu::settings::ui_scalar = 175;
				break;
			case 8:
				Menu::settings::ui_scalar = 200;
				break;
			case 9:
				Menu::settings::ui_scalar = 250;
				break;
			default:
				Menu::settings::ui_scalar = 100;
				break;
			}
		}


		namespace Aimbot {
			inline bool enabled = false;
			inline bool teamCheck = false;
			inline bool autofire = false;
			inline bool auto_revolver = false;
			inline bool auto_wall = false;

			inline float min_damage = 0.f;
			inline float aimbot_fov = 0.f;
			
			namespace hitboxes {
				inline bool head = false;

			}
		}

		namespace Esp {
			inline int espTeamState = 0;
			inline int selected_team = 0;
			inline bool test = false;

			inline bool enabledBase[] = { false, false, false };
			inline bool enabledEsp[] = {false, false, false};
			inline bool enabledBox[] = {false, false, false};
			inline bool enabledLine[] = {false, false, false};
			inline bool enabledGethers[] = {false, false, false};
		}





	};
	void runCustomGui(LPDIRECT3DDEVICE9, bool paramval = 0);
};
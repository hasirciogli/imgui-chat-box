#include "Menu.h"

#include <algorithm>
#include <iostream>
#include <string> 

void imspaceMacro(float x, float y) {
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + x, ImGui::GetCursorPos().y + y));
}
 
struct windowSize {
	float width;
	float height;
};

void GetWindowSize(windowSize& size, LPDIRECT3DDEVICE9 pDevice) {
	D3DDEVICE_CREATION_PARAMETERS cparams;
	RECT rect;

	pDevice->GetCreationParameters(&cparams);
	GetWindowRect(cparams.hFocusWindow, &rect);
	

	size.width = (float)rect.right - rect.left;
	size.height = (float)rect.bottom - rect.top;
}

int calculateUiScalar(int scalaDef) {
	if (false) 
	{
		if (Menu::settings::ui_scalar == 200)
			return scalaDef + ((scalaDef * 200) / 100);
		else if (Menu::settings::ui_scalar == 175)
			return 0000;
		else if (Menu::settings::ui_scalar == 150)
			return 0000;
		else if (Menu::settings::ui_scalar == 125)
			return 0000;
		else if (Menu::settings::ui_scalar == 100)
			return 0000;
		else if (Menu::settings::ui_scalar == 75)
			return 0000;
		else if (Menu::settings::ui_scalar == 50)
			return 0000;
		else if (Menu::settings::ui_scalar == 25)
			return 0000;
		else if (Menu::settings::ui_scalar == 0)
			return 0000;
		else
			return scalaDef;
	}

	return (scalaDef * Menu::settings::ui_scalar) / 100;
}

void customComboBox(int& selectedItemID, const char* items[10]) {
	if (ImGui::BeginCombo("##combo", *items)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (selectedItemID == *items[n]); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable((const char*)*items[n], is_selected))
			{
				selectedItemID = n;
				ImGui::SetItemDefaultFocus();
					
			}// You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

		}
		ImGui::EndCombo();
	}
}
bool Spinner(const char* label, float radius, int thickness, const ImU32& color) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	// Render
	window->DrawList->PathClear();

	int num_segments = 30;
	int start = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

	const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
	const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

	const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

	for (int i = 0; i < num_segments; i++) {
		const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
		window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
			centre.y + ImSin(a + g.Time * 8) * radius));
	}

	window->DrawList->PathStroke(color, false, thickness);
}


void Menu::runCustomGui(LPDIRECT3DDEVICE9 pDevice, bool param) {
	

	static float time = 0;
	static bool ad = false;

	float deltaTime = ImGui::GetIO().DeltaTime;
	float speed = 2000.f;

	static float alpha = 255.f;

	ImClamp(alpha, 0.f, 255.f);

	
	if (Menu::settings::isOpened)
	{
		if (alpha < 255)
		{
			alpha += speed * deltaTime;
		}
		else
			alpha = 255.f;
	}
	else
	{
		if (alpha > 0)
			alpha -= speed * deltaTime;
		else
			alpha = 0;
	}

	if (alpha > 255.0f)
	{
		alpha = 255.0f;
	}
	else if (alpha < 0.0f)
	{
		alpha = 0.0f;
	}

	ImGuiStyle& zzStyles = ImGui::GetStyle();

	zzStyles.Alpha = alpha / 255.f;

	if (alpha == 0)
		return;

	ImGui::Begin("Debug Window");
	{
		ImGui::SliderFloat("test", &Menu::settings::uiSelectedDPI, -10.f, 10.f);
		ImGui::Text(std::to_string(alpha).c_str());

	}
	ImGui::End();



	return;

	static bool loaded = false;


	static bool firstanim = true;
	int currenttime = (float)(clock() / 1000.f);
	static int startedtime = 0;
	static bool savetime = true;
	if (savetime) {
		startedtime = (float)(clock() / 1000.f);
		savetime = false;
	}

	if (currenttime < startedtime + (firstanim ? 6.f : 2.f)) {

		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGui::SetNextWindowSize(ImVec2(1920, 1080));

		windowSize wsz;

		GetWindowSize(wsz, pDevice);

		ImGui::Begin("Loader :)", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);

		imspaceMacro((wsz.width / 2) - 34, (wsz.height / 2) - 34);
		Spinner("#19954", 30, 4, ImColor(255, 255, 255, 255));

		ImGui::End();
	}

	//Erasing
	if (currenttime > startedtime + (firstanim ? 5.f : 1.f)) {
		loaded = true;
		firstanim = false;
	}

	if (!loaded)
		return;





	if (!Menu::settings::isOpened)
		return;

	Menu::settings::ui_width_s = calculateUiScalar(Menu::settings::ui_width);
	Menu::settings::ui_height_s = calculateUiScalar(Menu::settings::ui_height);

	static bool firstLoadInit = false;

	ImGuiStyle& imguiStyles = ImGui::GetStyle();



	//inline functions

	auto getMenuPos = [](ImVec2& pos) {
		pos = ImGui::GetWindowPos();
	};

	auto getCursorPos = [](ImVec2& pos) {
		pos = ImGui::GetCursorPos();
	};



	if (!firstLoadInit)
	{
		firstLoadInit = true;
		imguiStyles.WindowPadding = ImVec2(0, 0);
		imguiStyles.FramePadding = ImVec2(0, 0);
		imguiStyles.CellPadding = ImVec2(0, 0);
		imguiStyles.ItemSpacing = ImVec2(0, 0);
		imguiStyles.ItemInnerSpacing = ImVec2(10, 0);

		imguiStyles.Colors[ImGuiCol_WindowBg] = ImColor(20, 20, 20);

		windowSize wsz;

		GetWindowSize(wsz, pDevice);


		ImGui::SetNextWindowPos(ImVec2((wsz.width / 2) - (Menu::settings::ui_width_s / 2), (wsz.height / 2) - (Menu::settings::ui_height_s / 2)));
	}

	ImGui::SetNextWindowSize(ImVec2(Menu::settings::ui_width_s, Menu::settings::ui_height_s));

	
	ImGui::GetIO().FontGlobalScale = Menu::settings::uiSelectedDPI;


	imguiStyles.WindowRounding = 8;
	ImGui::Begin("NAME", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
	{
		imspaceMacro(5, 5);

		ImVec2 nPos = {};
		ImVec2 nPos2 = {};

		getMenuPos(nPos);
		getCursorPos(nPos2);


		ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + calculateUiScalar(230), nPos.y + nPos2.y), ImVec2(nPos.x + calculateUiScalar(230), nPos.y + calculateUiScalar(Menu::settings::ui_height_s - 5)), ImColor(150, 150, 150), 2);

		
		ImGui::BeginChild("#left-side", ImVec2(calculateUiScalar(220), calculateUiScalar(Menu::settings::ui_height_s - 10)), false);
		{

			ImGui::BeginChild("#left-upper", ImVec2(calculateUiScalar(220), calculateUiScalar(Menu::settings::ui_height_s - 75)), false);
			{
				ImGui::PushFont(Menu::settings::fonts_GUIBIGFONT);
				imspaceMacro(calculateUiScalar(220) / 2 - (ImGui::CalcTextSize("PC", nullptr).x / 2), 5);
				ImGui::Text("PC");
				ImGui::PopFont();


				//ImGui::Text(std::to_string(ImGui::GetIO().FontGlobalScale).c_str());



				ImGui::GetStyle().FrameRounding = 3;

				imguiStyles.Colors[ImGuiCol_Button] = ImColor(20, 20, 20);
				imguiStyles.Colors[ImGuiCol_ButtonHovered] = ImColor(35, 120, 30);
				imguiStyles.Colors[ImGuiCol_ButtonActive] = ImColor(35, 90, 30);


				imguiStyles.Colors[ImGuiCol_CheckMark] = ImColor(30, 220, 30);

				imguiStyles.Colors[ImGuiCol_FrameBg] = ImColor(40, 40, 40);
				imguiStyles.Colors[ImGuiCol_FrameBgHovered] = ImColor(50, 50, 50);
				imguiStyles.Colors[ImGuiCol_FrameBgActive] = ImColor(50, 110, 50);


				imguiStyles.Colors[ImGuiCol_SliderGrab] = ImColor(30, 220, 30);
				imguiStyles.Colors[ImGuiCol_SliderGrabActive] = ImColor(30, 220, 30);



				auto selButItem = [](std::string item_name, int page_id, bool isFirst = false) {
					imspaceMacro(5, 0);
					if (ImGui::Button(item_name.c_str(), ImVec2(calculateUiScalar(210), calculateUiScalar(35))))
						Menu::settings::selected_page = page_id;
				};

				selButItem("Aim Bot", 0, false);
				selButItem("Anti Aim", 1);
				 

				ImVec2 nPos = {};
				ImVec2 nPos2 = {};
				
				getMenuPos(nPos);
				getCursorPos(nPos2);

				ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + calculateUiScalar(40), nPos.y + nPos2.y + 9), ImVec2(nPos.x + calculateUiScalar(220 - 40), nPos.y + nPos2.y + 9), ImColor(150, 150, 150), 2);
				imspaceMacro(0, 20);

				selButItem("Esp", 2);
				selButItem("Chams", 3);
				selButItem("World", 4);

				getMenuPos(nPos);
				getCursorPos(nPos2);
				ImGui::GetWindowDrawList()->AddLine(ImVec2(nPos.x + calculateUiScalar(40), nPos.y + nPos2.y + 9), ImVec2(nPos.x + calculateUiScalar(220 - 40), nPos.y + nPos2.y + 9), ImColor(150, 150, 150), 2);
				imspaceMacro(0, 20);

				selButItem("Misc", 5);
				selButItem("Skins", 6);
				selButItem("Scripts", 7);
				selButItem("Settings", 8);

				ImGui::GetStyle().FrameRounding = 2;
			}
			ImGui::EndChild();

			ImGui::BeginChild("#left-bottomer", ImVec2(calculateUiScalar(220), calculateUiScalar(65)), false);
			{
				ImVec2 mPos = {};

				getMenuPos(mPos); 

				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(mPos.x + calculateUiScalar((220 / 3) / 2), mPos.y + calculateUiScalar(65 / 2)), 20, ImColor(140, 140, 140));
				imspaceMacro(calculateUiScalar((220 / 3) - 10), calculateUiScalar(65 / 2) - (ImGui::CalcTextSize("Hasirciogli", nullptr).y / 2));
				ImGui::Text("Hasirciogli");
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImVec2 cMenuSize = ImVec2(calculateUiScalar(Menu::settings::ui_width_s - 235 - 8), calculateUiScalar(Menu::settings::ui_height_s - 10));
		imspaceMacro(12, 0);
		ImGui::BeginChild("#right-side", cMenuSize);
		{
			
			imguiStyles.Colors[ImGuiCol_Button] = ImColor(40, 40, 40);

			ImGui::BeginChild("#state-upper", ImVec2(cMenuSize.x, 50), true); {

			}
			ImGui::EndChild();

			switch (Menu::settings::selected_page)
			{
			case 0:
				renderAimbotPage();
				break;
			case 1:
				renderAntiAimPage();
				break;
			case 2:
				renderEspPage();
				break;
			default:
				break;
			}

		}
		ImGui::EndChild();
	}
	ImGui::End();

	ImGui::GetIO().FontGlobalScale = 1;

	ImGui::Begin("Debug Window");
	{
		ImGui::SliderFloat("test", &Menu::settings::uiSelectedDPI, -10.f, 10.f);
		ImGui::Text(std::to_string(ImGui::GetIO().FontGlobalScale).c_str());
		//customComboBox(Menu::settings::uiSelectedScalarID, Menu::settings::uiSelectedScalarName);
		if (ImGui::Button("reset", ImVec2(150, 50)))
			Menu::settings::uiSelectedDPI = 1;

	}
	ImGui::End();
	
	Menu::settings::updateMenuScalar(Menu::settings::uiSelectedScalarID);

}

void Menu::runCustomGui2(bool param) {
}
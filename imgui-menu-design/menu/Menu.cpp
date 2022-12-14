#include "Menu.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <string.h>
#include <list>

using namespace ImGui;
using namespace std;

void setOurCustomImguiColorsAndEtc(LPDIRECT3DDEVICE9 pDevice);

auto getZtkColor = [](int red, int green, int blue, int alpha = 255)
{
	return ImVec4((float)red / 255.0f, (float)green / 255.0f, (float)blue / 255.0f, (float)alpha / 255.0f);
};


void imspaceMacro(float x, float y) {
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + x, ImGui::GetCursorPos().y + y));
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

struct ChatboxItem
{
	std::string name = "";
	std::string message = "";
	ImVec4 chatColor;
	ImVec4 nameColor;
};

char DDKmessage[200] = {};

std::list<ChatboxItem> chatboxItems;
string convertToString(char* a, int size)
{
	int i;
	string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

void renderChatBoxItem(ChatboxItem item, std::string cName)
{
	
	PushStyleVar(ImGuiStyleVar_ChildRounding, 6);
	{
		ImGui::BeginChild(cName.c_str(), ImVec2(430, 55), false);
		{
			imspaceMacro(0, 5);
			ImGui::BeginChild(string("#chatbox-item-title" + cName).c_str(), ImVec2(430, 20), false);
			{
				imspaceMacro(10,0);
				TextColored(item.nameColor, item.name.c_str());
			}
			ImGui::EndChild();

			imspaceMacro(0, 5);
			ImGui::BeginChild(string("#chatbox-item-content" + cName).c_str(), ImVec2(430, 20), false);
			{
				imspaceMacro(10, 0);
				TextColored(item.chatColor, item.message.c_str());
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();
		Separator();
	}
	PopStyleVar();
}


void Menu::runCustomGui(LPDIRECT3DDEVICE9 pDevice, bool param) {

	setOurCustomImguiColorsAndEtc(pDevice);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.02f, 0.02f, 0.02f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(1, 100, 50, 25));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(35.0f / 255.0f, 160.0f / 255.0f, 30.0f / 255.0f, 1.0f));



	ImGui::Begin("#name", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
	{
		ImGui::BeginChild("#title", ImVec2(450, 40), false);
		{
			std::string text = "Proximity Chat";
			ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
			imspaceMacro(10, 40 / 2 - textSize.y / 2);
			ImGui::TextColored(getZtkColor(20, 160, 20), text.c_str());
		}
		ImGui::EndChild();

		ImGui::Separator();

		ImGui::BeginChild("#chatbox", ImVec2(450, 500), false);
		{
			imspaceMacro(10, 10);
			ImGui::BeginChild("#chatbox", ImVec2(430, 480), true);
			{
				int bbEka = 0;
				for (auto item : chatboxItems)
				{
					renderChatBoxItem(item, std::string(std::string("#message_item_") + std::to_string(bbEka)));
					bbEka++;
				}
			}
			ImGui::EndChild();
		}
		ImGui::EndChild();

		ImGui::BeginChild("#bottom-tab", ImVec2(450, 60), false);
		{
			ImGuiStyle& style = GetStyle();
			imspaceMacro(10, 0);
			PushStyleVar(ImGuiStyleVar_ChildRounding, 3);
			{
				ImGui::BeginChild("#input-child", ImVec2(430 - 100 - 20, 40), true);
				{
					PushItemWidth(-1);
					PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 10));
					PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 10));
					{
						ImGui::InputText("#chatbox-inputtext", DDKmessage, IM_ARRAYSIZE(DDKmessage));
					}
					PopStyleVar();
					PopStyleVar();
					PopItemWidth();
				}
				ImGui::EndChild();
			}
			PopStyleVar();

			SameLine();
			imspaceMacro(10, 0);

			if (Button("Send", ImVec2(100, 40)))
			{
				ChatboxItem item = ChatboxItem();

				item.chatColor = getZtkColor(255, 255, 255);
				item.nameColor = getZtkColor(255, 150, 20);
				item.name = "Noxy"; 
				item.message = DDKmessage;

				chatboxItems.push_back(item);

				DDKmessage[0] = {};
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();



	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}


void setOurCustomImguiColorsAndEtc(LPDIRECT3DDEVICE9 pDevice)
{
	ImGuiStyle& imguiStyles = ImGui::GetStyle();

	static bool firstLoadInit = false;


	if (!firstLoadInit)
	{
		firstLoadInit = true;
		imguiStyles.WindowPadding = ImVec2(0, 0);
		imguiStyles.FramePadding = ImVec2(1.2f, 1.2f);
		imguiStyles.CellPadding = ImVec2(0, 0);
		imguiStyles.ItemSpacing = ImVec2(0, 0);
		imguiStyles.ItemInnerSpacing = ImVec2(10, 0);


		ImGui::SetNextWindowPos(ImVec2(200, 100));
		ImGui::SetNextWindowSize(ImVec2(450, 590));
	}



	imguiStyles.Colors[ImGuiCol_WindowBg] = getZtkColor(20, 20, 20);
	

	imguiStyles.Colors[ImGuiCol_Button] = getZtkColor(20, 20, 20, 0);
	imguiStyles.Colors[ImGuiCol_ButtonHovered] = getZtkColor(35, 120, 30);
	imguiStyles.Colors[ImGuiCol_ButtonActive] = getZtkColor(35, 90, 30);


	imguiStyles.Colors[ImGuiCol_CheckMark] = getZtkColor(30, 220, 30);

	imguiStyles.Colors[ImGuiCol_FrameBg] = getZtkColor(40, 40, 40);
	imguiStyles.Colors[ImGuiCol_FrameBgHovered] = getZtkColor(50, 50, 50);
	imguiStyles.Colors[ImGuiCol_FrameBgActive] = getZtkColor(50, 110, 50);



	imguiStyles.Colors[ImGuiCol_Header] = getZtkColor(50, 180, 50);
	imguiStyles.Colors[ImGuiCol_HeaderHovered] = getZtkColor(35, 120, 30);
	imguiStyles.Colors[ImGuiCol_HeaderActive] = getZtkColor(35, 90, 30);


	imguiStyles.Colors[ImGuiCol_SliderGrab] = getZtkColor(30, 220, 30);
	imguiStyles.Colors[ImGuiCol_SliderGrabActive] = getZtkColor(30, 220, 30);


	imguiStyles.Colors[ImGuiCol_Border] = getZtkColor(30, 220, 30);


}
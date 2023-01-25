#include "menu.h"
#include "imguipp_v2.h"

#define SET_ITEM_POSY(newY) ImGui::SetCursorPosY(newY)
#define SET_ITEM_POSX(newX) ImGui::SetCursorPosX(newX)

enum class GameState
{
	Undetected,
	Updating,
	Detected,
	Unknown
};

struct GameHandler
{
	std::string Name;
	std::string LicenseTime;
	GameState State;
	int Version;

	GameHandler(std::string Name, std::string LicenseTime, GameState State, int Version)
	{
		this->Name = Name;
		this->State = State;
		this->Version = Version;
		this->LicenseTime = LicenseTime;
	}
};

std::string GetGameState(GameHandler* ptr)
{
	if (!ptr)
	{
		throw std::exception("GameHandler* ptr was invalid!");
		return nullptr;
	}

	switch (ptr->State)
	{
	case GameState::Undetected:
		return "Undetected";
		break;
	case GameState::Updating:
		return "Updating";
		break;
	case GameState::Detected:
		return "Detected";
		break;
	case GameState::Unknown:
		return "Unknown";
		break;
	}
}

GameHandler* GetGameInfo(int game)
{
	if (!ImGui::GetCurrentContext())
	{
		throw std::exception("ImGui isn't initialized!");
		return new GameHandler("**INVALID**","**INVALID**",GameState::Unknown, 0.0);
	}
	else
	{
		if (game == 0) // FiveM
		{
			return new GameHandler("FiveM", "Lifetime", GameState::Undetected, 5.50);
		}
		else if (game == 1) // Polygon
		{
			return new GameHandler("Polygon", "Lifetime", GameState::Updating, 1.0);
		}
		else if (game == 2) // Rust
		{
			return new GameHandler("Rust", "Lifetime", GameState::Detected, 2.0);
		}
		else if (game == 3) // Splitgate
		{
			return new GameHandler("Splitgate", "Week", GameState::Undetected, 1.1);
		}
	}
}

void Menu::Render()
{
	static auto G = Globals::Get();

	auto X = ImGui::GetWindowSize().x;
	auto Y = ImGui::GetWindowSize().y;

	if (!G->IsLogged)
	{
		SET_ITEM_POSY(Y - 240);
		SET_ITEM_POSX(X / 6);
		ImGui::InputText("##UInput", G->Username, 248);
		ImGui::Spacing();

		SET_ITEM_POSX(X / 6);
		ImGui::InputText("##PInput", G->Password, 248, ImGuiInputTextFlags_Password);
		ImGui::Spacing();

		SET_ITEM_POSX(X / 6);
		ImGui::Checkbox("Remember Me", &G->RememberMe); ImGui::Spacing();

		SET_ITEM_POSX(X / 6);
		if (ImGui::Button("Login", ImVec2((X / 3.2), 29))) { G->IsLogged = true; } ImGui::SameLine();
		if (ImGui::Button("Register", ImVec2((X / 3.2), 29))) {}
	}
	else
	{
		SET_ITEM_POSY(60);
		
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
		if (ImGui::BeginChild("##LeftInternal", ImVec2(ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y)))
		{
			ImGui::Text("Game");
			ImGui::PopStyleColor();

			if (ImGui::BeginChild("##GamesPageLeft", ImVec2(ImGui::GetContentRegionAvail().x, Y - 200)))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(60, 8));
				for (unsigned int i = 0; i < G->Games.size(); i++)
				{
					bool selected = (G->Game == i);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 2);
					if (ImGui::Selectable(G->Games[i].c_str(), &selected))
					{
						G->Game = i;
					}
					if (selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::PopStyleVar();
				ImGui::EndChild();
			}

			ImGui::Spacing();

			if (ImGui::Button("Inject", ImVec2(ImGui::GetContentRegionAvail().x, 24))) 
			{
				static bool DidInject = false;
				if (!DidInject)
				{
					/* Your inject code */
					DidInject = true;
				}
				if (DidInject) 
				{
					ExitProcess(0);
				}
			}
			ImGui::Spacing();
			if (ImGui::Button("Activate Serial", ImVec2(ImGui::GetContentRegionAvail().x, 24))) 
			{

			}
			ImGui::EndChild();
		}
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
		if (ImGui::BeginChild("##RInternal", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
		{
			auto ptr = GetGameInfo(G->Game);

			ImGui::PopStyleColor();
			ImGui::Text("Cheat Info");
			if (ImGui::BeginChild("##GamesPageRight", ImVec2(ImGui::GetContentRegionAvail().x, Y - 135)))
			{
				/*ImGui::Spacing();

				ImGui::Text( "N.Mccrorey2" );*/

				SET_ITEM_POSX(ImGui::GetCursorPosX() + 5); ImGui::TextColored(ImColor(38, 187, 177), GetGameState(ptr).c_str());
				ImGui::Spacing();

				SET_ITEM_POSX(ImGui::GetCursorPosX() + 5); ImGui::Text(ptr->LicenseTime.c_str());
				ImGui::Spacing();

				SET_ITEM_POSX(ImGui::GetCursorPosX() + 5); ImGui::Text(("Version: " + std::to_string(ptr->Version)).append(".0").c_str());
				ImGui::Spacing();
			}
			ImGui::EndChild();
		}
	}
}

void Menu::Theme()
{
	ImGuiStyle* Style = &ImGui::GetStyle();
	auto Color = Style->Colors;
	
	Style->WindowMinSize     = ImVec2(400, 325);
	Style->WindowBorderSize  = 0;

	Style->ChildRounding     = 6;
	Style->FrameRounding     = 6;
	Style->ScrollbarRounding = 6;
	Style->WindowRounding	= 6;

	Color[ImGuiCol_TitleBg] = ImColor(39, 45, 59);
	Color[ImGuiCol_TitleBgActive] = ImColor(39, 45, 59);
	Color[ImGuiCol_TitleBgCollapsed] = ImColor(39, 45, 59);

	Color[ImGuiCol_WindowBg]            = ImColor(32, 36, 47, 255);
	
	Color[ImGuiCol_FrameBg]		        = ImColor(45, 55, 78, 255);
	Color[ImGuiCol_FrameBgActive]       = ImColor(45, 55, 78, 255);
	Color[ImGuiCol_FrameBgHovered]      = ImColor(45, 55, 78, 255);

	Color[ImGuiCol_Button]	            = ImColor(45, 55, 78, 255);
	Color[ImGuiCol_ButtonActive]        = ImColor(48, 59, 80, 255);
	Color[ImGuiCol_ButtonHovered]       = ImColor(48, 59, 80, 255);

	Color[ImGuiCol_Border]              = ImColor(0, 0, 0, 0);
	Color[ImGuiCol_Separator]           = ImColor(70, 70, 70, 255);

	Color[ImGuiCol_ResizeGrip]		    = ImColor(0, 0, 0, 255);
	Color[ImGuiCol_ResizeGripActive]    = ImColor(0, 0, 0, 255);
	Color[ImGuiCol_ResizeGripHovered]   = ImColor(0, 0, 0, 255);

	Color[ImGuiCol_ChildBg]			    = ImColor(45, 55, 78, 255);

	Color[ImGuiCol_ScrollbarBg]         = ImColor(35,35,35, 255);
	Color[ImGuiCol_ScrollbarGrab]       = ImColor(255, 255, 255, 255);
	Color[ImGuiCol_ScrollbarGrabActive] = ImColor(255, 255, 255, 255);
	Color[ImGuiCol_ScrollbarGrabActive] = ImColor(255, 255, 255, 255);

	Color[ImGuiCol_Header]			    = ImColor(60, 67, 83, 255);
	Color[ImGuiCol_HeaderActive]	    = ImColor(60, 67, 83, 255);
	Color[ImGuiCol_HeaderHovered]       = ImColor(60, 67, 83, 255);

	Color[ImGuiCol_CheckMark]		    = ImColor(255, 255, 255, 255);

	Color[ImGuiCol_Text] = ImColor(255, 255, 255);
	Color[ImGuiCol_TextDisabled] = ImColor(81, 95, 123);
}
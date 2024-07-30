#include "console.h"

Console::Console(HWND* hWnd) {
	Console::hWnd = hWnd;
	open = true;
}

void Console::Draw(bool windowFollow) {
	ImGui::Begin("Console Log", &open);
	if (!open)
		SendMessage(*hWnd, WM_CLOSE, NULL, NULL);
	if (windowFollow) {
		SetWindowPos(*hWnd, NULL, ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, 0, 0, SWP_NOSIZE);
	}
	ImGui::Text("Welcome To Teddy");
    ImGui::Separator();

    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NavFlattened))
    {
		for (Line item : Items) {
			ImVec4 colour;
			switch (item.color) {
			case ERR:
				colour = ImVec4(1, 0, 0, 1);
				break;
			case SUCCESS:
				colour = ImVec4(0, 1, 0, 1);
				break;
			case WARNING:
				colour = ImVec4(1, 1, 0, 1);
				break;
			default:
				colour = ImVec4(1, 1, 1, 1);
				break;
			}
			ImGui::PushStyleColor(ImGuiCol_Text, colour);
			ImGui::TextUnformatted(item.line);
			ImGui::PopStyleColor();
		}
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();

	ImGui::Separator();

	if (showStart) {
		if (ImGui::Button("Start")) {
			finished = true;
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Cancel")) {
		SendMessage(*hWnd, WM_CLOSE, NULL, NULL);
	}

	ImGui::End();
}

void Console::AddLog(const char* log, int col) {
	char buf[1024];
	const char* prefix;
	switch (col) {
	case ERR:
		prefix = "[ERROR] ";
		break;
	case SUCCESS:
		prefix = "[SUCCESS] ";
		break;
	case WARNING:
		prefix = "[WARNING] ";
		break;
	default:
		prefix = "";
		break;
	}

	strcpy_s(buf, prefix);
	strcat_s(buf, log);
	Items.push_back(Line(_strdup(buf), col));
}

void Console::Done() {
	showStart = true;
}


Line::Line(char* newLine, int newCol) {
	line = newLine;
	color = newCol;
}
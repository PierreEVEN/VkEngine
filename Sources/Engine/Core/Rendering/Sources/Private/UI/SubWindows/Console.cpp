#include "UI/SubWindows/Console.h"

Rendering::Console::Console()
	: SubWindow("console") 
{
	EngineIO.OnSendMessage.Add(this, &Console::OnSendMessage);

	for (int i = 0; i < IM_ARRAYSIZE(ConsoleInputBuffer); ++i) ConsoleInputBuffer[i] = '\0';

}

void Rendering::Console::OnSendMessage(const String& message)
{
	currentMessage.textColor = EngineInputOutput::GetConsoleColor();

	bool bFoundEndl = false;
	for (int64_t i = (int64_t)message.Length() - 1; i >= 0; --i) {

		if (message[i] == String::ENDL) {
			bFoundEndl = true;
			break;
		}
	
	}
	if (bFoundEndl) {
		currentMessage.text += message;
		messages.push_back(currentMessage);
		currentMessage.text = "";
	}
	else {
		currentMessage.text += message;
	}
	bScrollToEnd = true;
}


void Rendering::Console::DrawContent(const size_t& imageIndex)
{
	ImGui::SetWindowFontScale(0.85f);
	if (ImGui::BeginChild("Console content", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 40), true)) {
		for (const auto& message : messages) {
			ImVec4 fontColor(0.f, 1.f, 0.f, 1.f);
			switch (message.textColor) {
			case CONSOLE_DISPLAY:
				fontColor = ImVec4(0.5f, 1.f, 0.5f, 1.f);
				break;
			case CONSOLE_WARNING:
				fontColor = ImVec4(0.5f, 0.5f, 0.f, 1.f);
				break;
			case CONSOLE_ERROR:
				fontColor = ImVec4(1.f, 0.5f, 0.5f, 1.f);
				break;
			default:
				fontColor = ImVec4(1.f, 1.f, 1.f, 1.f);
				break;
			}


			ImGui::PushStyleColor(ImGuiCol_Text, fontColor);
			ImGui::Text(message.text.GetData());
			ImGui::PopStyleColor();
		}
		ImGui::Text(currentMessage.text.GetData());

		if (bScrollToEnd) {
			bScrollToEnd = false;
			ImGui::SetScrollHere();
		}
		ImGui::SetWindowFontScale(1.f);
		ImGui::EndChild();

		if (ImGui::InputText("input", ConsoleInputBuffer, IM_ARRAYSIZE(ConsoleInputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			LOG("Console - " + String(ConsoleInputBuffer));
			for (int i = 0; i < IM_ARRAYSIZE(ConsoleInputBuffer); ++i) ConsoleInputBuffer[i] = '\0';
		}


	}
}

Rendering::Console::~Console()
{
	EngineIO.OnSendMessage.UnbindObj(this);
}


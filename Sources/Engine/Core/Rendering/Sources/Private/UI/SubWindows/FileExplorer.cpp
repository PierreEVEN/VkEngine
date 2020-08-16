#include "UI/SubWindows/FileExplorer.h"
#include <filesystem>

Rendering::FileExplorer::FileExplorer(const String& defaultPath, const String& windowName, SubWindow* parent, const std::vector<String>& inExtensionFilters)
	: SubWindow(windowName, parent)
{
	if (std::filesystem::exists(G_LAST_CONTENT_BROWSER_DIRECTORY.GetValue().GetData()))
		SetCurrentPath(G_LAST_CONTENT_BROWSER_DIRECTORY.GetValue());
	else
		SetCurrentPath(defaultPath);

	extensionFilters.push_back({});
	if (extensionFilters.size() != 0) {
		for (const auto& ext : inExtensionFilters)
		{
			extensionFilters.push_back({ ext });
		}
		if (inExtensionFilters.size() != 1)
			extensionFilters.push_back(inExtensionFilters);
		currentFilter = (int)extensionFilters.size() - 1;
	}
}

void Rendering::FileExplorer::DrawContent(const size_t& imageIndex)
{
	ImGui::Separator();
	bool bIsPathValid = true;
	if (!std::filesystem::exists(currentPath))
	{
		bIsPathValid = false;
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.f, .2f, .2f, .5f));
	}
	ImGui::InputText("", currentPath, 256);
	if (std::filesystem::path(currentPath).has_parent_path())
	{
		ImGui::SameLine();
		ImGui::ImageButton(upArrowCircleIcon->GetTextureID(imageIndex), ImVec2(32, 32), ImVec2(0,0), ImVec2(1, 1), 0);
		if (ImGui::IsItemActive()) {
			SetCurrentPath(std::filesystem::path(currentPath).parent_path().u8string().c_str());
		}
	}
	if (!bIsPathValid) ImGui::PopStyleColor();

	ImGui::Separator();


	ImGui::BeginChild("outer_child", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 100), false);
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5));
	if (bIsPathValid) DrawDirContent(currentPath, imageIndex);
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();
	ImGui::Dummy(ImVec2(20, 0));
	ImGui::SameLine();
	ImGui::Text(selectedElement.GetData());
	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 400);


	char** extensionItems = new char*[extensionFilters.size()];
	for (int i = 0; i < extensionFilters.size(); ++i) {
		String filtValue = String::ConcatenateArray(extensionFilters[i]).GetData();
		extensionItems[i] = new char[filtValue.Length()];
		memcpy(extensionItems[i], filtValue.GetData(), filtValue.Length() + 1);
	}
	ImGui::Text("Extensions");
	ImGui::SameLine();
	ImGui::Combo("Extension", &currentFilter, extensionItems, (int)extensionFilters.size());

	for (int i = 0; i < extensionFilters.size(); ++i) {
		free(extensionItems[i]);
	}
	free(extensionItems);

	ImGui::Dummy(ImVec2(0, 10));
	ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x - 300, 0));
	ImGui::SameLine();
	if (ImGui::Button("validate", ImVec2(300, 35))) {
		OnApplyPath.Execute(selectedElement);
		bHasBeenValidated = true;
		RequestClose();
	}
}

Rendering::FileExplorer::~FileExplorer()
{
	G_LAST_CONTENT_BROWSER_DIRECTORY.SetValue(currentPath);
	if (!bHasBeenValidated) {
		OnCancelExplorer.Execute();
	}
}

void Rendering::FileExplorer::SetCurrentPath(const String& path)
{
	for (auto& chr : currentPath) chr = 0;
	memcpy(currentPath, path.GetData(), path.Length() < 256 ? path.Length() : 256);
}

void Rendering::FileExplorer::DrawDirContent(const String& dirPath, const size_t& imageIndex)
{
	String curDir(dirPath);

	for (const std::filesystem::directory_entry& elem : std::filesystem::directory_iterator(dirPath.GetData()))
	{
		if (elem.is_directory()) {
			ImGui::Image(directoryIcon->GetTextureID(imageIndex), ImVec2(32, 32));
			ImGui::SameLine();
			if (ImGui::Button(String::GetFileName(elem.path().u8string().c_str()).GetData(), ImVec2(ImGui::GetContentRegionAvail().x * 0.8f, 0))) {
				SetCurrentPath(elem.path().u8string().c_str());
			}
		}
	}
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.7f, .7f, .8f, .5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.8f, .8f, .9f, .7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.6f, .6f, .8f, .5f));
	for (const std::filesystem::directory_entry& elem : std::filesystem::directory_iterator(dirPath.GetData()))
	{
		if (extensionFilters[currentFilter].size() > 0)
		{
			bool bfound = false;
			String fileExt = String::GetFileExtension(elem.path().u8string().c_str());
			for (const auto& ext : extensionFilters[currentFilter]) {
				if (fileExt == ext) {
					bfound = true;
				}
			}
			if (!bfound) continue;
		}
		if (!elem.is_directory()) {
			ImGui::Image(fileIcon->GetTextureID(imageIndex), ImVec2(32, 32));
			ImGui::SameLine();
			if (ImGui::Button(String::GetFileName(elem.path().u8string().c_str()).GetData(), ImVec2(ImGui::GetContentRegionAvail().x * 0.8f, 0))) {
				selectedElement = elem.path().u8string().c_str();
			}
		}
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}


#include "UI/SubWindows/FileExplorer.h"
#include "Assets/Texture2D.h"
#include <filesystem>

#if _WIN32
#ifdef APIENTRY
#undef APIENTRY
#endif
#include <windows.h>
#endif

Rendering::FileExplorer::FileExplorer(const String& defaultPath, const String& windowName, SubWindow* parent, const std::vector<String>& inExtensionFilters, bool bDrawInParent)
	: SubWindow(windowName, parent, bDrawInParent)
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


	/** Search bar */
	ImGui::InputText("", currentPath, 256);
	if (std::filesystem::path(currentPath).has_parent_path())
	{
		ImGui::SameLine();
		ImGui::ImageButton(UIRessources::upArrowCircleIcon->GetTextureID(imageIndex), ImVec2(32, 32), ImVec2(0,0), ImVec2(1, 1), 0);
		if (ImGui::IsItemActive()) {
			SetCurrentPath(std::filesystem::path(currentPath).parent_path().u8string().c_str());
		}
	}
	if (!bIsPathValid) ImGui::PopStyleColor();
	ImGui::Separator();

	float windowSize = Maths::GetMax(ImGui::GetContentRegionAvail().x * .15f, 150.f);

	ImGui::Columns(2);
	if (!bSetColumnWidth) {
		ImGui::SetColumnWidth(0, windowSize);
		bSetColumnWidth = true;
	}
	/**  default path */


	ImGui::Image(UIRessources::sourceIcon->GetTextureID(imageIndex), ImVec2(32, 32));
	ImGui::SameLine();
	if (ImGui::Button("project", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
		SetCurrentPath(".");
	}
#if _WIN32
	DWORD mydrives = 100;
	WCHAR lpBuffer[100];
	char chrBuffer[100];
	for (auto& chr : lpBuffer) chr = '\0';
	GetLogicalDriveStrings(mydrives, lpBuffer);
	for (int i = 0; i < 100; ++i) chrBuffer[i] = (char)lpBuffer[i];
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5));
	for (const auto& drive : (String::ParseStringCharArray(chrBuffer, 100))) {
		ImGui::Image(UIRessources::hardDiskIcon->GetTextureID(imageIndex), ImVec2(32, 32));
		ImGui::SameLine();
		if (ImGui::Button(drive.GetData(), ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
			SetCurrentPath(drive);
		}
	}
	ImGui::PopStyleVar();
#endif

	ImGui::NextColumn();

	/** Content */
	ImGui::BeginChild("outer_child", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 100), false);
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0.5));
	if (bIsPathValid) DrawDirContent(currentPath, imageIndex);
	ImGui::PopStyleVar();
	ImGui::EndChild();

	ImGui::NextColumn();
	ImGui::Columns(1);

	ImGui::Separator();
	if (ImGui::BeginChild("selected_element", ImVec2(ImGui::GetContentRegionAvail().x - 450, 32), true)) {
		ImGui::Text(selectedElement.GetData());
		ImGui::EndChild();
	}
	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 400);

	/** Extensions */
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

	/** Validate */
	ImGui::Dummy(ImVec2(0, 10));
	ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x - 320, 0));
	ImGui::SameLine();
	if (selectedElement != "") {
		if (ImGui::Button("validate", ImVec2(320, 35))) {
			OnApplyPath.Execute(selectedElement);
			bHasBeenValidated = true;
			RequestClose();
		}
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
			ImGui::Image(UIRessources::directoryIcon->GetTextureID(imageIndex), ImVec2(32, 32));
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
			ImGui::Image(UIRessources::fileIcon->GetTextureID(imageIndex), ImVec2(32, 32));
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


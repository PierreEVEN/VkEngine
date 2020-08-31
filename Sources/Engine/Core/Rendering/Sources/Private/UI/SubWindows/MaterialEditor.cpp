#include "UI/SubWindows/MaterialEditor.h"
#include "Assets/ShaderModule.h"
#include "Assets/Texture2D.h"
#include "Assets/Material.h"

Rendering::ShaderEditor::ShaderEditor(ShaderModule* sourceFile, String& inLogText, bool& bInIsCompiling, MaterialEditor* inParent)
	: sourceShaderModule(sourceFile), logText(inLogText), bIsCompiling(bInIsCompiling), parent(inParent)
{
	sourceFile->OnShaderCompillationGetError.Add(this, &ShaderEditor::OnShaderCompilationError);
	sourceFile->OnRecompiledShaderModule.Add(this, &ShaderEditor::OnSuccessfullyCompiledShader);
	text = sourceShaderModule->GetShaderCode();
	String::ResetCharArray(shaderCode, sizeof(shaderCode));
	memcpy(shaderCode, text.GetData(), text.Length());
}

void Rendering::ShaderEditor::DrawContent(const size_t& imageIndex)
{
	if (ImGui::InputTextMultiline("shader", shaderCode, sizeof(shaderCode), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y))) {
		bHasBeenModified = true;
		lastModificationTime = std::chrono::steady_clock::now();
	}
}

void Rendering::ShaderEditor::OnSuccessfullyCompiledShader()
{
	parent->LogText(sourceShaderModule->GetName() + " : " + "Compilation succeed");
}

void Rendering::ShaderEditor::OnShaderCompilationError(const String& error)
{
	parent->LogText(sourceShaderModule->GetName() + " : " + error);
}

Rendering::MaterialEditor::MaterialEditor(Material* sourceFile)
	: SubWindow("Material editor")
{
	vertexShaderEditor = new ShaderEditor(sourceFile->materialProperties.vertexShaderModule, compilationLogs, bIsCompiling, this);
	fragmentShaderEditor = new ShaderEditor(sourceFile->materialProperties.fragmentShaderModule, compilationLogs, bIsCompiling, this);
}

void Rendering::MaterialEditor::LogText(const String& text)
{
	logTextMutex.lock();
	compilationLogs << text << String::ENDL;
	logTextMutex.unlock();
}

void Rendering::MaterialEditor::DrawContent(const size_t& imageIndex)
{
	ImGui::Image(UIRessources::applyIcon->GetTextureID(imageIndex), ImVec2(32, 32));
	ImGui::SameLine();
	ImGui::Button("Recompile shader");

	ImGui::SameLine();

	ImGui::Image(UIRessources::autoBuildIcon->GetTextureID(imageIndex), ImVec2(32, 32));
	ImGui::SameLine();
	ImGui::Checkbox("Auto recompile", &bAutoRebuild);

	ImGui::Separator();
	if (ImGui::BeginChild("shader code", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 150), true, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x / 2.f - ImGui::CalcTextSize("shader code").x / 2.f, 0));
			ImGui::Text("shader code");
			ImGui::EndMenuBar();
		}
		if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None)) {
			if (ImGui::BeginTabItem("Vertex shader", nullptr, vertexShaderEditor->hasBeenModified() ? ImGuiTabItemFlags_UnsavedDocument : ImGuiTabItemFlags_None)) {
				vertexShaderEditor->DrawContent(imageIndex);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Fragment shader", nullptr, fragmentShaderEditor->hasBeenModified() ? ImGuiTabItemFlags_UnsavedDocument : ImGuiTabItemFlags_None)) {
				fragmentShaderEditor->DrawContent(imageIndex);
				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}
	ImGui::EndChild();

	if (ImGui::BeginChild("output", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x / 2.f - ImGui::CalcTextSize("shader code").x / 2.f, 0));
			ImGui::Text("output");
			ImGui::EndMenuBar();
		}

		ImGui::Text("%s", compilationLogs.GetData());
	}
	ImGui::EndChild();
}

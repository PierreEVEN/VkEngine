#include "UI/SubWindows/ShaderCodeEditor.h"
#include "Assets/ShaderModule.h"

Rendering::ShaderCodeEditor::ShaderCodeEditor(ShaderModule* sourceFile)
	: sourceShaderModule(sourceFile), SubWindow("Shader editor")
{
	sourceFile->OnShaderCompillationGetError.Add(this, &ShaderCodeEditor::OnShaderCompilationError);
	sourceFile->OnRecompiledShaderModule.Add(this, &ShaderCodeEditor::OnSuccessfullyCompiledShader);
	text = sourceShaderModule->GetShaderCode();
	String::ResetCharArray(shaderCode, sizeof(shaderCode));
	memcpy(shaderCode, text.GetData(), text.Length());
}

void Rendering::ShaderCodeEditor::DrawContent(const size_t& imageIndex)
{
	if (ImGui::Button("Compile") || (bHasBeenModified && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastModificationTime).count() > 500)) {
		String outText = shaderCode;
		std::vector<char> outData(outText.Length() + 1);
		memcpy(outData.data(), outText.GetData(), outText.Length() + 1);
		outData[outText.Length()] = '\0';
		compilationLogs = "";
		compilationLogs << "Compilling shader..." << String::ENDL;
		bHasBeenModified = false;
		sourceShaderModule->UpdateShaderCode(outData);
	}

	if (ImGui::BeginChild("shader code", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x / 2.f - ImGui::CalcTextSize("shader code").x / 2.f, 0));
			ImGui::Text("shader code");
			ImGui::EndMenuBar();
		}
		if (ImGui::InputTextMultiline("shader", shaderCode, sizeof(shaderCode), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 150))) {
			bHasBeenModified = true;
			lastModificationTime = std::chrono::steady_clock::now();
		}
	}

	if (ImGui::BeginChild("compilationResult", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x / 2.f - ImGui::CalcTextSize("output").x / 2.f, 0));
			ImGui::Text("output");
			ImGui::EndMenuBar();
		}
		ImGui::Text(compilationLogs.GetData());
	}
	ImGui::EndChild();
}


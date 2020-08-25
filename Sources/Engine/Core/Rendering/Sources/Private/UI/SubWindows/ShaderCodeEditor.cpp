#include "UI/SubWindows/ShaderCodeEditor.h"
#include "Assets/ShaderModule.h"

Rendering::ShaderCodeEditor::ShaderCodeEditor(ShaderModule* sourceFile)
	: sourceShaderModule(sourceFile), SubWindow("Shader editor")
{
	text = sourceShaderModule->GetShaderCode();
	String::ResetCharArray(shaderCode, sizeof(shaderCode));
	memcpy(shaderCode, text.GetData(), text.Length());
}

void Rendering::ShaderCodeEditor::DrawContent(const size_t& imageIndex)
{
	if (ImGui::Button("Compile")) {


		String outText = shaderCode;
		std::vector<char> outData(outText.Length());
		memcpy(outData.data(), outText.GetData(), outText.Length());
		sourceShaderModule->UpdateShaderCode(outData);
	}


	ImGui::InputTextMultiline("shader code", shaderCode, sizeof(shaderCode), ImGui::GetContentRegionAvail());
}


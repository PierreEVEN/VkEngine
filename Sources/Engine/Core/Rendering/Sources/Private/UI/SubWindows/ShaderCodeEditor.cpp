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


		std::vector<char> outData(outText.Length() + 1);
		memcpy(outData.data(), outText.GetData(), outText.Length() + 1);
		outData[outText.Length()] = '\0';
		sourceShaderModule->UpdateShaderCode(outData);
	}


	ImGui::InputTextMultiline("shader code", shaderCode, sizeof(shaderCode), ImGui::GetContentRegionAvail());
}


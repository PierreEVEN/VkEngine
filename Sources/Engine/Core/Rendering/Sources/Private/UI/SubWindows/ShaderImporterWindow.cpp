#include "UI/SubWindows/ShaderImporterWindow.h"
#include "UI/SubWindows/FileExplorer.h"
#include "Assets/AssetFactory.h"
#include "Assets/ShaderModule.h"
#include "Assets/Material.h"
#include "UI/SubWindows/AssetSelector.h"

Rendering::ShaderImportWindow::ShaderImportWindow()
	: SubWindow("Import shader")
{
	FileExplorer* fe = new FileExplorer(".", "Selecte shader", this, TAssetFactory<ShaderModule>::GetDesiredImportFormats(), true);
	fe->OnApplyPath.Add(this, &ShaderImportWindow::OnPickedShader);
	fe->OnCancelExplorer.Add((SubWindow*)this, &SubWindow::RequestClose);
}

void Rendering::ShaderImportWindow::OnPickedShader(const String& path)
{
	TAssetFactory<ShaderModule>::MakeTransient(TAssetFactory<ShaderModule>::ImportFromPath(path));
	RequestClose();
}

Rendering::MaterialConstructorWindow::MaterialConstructorWindow()
	: SubWindow("Make material") {}

void Rendering::MaterialConstructorWindow::DrawContent(const size_t& imageIndex)
{

	String vertName = materialProperties.vertexShaderModule ? materialProperties.vertexShaderModule->GetName() : "pick vertex shader";
	String fragName = materialProperties.fragmentShaderModule ? materialProperties.fragmentShaderModule->GetName() : "pick fragment shader";

	ImGui::Text("Vertex shader :      ");
	ImGui::SameLine();
	if (ImGui::Button(vertName.GetData(), ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
		AssetSelector<ShaderModule>* assetSelector = new AssetSelector<ShaderModule>(this, materialProperties.vertexShaderModule);
		assetSelector->OnAssetSelected.Add(this, &MaterialConstructorWindow::OnPickedVShader);
	}
	ImGui::Text("Fragment shader : ");
	ImGui::SameLine();
	if (ImGui::Button(fragName.GetData(), ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
		AssetSelector<ShaderModule>* assetSelector = new AssetSelector<ShaderModule>(this, materialProperties.fragmentShaderModule);
		assetSelector->OnAssetSelected.Add(this, &MaterialConstructorWindow::OnPickedFShader);
	}
	ImGui::SliderInt("vertex texture count", (int*)&materialProperties.VertexTexture2DCount, 0, 16);
	ImGui::SliderInt("fragment texture count", (int*)&materialProperties.FragmentTexture2DCount, 0, 16);

	ImGui::Checkbox("Use world uniform buffer", &materialProperties.bUseGlobalUbo);
	ImGui::Checkbox("Translucent", &bIsTranslucent);
	ImGui::Checkbox("Double sided", &bDoubleSided);
	ImGui::Checkbox("Disable depth test", &bDisableDepthTest);
	ImGui::Checkbox("Enable stencil test", &bEnableStencilTest);
	ImGui::Checkbox("Wireframe", &bWireframe);
	ImGui::Dummy(ImVec2(0, ImGui::GetContentRegionAvail().y - 50));
	if (ImGui::Button("Cancel", ImVec2(150, 0))) RequestClose();
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvailWidth() - 150, 0));
	ImGui::SameLine();
	if (ImGui::Button("Import", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
		CreateMaterial();
	}
}

void Rendering::MaterialConstructorWindow::CreateMaterial()
{
	if (materialProperties.vertexShaderModule && materialProperties.fragmentShaderModule) {
		EMaterialCreationFlags matFlags = EMATERIAL_CREATION_FLAG_NONE;
		if (bIsTranslucent) matFlags |= EMATERIAL_CREATION_FLAG_TRANSLUCENT;
		if (bWireframe) matFlags |= EMATERIAL_CREATION_FLAG_WIREFRAME;
		if (bDoubleSided) matFlags |= EMATERIAL_CREATION_FLAG_DOUBLESIDED;
		if (bDisableDepthTest) matFlags |= EMATERIAL_CREATION_FLAG_DISABLE_DEPTH_TEST;
		if (bEnableStencilTest) matFlags |= EMATERIAL_CREATION_FLAG_ENABLE_STENCIL_TEST;

		materialProperties.materialCreationFlag = matFlags;
		TAssetFactory<Material>::MakeTransient(TAssetFactory<Material>::CreateFromData(materialProperties, "Lambda material"));
		RequestClose();
	}
}

void Rendering::MaterialConstructorWindow::OnPickedVShader(ShaderModule* module)
{
	materialProperties.vertexShaderModule = module;
}

void Rendering::MaterialConstructorWindow::OnPickedFShader(ShaderModule* module)
{
	materialProperties.fragmentShaderModule = module;
}

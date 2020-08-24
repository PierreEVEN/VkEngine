#include "Assets/Material.h"
#include "Scene/Scene.h"
#include "Scene/VkSceneElements/MatrixUniformBuffers.h"
#include "Assets/Texture2D.h"

void Rendering::Material::CreateDefaultRessources()
{
	if (!G_DEFAULT_VERTEX_MODULE || !G_DEFAULT_FRAGMENT_MODULE) LOG_ASSERT("Default vertex and fragment modules should be created before creating default material");
	
	SMaterialStaticProperties defaultMaterialProperties{};
	defaultMaterialProperties.bUseGlobalUbo = true;
	defaultMaterialProperties.vertexShaderModule = G_DEFAULT_VERTEX_MODULE;
	defaultMaterialProperties.fragmentShaderModule = G_DEFAULT_FRAGMENT_MODULE;
	defaultMaterialProperties.VertexTexture2DCount = 0;
	defaultMaterialProperties.FragmentTexture2DCount = 3;
	defaultMaterialProperties.materialCreationFlag = EMATERIAL_CREATION_FLAG_NONE;

	SMaterialDynamicProperties defaultMaterialDynProperties{};
	defaultMaterialDynProperties.fragmentTextures2D.push_back(Texture2D::GetDefaultTexture());
	defaultMaterialDynProperties.fragmentTextures2D.push_back(Texture2D::GetDefaultTexture());
	defaultMaterialDynProperties.fragmentTextures2D.push_back(Texture2D::GetDefaultTexture());

	defaultMaterial = new Material(defaultMaterialProperties, "DefaultMaterial", defaultMaterialDynProperties, false);
	defaultMaterialRessource = defaultMaterial->materialRessource;
	materialIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/Assets/icon-Material.png");
}

void Rendering::Material::CreateMaterialRessource(MaterialRessourceItem** outMaterial, const SMaterialStaticProperties inMaterialProperties, const SMaterialDynamicProperties inMaterialDynamicProperties)
{
	MaterialRessourceItem* newRessource = new MaterialRessourceItem(inMaterialProperties, inMaterialDynamicProperties);
	(*outMaterial) = newRessource;
}

Rendering::Material::Material(const SMaterialStaticProperties& inMaterialProperties, const String& assetName, const SMaterialDynamicProperties& inMaterialDynamicProperties, bool bLoadAsync)
	: Asset(assetName), materialProperties(inMaterialProperties), materialDynamicProperties(inMaterialDynamicProperties) {
	if (bLoadAsync) {
		MaterialRessourceItem** ressource = &materialRessource;
		JobSystem::NewJob([ressource, inMaterialProperties, inMaterialDynamicProperties] 
			{
			CreateMaterialRessource(ressource, inMaterialProperties, inMaterialDynamicProperties);
			});
	}
	else {
		CreateMaterialRessource(&materialRessource, inMaterialProperties, inMaterialDynamicProperties);
	}
}

void Rendering::Material::PreDraw(ViewportInstance* writeViewport, const size_t& imageIndex)
{
	if (materialRessource) {
		bUseDefaultRessourceThisFrame = false;
		materialRessource->PreDraw(writeViewport, imageIndex);
	}
	else {
		bUseDefaultRessourceThisFrame = true;
		defaultMaterialRessource->PreDraw(writeViewport, imageIndex);
	}
}

void Rendering::Material::Draw(VkCommandBuffer commandBuffer, ViewportInstance* writeViewport, const size_t& imageIndex)
{
	if (!bUseDefaultRessourceThisFrame) {
		materialRessource->Draw(commandBuffer, writeViewport, imageIndex);
	}
	else {
		defaultMaterialRessource->Draw(commandBuffer, writeViewport, imageIndex);
	}
}
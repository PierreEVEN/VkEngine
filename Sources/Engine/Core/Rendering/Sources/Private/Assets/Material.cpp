#include "Assets/Material.h"
#include "Scene/Scene.h"
#include "Scene/VkSceneElements/MatrixUniformBuffers.h"
#include "Assets/Texture2D.h"

Rendering::Material::Material(const SMaterialStaticProperties& inMaterialProperties, const String& assetName, const SMaterialDynamicProperties& inMaterialDynamicProperties)
	: Asset(assetName), materialProperties(inMaterialProperties), materialDynamicProperties(inMaterialDynamicProperties) {
	materialRessource = new MaterialRessourceItem(materialProperties, materialDynamicProperties);
}

void Rendering::Material::PreDraw(ViewportInstance* writeViewport, const size_t& imageIndex)
{
	materialRessource->PreDraw(writeViewport, imageIndex);
}

void Rendering::Material::Draw(VkCommandBuffer commandBuffer, ViewportInstance* writeViewport, const size_t& imageIndex)
{
	materialRessource->Draw(commandBuffer, writeViewport, imageIndex);
}

Rendering::Texture2D* Rendering::Material::GetAssetIcon() const
{
	if (!materialIcon) {
		materialIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/Assets/icon-Material.png");
	}
	return materialIcon;
}
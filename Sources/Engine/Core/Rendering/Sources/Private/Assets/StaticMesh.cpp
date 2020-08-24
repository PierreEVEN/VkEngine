#include "Assets/StaticMesh.h"
#include "Ressources/MeshRessource.h"
#include "Assets/Texture2D.h"
#include "Assets/Material.h"

void Rendering::StaticMesh::CreateMeshRessource(SMeshSectionData* inSectionDatas)
{
	MeshRessource* newRessource = new MeshRessource(inSectionDatas->vertices, inSectionDatas->indices);
	inSectionDatas->MeshLink = newRessource;
}

Rendering::StaticMesh::StaticMesh(const std::vector<SMeshSectionData>& inSectionDatas, const String& assetName, bool bLoadAsync)
	: Asset(assetName), sectionDatas(inSectionDatas)
{
	auto start = std::chrono::high_resolution_clock::now();

	for (auto& section : sectionDatas) {
		if (!section.MeshLink) {
			if (bLoadAsync) {
				SMeshSectionData* sectionData = &section;
				JobSystem::NewJob([sectionData] 
					{
					CreateMeshRessource(sectionData);
					});
			}
			else {
				section.MeshLink = new MeshRessource(section.vertices, section.indices);
			}
		}
	}

}

Rendering::StaticMesh::~StaticMesh()
{
	for (auto& section : sectionDatas) {
		if (section.MeshLink) section.MeshLink->DestroyRessource();
	}
}

Rendering::Texture2D* Rendering::StaticMesh::GetAssetIcon() const
{
	if (!staticMeshIcon) {
		staticMeshIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/Assets/icon-staticMesh.png");
	}
	return staticMeshIcon;
}

void Rendering::StaticMesh::PreDraw(ViewportInstance* writeViewport, const size_t& imageIndex) {
	for (SMeshSectionData& section : sectionDatas)
	{
		if (section.materialLink) {
			section.materialLink->PreDraw(writeViewport, imageIndex);
		}
		else {
			LOG_WARNING("Static mesh section has no material to draw");
		}
	}
}

void Rendering::StaticMesh::Draw(VkCommandBuffer& inCommandBuffer, ViewportInstance* viewport, const size_t& imageIndex, Mat4f objectTransform)
{

	for (SMeshSectionData& section : sectionDatas)
	{
		if (section.MeshLink) {
			if (section.materialLink) {
				section.materialLink->Draw(inCommandBuffer, viewport, imageIndex);
			}
			vkCmdPushConstants(inCommandBuffer, section.materialLink->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Mat4f), &objectTransform);
			section.MeshLink->Draw(inCommandBuffer);
		}
	}
}


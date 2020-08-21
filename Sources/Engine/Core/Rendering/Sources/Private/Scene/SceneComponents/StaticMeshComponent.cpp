#include "Scene/SceneComponents/StaticMeshComponent.h"
#include "Assets/StaticMesh.h"
#include "Ressources/MeshRessource.h"
#include "Assets/Material.h"

Rendering::StaticMeshComponent::StaticMeshComponent(const STransform& inTransform, StaticMesh* meshLink)
	: ScenePrimitive(inTransform)
{
	staticMesh = meshLink;
}

void Rendering::StaticMeshComponent::PreDraw(ViewportInstance* inViewport, const size_t& inImageIndex)
{
	for (SMeshSectionData& section : staticMesh->GetSectionData())
	{
		section.materialLink->PreDraw(inViewport, inImageIndex);
	}
}

void Rendering::StaticMeshComponent::Draw(VkCommandBuffer& inCommandBuffer, ViewportInstance* viewport, const size_t& imageIndex)
{
	for (SMeshSectionData& section : staticMesh->GetSectionData())
	{
		section.materialLink->Draw(inCommandBuffer, viewport, imageIndex);

		Mat4f objTransform(GetLocation(), GetRotation(), GetScale());
		vkCmdPushConstants(inCommandBuffer, section.materialLink->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Mat4f), &objTransform);
		section.MeshLink->Draw(inCommandBuffer);
	}
}
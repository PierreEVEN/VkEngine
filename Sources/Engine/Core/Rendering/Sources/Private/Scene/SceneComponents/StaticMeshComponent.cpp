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
	staticMesh->PreDraw(inViewport, inImageIndex);
}

void Rendering::StaticMeshComponent::Draw(VkCommandBuffer& inCommandBuffer, ViewportInstance* viewport, const size_t& imageIndex)
{
	staticMesh->Draw(inCommandBuffer, viewport, imageIndex, Mat4f(GetLocation(), GetRotation(), GetScale()));
}
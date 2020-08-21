#include "Assets/StaticMesh.h"
#include "Ressources/MeshRessource.h"
#include "Assets/Texture2D.h"

Rendering::StaticMesh::StaticMesh(const std::vector<SMeshSectionData>& inSectionDatas, const String& assetName)
	: Asset(assetName), sectionDatas(inSectionDatas)
{
	for (auto& section : sectionDatas) {
		if (!section.MeshLink) {
			section.MeshLink = new MeshRessource(section.vertices, section.indices);
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


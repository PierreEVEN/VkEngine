#include "UI/SubWindows/ContentBrowser.h"
#include "Assets/Asset.h"
#include "Assets/Texture2D.h"
#include "Assets/StaticMesh.h"
#include "Scene/SceneComponents/StaticMeshComponent.h"
#include "Scene/Scene.h"

void Rendering::ContentBrowser::DrawContent(const size_t& imageIndex)
{
	float width = ImGui::GetContentRegionAvailWidth();

	float currentWidth = 0;
	const float itemWidth = 300;
	for (const auto& asset : Asset::FindAssetByClass<Asset>(Asset::GetStaticClass())) {

		if (asset->GetThumbnail()) {
			ImGui::Image(asset->GetThumbnail()->GetTextureID(imageIndex), ImVec2(100, 100));
			ImGui::SameLine();
		}

		ImGui::Image(asset->GetAssetIcon()->GetTextureID(imageIndex), ImVec2(24, 24));
		ImGui::SameLine();
		if (asset->GetClass() == StaticMesh::GetStaticClass()) {

			if (!G_TEST_COMP) {
				G_TEST_COMP = new StaticMeshComponent(STransform(), (StaticMesh*)asset);
			}

// 			if (ImGui::Button(String(asset->GetName().GetData()).GetData())) {
// 				G_TEST_COMP = new StaticMeshComponent(STransform(), (StaticMesh*)asset);
// 			}
		}
		else {
			ImGui::Text(String(asset->GetName().GetData()).GetData());
		}

	}
}


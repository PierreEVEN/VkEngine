#include "UI/SubWindows/StaticMeshImportWindow.h"
#include "UI/SubWindows/FileExplorer.h"
#include "Importers/GltfImporter.h"
#include "UI/SubWindows/AssetSelector.h"
#include "Assets/Texture2D.h"
#include "Assets/Material.h"
#include "Assets/AssetFactory.h"
#include "Assets/StaticMesh.h"

Rendering::StaticMeshImportWindow::StaticMeshImportWindow() : SubWindow("Import gltf object")
{
	filePath = "";
	FileExplorer* explorer = new FileExplorer("./", "Find gltf asset", this, { "gltf", "glb" }, true);
	explorer->OnApplyPath.Add(this, &StaticMeshImportWindow::OnFileExplorerChosedPath);
	explorer->OnCancelExplorer.Add(this, &StaticMeshImportWindow::OnFillExplorerClosed);
}

void Rendering::StaticMeshImportWindow::DrawContent(const size_t& imageIndex)
{
	if (filePath == "") return;
	ImGui::Text("Import File : %s", filePath.GetData());
	ImGui::Text("Import to %s", "none");
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Columns(2);

	ImGui::Dummy(ImVec2(10, 0));
	ImGui::SameLine();
	if (ImGui::BeginChild("itemDetails", ImVec2(ImGui::GetContentRegionAvailWidth(), (ImGui::GetContentRegionAvail().y - 100)), true, ImGuiWindowFlags_MenuBar)) {

		if (ImGui::BeginMenuBar())
		{
			ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x / 2.f - ImGui::CalcTextSize(String("Import details").GetData()).x / 2.f, 0));
			ImGui::Text("Import details");
			ImGui::EndMenuBar();
		}
		DrawImportDetails();
	}
	ImGui::EndChild();

	ImGui::NextColumn();

	ImGui::SameLine();
	if (ImGui::BeginChild("sceneDetails", ImVec2(ImGui::GetContentRegionAvailWidth() - 10, (ImGui::GetContentRegionAvail().y - 100)), true, ImGuiWindowFlags_MenuBar)) {

		if (ImGui::BeginMenuBar())
		{
			ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x / 2.f - ImGui::CalcTextSize(String("Import details").GetData()).x / 2.f, 0));
			ImGui::Text("Scene details");
			ImGui::EndMenuBar();
		}
		DrawSceneDetails();
	}
	ImGui::EndChild();

	ImGui::Columns(1);

	ImGui::Dummy(ImVec2(0, 5));
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 15));
	ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvailWidth() - 320, 0));
	ImGui::SameLine();
	if (ImGui::Button("Import", ImVec2(230, 0))) {

		Importers::GltfImporter* gltfImporter = importer;
		const String sourceFilePath = filePath;
		JobSystem::NewJob([gltfImporter, sourceFilePath] {
			std::vector<SMeshSectionData> generatedData = gltfImporter->GenerateData();
			TAssetFactory<StaticMesh>::MakeTransient(TAssetFactory<StaticMesh>::CreateFromData(generatedData, String::GetFileShortName(sourceFilePath)));
			delete gltfImporter;
			});

		RequestClose();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel")) { RequestClose(); }
}

void Rendering::StaticMeshImportWindow::DrawSceneDetails()
{
	size_t sceneIndex = 0;
	for (const auto& scene : importer->scenes) {
		ImGui::Separator();
		ImGui::Text("Scene %s", ToString(sceneIndex++).GetData());
		ImGui::Indent(20);

		for (int i = 0; i < scene.nodes.size(); ++i) {
			if (ImGui::TreeNode(("Node #" + ToString(scene.nodes[i])).GetData())) {
				DrawNode(scene.nodes[i]);
				ImGui::TreePop();
			}
		}


		ImGui::Unindent(20);
	}
}

void Rendering::StaticMeshImportWindow::DrawImportDetails()
{
	ImGui::Text("Object name : %s", String::GetFileShortName(filePath).GetData());
	if (importer->assetData.generator) ImGui::Text("Importer : %s", importer->assetData.generator.GetData());
	if (importer->assetData.version) ImGui::Text("Version : %s", importer->assetData.version.GetData());
	if (importer->assetData.copyright) ImGui::Text("License : %s", importer->assetData.copyright.GetData());




	if (ImGui::BeginChild("materials", ImVec2(ImGui::GetContentRegionAvailWidth(), (ImGui::GetContentRegionAvail().y / 2)), true, ImGuiWindowFlags_MenuBar)) {

		if (ImGui::BeginMenuBar())
		{
			ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x / 2.f - ImGui::CalcTextSize(String("materials").GetData()).x / 2.f, 0));
			ImGui::Text("materials");
			if (importer->materials.size() != 0) {
				String text = importer->materials[0].bImport ? "Import any" : "Import all";
				ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(text.GetData()).x - 20, 0));
				if (ImGui::Button(text.GetData())) {
					for (int64_t i = importer->materials.size() - 1; i >= 0; --i) {
						importer->materials[i].bImport = !importer->materials[0].bImport;
					}
				}
			}
			ImGui::EndMenuBar();
		}

		for (Importers::GLtfMaterial& material : importer->materials) {
			if (ImGui::TreeNode(material.name.GetData())) {
				
				if (!material.selectedMaterial) {
					const char* extensionItems[3] = { "None", "Import", "pick existing item" };
					int selectedItem = material.bImport;
					ImGui::Combo("Import material", &selectedItem, extensionItems, 3);
					material.bImport = selectedItem == 1;
					if (selectedItem == 2) {
						AssetSelector<Material>* aselect = new AssetSelector<Material>(this, material.selectedMaterial);
						aselect->OnAssetSelected.Add(&material, &Importers::GLtfMaterial::OnSelectedMaterial);
					}
				}
				else {
					if (ImGui::Button(material.selectedMaterial->GetName().GetData())) {
						AssetSelector<Material>* aselect = new AssetSelector<Material>(this, material.selectedMaterial);
						aselect->OnAssetSelected.Add(&material, &Importers::GLtfMaterial::OnSelectedMaterial);
					}
				}
				ImGui::TreePop();
			}
		}


	}
	ImGui::EndChild();



	if (ImGui::BeginChild("images", ImVec2(ImGui::GetContentRegionAvailWidth(), (ImGui::GetContentRegionAvail().y)), true, ImGuiWindowFlags_MenuBar)) {

		if (ImGui::BeginMenuBar())
		{
			ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x / 2.f - ImGui::CalcTextSize(String("images").GetData()).x / 2.f, 0));
			ImGui::Text("images");
			if (importer->images.size() != 0)  {
				String text = importer->images[0].bImportTexture ? "Import any" : "Import all";
				ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(text.GetData()).x - 20, 0));
				if (ImGui::Button(text.GetData())) {
					for (int64_t i = importer->images.size() - 1; i >= 0; --i) {
						importer->images[i].bImportTexture = !importer->images[0].bImportTexture;
					}
				}
			}
			ImGui::EndMenuBar();
		}

		for (Importers::GltfImage& texture : importer->images) {
			if (ImGui::TreeNode(texture.uri.GetData())) {
				ImGui::Text("Image type : %s", texture.mimeType.GetData());
				if (!texture.linkedTexture) {
					const char* extensionItems[3] = { "None", "Import", "pick existing item" };
					int selectedItem = texture.bImportTexture;
					ImGui::Combo("Import texture", &selectedItem, extensionItems, 3);
					texture.bImportTexture = selectedItem == 1;
					if (selectedItem == 2) {
						AssetSelector<Texture2D>* aselect = new AssetSelector(this, texture.linkedTexture);
						aselect->OnAssetSelected.Add(&texture, &Importers::GltfImage::OnSelectedTexture);
					}
				}
				else {
					if (ImGui::Button(texture.linkedTexture->GetName().GetData())) {
						AssetSelector<Texture2D>* aselect = new AssetSelector(this, texture.linkedTexture);
						aselect->OnAssetSelected.Add(&texture, &Importers::GltfImage::OnSelectedTexture);
					}
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::EndChild();

}

void Rendering::StaticMeshImportWindow::DrawNode(const size_t& nodeIndex)
{
	for (int i = 0; i < importer->nodes[nodeIndex].children.size(); ++i) {
		if (ImGui::TreeNode(("Child #" + ToString(importer->nodes[nodeIndex].children[i])).GetData())) {
			DrawNode(importer->nodes[nodeIndex].children[i]);
			ImGui::TreePop();
		}
	}


	if (importer->nodes[nodeIndex].mesh != -1) {
		ImGui::Text("Mesh : ");
		ImGui::Indent(20);
		Drawmesh(importer->nodes[nodeIndex].mesh);
		ImGui::Unindent(20);
	}
}

void Rendering::StaticMeshImportWindow::Drawmesh(const size_t& meshIndex)
{

}

void Rendering::StaticMeshImportWindow::OnFileExplorerChosedPath(const String& path)
{
	filePath = path;
	importer = new Importers::GltfImporter(path);
}
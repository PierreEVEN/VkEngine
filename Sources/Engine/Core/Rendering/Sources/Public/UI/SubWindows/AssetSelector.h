#pragma once

#include "UI/SubWindows/SubWindow.h"
#include "FileExplorer.h"
#include "Assets/Asset.h"
#include "Assets/Texture2D.h"

namespace Rendering {

	class SubWindow;

	template<typename T>
	DECLARE_DELEGATE_MULTICAST(OnAssetSelectedEvent, T*);

	DECLARE_DELEGATE_MULTICAST(OnClosedAssetSelectorEvent);

	template<typename T>
	class AssetSelector : public SubWindow {
	public:
		AssetSelector(SubWindow* parent, T* currentItem = nullptr)
			: SubWindow(String("Select ") + T::GetStaticClass()->GetName(), parent), pickedItem(currentItem) {
			assets = Asset::FindAssetByClass<T>(T::GetStaticClass());
			String::ResetCharArray(filter, sizeof(filter));
		}

		OnAssetSelectedEvent<T> OnAssetSelected;
		OnClosedAssetSelectorEvent OnClosedAssetSelector;

	protected:

		virtual ~AssetSelector() {
			if (!pickedItem) {
				OnClosedAssetSelector.Execute();
			}
		}

		virtual void DrawContent(const size_t& imageIndex) {

			if constexpr (AssetFactory::CanImportFromPath<T>::value && AssetFactory::CanBeTransient<T>::value) {
				if (ImGui::Button("Import", ImVec2(ImGui::GetContentRegionAvailWidth() / 2, 0))) {
					if constexpr (AssetFactory::HasDesiredImportFormats<T>::value) {
						FileExplorer* expolorer = new FileExplorer(".", String("Import "), this, TAssetFactory<T>::GetDesiredImportFormats());
						expolorer->OnApplyPath.Add(this, &AssetSelector<T>::OnPickedFileExplorerItem);
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("clear", ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
					pickedItem = nullptr;
					OnAssetSelected.Execute(pickedItem);
					RequestClose();
				}
			}

			ImGui::InputText("filter", filter, sizeof(filter));


			if (ImGui::BeginChild("select asset", ImGui::GetContentRegionAvail(), true, ImGuiWindowFlags_MenuBar))
			{
				if (ImGui::BeginMenuBar())
				{
					ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvail().x / 2.f - ImGui::CalcTextSize("select asset").x / 2.f, 0));
					ImGui::Text("select asset");
					ImGui::EndMenuBar();
				}

				assets = Asset::FindAssetByClass<T>(T::GetStaticClass());


				for (const auto& asset : assets) {
					if (asset->GetName().Contains(filter)) {
						if (asset->GetAssetIcon())
						{
							ImGui::Image(asset->GetAssetIcon()->GetTextureID(imageIndex), ImVec2(32, 32));
							ImGui::SameLine();
						}
						if (ImGui::Button(asset->GetName().GetData(), ImVec2(ImGui::GetContentRegionAvailWidth(), 0))) {
							pickedItem = asset;
							OnAssetSelected.Execute(pickedItem);
							RequestClose();
						}
					}
				}

				ImGui::EndChild();
			}
			

		}

	private:


		char filter[256];

		void OnPickedFileExplorerItem(const String& item) {
			pickedItem = TAssetFactory<T>::ImportFromPath(item);
			TAssetFactory<T>::MakeTransient(pickedItem);
			OnAssetSelected.Execute(pickedItem);
			RequestClose();
		}

		std::vector<T*> assets;
		T* pickedItem;
	};

}
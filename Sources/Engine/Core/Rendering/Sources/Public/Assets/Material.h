#pragma once

#include "Asset.h"
#include "Ressources/MaterialRessource.h"
#include "AssetFactory.h"
#include "Assets\Material.refl.h" // automatically generated reflection header
namespace Rendering {

	class ViewportInstance;
	class Texture2D;

	REFLECT()
		class Material : public Asset {
		REFLECT_BODY()
		public:

			friend TAssetFactory<Material>;

			/** Draw material on desired viewport */
			void PreDraw(ViewportInstance* writeViewport, const size_t& imageIndex);
			void Draw(VkCommandBuffer commandBuffer, ViewportInstance* writeViewport, const size_t& imageIndex);

			virtual Texture2D* GetAssetIcon() const;
			inline VkPipelineLayout& GetPipelineLayout() { return materialRessource->GetPipelineLayout(); }

		private:

			Material(const SMaterialStaticProperties& inMaterialProperties, const String& assetName, const SMaterialDynamicProperties& inMaterialDynamicProperties = {});

			inline static Texture2D* materialIcon = nullptr;

			/** Material properties */
			const SMaterialDynamicProperties materialDynamicProperties;
			const SMaterialStaticProperties materialProperties;

			/** Material ressources */
			MaterialRessourceItem* materialRessource;
	};


	template<>
	struct TAssetFactory<Material> {
		template<typename ...Args>
		inline static Material* CreateFromData(Args&&... inArgs) { return new Material(std::forward<Args>(inArgs)...); }
		inline static Material* MakeTransient(Material* inMat) { inMat->MakeConstant(); return inMat; }
	};
}
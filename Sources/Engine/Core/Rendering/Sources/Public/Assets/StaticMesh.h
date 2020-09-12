#pragma once

#include "Assets/Asset.h"
#include "AssetFactory.h"
#include "Assets\StaticMesh.refl.h" // automatically generated reflection header

namespace Rendering {

	struct MeshRessource;
	class ViewportInstance;


	REFLECT()
		class StaticMesh : public Asset {
		REFLECT_BODY()
		public:

			friend TAssetFactory<StaticMesh>;

			virtual ~StaticMesh();

			virtual Texture2D* GetAssetIcon() const;


			void PreDraw(ViewportInstance* writeViewport, const size_t& imageIndex);

			void Draw(VkCommandBuffer& inCommandBuffer, ViewportInstance* viewport, const size_t& imageIndex, Mat4f objectTransform);


		private:
			static void CreateMeshRessource(SMeshSectionData* inSectionDatas);

			inline static Texture2D* staticMeshIcon = nullptr;

			RPROPERTY()
			std::vector<SMeshSectionData> sectionDatas;


			StaticMesh(const std::vector<SMeshSectionData>& inSectionDatas, const String& assetName, bool bLoadAsync = true);

	};

	template<>
	struct TAssetFactory<StaticMesh> {
		template<typename ...Args>
		inline static StaticMesh* CreateFromData(Args&&... inArgs) { return new StaticMesh(std::forward<Args>(inArgs)...); }
		inline static StaticMesh* MakeTransient(StaticMesh* inMat) { inMat->MakeConstant(); return inMat; }
	};
}

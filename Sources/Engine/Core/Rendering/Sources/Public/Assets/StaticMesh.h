#pragma once

#include "Assets/Asset.h"
#include "AssetFactory.h"
#include "Assets\StaticMesh.refl.h" // automatically generated reflection header

namespace Rendering {

	struct MeshRessource;

	REFLECT()
		class StaticMesh : public Asset {
		REFLECT_BODY()
		public:

			friend TAssetFactory<StaticMesh>;

			virtual ~StaticMesh();

			inline std::vector<SMeshSectionData>& GetSectionData() { return sectionDatas; }

			virtual Texture2D* GetAssetIcon() const;

		private:

			inline static Texture2D* staticMeshIcon = nullptr;


			std::vector<SMeshSectionData> sectionDatas;


			StaticMesh(const std::vector<SMeshSectionData>& inSectionDatas, const String& assetName);

	};

	template<>
	struct TAssetFactory<StaticMesh> {
		template<typename ...Args>
		inline static StaticMesh* CreateFromData(Args&&... inArgs) { return new StaticMesh(std::forward<Args>(inArgs)...); }
		inline static StaticMesh* MakeTransient(StaticMesh* inMat) { inMat->MakeConstant(); return inMat; }
	};
}

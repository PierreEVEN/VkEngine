#pragma once

#include "Constants.h"

namespace Rendering {

	class Asset;

	DECLARE_DELEGATE_MULTICAST(DeleteAssetEvent, Asset*);

	class Asset {
	public:
		Asset();
		virtual ~Asset();

		inline static DeleteAssetEvent OnDeleteAsset;

	protected:

		virtual void CreateRessources() = 0;
		virtual void DeleteRessources() = 0;

	private:

		TextureRessource* thumbnail;

		inline static std::vector<Asset*> assets;
	};
}
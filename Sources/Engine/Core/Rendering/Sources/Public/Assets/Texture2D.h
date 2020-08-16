#pragma once
#include "Asset.h"

namespace Rendering {

	class Texture2D : public Asset {
	public:
		Texture2D() : Asset() {}

		virtual void CreateRessources() {}
		virtual void DeleteRessources() {}

	};

}
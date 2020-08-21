#pragma once

#include "SubWindow.h"

namespace Rendering::Importers {
	class GltfImporter;
}

namespace Rendering {

	class StaticMeshImportWindow : public SubWindow {
	public:
		StaticMeshImportWindow();

	protected:
		virtual void DrawContent(const size_t& imageIndex);

	private:

		void DrawImportDetails();


		void DrawSceneDetails();
		void DrawNode(const size_t& nodeIndex);
		void Drawmesh(const size_t& meshIndex);


		void OnFileExplorerChosedPath(const String& path);
		inline void OnFillExplorerClosed() { RequestClose(); }

		Importers::GltfImporter* importer;

		String filePath;
	};
}
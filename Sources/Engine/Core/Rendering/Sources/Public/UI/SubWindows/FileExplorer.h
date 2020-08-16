#pragma once

#include "SubWindow.h"
#include "EngineConfig.h"

namespace Rendering {


	inline TConVar<String> G_LAST_CONTENT_BROWSER_DIRECTORY(Config::G_CONFIG_INI, "Editor", "LastContentBrowserDirectory", "");

	DECLARE_DELEGATE_MULTICAST(ApplyExplorerEvent, const String&);
	DECLARE_DELEGATE_MULTICAST(CancelExplorerEvent);

	class FileExplorer : public SubWindow {
	public:
		FileExplorer(const String& defaultPath, const String& windowName, SubWindow* parent, const std::vector<String>& inExtensionFilters = {});

		ApplyExplorerEvent OnApplyPath;
		CancelExplorerEvent OnCancelExplorer;

	protected:

		virtual void DrawContent(const size_t& imageIndex);

		virtual ~FileExplorer();

	private:

		void SetCurrentPath(const String& path);

		void DrawDirContent(const String& dirPath, const size_t& imageIndex);

		bool bHasBeenValidated = false;

		char currentPath[256];
		String selectedElement;
		std::vector<std::vector<String>> extensionFilters;
		int currentFilter = 0;
	};


}
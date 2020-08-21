#pragma once

#include "Constants.h"
#include "Ressources/TextureRessource.h"
#include "UI/UiRessources.h"

namespace Rendering {

	class SubWindow;

	DECLARE_DELEGATE_MULTICAST(DeleteWindowEvent, SubWindow*);

	class SubWindow {
	public:
		SubWindow(const String& name, SubWindow* parentWindow = nullptr, bool bInRenderInParent = false);

		static void ProcessSubWindows(const size_t& imageIndex);

		DeleteWindowEvent OnDeleteWindow;

		inline void RequestClose() { bKeepOpen = false; }

	protected:

		virtual void DrawContent(const size_t& imageIndex) = 0;

		virtual ~SubWindow();



	private:

		bool bIsDocked = false;

		size_t myWindowId;
		void Draw(const size_t& imageIndex);
		String windowName;

		SubWindow* parent;
		std::vector<SubWindow*> childs;

		bool bKeepOpen;
		bool bRenderInParent;

		inline static std::unordered_map<String, size_t> windowIdMap;
		inline static std::vector<SubWindow*> registeredWindows;
	};
}
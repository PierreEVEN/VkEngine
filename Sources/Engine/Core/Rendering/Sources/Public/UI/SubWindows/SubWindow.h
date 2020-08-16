#pragma once

#include "Constants.h"
#include "Ressources/Texture.h"

namespace Rendering {

	class SubWindow;

	DECLARE_DELEGATE_MULTICAST(DeleteWindowEvent, SubWindow*);

	class SubWindow {
	public:
		SubWindow(const String& name, SubWindow* parentWindow = nullptr);

		static void ProcessSubWindows(const size_t& imageIndex);

		DeleteWindowEvent OnDeleteWindow;

		static void LoadUIRessources();
		static void DestroyUIRessources();

		inline void RequestClose() { bKeepOpen = false; }

	protected:

		virtual void DrawContent(const size_t& imageIndex) = 0;

		virtual ~SubWindow();


		inline static TextureRessource* upArrowCircleIcon = nullptr;
		inline static TextureRessource* directoryIcon = nullptr;
		inline static TextureRessource* fileIcon = nullptr;


	private:



		size_t myWindowId;
		void Draw(const size_t& imageIndex);
		String windowName;

		SubWindow* parent;
		std::vector<SubWindow*> childs;

		bool bKeepOpen;

		inline static size_t currentWindowID = 0;
		inline static std::vector<SubWindow*> registeredWindows;
	};
}
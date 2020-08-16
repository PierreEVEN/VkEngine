#pragma once
#include "SubWindow.h"

namespace Rendering {

	class ViewportInstance;

	class DebugUI : public SubWindow {
	public:
		DebugUI(ViewportInstance* parentScene) : SubWindow("Debug UI") {
			parent = parentScene;
		}

	protected:

		SVector cameraPosition = SVector(-2, 0, 0.5);
		SRotatorf cameraRotation;
		float fov = 45, mi = 0.1f, mx = 1000.f;

		float fpsHistory[1000];
		int fpsFrameIndex = 0;
		float maxFpsHistory = 0;

		void DrawContent(const size_t& imageIndex);
		ViewportInstance* parent;
	};
}
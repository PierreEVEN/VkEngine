#pragma once

#include "Constants.h"

namespace Rendering
{
	class ViewportInstance;

	class FrameObjects
	{
	public:

		FrameObjects(ViewportInstance* viewportInstance);
		~FrameObjects();

		inline VkFence& GetInFlightFence(const size_t& imageIndex) { return inFlightFences[imageIndex]; }
		inline VkFence& GetImageInFlightFence(const size_t& imageIndex) { return imagesInFlight[imageIndex]; }
		inline VkSemaphore& GetImageAvailableAvailableSemaphore(const size_t& imageIndex) { return imageAvailableSemaphores[imageIndex]; }
		inline VkSemaphore& GetRenderFinnishedSemaphore(const size_t& imageIndex) { return renderFinishedSemaphores[imageIndex]; }

	private:

		void CreateSemaphores(ViewportInstance* viewportInstance);
		void DestroySemaphores();

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
	};
}
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vk_mem_alloc.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <set>
#include <array>
#include <optional>

#include "IO/Log.h"
#include "EngineTypes.h"

#define VERTEX_ENABLE_LOCATION
#define VERTEX_ENABLE_TEX_COORD
#define VERTEX_ENABLE_COLOR
#define VERTEX_ENABLE_NORMAL
#define VERTEX_ENABLE_TANGENT
//#define VERTEX_ENABLE_BITANGENT


namespace Rendering
{
	struct MatrixUniformBufferObject {
		alignas(16) glm::mat4 worldProjection;
		alignas(16) glm::mat4 viewMatrix;
		alignas(16) glm::vec4 cameraLocation;
		alignas(16) float time;
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices {
		QueueFamilyIndices() {}
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		inline bool IsComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
	};

	struct Vertex
	{
#ifdef VERTEX_ENABLE_LOCATION
		glm::vec3 pos;
#endif
#ifdef VERTEX_ENABLE_TEX_COORD
		glm::vec3 color;
#endif
#ifdef VERTEX_ENABLE_COLOR
		glm::vec2 texCoord;
#endif
#ifdef VERTEX_ENABLE_NORMAL
		glm::vec3 normal;
#endif
#ifdef VERTEX_ENABLE_TANGENT
		glm::vec3 tangent;
#endif
#ifdef VERTEX_ENABLE_BITANGENT
		glm::vec3 bitangent;
#endif

		bool operator==(const Vertex& other) const;

		static VkVertexInputBindingDescription GetBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
	};

}


namespace std {
	template<> struct hash< Rendering::Vertex> {
		size_t operator()(Rendering::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

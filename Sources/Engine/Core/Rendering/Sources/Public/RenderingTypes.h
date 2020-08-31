#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vk_mem_alloc.h>

#include <UI/imgui.h>
#include <UI/imgui_internal.h>
#include <UI/imgui_impl_glfw.h>
#include <UI/imgui_impl_vulkan.h>

#include "EngineMinimal.h"

#define VERTEX_ENABLE_LOCATION
#define VERTEX_ENABLE_TEX_COORD
#define VERTEX_ENABLE_COLOR
#define VERTEX_ENABLE_NORMAL
#define VERTEX_ENABLE_TANGENT
//#define VERTEX_ENABLE_BITANGENT


namespace Rendering
{
	struct MatrixUniformBufferObject {
		alignas(16) Mat4f worldProjection;
		alignas(16) Mat4f viewMatrix;
		alignas(16) SVector cameraLocation;
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
		std::optional<uint32_t> transfertFamily;
		std::optional<uint32_t> presentFamily;
		inline bool IsComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
	};

	struct Vertex
	{

#ifdef VERTEX_ENABLE_LOCATION
		SVector pos;
#endif
#ifdef VERTEX_ENABLE_TEX_COORD
		SLinearColor color;
#endif
#ifdef VERTEX_ENABLE_COLOR
		SVector2D texCoord;
#endif
#ifdef VERTEX_ENABLE_NORMAL
		SVector normal;
#endif
#ifdef VERTEX_ENABLE_TANGENT
		SVector tangent;
#endif
#ifdef VERTEX_ENABLE_BITANGENT
		SVector bitangent;
#endif

		inline bool operator==(const Vertex& other) const
		{
#ifdef VERTEX_ENABLE_LOCATION
			if (pos != other.pos) return false;
#endif
#ifdef VERTEX_ENABLE_TEX_COORD
			if (texCoord != other.texCoord) return false;
#endif
#ifdef VERTEX_ENABLE_NORMAL
			if (normal != other.normal && (abs(normal.x - other.normal.x) > 0.000001f || abs(normal.y - other.normal.y) > 0.000001f || abs(normal.z - other.normal.z) > 0.000001f)) return false;
#endif
#ifdef VERTEX_ENABLE_COLOR
			if (color != other.color) return false;
#endif
#ifdef VERTEX_ENABLE_TANGENT
			if (tangent != other.tangent) return false;
#endif
#ifdef VERTEX_ENABLE_BITANGENT
			if (bitangent != other.bitangent) return false;
#endif
			return true;
		}

		inline operator String() const {
			return "{p=" + pos + ", n=" + normal + ", tc=" + texCoord + ", col=" + color + '}';
		}

		static VkVertexInputBindingDescription GetBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
	};

	struct SMeshSectionData {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		Mat4d sectionTransform;
		class Material* materialLink = nullptr;
		struct MeshRessource* MeshLink = nullptr;
	};

}

MAKE_HASHABLE(Rendering::Vertex, t.pos, t.color, t.texCoord, t.normal);

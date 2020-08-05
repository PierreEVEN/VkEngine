#include "RenderingTypes.h"

bool Rendering::Vertex::operator==(const Vertex& other) const
{
#ifdef VERTEX_ENABLE_LOCATION
	if (pos != other.pos) return false;
#endif
#ifdef VERTEX_ENABLE_TEX_COORD
	if (texCoord != other.texCoord) return false;
#endif
#ifdef VERTEX_ENABLE_COLOR
	if (color != other.color) return false;
#endif
#ifdef VERTEX_ENABLE_NORMAL
	if (normal != other.normal) return false;
#endif
#ifdef VERTEX_ENABLE_TANGENT
	if (tangent != other.tangent) return false;
#endif
#ifdef VERTEX_ENABLE_BITANGENT
	if (bitangent != other.bitangent) return false;
#endif
	return true;
}

VkVertexInputBindingDescription Rendering::Vertex::GetBindingDescription()
{
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

std::vector<VkVertexInputAttributeDescription> Rendering::Vertex::GetAttributeDescriptions() {
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

	VkVertexInputAttributeDescription newAttribute;
	uint8_t currentLocation = 0;

#ifdef VERTEX_ENABLE_LOCATION
	newAttribute.binding = 0;
	newAttribute.location = currentLocation;
	newAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	newAttribute.offset = offsetof(Vertex, pos);
	attributeDescriptions.push_back(newAttribute);
	currentLocation++;
#endif
#ifdef VERTEX_ENABLE_TEX_COORD
	newAttribute.binding = 0;
	newAttribute.location = currentLocation;
	newAttribute.format = VK_FORMAT_R32G32_SFLOAT;
	newAttribute.offset = offsetof(Vertex, texCoord);
	attributeDescriptions.push_back(newAttribute);
	currentLocation++;
#endif
#ifdef VERTEX_ENABLE_COLOR
	newAttribute.binding = 0;
	newAttribute.location = currentLocation;
	newAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	newAttribute.offset = offsetof(Vertex, color);
	attributeDescriptions.push_back(newAttribute);
	currentLocation++;
#endif
#ifdef VERTEX_ENABLE_NORMAL
	newAttribute.binding = 0;
	newAttribute.location = currentLocation;
	newAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	newAttribute.offset = offsetof(Vertex, normal);
	attributeDescriptions.push_back(newAttribute);
	currentLocation++;
#endif
#ifdef VERTEX_ENABLE_TANGENT
	newAttribute.binding = 0;
	newAttribute.location = currentLocation;
	newAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	newAttribute.offset = offsetof(Vertex, tangent);
	attributeDescriptions.push_back(newAttribute);
	currentLocation++;
#endif
#ifdef VERTEX_ENABLE_BITANGENT
	newAttribute.binding = 0;
	newAttribute.location = currentLocation;
	newAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	newAttribute.offset = offsetof(Vertex, bitangent);
	attributeDescriptions.push_back(newAttribute);
	currentLocation++;
#endif
	return attributeDescriptions;
}
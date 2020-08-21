#pragma once

#include "Constants.h"


#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "Ressources/MeshRessource.h"

namespace Rendering::Importers
{
	class GltfImporter;

	/** ACCESSOR */
	struct GltfAccessor {
		GltfAccessor() = default;
		GltfAccessor(const rapidjson::Value& source);

		int64_t bufferView = -1;
		int64_t byteOffset = 0;
		int64_t componentType = -1;
		int64_t count = -1;
		String type = "Unknown";
		std::vector<double> max;
		std::vector<double> min;
	};

	/** BUFFER */
	struct GltfBuffer {
		GltfBuffer() = default;
		GltfBuffer(const rapidjson::Value& source, const String& sourceFilePath);
		~GltfBuffer() { delete buffer; }

		char* GetBuffer();
		String sourcePath = nullptr;
		int64_t byteLength = -1;
		String uri = "Unknown";
	private:
		char* buffer = nullptr;
	};

	/** BUFFER VIEW */
	struct GltfBufferView {
		GltfBufferView() = default;
		GltfBufferView(const rapidjson::Value& source);


		int64_t buffer = -1;
		int64_t byteOffset = 0;
		int64_t byteLength = -1;
		int64_t byteStride = -1;
		int64_t target = -1;
	};

	/** MATERIAL */
	struct GltfImage {
		GltfImage() = default;
		GltfImage(const rapidjson::Value& source, const String& inFilePath);

		String uri = "";
		String mimeType = "Unknown";

		bool bImportTexture = false;
		Texture2D* linkedTexture = nullptr;

		void OnSelectedTexture(Texture2D* inTexture) { linkedTexture = inTexture; }

		Texture2D* GetTexture();
	private:
		String filePath;
		Texture2D* ImportTexture();
	};

	/** MATERIAL */
	struct GltfTexture {
		GltfTexture() = default;
		GltfTexture(const rapidjson::Value& source);

		int64_t sourceId = -1;
	};

	/** MATERIAL */
	struct GLtfMaterial {
		GLtfMaterial() = default;
		GLtfMaterial(const rapidjson::Value& source, GltfImporter* inParent, size_t myIndex);

		String name = "None";
		int64_t baseColorIndex = -1;
		int64_t metallicRoughnessIndex = -1;
		int64_t normalTextureIndex = -1;

		Material* GetMaterial();

		bool bImport = false;
		Material* selectedMaterial;

		inline void OnSelectedMaterial(Material* selectedAsset) { selectedMaterial = selectedAsset; }

	private:



		Material* CreateMaterial();
		GltfImporter* parent;
	};

	/** PRIMITIVE */
	struct GltfPrimitive {
		GltfPrimitive() = default;
		GltfPrimitive(const rapidjson::Value& source, GltfImporter* inParent);

		void LoadData();

		std::vector<Vertex> vertices;
		std::vector<uint32_t> triangles;

		int64_t attributePosition = -1;
		int64_t attributeNormal = -1;
		int64_t attributeTangent = -1;
		int64_t attributeTexCoord = -1;
		int64_t attributeColor = -1;
		int64_t indices = -1;

		int64_t mode = -1;
		int64_t material = -1;
	private:
		GltfImporter* parent;

	};

	/** MESH */
	struct GLtfMesh {
		GLtfMesh() = default;
		GLtfMesh(const rapidjson::Value& source, GltfImporter* inParent);

		std::vector<GltfPrimitive> primitives;
		String name = "none";
	};

	/** NODE */
	struct GltfNode {
		GltfNode() = default;
		GltfNode(const rapidjson::Value& source);

		std::vector<size_t> children;
		int64_t mesh = -1;
		Mat4d nodeMatrix;
	};

	/** SCENE */
	struct GltfScene {
		GltfScene() = default;
		GltfScene(const rapidjson::Value& source);

		std::vector<size_t> nodes;
	};

	struct GltfAssetData {
		GltfAssetData() = default;
		GltfAssetData(const rapidjson::Value& source);

		String generator = "Unknown";
		String version = "Unknown";
		String copyright = "Unknown";
	};

	class GltfImporter {
	public:

		GltfImporter(const String& filePath) : iFilePath(filePath) { FillJsonData(filePath); }

		GltfAssetData assetData;
		std::vector<GltfScene> scenes;
		std::vector<GltfNode> nodes;
		std::vector<GLtfMesh> meshes;
		std::vector<GltfAccessor> accessors;
		std::vector<GLtfMaterial> materials;
		std::vector<GltfImage> images;
		std::vector<GltfTexture> textures;
		std::vector<GltfBufferView> bufferViews;
		std::vector<GltfBuffer> buffers;

		inline const String& GetPath() const { return iFilePath; }

		std::vector<SMeshSectionData> GenerateData();

	private:

		std::vector<SMeshSectionData> GetNodeData(size_t NodeIndex);

		std::vector<Rendering::SMeshSectionData> GetMeshData(size_t MeshIndex);

		String iFilePath;

		void FillJsonData(const String& filePath);
	};
}
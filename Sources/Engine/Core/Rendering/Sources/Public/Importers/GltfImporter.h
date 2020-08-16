#pragma once

#include "Constants.h"


#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include <fstream>
#include "Ressources/Mesh.h"
#include "UI/SubWindows/SubWindow.h"

namespace Rendering::Importers
{
	struct GltfObject;

	/** ACCESSOR */
	struct GltfAccessor {
		GltfAccessor() = default;
		GltfAccessor(const rapidjson::Value& source);

		void DisplayData();

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
		GltfBuffer(const rapidjson::Value& source, const char* sourceFilePath);
		~GltfBuffer() { delete buffer; }

		char* GetBuffer();
		void DisplayData();

		const char* sourcePath = nullptr;
		int64_t byteLength = -1;
		String uri = "Unknown";
	private:
		char* buffer = nullptr;
	};

	/** BUFFER VIEW */
	struct GltfBufferView {
		GltfBufferView() = default;
		GltfBufferView(const rapidjson::Value& source);

		void DisplayData();


		int64_t buffer = -1;
		int64_t byteOffset = 0;
		int64_t byteLength = -1;
		int64_t byteStride = -1;
		int64_t target = -1;
	};

	/** MATERIAL */
	struct GLtfMaterial {
		GLtfMaterial() = default;
		GLtfMaterial(const rapidjson::Value& source);

		void DisplayData();

	};

	/** PRIMITIVE */
	struct GltfPrimitive {
		GltfPrimitive() = default;
		GltfPrimitive(const rapidjson::Value& source, GltfObject* inParent);

		void DisplayData();
		void LoadData();

		std::vector<Vertex> vertices;
		std::vector<uint32_t> triangles;

		GltfObject* parent;

		int64_t attributePosition = -1;
		int64_t attributeNormal = -1;
		int64_t attributeTangent = -1;
		int64_t attributeTexCoord = -1;
		int64_t attributeColor = -1;
		int64_t indices = -1;

		int64_t mode = -1;
		int64_t material = -1;
	};

	/** MESH */
	struct GLtfMesh {
		GLtfMesh() = default;
		GLtfMesh(const rapidjson::Value& source, GltfObject* inParent);

		void DisplayData();

		std::vector<GltfPrimitive> primitives;
		String name = "none";
	};

	/** NODE */
	struct GltfNode {
		GltfNode() = default;
		GltfNode(const rapidjson::Value& source);

		void DisplayData();

		std::vector<size_t> children;
		size_t mesh = -1;
		Mat4d nodeMatrix;
	};

	/** SCENE */
	struct GltfScene {
		GltfScene() = default;
		GltfScene(const rapidjson::Value& source);

		void DisplayData();

		std::vector<size_t> nodes;
	};

	struct GltfAssetData {
		GltfAssetData() = default;
		GltfAssetData(const rapidjson::Value& source);

		void DisplayData();

		String generator = "Unknown";
		String version = "Unknown";
		String copyright = "Unknown";
	};

	struct GltfObject {
		GltfAssetData assetData;
		std::vector<GltfScene> scenes;
		std::vector<GltfNode> nodes;
		std::vector<GLtfMesh> meshes;
		std::vector<GltfAccessor> accessors;
		std::vector<GLtfMaterial> materials;
		std::vector<GltfBufferView> bufferViews;
		std::vector<GltfBuffer> buffers;
	};

	class GltfImporter : public SubWindow {
	public:

		GltfImporter();

		MeshRessource* generatedMesh = nullptr;

	protected:

		~GltfImporter() { if (generatedMesh) delete generatedMesh; }

		virtual void DrawContent(const size_t& imageIndex);

		void OnFileExplorerChosedPath(const String& path);

		void OnFillExplorerClosed() {
			RequestClose();
		}

	private:

		void CreateRessources();

		void FillJsonData();

		size_t importState = 0;
		size_t importCount = 0;

		GltfObject objectData;

		bool bHasFinnishedImport = false;

		const char* filePath = nullptr;
	};
}
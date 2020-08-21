#include "Importers/GltfImporter.h"
#include <filesystem>
#include "ThirdParty/base64.h"
#include "UI/SubWindows/FileExplorer.h"
#include "Assets/StaticMesh.h"
#include "Scene/SceneComponents/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include <fstream>
#include "Assets/Texture2D.h"
#include "Ressources/MaterialRessource.h"
#include "Assets/AssetFactory.h"
#include "Assets/Material.h"

void Rendering::Importers::GltfImporter::FillJsonData(const String& filePath)
{
	std::ifstream inputFile(filePath.GetData());
	if (!inputFile.is_open()) { LOG_ASSERT(String("cannot open file") + filePath.GetData()); }

	String fileString;
	char* line = new char[1000];
	while (inputFile.getline(line, 1000, '\n'))
		fileString += line;
	delete line;

	rapidjson::Document d;
	d.Parse(fileString.GetData());

	assetData = GltfAssetData(d["asset"]);

	const rapidjson::Value& scenesarray = d["scenes"];
	scenes.resize(scenesarray.Size());
	for (rapidjson::SizeType i = 0; i < scenesarray.Size(); ++i) {
		scenes[i] = GltfScene(scenesarray[i]);
	}

	const rapidjson::Value& nodesArray = d["nodes"];
	nodes.resize(nodesArray.Size());
	for (rapidjson::SizeType i = 0; i < nodesArray.Size(); ++i) {
		nodes[i] = GltfNode(nodesArray[i]);
	}

	const rapidjson::Value& meshesArray = d["meshes"];
	meshes.resize(meshesArray.Size());
	for (rapidjson::SizeType i = 0; i < meshesArray.Size(); ++i) {
		meshes[i] = GLtfMesh(meshesArray[i], this);
	}

	const rapidjson::Value& accessorArray = d["accessors"];
	accessors.resize(accessorArray.Size());
	for (rapidjson::SizeType i = 0; i < accessorArray.Size(); ++i) {
		accessors[i] = GltfAccessor(accessorArray[i]);
	}

	const rapidjson::Value& materialArray = d["materials"];
	materials.resize(materialArray.Size());
	for (rapidjson::SizeType i = 0; i < materialArray.Size(); ++i) {
		materials[i] = GLtfMaterial(materialArray[i], this, i);
	}

	const rapidjson::Value& imagesArray = d["images"];
	images.resize(imagesArray.Size());
	for (rapidjson::SizeType i = 0; i < imagesArray.Size(); ++i) {
		images[i] = GltfImage(imagesArray[i], filePath);
	}

	
	const rapidjson::Value& bufferViewArray = d["bufferViews"];
	bufferViews.resize(bufferViewArray.Size());
	for (rapidjson::SizeType i = 0; i < bufferViewArray.Size(); ++i) {
		bufferViews[i] = GltfBufferView(bufferViewArray[i]);
	}

	const rapidjson::Value& bufferArray = d["buffers"];
	buffers.resize(bufferArray.Size());
	for (rapidjson::SizeType i = 0; i < bufferArray.Size(); ++i) {
		buffers[i] = GltfBuffer(bufferArray[i], filePath);
	}

	const rapidjson::Value& texturesArray = d["textures"];
	textures.resize(texturesArray.Size());
	for (rapidjson::SizeType i = 0; i < texturesArray.Size(); ++i) {
		textures[i] = GltfTexture(texturesArray[i]);
	}

	inputFile.close();
}

std::vector<Rendering::SMeshSectionData> Rendering::Importers::GltfImporter::GenerateData()
{
	std::vector<SMeshSectionData> output;
	for (auto& scene : scenes) {
		for (int i = 0; i < scene.nodes.size(); ++i) {
			for (auto& elem : GetNodeData(scene.nodes[i])) {
				output.push_back(elem);
			}
		}
	}
	return output;
}

std::vector<Rendering::SMeshSectionData> Rendering::Importers::GltfImporter::GetNodeData(size_t NodeIndex)
{
	std::vector<SMeshSectionData> output;
	for (auto& cild : nodes[NodeIndex].children) {
		for (const auto& childData : GetNodeData(cild)) {
			output.push_back(childData);
		}
	}

	if (nodes[NodeIndex].mesh >= 0) {
		for (auto& childData : GetMeshData(nodes[NodeIndex].mesh)) {
			output.push_back(childData);
		}
	}
	return output;
}

std::vector<Rendering::SMeshSectionData> Rendering::Importers::GltfImporter::GetMeshData(size_t MeshIndex)
{
	std::vector<SMeshSectionData> output;
	for (GltfPrimitive& primitive : meshes[MeshIndex].primitives) {
		primitive.LoadData();
		SMeshSectionData newItem;

		newItem.indices = primitive.triangles;
		newItem.vertices = primitive.vertices;
		if (primitive.material >= 0) {
			newItem.materialLink = materials[primitive.material].GetMaterial();
		}
		else {
			newItem.materialLink = G_DEFAULT_MATERIAL;
		}

		output.push_back(newItem);
	}
	return output;
}

/************************************************************************/
/* PARSING                                                              */
/************************************************************************/

Rendering::Importers::GltfAssetData::GltfAssetData(const rapidjson::Value& source)
{
	if (source["generator"].IsString()) generator = source["generator"].GetString();
	if (source["version"].IsString()) version = source["version"].GetString();
	if (source["copyright"].IsString()) copyright = source["copyright"].GetString();
}


Rendering::Importers::GltfScene::GltfScene(const rapidjson::Value& source) {
	const rapidjson::Value& nodeArray = source["nodes"];
	nodes.resize(nodeArray.Size());
	for (rapidjson::SizeType i = 0; i < nodeArray.Size(); ++i) {
		nodes[i] = nodeArray[i].GetInt();
	}
}

Rendering::Importers::GltfNode::GltfNode(const rapidjson::Value& source)
{
	if (source["mesh"].IsInt()) mesh = source["mesh"].GetInt();

	if (source["children"].IsArray())
	{
		const rapidjson::Value& childrenArray = source["children"];
		children.resize(childrenArray.Size());
		for (rapidjson::SizeType i = 0; i < childrenArray.Size(); ++i) {
			children[i] = childrenArray[i].GetInt();
		}
	}

	if (source["matrix"].IsArray()) {
		const rapidjson::Value& matrixArray = source["matrix"];
		for (rapidjson::SizeType i = 0; i < 16; ++i) {
			nodeMatrix.coords[i] = matrixArray[i].GetDouble();
		}
	}
	else {
		SVectorDouble position;
		SQuatd rotation;
		SVectorDouble scale(1);
		if (source["translation"].IsArray()) {
			const rapidjson::Value& translationArray = source["translation"];
			for (rapidjson::SizeType i = 0; i < 3; ++i) {
				position.coords[i] = translationArray[i].GetDouble();
			}
		}
		if (source["rotation"].IsArray()) {
			const rapidjson::Value& rotationArray = source["rotation"];
			for (rapidjson::SizeType i = 0; i < 4; ++i) {
				rotation.coords[i] = rotationArray[i].GetDouble();
			}
		}
		if (source["scale"].IsArray()) {
			const rapidjson::Value& scaleArray = source["scale"];
			for (rapidjson::SizeType i = 0; i < 3; ++i) {
				scale.coords[i] = scaleArray[i].GetDouble();
			}
		}

		nodeMatrix = Mat4d(position, rotation, scale);
	}
}

Rendering::Importers::GLtfMesh::GLtfMesh(const rapidjson::Value& source, GltfImporter* inParent)
{
	name = source["name"].GetString();
	const rapidjson::Value& primitiveArray = source["primitives"];
	primitives.resize(primitiveArray.Size());
	for (rapidjson::SizeType i = 0; i < primitiveArray.Size(); ++i) {
		primitives[i] = GltfPrimitive(primitiveArray[i], inParent);
	}
}

Rendering::Importers::GltfPrimitive::GltfPrimitive(const rapidjson::Value& source, GltfImporter* inParent)
	: parent(inParent)
{
	if (source["attributes"]["POSITION"].IsInt()) attributePosition = source["attributes"]["POSITION"].GetInt();
	if (source["attributes"]["NORMAL"].IsInt()) attributeNormal = source["attributes"]["NORMAL"].GetInt();
	if (source["attributes"]["TEXCOORD_0"].IsInt()) attributeTexCoord = source["attributes"]["TEXCOORD_0"].GetInt();
	if (source["attributes"]["TANGENT"].IsInt()) attributeTangent = source["attributes"]["TANGENT"].GetInt();

	if (source["indices"].IsInt()) indices = source["indices"].GetInt();
	if (source["mode"].IsInt()) mode = source["mode"].GetInt();
	if (source["material"].IsInt()) material = source["material"].GetInt();
}

void Rendering::Importers::GltfPrimitive::LoadData()
{
	if (attributePosition < 0) { return;  LOG_ERROR("Gltf importer require vertex posisions"); }


	GltfAccessor& posAccessor = parent->accessors[attributePosition];
	GltfBufferView& posBufferView = parent->bufferViews[posAccessor.bufferView];
	char* vectorData = parent->buffers[posBufferView.buffer].GetBuffer() + posBufferView.byteOffset + posAccessor.byteOffset;
	vertices.resize(posAccessor.count);
	for (int i = 0; i < posAccessor.count; ++i)
	{
		vertices[i].pos = ((SVector*)vectorData)[i];
	}

	if (indices >= 0) {
		GltfAccessor& indicesAccessor = parent->accessors[indices];
		GltfBufferView& trianglesView = parent->bufferViews[indicesAccessor.bufferView];
		char* trianglesData = parent->buffers[trianglesView.buffer].GetBuffer() + trianglesView.byteOffset + indicesAccessor.byteOffset;
		triangles.resize(indicesAccessor.count);
		for (int i = 0; i < indicesAccessor.count; ++i)
		{
			if (indicesAccessor.componentType == 5123) { //unsigned short
				triangles[i] = ((uint16_t*)trianglesData)[i];
			}
		}
	}

	if (attributeNormal >= 0) {
		GltfAccessor& normalAccessor = parent->accessors[attributeNormal];
		GltfBufferView& normalViews = parent->bufferViews[normalAccessor.bufferView];
		char* normalDatas = (parent->buffers[normalViews.buffer].GetBuffer() + normalViews.byteOffset + normalAccessor.byteOffset);

		for (int i = 0; i < normalAccessor.count; ++i)
		{
			vertices[i].normal = ((SVector*)normalDatas)[i];
		}
	}

	if (attributeTexCoord >= 0) {
		GltfAccessor& texCoordsAccessor = parent->accessors[attributeTexCoord];
		GltfBufferView& texCoordView = parent->bufferViews[texCoordsAccessor.bufferView];
		SVector2D* textCoordData = (SVector2D*)(parent->buffers[texCoordView.buffer].GetBuffer() + texCoordView.byteOffset + texCoordsAccessor.byteOffset);
		for (int i = 0; i < texCoordsAccessor.count; ++i)
		{
			vertices[i].texCoord = textCoordData[i];
		}
	}

	if (attributeTangent >= 0) {
		GltfAccessor& tangentAccessor = parent->accessors[attributeTangent];
		GltfBufferView& tangentView = parent->bufferViews[tangentAccessor.bufferView];
		SVector* tangentData = (SVector*)(parent->buffers[tangentView.buffer].GetBuffer() + tangentView.byteOffset + tangentAccessor.byteOffset);
		for (int i = 0; i < tangentAccessor.count; ++i)
		{
			vertices[i].tangent = tangentData[i];
		}
	}
}

Rendering::Importers::GLtfMaterial::GLtfMaterial(const rapidjson::Value& source, GltfImporter* inParent, size_t myIndex)
	: parent(inParent)
{
	if (source["pbrMetallicRoughness"]["baseColorTexture"]["index"].IsInt()) baseColorIndex = source["pbrMetallicRoughness"]["baseColorTexture"]["index"].GetInt();
	if (source["pbrMetallicRoughness"]["metallicRoughnessTexture"]["index"].IsInt()) metallicRoughnessIndex = source["pbrMetallicRoughness"]["metallicRoughnessTexture"]["index"].GetInt();
	if (source["normalTexture"]["index"].IsInt()) normalTextureIndex = source["normalTexture"]["index"].GetInt();
	if (source["name"].IsString()) name = source["name"].GetString();
	else name = String::GetFileShortName(parent->GetPath()) + "_material_" + ToString(myIndex);
}

Rendering::Material* Rendering::Importers::GLtfMaterial::GetMaterial()
{
	if (selectedMaterial) return selectedMaterial;
	if (bImport) return CreateMaterial();
	return G_DEFAULT_MATERIAL;
}

Rendering::Material* Rendering::Importers::GLtfMaterial::CreateMaterial()
{
	/** Default material */
	SMaterialStaticProperties defaultMaterialProperties{};
	defaultMaterialProperties.bUseGlobalUbo = true;
	defaultMaterialProperties.vertexShaderModule = G_DEFAULT_VERTEX_MODULE;
	defaultMaterialProperties.fragmentShaderModule = G_DEFAULT_FRAGMENT_MODULE;
	defaultMaterialProperties.VertexTexture2DCount = 0;
	defaultMaterialProperties.FragmentTexture2DCount = 3;
	defaultMaterialProperties.materialCreationFlag = EMATERIAL_CREATION_FLAG_NONE;

	SMaterialDynamicProperties defaultMaterialDynProperties{};

	if (baseColorIndex >= 0) {
		defaultMaterialDynProperties.fragmentTextures2D.push_back(
			parent->images[parent->textures[baseColorIndex].sourceId].GetTexture()
			);
	}
	else {
		defaultMaterialDynProperties.fragmentTextures2D.push_back(G_DEFAULT_TEXTURE);
	}
	if (metallicRoughnessIndex >= 0) {
		defaultMaterialDynProperties.fragmentTextures2D.push_back(
			parent->images[parent->textures[metallicRoughnessIndex].sourceId].GetTexture()
		);
	}
	else {
		defaultMaterialDynProperties.fragmentTextures2D.push_back(G_DEFAULT_TEXTURE);
	}
	if (normalTextureIndex >= 0) {
		defaultMaterialDynProperties.fragmentTextures2D.push_back(
			parent->images[parent->textures[normalTextureIndex].sourceId].GetTexture()
		);
	}
	else {
		defaultMaterialDynProperties.fragmentTextures2D.push_back(G_DEFAULT_TEXTURE);
	}

	return  TAssetFactory<Material>::MakeTransient(TAssetFactory<Material>::CreateFromData(defaultMaterialProperties, name, defaultMaterialDynProperties));
}

Rendering::Importers::GltfBufferView::GltfBufferView(const rapidjson::Value& source)
{
	if (source["buffer"].IsInt()) buffer = source["buffer"].GetInt();
	if (source["byteOffset"].IsInt()) byteOffset = source["byteOffset"].GetInt();
	if (source["byteLength"].IsInt()) byteLength = source["byteLength"].GetInt();
	if (source["byteStride"].IsInt()) byteStride = source["byteStride"].GetInt();
	if (source["target"].IsInt()) target = source["target"].GetInt();
}


Rendering::Importers::GltfBuffer::GltfBuffer(const rapidjson::Value& source, const String& sourceFilePath)
	: sourcePath(sourceFilePath)
{
	if (source["byteLength"].IsInt()) byteLength = source["byteLength"].GetInt();
	if (source["uri"].GetString()) uri = source["uri"].GetString();
}

Rendering::Importers::GltfAccessor::GltfAccessor(const rapidjson::Value& source)
{

	if (source["bufferView"].IsInt()) bufferView = source["bufferView"].GetInt();
	if (source["byteOffset"].IsInt()) byteOffset = source["byteOffset"].GetInt();
	if (source["componentType"].IsInt()) componentType = source["componentType"].GetInt();
	if (source["count"].IsInt()) count = source["count"].GetInt();
	if (source["count"].IsInt()) count = source["count"].GetInt();
	if (source["type"].IsString()) type = source["type"].GetString();

	if (source["max"].IsArray())
	{
		const rapidjson::Value& maxArray = source["max"];
		max.resize(maxArray.Size());
		for (rapidjson::SizeType i = 0; i < maxArray.Size(); ++i) {
			max[i] = maxArray[i].GetDouble();
		}
	}

	if (source["min"].IsArray())
	{
		const rapidjson::Value& minArray = source["min"];
		min.resize(minArray.Size());
		for (rapidjson::SizeType i = 0; i < minArray.Size(); ++i) {
			min[i] = minArray[i].GetDouble();
		}
	}
}

Rendering::Importers::GltfImage::GltfImage(const rapidjson::Value& source, const String& inFilePath)
	: filePath(inFilePath)
{
	if (source["uri"].IsString()) uri = source["uri"].GetString();
	if (source["mimeType"].IsString()) mimeType = source["mimeType"].GetString();
}

Rendering::Texture2D* Rendering::Importers::GltfImage::GetTexture()
{
	if (linkedTexture) return linkedTexture;
	else if (bImportTexture) return ImportTexture();
	return G_DEFAULT_TEXTURE;
}

Rendering::Texture2D* Rendering::Importers::GltfImage::ImportTexture()
{
	return TAssetFactory<Texture2D>::MakeTransient(TAssetFactory<Texture2D>::ImportFromPath(String::GetFilePath(filePath) / uri));
}

Rendering::Importers::GltfTexture::GltfTexture(const rapidjson::Value& source)
{
	if (source["source"].IsInt()) sourceId = source["source"].GetInt();
}

/************************************************************************/
/* Binary import                                                        */
/************************************************************************/
char* Rendering::Importers::GltfBuffer::GetBuffer()
{
	if (buffer) {
		return buffer;
	}

	if (String::IsStartingWith(uri, "data:application/octet-stream;base64,"))
	{
		buffer = new char[byteLength];
		String dataToDecode(uri.GetData() + 37);

		memcpy(buffer, base64_decode(dataToDecode.GetData()).c_str(), byteLength);
		return buffer;
	}
	else {
		std::ifstream ifile(String(String::GetFilePath(sourcePath) / uri).GetData(), std::ios_base::binary);
		if (byteLength > 0 && ifile.is_open())
		{
			buffer = new char[byteLength];
			ifile.read(buffer, byteLength);
			ifile.close();
			return buffer;
		}
		LOG_ASSERT("failed to read path " + uri);
	}
	return nullptr;
}

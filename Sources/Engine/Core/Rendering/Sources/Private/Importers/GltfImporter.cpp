#include "Importers/GltfImporter.h"
#include <filesystem>
#include "ThirdParty/base64.h"
#include "UI/SubWindows/FileExplorer.h"

void Rendering::Importers::GltfImporter::FillJsonData()
{
	std::ifstream inputFile(filePath);
	if (!inputFile.is_open()) { LOG_ASSERT(String("cannot open file") + filePath); }

	String fileString;
	char* line = new char[1000];
	while (inputFile.getline(line, 1000, '\n'))
		fileString += line;
	delete line;

	rapidjson::Document d;
	d.Parse(fileString.GetData());

	objectData.assetData = GltfAssetData(d["asset"]);

	const rapidjson::Value& scenesarray = d["scenes"];
	objectData.scenes.resize(scenesarray.Size());
	for (rapidjson::SizeType i = 0; i < scenesarray.Size(); ++i) {
		objectData.scenes[i] = GltfScene(scenesarray[i]);
	}

	const rapidjson::Value& nodesArray = d["nodes"];
	objectData.nodes.resize(nodesArray.Size());
	for (rapidjson::SizeType i = 0; i < nodesArray.Size(); ++i) {
		objectData.nodes[i] = GltfNode(nodesArray[i]);
	}

	const rapidjson::Value& meshesArray = d["meshes"];
	objectData.meshes.resize(meshesArray.Size());
	for (rapidjson::SizeType i = 0; i < meshesArray.Size(); ++i) {
		objectData.meshes[i] = GLtfMesh(meshesArray[i], &objectData);
	}

	const rapidjson::Value& accessorArray = d["accessors"];
	objectData.accessors.resize(accessorArray.Size());
	for (rapidjson::SizeType i = 0; i < accessorArray.Size(); ++i) {
		objectData.accessors[i] = GltfAccessor(accessorArray[i]);
	}

	const rapidjson::Value& materialArray = d["materials"];
	objectData.materials.resize(materialArray.Size());
	for (rapidjson::SizeType i = 0; i < materialArray.Size(); ++i) {
		objectData.materials[i] = GLtfMaterial(materialArray[i]);
	}

	const rapidjson::Value& bufferViewArray = d["bufferViews"];
	objectData.bufferViews.resize(bufferViewArray.Size());
	for (rapidjson::SizeType i = 0; i < bufferViewArray.Size(); ++i) {
		objectData.bufferViews[i] = GltfBufferView(bufferViewArray[i]);
	}

	const rapidjson::Value& bufferArray = d["buffers"];
	objectData.buffers.resize(bufferArray.Size());
	for (rapidjson::SizeType i = 0; i < bufferArray.Size(); ++i) {
		objectData.buffers[i] = GltfBuffer(bufferArray[i], filePath);
	}

	inputFile.close();
}

void Rendering::Importers::GltfImporter::DrawContent(const size_t& imageIndex)
{
	if (!filePath) return;
	ImGui::Text("File : %s", filePath);

	ImGui::Separator();
	if (ImGui::TreeNode(String("asset data ").GetData()))
	{
		objectData.assetData.DisplayData();
		ImGui::TreePop();
	}

	ImGui::Separator();
	if (ImGui::TreeNode(String("Scenes").GetData()))
	{
		for (int i = 0; i < objectData.scenes.size(); ++i) {
			if (ImGui::TreeNode(String("---scene" + ToString(i)).GetData()))
			{
				objectData.scenes[i].DisplayData();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::Separator();
	if (ImGui::TreeNode(String("Nodes").GetData()))
	{
		for (int i = 0; i < objectData.nodes.size(); ++i) {
			if (ImGui::TreeNode(String("---node" + ToString(i)).GetData()))
			{
				objectData.nodes[i].DisplayData();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::Separator();
	if (ImGui::TreeNode(String("Meshes").GetData()))
	{
		for (int i = 0; i < objectData.meshes.size(); ++i) {
			if (ImGui::TreeNode(String("---mesh" + ToString(i)).GetData()))
			{
				objectData.meshes[i].DisplayData();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::Separator();
	if (ImGui::TreeNode(String("accessors").GetData()))
	{
		for (int i = 0; i < objectData.accessors.size(); ++i) {
			if (ImGui::TreeNode(String("---a" + ToString(i)).GetData()))
			{
				objectData.accessors[i].DisplayData();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::Separator();
	if (ImGui::TreeNode(String("materials").GetData()))
	{
		for (int i = 0; i < objectData.materials.size(); ++i) {
			if (ImGui::TreeNode(String("---mat" + ToString(i)).GetData()))
			{
				objectData.materials[i].DisplayData();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::Separator();
	if (ImGui::TreeNode(String("buffer views").GetData()))
	{
		for (int i = 0; i < objectData.bufferViews.size(); ++i) {
			if (ImGui::TreeNode(String("---bv" + ToString(i)).GetData()))
			{
				objectData.bufferViews[i].DisplayData();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::Separator();
	if (ImGui::TreeNode(String("buffers").GetData()))
	{
		for (int i = 0; i < objectData.buffers.size(); ++i) {
			if (ImGui::TreeNode(String("---b" + ToString(i)).GetData()))
			{
				objectData.buffers[i].DisplayData();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::Separator();
	if (ImGui::Button("Create mesh")) CreateRessources();
}

void Rendering::Importers::GltfImporter::CreateRessources()
{
	for (auto& mesh : objectData.meshes)
	{
		for (auto& primitive : mesh.primitives) {
			primitive.LoadData();
		}
	}

	LOG_WARNING("created wip mesh");
	generatedMesh = new MeshRessource(objectData.meshes[0].primitives[0].vertices, objectData.meshes[0].primitives[0].triangles);
}

Rendering::Importers::GltfImporter::GltfImporter()
	: SubWindow("GLTF importer") {
	FileExplorer* explorer = new FileExplorer("./", "Find gltf asset", this, { "gltf", "glb" }, true);
	explorer->OnApplyPath.Add(this, &GltfImporter::OnFileExplorerChosedPath);
	explorer->OnCancelExplorer.Add(this, &GltfImporter::OnFillExplorerClosed);
}

void Rendering::Importers::GltfImporter::OnFileExplorerChosedPath(const String& path)
{
	filePath = new const char[path.Length()];
	memcpy((char*)filePath, path.GetData(), path.Length() + 1);
	FillJsonData();
}

/************************************************************************/
/* Display                                                              */
/************************************************************************/

void Rendering::Importers::GltfAssetData::DisplayData()
{
	ImGui::Text("Generator : %s", generator.GetData());
	ImGui::Text("version : %s", version.GetData());
	ImGui::Text("copyright : %s", copyright.GetData());
}

void Rendering::Importers::GltfScene::DisplayData()
{
	ImGui::Text("nodes : %s", String::ConcatenateArray(nodes).GetData());
}

void Rendering::Importers::GltfNode::DisplayData()
{
	ImGui::Text("mesh : %d", mesh);
	ImGui::Text("Children : %s", String::ConcatenateArray(children).GetData());

	String matrixString = "Node matrix :";
	for (const double& elem : nodeMatrix.coords) {
		matrixString += ToString(elem) + ", ";
	}
	ImGui::Text(matrixString.GetData());
}

void Rendering::Importers::GLtfMesh::DisplayData()
{
	ImGui::Text("Name : %s", name.GetData());
	if (ImGui::TreeNode(String("primitives").GetData()))
	{
		for (int i = 0; i < primitives.size(); ++i) {
			if (ImGui::TreeNode(String("---p" + ToString(i)).GetData()))
			{
				primitives[i].DisplayData();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

void Rendering::Importers::GltfPrimitive::DisplayData()
{
	if (ImGui::Button("load data")) LoadData();

	ImGui::Text("triangles : %s", String::ConcatenateArray(triangles).GetData());
	ImGui::Text("vertices : %s", String::ConcatenateArray(vertices, "\n").GetData());



	ImGui::Separator();
	ImGui::Text("indices : %d", indices);
	if (ImGui::TreeNode(String("attributes").GetData()))
	{
		ImGui::Text("Position : %d", attributePosition);
		ImGui::Text("Normal : %d", attributeNormal);
		ImGui::Text("Texture coordinates : %d", attributeTexCoord);
		ImGui::Text("Tangent : %d", attributeTangent);
		ImGui::Text("Color : %d", attributeColor);
		ImGui::TreePop();
	}
	ImGui::Text("mode : %d", mode);
	ImGui::Text("material : %d", material);
}

void Rendering::Importers::GLtfMaterial::DisplayData()
{
}

void Rendering::Importers::GltfBufferView::DisplayData()
{
	ImGui::Text("buffer : %d", buffer);
	ImGui::Text("byteOffset : %d", byteOffset);
	ImGui::Text("byteLength : %d", byteLength);
	ImGui::Text("byteStride : %d", byteStride);
	ImGui::Text("target : %d", target);
}

void Rendering::Importers::GltfBuffer::DisplayData()
{
	ImGui::Text("byteLength : %d", byteLength);
	ImGui::Text("uri : %s", uri.GetData());
}


void Rendering::Importers::GltfAccessor::DisplayData()
{
	ImGui::Text("bufferView : %d", bufferView);
	ImGui::Text("byteOffset : %d", byteOffset);
	ImGui::Text("componentType : %d", componentType);
	ImGui::Text("count : %d", count);
	ImGui::Text("type : %s", type.GetData());
	ImGui::Text("max : %s", String::ConcatenateArray(max).GetData());
	ImGui::Text("min : %s", String::ConcatenateArray(min).GetData());
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


Rendering::Importers::GLtfMesh::GLtfMesh(const rapidjson::Value& source, GltfObject* inParent)
{
	name = source["name"].GetString();
	const rapidjson::Value& primitiveArray = source["primitives"];
	primitives.resize(primitiveArray.Size());
	for (rapidjson::SizeType i = 0; i < primitiveArray.Size(); ++i) {
		primitives[i] = GltfPrimitive(primitiveArray[i], inParent);
	}
}

Rendering::Importers::GltfPrimitive::GltfPrimitive(const rapidjson::Value& source, GltfObject* inParent)
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

Rendering::Importers::GLtfMaterial::GLtfMaterial(const rapidjson::Value& source)
{

}

Rendering::Importers::GltfBufferView::GltfBufferView(const rapidjson::Value& source)
{
	if (source["buffer"].IsInt()) buffer = source["buffer"].GetInt();
	if (source["byteOffset"].IsInt()) byteOffset = source["byteOffset"].GetInt();
	if (source["byteLength"].IsInt()) byteLength = source["byteLength"].GetInt();
	if (source["byteStride"].IsInt()) byteStride = source["byteStride"].GetInt();
	if (source["target"].IsInt()) target = source["target"].GetInt();
}


Rendering::Importers::GltfBuffer::GltfBuffer(const rapidjson::Value& source, const char* sourceFilePath)
	: sourcePath(sourceFilePath)
{
	if (source["byteLength"].IsInt()) byteLength = source["byteLength"].GetInt();
	if (source["uri"].GetString()) uri = source["uri"].GetString();
}


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
		std::ifstream ifile(String(std::filesystem::path(sourcePath).parent_path().u8string().c_str() / uri).GetData(), std::ios_base::binary);
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
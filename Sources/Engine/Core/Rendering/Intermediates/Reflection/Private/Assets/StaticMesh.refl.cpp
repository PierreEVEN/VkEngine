//VERSION : Monday, 14 September 2020 15:23:04

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Rendering/Sources/Public\Assets\StaticMesh.h"

#include <assert.h>

#include "Reflection.h"





/* ##############################  Template bodies std__vector_SMeshSectionData  ############################## */

namespace Rendering
{
		struct _Refl_static_type_builder_std__vector_SMeshSectionData { // Type builder - Build reflection data
			_Refl_static_type_builder_std__vector_SMeshSectionData() { // Builder constructor
				REFL_REGISTER_TYPE(std::vector<SMeshSectionData>); // Call to builder function
			}
		} _Refl_static_type_builder_var_std__vector_SMeshSectionData; //Build item when compiled


		/** Create serializer for std::vector<SMeshSectionData> */
		struct std__vector_SMeshSectionData : public Serialization::ISerializer {
			using Serialization::ISerializer::ISerializer; // Implement constructor

			bool SerializeProperty(RProperty* inProperty, void* objectPointer, Serialization::SerializedProperty& outValue) {
				return Serialization::TSerializer<std::vector<SMeshSectionData>>::Serialize(inProperty->Value<std::vector<SMeshSectionData>>(objectPointer), outValue); // Call to serializer
			}

			bool DeserializeProperty(RProperty* inProperty, void* objectPointer, const Serialization::SerializedProperty& inValue) {
				return Serialization::TSerializer<std::vector<SMeshSectionData>>::Deserialize(inValue, inProperty->Value<std::vector<SMeshSectionData>>(objectPointer)); // Call to deserializer
			}

		} inline _std__vector_SMeshSectionData_Value("std::vector<SMeshSectionData>"); // structure registration
}


/* ##############################  Reflection for StaticMesh  ############################## */

namespace Rendering
{
	static RClass* _static_Item_Class_StaticMesh = nullptr; //Static class reference
	RClass* StaticMesh::GetStaticClass() { return _static_Item_Class_StaticMesh; } //Static class getter

	RClass* StaticMesh::GetClass() const { return _static_Item_Class_StaticMesh; } //class getter

	void _Refl_Register_Item_StaticMesh() { // Register function
			_static_Item_Class_StaticMesh = RClass::RegisterClass<StaticMesh>("StaticMesh"); //Register Class
			if (RIsReflected<Asset>::Reflect) // Is parent reflected
				_static_Item_Class_StaticMesh->AddParent("Asset"); // register parent
			size_t VarOffset; // Var offset
			VarOffset = (char*)&((StaticMesh*)nullptr->* & StaticMesh::sectionDatas) - (char*)nullptr; // Retrieve var offset
			_static_Item_Class_StaticMesh->RegisterProperty(new RProperty("std::vector<SMeshSectionData>", VarOffset, sizeof(std::vector<SMeshSectionData>), "sectionDatas")); // Register property
	}

	struct _Refl_Static_Item_Builder_StaticMesh{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_StaticMesh() { // Builder constructor
			_Refl_Register_Item_StaticMesh(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_StaticMesh _Refl_Static_Item_Builder_Var_StaticMesh; //Build item when compiled
}

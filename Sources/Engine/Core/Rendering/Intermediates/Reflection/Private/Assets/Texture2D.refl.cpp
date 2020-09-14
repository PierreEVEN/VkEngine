//VERSION : Sunday, 06 September 2020 09:50:55

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Rendering/Sources/Public\Assets\Texture2D.h"

#include <assert.h>

#include "Reflection.h"





/* ##############################  Template bodies std__vector_uint8_t  ############################## */

namespace Rendering
{
		struct _Refl_static_type_builder_std__vector_uint8_t { // Type builder - Build reflection data
			_Refl_static_type_builder_std__vector_uint8_t() { // Builder constructor
				REFL_REGISTER_TYPE(std::vector<uint8_t>); // Call to builder function
			}
		} _Refl_static_type_builder_var_std__vector_uint8_t; //Build item when compiled


		/** Create serializer for std::vector<uint8_t> */
		struct std__vector_uint8_t : public Serialization::ISerializer {
			using Serialization::ISerializer::ISerializer; // Implement constructor

			bool SerializeProperty(RProperty* inProperty, void* objectPointer, Serialization::SerializedProperty& outValue) {
				return Serialization::TSerializer<std::vector<uint8_t>>::Serialize(inProperty->Value<std::vector<uint8_t>>(objectPointer), outValue); // Call to serializer
			}

			bool DeserializeProperty(RProperty* inProperty, void* objectPointer, const Serialization::SerializedProperty& inValue) {
				return Serialization::TSerializer<std::vector<uint8_t>>::Deserialize(inValue, inProperty->Value<std::vector<uint8_t>>(objectPointer)); // Call to deserializer
			}

		} inline _std__vector_uint8_t_Value("std::vector<uint8_t>"); // structure registration
}


/* ##############################  Reflection for Texture2D  ############################## */

namespace Rendering
{
	static RClass* _static_Item_Class_Texture2D = nullptr; //Static class reference
	RClass* Texture2D::GetStaticClass() { return _static_Item_Class_Texture2D; } //Static class getter

	RClass* Texture2D::GetClass() const { return _static_Item_Class_Texture2D; } //class getter

	void _Refl_Register_Item_Texture2D() { // Register function
			_static_Item_Class_Texture2D = RClass::RegisterClass<Texture2D>("Texture2D"); //Register Class
			if (RIsReflected<Asset>::Reflect) // Is parent reflected
				_static_Item_Class_Texture2D->AddParent("Asset"); // register parent
			size_t VarOffset; // Var offset
			VarOffset = (char*)&((Texture2D*)nullptr->* & Texture2D::textureData) - (char*)nullptr; // Retrieve var offset
			_static_Item_Class_Texture2D->RegisterProperty(new RProperty("std::vector<uint8_t>", VarOffset, sizeof(std::vector<uint8_t>), "textureData")); // Register property
	}

	struct _Refl_Static_Item_Builder_Texture2D{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_Texture2D() { // Builder constructor
			_Refl_Register_Item_Texture2D(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_Texture2D _Refl_Static_Item_Builder_Var_Texture2D; //Build item when compiled
}

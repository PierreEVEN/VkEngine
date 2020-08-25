//VERSION : Friday, 07 August 2020 13:05:24

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Engine/Sources/Public\Object\Object.h"

#include <assert.h>

#include "Reflection.h"




/* ##############################  Reflection for OObject  ############################## */

	static RClass* _static_Item_Class_OObject = nullptr; //Static class reference
	RClass* OObject::GetStaticClass() { return _static_Item_Class_OObject; } //Static class getter

	RClass* OObject::GetClass() const { return _static_Item_Class_OObject; } //class getter

	void _Refl_Register_Item_OObject() { // Register function
			_static_Item_Class_OObject = RClass::RegisterClass<OObject>("OObject"); //Register Class
			if (RIsReflected<Serializable>::Reflect) // Is parent reflected
				_static_Item_Class_OObject->AddParent("Serializable"); // register parent
			size_t VarOffset; // Var offset
			VarOffset = (char*)&((OObject*)nullptr->* & OObject::var1) - (char*)nullptr; // Retrieve var offset
			_static_Item_Class_OObject->RegisterProperty(new RProperty(RType::GetType("int"), VarOffset, sizeof(int), "var1")); // Register property
			VarOffset = (char*)&((OObject*)nullptr->* & OObject::var2) - (char*)nullptr; // Retrieve var offset
			_static_Item_Class_OObject->RegisterProperty(new RProperty(RType::GetType("int"), VarOffset, sizeof(int), "var2")); // Register property
			VarOffset = (char*)&((OObject*)nullptr->* & OObject::var3) - (char*)nullptr; // Retrieve var offset
			_static_Item_Class_OObject->RegisterProperty(new RProperty(RType::GetType("int"), VarOffset, sizeof(int), "var3")); // Register property
	}

	struct _Refl_Static_Item_Builder_OObject{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_OObject() { // Builder constructor
			_Refl_Register_Item_OObject(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_OObject _Refl_Static_Item_Builder_Var_OObject; //Build item when compiled

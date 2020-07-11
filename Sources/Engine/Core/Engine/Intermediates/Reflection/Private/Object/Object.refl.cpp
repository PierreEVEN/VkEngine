

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Engine/Sources/Public\Object\Object.h"

#include "Reflection.h"




/* ##############################  Reflection for OObject  ############################## */

static RClass* _static_Item_Class_OObject = nullptr; //Static class reference
RClass* OObject::GetStaticClass() { return _static_Item_Class_OObject; } //Static class getter

RClass* OObject::GetClass() { return _static_Item_Class_OObject; } //class getter

void _Refl_Register_Item_OObject() { // Register function
		_static_Item_Class_OObject = RClass::RegisterClass<OObject>("OObject"); //Register Class
		if (RIsReflected<IStringable>::Reflect) // Is parent reflected
			_static_Item_Class_OObject->AddParent(RStruct::GetStruct("IStringable")); // register parent
		if (RIsReflected<Serializable>::Reflect) // Is parent reflected
			_static_Item_Class_OObject->AddParent(RStruct::GetStruct("Serializable")); // register parent
		size_t VarOffset; // Var offset
		VarOffset = (char*)&((OObject*)nullptr->* & OObject::var) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_OObject->RegisterProperty(new RProperty(RType::GetType("int"), VarOffset, sizeof(int), "var")); // Register property
}

struct _Refl_Static_Item_Builder_OObject{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_OObject() { // Builder constructor
		_Refl_Register_Item_OObject(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_OObject _Refl_Static_Item_Builder_Var_OObject; //Build item when compiled

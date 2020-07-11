

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Utils/Sources/Public\Types\String.h"

#include "Reflection.h"




/* ##############################  Reflection for String  ############################## */

static RClass* _static_Item_Class_String = nullptr; //Static class reference
RClass* String::GetStaticClass() { return _static_Item_Class_String; } //Static class getter

RClass* String::GetClass() { return _static_Item_Class_String; } //class getter

void _Refl_Register_Item_String() { // Register function
		_static_Item_Class_String = RClass::RegisterClass<String>("String"); //Register Class
		if (RIsReflected<std::string>::Reflect) // Is parent reflected
			_static_Item_Class_String->AddParent(RStruct::GetStruct("std::string")); // register parent
}

struct _Refl_Static_Item_Builder_String{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_String() { // Builder constructor
		_Refl_Register_Item_String(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_String _Refl_Static_Item_Builder_Var_String; //Build item when compiled

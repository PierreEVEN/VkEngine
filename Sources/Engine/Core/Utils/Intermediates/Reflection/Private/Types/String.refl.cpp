//VERSION : Sunday, 26 July 2020 16:45:09

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Utils/Sources/Public\Types\String.h"

#include <assert.h>

#include "Reflection.h"




/* ##############################  Reflection for String  ############################## */

static RClass* _static_Item_Class_String = nullptr; //Static class reference
RClass* String::GetStaticClass() { return _static_Item_Class_String; } //Static class getter

RClass* String::GetClass() const { return _static_Item_Class_String; } //class getter

void _Refl_Register_Item_String() { // Register function
		_static_Item_Class_String = RClass::RegisterClass<String>("String"); //Register Class
}

struct _Refl_Static_Item_Builder_String{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_String() { // Builder constructor
		_Refl_Register_Item_String(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_String _Refl_Static_Item_Builder_Var_String; //Build item when compiled

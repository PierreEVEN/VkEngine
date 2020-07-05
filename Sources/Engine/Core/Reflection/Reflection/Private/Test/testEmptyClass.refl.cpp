//VERSION : Sunday, 05 July 2020 13:51:09

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/

#include "Private\Test\testEmptyClass.refl.h"
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Reflection/Sources/Private\Test\testEmptyClass.h"
#include "RStruct.h"
#include "RClass.h"




/* ##############################  Reflection for StrEmpTest  ############################## */

static RStruct* _static_Item_Class_StrEmpTest = nullptr; //Static struct reference
RStruct* StrEmpTest::GetStaticStruct() { return _static_Item_Class_StrEmpTest; } //Static struct getter

void _Refl_Register_Item_StrEmpTest() { // Register function
		_static_Item_Class_StrEmpTest = RStruct::RegisterStruct<StrEmpTest>("StrEmpTest"); //Register Struct
}

struct _Refl_Static_Item_Builder_StrEmpTest{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_StrEmpTest() { // Builder constructor
		_Refl_Register_Item_StrEmpTest(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_StrEmpTest _Refl_Static_Item_Builder_Var_StrEmpTest; //Build item when compiled

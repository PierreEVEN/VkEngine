//VERSION : Tuesday, 25 August 2020 00:42:58

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Rendering/Sources/Public\Assets\Asset.h"

#include <assert.h>

#include "Reflection.h"




/* ##############################  Reflection for Asset  ############################## */

namespace Rendering
{
	static RClass* _static_Item_Class_Asset = nullptr; //Static class reference
	RClass* Asset::GetStaticClass() { return _static_Item_Class_Asset; } //Static class getter

	RClass* Asset::GetClass() const { return _static_Item_Class_Asset; } //class getter

	void _Refl_Register_Item_Asset() { // Register function
			_static_Item_Class_Asset = RClass::RegisterClass<Asset>("Asset"); //Register Class
	}

	struct _Refl_Static_Item_Builder_Asset{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_Asset() { // Builder constructor
			_Refl_Register_Item_Asset(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_Asset _Refl_Static_Item_Builder_Var_Asset; //Build item when compiled
}

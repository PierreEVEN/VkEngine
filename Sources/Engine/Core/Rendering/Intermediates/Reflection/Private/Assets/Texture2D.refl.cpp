//VERSION : Thursday, 20 August 2020 17:37:04

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Rendering/Sources/Public\Assets\Texture2D.h"

#include <assert.h>

#include "Reflection.h"




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
	}

	struct _Refl_Static_Item_Builder_Texture2D{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_Texture2D() { // Builder constructor
			_Refl_Register_Item_Texture2D(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_Texture2D _Refl_Static_Item_Builder_Var_Texture2D; //Build item when compiled
}

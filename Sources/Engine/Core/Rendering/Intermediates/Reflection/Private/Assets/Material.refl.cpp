//VERSION : Monday, 14 September 2020 15:23:04

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Rendering/Sources/Public\Assets\Material.h"

#include <assert.h>

#include "Reflection.h"





/* ##############################  Reflection for Material  ############################## */

namespace Rendering
{
	static RClass* _static_Item_Class_Material = nullptr; //Static class reference
	RClass* Material::GetStaticClass() { return _static_Item_Class_Material; } //Static class getter

	RClass* Material::GetClass() const { return _static_Item_Class_Material; } //class getter

	void _Refl_Register_Item_Material() { // Register function
			_static_Item_Class_Material = RClass::RegisterClass<Material>("Material"); //Register Class
			if (RIsReflected<Asset>::Reflect) // Is parent reflected
				_static_Item_Class_Material->AddParent("Asset"); // register parent
	}

	struct _Refl_Static_Item_Builder_Material{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_Material() { // Builder constructor
			_Refl_Register_Item_Material(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_Material _Refl_Static_Item_Builder_Var_Material; //Build item when compiled
}

//VERSION : Monday, 24 August 2020 23:11:27

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Rendering/Sources/Public\Assets\ShaderModule.h"

#include <assert.h>

#include "Reflection.h"




/* ##############################  Reflection for ShaderModule  ############################## */

namespace Rendering
{
	static RClass* _static_Item_Class_ShaderModule = nullptr; //Static class reference
	RClass* ShaderModule::GetStaticClass() { return _static_Item_Class_ShaderModule; } //Static class getter

	RClass* ShaderModule::GetClass() const { return _static_Item_Class_ShaderModule; } //class getter

	void _Refl_Register_Item_ShaderModule() { // Register function
			_static_Item_Class_ShaderModule = RClass::RegisterClass<ShaderModule>("ShaderModule"); //Register Class
			if (RIsReflected<Asset>::Reflect) // Is parent reflected
				_static_Item_Class_ShaderModule->AddParent("Asset"); // register parent
	}

	struct _Refl_Static_Item_Builder_ShaderModule{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_ShaderModule() { // Builder constructor
			_Refl_Register_Item_ShaderModule(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_ShaderModule _Refl_Static_Item_Builder_Var_ShaderModule; //Build item when compiled
}

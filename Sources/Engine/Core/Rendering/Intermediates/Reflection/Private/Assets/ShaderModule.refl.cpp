//VERSION : Thursday, 27 August 2020 11:06:54

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




/* ##############################  Reflection for VertexShaderModule  ############################## */

namespace Rendering
{
	static RClass* _static_Item_Class_VertexShaderModule = nullptr; //Static class reference
	RClass* VertexShaderModule::GetStaticClass() { return _static_Item_Class_VertexShaderModule; } //Static class getter

	RClass* VertexShaderModule::GetClass() const { return _static_Item_Class_VertexShaderModule; } //class getter

	void _Refl_Register_Item_VertexShaderModule() { // Register function
			_static_Item_Class_VertexShaderModule = RClass::RegisterClass<VertexShaderModule>("VertexShaderModule"); //Register Class
			if (RIsReflected<ShaderModule>::Reflect) // Is parent reflected
				_static_Item_Class_VertexShaderModule->AddParent("ShaderModule"); // register parent
	}

	struct _Refl_Static_Item_Builder_VertexShaderModule{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_VertexShaderModule() { // Builder constructor
			_Refl_Register_Item_VertexShaderModule(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_VertexShaderModule _Refl_Static_Item_Builder_Var_VertexShaderModule; //Build item when compiled
}




/* ##############################  Reflection for FragmentShaderModule  ############################## */

namespace Rendering
{
	static RClass* _static_Item_Class_FragmentShaderModule = nullptr; //Static class reference
	RClass* FragmentShaderModule::GetStaticClass() { return _static_Item_Class_FragmentShaderModule; } //Static class getter

	RClass* FragmentShaderModule::GetClass() const { return _static_Item_Class_FragmentShaderModule; } //class getter

	void _Refl_Register_Item_FragmentShaderModule() { // Register function
			_static_Item_Class_FragmentShaderModule = RClass::RegisterClass<FragmentShaderModule>("FragmentShaderModule"); //Register Class
			if (RIsReflected<ShaderModule>::Reflect) // Is parent reflected
				_static_Item_Class_FragmentShaderModule->AddParent("ShaderModule"); // register parent
	}

	struct _Refl_Static_Item_Builder_FragmentShaderModule{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_FragmentShaderModule() { // Builder constructor
			_Refl_Register_Item_FragmentShaderModule(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_FragmentShaderModule _Refl_Static_Item_Builder_Var_FragmentShaderModule; //Build item when compiled
}

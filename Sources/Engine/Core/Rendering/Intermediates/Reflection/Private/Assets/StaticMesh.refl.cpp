//VERSION : Tuesday, 25 August 2020 00:42:58

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Rendering/Sources/Public\Assets\StaticMesh.h"

#include <assert.h>

#include "Reflection.h"




/* ##############################  Reflection for StaticMesh  ############################## */

namespace Rendering
{
	static RClass* _static_Item_Class_StaticMesh = nullptr; //Static class reference
	RClass* StaticMesh::GetStaticClass() { return _static_Item_Class_StaticMesh; } //Static class getter

	RClass* StaticMesh::GetClass() const { return _static_Item_Class_StaticMesh; } //class getter

	void _Refl_Register_Item_StaticMesh() { // Register function
			_static_Item_Class_StaticMesh = RClass::RegisterClass<StaticMesh>("StaticMesh"); //Register Class
			if (RIsReflected<Asset>::Reflect) // Is parent reflected
				_static_Item_Class_StaticMesh->AddParent("Asset"); // register parent
	}

	struct _Refl_Static_Item_Builder_StaticMesh{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_StaticMesh() { // Builder constructor
			_Refl_Register_Item_StaticMesh(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_StaticMesh _Refl_Static_Item_Builder_Var_StaticMesh; //Build item when compiled
}

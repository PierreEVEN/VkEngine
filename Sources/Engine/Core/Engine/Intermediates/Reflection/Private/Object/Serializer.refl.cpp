//VERSION : Saturday, 22 August 2020 01:40:47

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Engine/Sources/Public\Object\Serializer.h"

#include <assert.h>

#include "Reflection.h"




/* ##############################  Reflection for Serializable  ############################## */

	static RClass* _static_Item_Class_Serializable = nullptr; //Static class reference
	RClass* Serializable::GetStaticClass() { return _static_Item_Class_Serializable; } //Static class getter

	RClass* Serializable::GetClass() const { return _static_Item_Class_Serializable; } //class getter

	void _Refl_Register_Item_Serializable() { // Register function
			_static_Item_Class_Serializable = RClass::RegisterClass<Serializable>("Serializable"); //Register Class
			if (RIsReflected<IStringable>::Reflect) // Is parent reflected
				_static_Item_Class_Serializable->AddParent("IStringable"); // register parent
	}

	struct _Refl_Static_Item_Builder_Serializable{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_Serializable() { // Builder constructor
			_Refl_Register_Item_Serializable(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_Serializable _Refl_Static_Item_Builder_Var_Serializable; //Build item when compiled

//VERSION : Saturday, 12 September 2020 11:40:17

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Engine/Sources/Public\Object\Object.h"

#include <assert.h>

#include "Reflection.h"





/* ##############################  Template bodies TVector_float  ############################## */

	struct _Refl_static_type_builder_TVector_float { // Type builder - Build reflection data
		_Refl_static_type_builder_TVector_float() { // Builder constructor
			REFL_REGISTER_TYPE(TVector<float>); // Call to builder function
		}
	} _Refl_static_type_builder_var_TVector_float; //Build item when compiled


	/** Create serializer for TVector<float> */
	struct TVector_float : public Serialization::ISerializer {
		using Serialization::ISerializer::ISerializer; // Implement constructor

		bool SerializeProperty(RProperty* inProperty, void* objectPointer, Serialization::SerializedProperty& outValue) {
			return Serialization::TSerializer<TVector<float>>::Serialize(inProperty->Value<TVector<float>>(objectPointer), outValue); // Call to serializer
		}

		bool DeserializeProperty(RProperty* inProperty, void* objectPointer, const Serialization::SerializedProperty& inValue) {
			return Serialization::TSerializer<TVector<float>>::Deserialize(inValue, inProperty->Value<TVector<float>>(objectPointer)); // Call to deserializer
		}

	} inline _TVector_float_Value("TVector<float>"); // structure registration


/* ##############################  Reflection for OObject  ############################## */

	static RClass* _static_Item_Class_OObject = nullptr; //Static class reference
	RClass* OObject::GetStaticClass() { return _static_Item_Class_OObject; } //Static class getter

	RClass* OObject::GetClass() const { return _static_Item_Class_OObject; } //class getter

	void _Refl_Register_Item_OObject() { // Register function
			_static_Item_Class_OObject = RClass::RegisterClass<OObject>("OObject"); //Register Class
			size_t VarOffset; // Var offset
			VarOffset = (char*)&((OObject*)nullptr->* & OObject::var1) - (char*)nullptr; // Retrieve var offset
			_static_Item_Class_OObject->RegisterProperty(new RProperty("float", VarOffset, sizeof(float), "var1")); // Register property
			VarOffset = (char*)&((OObject*)nullptr->* & OObject::var2) - (char*)nullptr; // Retrieve var offset
			_static_Item_Class_OObject->RegisterProperty(new RProperty("int32_t", VarOffset, sizeof(int32_t), "var2")); // Register property
			VarOffset = (char*)&((OObject*)nullptr->* & OObject::var3) - (char*)nullptr; // Retrieve var offset
			_static_Item_Class_OObject->RegisterProperty(new RProperty("float", VarOffset, sizeof(float), "var3")); // Register property
			VarOffset = (char*)&((OObject*)nullptr->* & OObject::var4) - (char*)nullptr; // Retrieve var offset
			_static_Item_Class_OObject->RegisterProperty(new RProperty("TVector<float>", VarOffset, sizeof(TVector<float>), "var4")); // Register property
	}

	struct _Refl_Static_Item_Builder_OObject{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_OObject() { // Builder constructor
			_Refl_Register_Item_OObject(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_OObject _Refl_Static_Item_Builder_Var_OObject; //Build item when compiled





/* ##############################  Reflection for TestClassA  ############################## */

	static RClass* _static_Item_Class_TestClassA = nullptr; //Static class reference
	RClass* TestClassA::GetStaticClass() { return _static_Item_Class_TestClassA; } //Static class getter

	RClass* TestClassA::GetClass() const { return _static_Item_Class_TestClassA; } //class getter

	void _Refl_Register_Item_TestClassA() { // Register function
			_static_Item_Class_TestClassA = RClass::RegisterClass<TestClassA>("TestClassA"); //Register Class
			size_t VarOffset; // Var offset
			VarOffset = (char*)&((TestClassA*)nullptr->* & TestClassA::var1) - (char*)nullptr; // Retrieve var offset
			_static_Item_Class_TestClassA->RegisterProperty(new RProperty("int32_t", VarOffset, sizeof(int32_t), "var1")); // Register property
	}

	struct _Refl_Static_Item_Builder_TestClassA{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_TestClassA() { // Builder constructor
			_Refl_Register_Item_TestClassA(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_TestClassA _Refl_Static_Item_Builder_Var_TestClassA; //Build item when compiled





/* ##############################  Reflection for TestClassB  ############################## */

	static RClass* _static_Item_Class_TestClassB = nullptr; //Static class reference
	RClass* TestClassB::GetStaticClass() { return _static_Item_Class_TestClassB; } //Static class getter

	RClass* TestClassB::GetClass() const { return _static_Item_Class_TestClassB; } //class getter

	void _Refl_Register_Item_TestClassB() { // Register function
			_static_Item_Class_TestClassB = RClass::RegisterClass<TestClassB>("TestClassB"); //Register Class
			if (RIsReflected<TestClassA>::Reflect) // Is parent reflected
				_static_Item_Class_TestClassB->AddParent("TestClassA"); // register parent
			size_t VarOffset; // Var offset
			VarOffset = (char*)&((TestClassB*)nullptr->* & TestClassB::var2) - (char*)nullptr; // Retrieve var offset
			_static_Item_Class_TestClassB->RegisterProperty(new RProperty("int32_t", VarOffset, sizeof(int32_t), "var2")); // Register property
	}

	struct _Refl_Static_Item_Builder_TestClassB{ // Item builder - Build reflection data
		_Refl_Static_Item_Builder_TestClassB() { // Builder constructor
			_Refl_Register_Item_TestClassB(); // Call to builder function
		}
	};

	static _Refl_Static_Item_Builder_TestClassB _Refl_Static_Item_Builder_Var_TestClassB; //Build item when compiled

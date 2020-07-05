//VERSION : Sunday, 05 July 2020 13:51:09

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/

#include "Private\Test\RTestReflection.refl.h"
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Reflection/Sources/Private\Test\RTestReflection.h"
#include "RStruct.h"
#include "RClass.h"




/* ##############################  Reflection for TestStr  ############################## */

static RStruct* _static_Item_Class_TestStr = nullptr; //Static struct reference
RStruct* TestStr::GetStaticStruct() { return _static_Item_Class_TestStr; } //Static struct getter

void _Refl_Register_Item_TestStr() { // Register function
		_static_Item_Class_TestStr = RStruct::RegisterStruct<TestStr,const int&, int, const unsigned int&, void*>("TestStr"); //Register Struct
		size_t VarOffset; // Var offset
		VarOffset = (char*)&((TestStr*)nullptr->* & TestStr::intProp) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestStr->RegisterProperty(RProperty(RType::GetType("int32_t"), VarOffset, sizeof(int32_t), "intProp")); // Register property
		VarOffset = (char*)&((TestStr*)nullptr->* & TestStr::propD) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestStr->RegisterProperty(RProperty(RType::GetType("uint32_t"), VarOffset, sizeof(uint32_t), "propD")); // Register property
		VarOffset = (char*)&((TestStr*)nullptr->* & TestStr::C_Prop_Test) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestStr->RegisterProperty(RProperty(RType::GetType("const char*"), VarOffset, sizeof(const char*), "C_Prop_Test")); // Register property
		VarOffset = (char*)&((TestStr*)nullptr->* & TestStr::auto_VaR) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestStr->RegisterProperty(RProperty(RType::GetType("const unsigned long*"), VarOffset, sizeof(const unsigned long*), "auto_VaR")); // Register property
}

struct _Refl_Static_Item_Builder_TestStr{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_TestStr() { // Builder constructor
		_Refl_Register_Item_TestStr(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_TestStr _Refl_Static_Item_Builder_Var_TestStr; //Build item when compiled




/* ##############################  Reflection for TestClass  ############################## */

static RClass* _static_Item_Class_TestClass = nullptr; //Static class reference
RClass* TestClass::GetStaticClass() { return _static_Item_Class_TestClass; } //Static class getter

void _Refl_Register_Item_TestClass() { // Register function
		_static_Item_Class_TestClass = RClass::RegisterClass<TestClass,int, float, double, long>("TestClass"); //Register Class
		size_t VarOffset; // Var offset
		VarOffset = (char*)&((TestClass*)nullptr->* & TestClass::testProp1) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestClass->RegisterProperty(RProperty(RType::GetType("int32_t"), VarOffset, sizeof(int32_t), "testProp1")); // Register property
		VarOffset = (char*)&((TestClass*)nullptr->* & TestClass::testProp2) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestClass->RegisterProperty(RProperty(RType::GetType("float"), VarOffset, sizeof(float), "testProp2")); // Register property
		VarOffset = (char*)&((TestClass*)nullptr->* & TestClass::testProp3) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestClass->RegisterProperty(RProperty(RType::GetType("double"), VarOffset, sizeof(double), "testProp3")); // Register property
		VarOffset = (char*)&((TestClass*)nullptr->* & TestClass::testProp4) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestClass->RegisterProperty(RProperty(RType::GetType("int64_t"), VarOffset, sizeof(int64_t), "testProp4")); // Register property
		VarOffset = (char*)&((TestClass*)nullptr->* & TestClass::testStruct) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestClass->RegisterProperty(RProperty(RType::GetType("TestStr"), VarOffset, sizeof(TestStr), "testStruct")); // Register property
		VarOffset = (char*)&((TestClass*)nullptr->* & TestClass::testPtr) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestClass->RegisterProperty(RProperty(RType::GetType("TestStr*"), VarOffset, sizeof(TestStr*), "testPtr")); // Register property
}

struct _Refl_Static_Item_Builder_TestClass{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_TestClass() { // Builder constructor
		_Refl_Register_Item_TestClass(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_TestClass _Refl_Static_Item_Builder_Var_TestClass; //Build item when compiled




/* ##############################  Reflection for FooStruct  ############################## */

static RStruct* _static_Item_Class_FooStruct = nullptr; //Static struct reference
RStruct* FooStruct::GetStaticStruct() { return _static_Item_Class_FooStruct; } //Static struct getter

void _Refl_Register_Item_FooStruct() { // Register function
		_static_Item_Class_FooStruct = RStruct::RegisterStruct<FooStruct,int>("FooStruct"); //Register Struct
}

struct _Refl_Static_Item_Builder_FooStruct{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_FooStruct() { // Builder constructor
		_Refl_Register_Item_FooStruct(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_FooStruct _Refl_Static_Item_Builder_Var_FooStruct; //Build item when compiled




/* ##############################  Reflection for Foo1  ############################## */

static RStruct* _static_Item_Class_Foo1 = nullptr; //Static struct reference
RStruct* Foo1::GetStaticStruct() { return _static_Item_Class_Foo1; } //Static struct getter

void _Refl_Register_Item_Foo1() { // Register function
		_static_Item_Class_Foo1 = RStruct::RegisterStruct<Foo1,int>("Foo1"); //Register Struct
		if (RIsReflected<FooStruct>::Reflect) // Is parent reflected
			_static_Item_Class_Foo1->AddParent(RStruct::GetStruct("FooStruct")); // register parent
		else std::cerr << "FooStruct is not actually reflected " << std::endl; // Warning, parent is not reflected
}

struct _Refl_Static_Item_Builder_Foo1{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_Foo1() { // Builder constructor
		_Refl_Register_Item_Foo1(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_Foo1 _Refl_Static_Item_Builder_Var_Foo1; //Build item when compiled




/* ##############################  Reflection for Foo2  ############################## */

static RStruct* _static_Item_Class_Foo2 = nullptr; //Static struct reference
RStruct* Foo2::GetStaticStruct() { return _static_Item_Class_Foo2; } //Static struct getter

void _Refl_Register_Item_Foo2() { // Register function
		_static_Item_Class_Foo2 = RStruct::RegisterStruct<Foo2>("Foo2"); //Register Struct
}

struct _Refl_Static_Item_Builder_Foo2{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_Foo2() { // Builder constructor
		_Refl_Register_Item_Foo2(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_Foo2 _Refl_Static_Item_Builder_Var_Foo2; //Build item when compiled




/* ##############################  Reflection for Foo3  ############################## */

static RStruct* _static_Item_Class_Foo3 = nullptr; //Static struct reference
RStruct* Foo3::GetStaticStruct() { return _static_Item_Class_Foo3; } //Static struct getter

void _Refl_Register_Item_Foo3() { // Register function
		_static_Item_Class_Foo3 = RStruct::RegisterStruct<Foo3,int>("Foo3"); //Register Struct
		if (RIsReflected<Foo1>::Reflect) // Is parent reflected
			_static_Item_Class_Foo3->AddParent(RStruct::GetStruct("Foo1")); // register parent
		else std::cerr << "Foo1 is not actually reflected " << std::endl; // Warning, parent is not reflected
}

struct _Refl_Static_Item_Builder_Foo3{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_Foo3() { // Builder constructor
		_Refl_Register_Item_Foo3(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_Foo3 _Refl_Static_Item_Builder_Var_Foo3; //Build item when compiled




/* ##############################  Reflection for Foo4  ############################## */

static RStruct* _static_Item_Class_Foo4 = nullptr; //Static struct reference
RStruct* Foo4::GetStaticStruct() { return _static_Item_Class_Foo4; } //Static struct getter

void _Refl_Register_Item_Foo4() { // Register function
		_static_Item_Class_Foo4 = RStruct::RegisterStruct<Foo4,int>("Foo4"); //Register Struct
		if (RIsReflected<Foo2>::Reflect) // Is parent reflected
			_static_Item_Class_Foo4->AddParent(RStruct::GetStruct("Foo2")); // register parent
		else std::cerr << "Foo2 is not actually reflected " << std::endl; // Warning, parent is not reflected
		if (RIsReflected<Foo3>::Reflect) // Is parent reflected
			_static_Item_Class_Foo4->AddParent(RStruct::GetStruct("Foo3")); // register parent
		else std::cerr << "Foo3 is not actually reflected " << std::endl; // Warning, parent is not reflected
}

struct _Refl_Static_Item_Builder_Foo4{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_Foo4() { // Builder constructor
		_Refl_Register_Item_Foo4(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_Foo4 _Refl_Static_Item_Builder_Var_Foo4; //Build item when compiled

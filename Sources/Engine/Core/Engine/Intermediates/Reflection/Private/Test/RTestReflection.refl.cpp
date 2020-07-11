//VERSION : Saturday, 11 July 2020 15:33:33

/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/
#include "C:/Users/pierre/Documents/Engine/Sources/Engine/Core/Engine/Sources/Public\Test\RTestReflection.h"

#include "Reflection.h"




/* ##############################  Reflection for OC  ############################## */

static RClass* _static_Item_Class_OC = nullptr; //Static class reference
RClass* OC::GetStaticClass() { return _static_Item_Class_OC; } //Static class getter

RClass* OC::GetClass() { return _static_Item_Class_OC; } //class getter

void _Refl_Register_Item_OC() { // Register function
		_static_Item_Class_OC = RClass::RegisterClass<OC>("OC"); //Register Class
}

struct _Refl_Static_Item_Builder_OC{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_OC() { // Builder constructor
		_Refl_Register_Item_OC(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_OC _Refl_Static_Item_Builder_Var_OC; //Build item when compiled




/* ##############################  Reflection for OA  ############################## */

static RClass* _static_Item_Class_OA = nullptr; //Static class reference
RClass* OA::GetStaticClass() { return _static_Item_Class_OA; } //Static class getter

RClass* OA::GetClass() { return _static_Item_Class_OA; } //class getter

void _Refl_Register_Item_OA() { // Register function
		_static_Item_Class_OA = RClass::RegisterClass<OA>("OA"); //Register Class
}

struct _Refl_Static_Item_Builder_OA{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_OA() { // Builder constructor
		_Refl_Register_Item_OA(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_OA _Refl_Static_Item_Builder_Var_OA; //Build item when compiled




/* ##############################  Reflection for OB  ############################## */

static RClass* _static_Item_Class_OB = nullptr; //Static class reference
RClass* OB::GetStaticClass() { return _static_Item_Class_OB; } //Static class getter

RClass* OB::GetClass() { return _static_Item_Class_OB; } //class getter

void _Refl_Register_Item_OB() { // Register function
		_static_Item_Class_OB = RClass::RegisterClass<OB>("OB"); //Register Class
		if (RIsReflected<OC>::Reflect) // Is parent reflected
			_static_Item_Class_OB->AddParent(RStruct::GetStruct("OC")); // register parent
		if (RIsReflected<OA>::Reflect) // Is parent reflected
			_static_Item_Class_OB->AddParent(RStruct::GetStruct("OA")); // register parent
}

struct _Refl_Static_Item_Builder_OB{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_OB() { // Builder constructor
		_Refl_Register_Item_OB(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_OB _Refl_Static_Item_Builder_Var_OB; //Build item when compiled




/* ##############################  Reflection for TestStr  ############################## */

static RStruct* _static_Item_Class_TestStr = nullptr; //Static struct reference
RStruct* TestStr::GetStaticStruct() { return _static_Item_Class_TestStr; } //Static struct getter

RStruct* TestStr::GetStruct() { return _static_Item_Class_TestStr; } //struct getter

void _Refl_Register_Item_TestStr() { // Register function
		_static_Item_Class_TestStr = RStruct::RegisterStruct<TestStr,int, float, double, long>("TestStr"); //Register Struct
		size_t VarOffset; // Var offset
		VarOffset = (char*)&((TestStr*)nullptr->* & TestStr::intProp) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestStr->RegisterProperty(new RProperty(RType::GetType("int32_t"), VarOffset, sizeof(int32_t), "intProp")); // Register property
		VarOffset = (char*)&((TestStr*)nullptr->* & TestStr::propD) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestStr->RegisterProperty(new RProperty(RType::GetType("uint32_t"), VarOffset, sizeof(uint32_t), "propD")); // Register property
		VarOffset = (char*)&((TestStr*)nullptr->* & TestStr::C_Prop_Test) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestStr->RegisterProperty(new RProperty(RType::GetType("const char*"), VarOffset, sizeof(const char*), "C_Prop_Test")); // Register property
		VarOffset = (char*)&((TestStr*)nullptr->* & TestStr::auto_VaR) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestStr->RegisterProperty(new RProperty(RType::GetType("const unsigned long*"), VarOffset, sizeof(const unsigned long*), "auto_VaR")); // Register property
		_static_Item_Class_TestStr->RegisterFunction(RFunction<void,TestStr>("PrintTest", std::function<void (TestStr&)>(&TestStr::PrintTest))); // Register function
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

RClass* TestClass::GetClass() { return _static_Item_Class_TestClass; } //class getter

void _Refl_Register_Item_TestClass() { // Register function
		_static_Item_Class_TestClass = RClass::RegisterClass<TestClass,int, float, double, long>("TestClass"); //Register Class
		if (RIsReflected<TestStr>::Reflect) // Is parent reflected
			_static_Item_Class_TestClass->AddParent(RStruct::GetStruct("TestStr")); // register parent
		size_t VarOffset; // Var offset
		VarOffset = (char*)&((TestClass*)nullptr->* & TestClass::testProp1) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestClass->RegisterProperty(new RProperty(RType::GetType("int32_t"), VarOffset, sizeof(int32_t), "testProp1")); // Register property
		VarOffset = (char*)&((TestClass*)nullptr->* & TestClass::testProp2) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestClass->RegisterProperty(new RProperty(RType::GetType("float"), VarOffset, sizeof(float), "testProp2")); // Register property
		VarOffset = (char*)&((TestClass*)nullptr->* & TestClass::testProp3) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestClass->RegisterProperty(new RProperty(RType::GetType("double"), VarOffset, sizeof(double), "testProp3")); // Register property
		VarOffset = (char*)&((TestClass*)nullptr->* & TestClass::testProp4) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestClass->RegisterProperty(new RProperty(RType::GetType("int64_t"), VarOffset, sizeof(int64_t), "testProp4")); // Register property
		VarOffset = (char*)&((TestClass*)nullptr->* & TestClass::testStruct) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestClass->RegisterProperty(new RProperty(RType::GetType("TestStr"), VarOffset, sizeof(TestStr), "testStruct")); // Register property
		VarOffset = (char*)&((TestClass*)nullptr->* & TestClass::testPtr) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestClass->RegisterProperty(new RProperty(RType::GetType("TestStr*"), VarOffset, sizeof(TestStr*), "testPtr")); // Register property
		VarOffset = (char*)&((TestClass*)nullptr->* & TestClass::testString) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_TestClass->RegisterProperty(new RProperty(RType::GetType("String"), VarOffset, sizeof(String), "testString")); // Register property
		_static_Item_Class_TestClass->RegisterFunction(RFunction<void,TestClass>("PrintTestClass", std::function<void (TestClass&)>(&TestClass::PrintTestClass))); // Register function
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

RStruct* FooStruct::GetStruct() { return _static_Item_Class_FooStruct; } //struct getter

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

RStruct* Foo1::GetStruct() { return _static_Item_Class_Foo1; } //struct getter

void _Refl_Register_Item_Foo1() { // Register function
		_static_Item_Class_Foo1 = RStruct::RegisterStruct<Foo1,int>("Foo1"); //Register Struct
		if (RIsReflected<FooStruct>::Reflect) // Is parent reflected
			_static_Item_Class_Foo1->AddParent(RStruct::GetStruct("FooStruct")); // register parent
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

RStruct* Foo2::GetStruct() { return _static_Item_Class_Foo2; } //struct getter

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

RStruct* Foo3::GetStruct() { return _static_Item_Class_Foo3; } //struct getter

void _Refl_Register_Item_Foo3() { // Register function
		_static_Item_Class_Foo3 = RStruct::RegisterStruct<Foo3,int>("Foo3"); //Register Struct
		if (RIsReflected<Foo1>::Reflect) // Is parent reflected
			_static_Item_Class_Foo3->AddParent(RStruct::GetStruct("Foo1")); // register parent
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

RStruct* Foo4::GetStruct() { return _static_Item_Class_Foo4; } //struct getter

void _Refl_Register_Item_Foo4() { // Register function
		_static_Item_Class_Foo4 = RStruct::RegisterStruct<Foo4,int>("Foo4"); //Register Struct
		if (RIsReflected<Foo2>::Reflect) // Is parent reflected
			_static_Item_Class_Foo4->AddParent(RStruct::GetStruct("Foo2")); // register parent
		if (RIsReflected<Foo3>::Reflect) // Is parent reflected
			_static_Item_Class_Foo4->AddParent(RStruct::GetStruct("Foo3")); // register parent
}

struct _Refl_Static_Item_Builder_Foo4{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_Foo4() { // Builder constructor
		_Refl_Register_Item_Foo4(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_Foo4 _Refl_Static_Item_Builder_Var_Foo4; //Build item when compiled




/* ##############################  Reflection for OObjectTest  ############################## */

static RClass* _static_Item_Class_OObjectTest = nullptr; //Static class reference
RClass* OObjectTest::GetStaticClass() { return _static_Item_Class_OObjectTest; } //Static class getter

RClass* OObjectTest::GetClass() { return _static_Item_Class_OObjectTest; } //class getter

void _Refl_Register_Item_OObjectTest() { // Register function
		_static_Item_Class_OObjectTest = RClass::RegisterClass<OObjectTest>("OObjectTest"); //Register Class
		if (RIsReflected<OObject>::Reflect) // Is parent reflected
			_static_Item_Class_OObjectTest->AddParent(RStruct::GetStruct("OObject")); // register parent
		size_t VarOffset; // Var offset
		VarOffset = (char*)&((OObjectTest*)nullptr->* & OObjectTest::testProperty1) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_OObjectTest->RegisterProperty(new RProperty(RType::GetType("int"), VarOffset, sizeof(int), "testProperty1")); // Register property
		VarOffset = (char*)&((OObjectTest*)nullptr->* & OObjectTest::testProperty2) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_OObjectTest->RegisterProperty(new RProperty(RType::GetType("double"), VarOffset, sizeof(double), "testProperty2")); // Register property
		VarOffset = (char*)&((OObjectTest*)nullptr->* & OObjectTest::testProperty3) - (char*)nullptr; // Retrieve var offset
		_static_Item_Class_OObjectTest->RegisterProperty(new RProperty(RType::GetType("int"), VarOffset, sizeof(int), "testProperty3")); // Register property
}

struct _Refl_Static_Item_Builder_OObjectTest{ // Item builder - Build reflection data
	_Refl_Static_Item_Builder_OObjectTest() { // Builder constructor
		_Refl_Register_Item_OObjectTest(); // Call to builder function
	}
};

static _Refl_Static_Item_Builder_OObjectTest _Refl_Static_Item_Builder_Var_OObjectTest; //Build item when compiled

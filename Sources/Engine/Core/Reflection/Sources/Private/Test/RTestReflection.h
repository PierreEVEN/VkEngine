#pragma once
#include "ReflectionMacro.h"
#include <iostream>
#include "Private\Test\RTestReflection.refl.h"

REFLECT()
struct TestStr
{
	TestStr() = default;

	RFUNCTION()
		void PrintTest()
	{
		std::cout << "called printTest on TestClass" << std::endl;
	}
	REFLECT_BODY()

public:

	RCONSTRUCTOR()
	TestStr(const int& val1, int val2, const unsigned int& val3, void* val4) {}

	RPROPERTY()
	int32_t intProp = -2;

	RPROPERTY()
	uint32_t propD = 6;

	RPROPERTY()
	const char* C_Prop_Test = nullptr;

	RPROPERTY()
	const unsigned long* auto_VaR;
};

REFLECT()
class TestClass
{
	REFLECT_BODY()

	TestClass() = default;

	RCONSTRUCTOR()
		TestClass(int a, float b, double c, long d)
		: testProp1(a), testProp2(b), testProp3(c), testProp4(d)
	{
		std::cout << "building test class with params : " << a << " " << b << " " << c << " " << d << std::endl;
	}

	RPROPERTY()
		int32_t testProp1 = 6;

	RPROPERTY()
		float testProp2 = 12;

	RPROPERTY()
		double testProp3 = 4;

	RPROPERTY()
		int64_t testProp4 = 4;

	RPROPERTY()
		TestStr testStruct;

	RPROPERTY()
		TestStr* testPtr;

};


REFLECT()
struct FooStruct {
	REFLECT_BODY()

		RCONSTRUCTOR()
		FooStruct(int test) { std::cout << "instancied obj with arg : " << test << std::endl; }

	FooStruct() { std::cout << "instancied obj with default ctor" << std::endl; }

};


REFLECT()
struct Foo1 : public FooStruct { 

	REFLECT_BODY()

		RCONSTRUCTOR()
	Foo1(int arg)
		: FooStruct(arg)
	{}
};


REFLECT()
struct Foo2 { 
	REFLECT_BODY() 
public:
	Foo2() = default;
};
REFLECT()
struct Foo3 : public Foo1 { 
	REFLECT_BODY()

		RCONSTRUCTOR()
		Foo3(int arg)
		: Foo1(arg)
	{}
};
REFLECT()
struct Foo4 : public Foo2, private Foo3 {
	
	REFLECT_BODY()

	RCONSTRUCTOR()
	Foo4(int arg)
		: Foo2(), Foo3(arg)
	{}
};
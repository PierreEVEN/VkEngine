#pragma once

#include "Reflection.h"
#include "Object\Object.h"
#include "IO\Log.h"
#include "Test\RTestReflection.refl.h"

REFLECT()
class OC
{
	REFLECT_BODY()
		int c1;
	int c2;
	int c5;
	virtual void Test() {}
};

REFLECT()
class OA
{
	REFLECT_BODY()
		int a1;
	int a2;
	int a3;
	int a4;
	virtual void Truc() {}
};

REFLECT()
class OB : public OC, public OA
{
	REFLECT_BODY()

public:
	virtual void vB() {}

	OB() {}

	int b1 = 4;

	int b2 = 3;
};

class OE
{
	virtual void vE() {}
};

class OD : public OB, public OE
{
public:

	virtual void vD() {}
	int varD = 8;
};


REFLECT()
struct TestStr
{
	TestStr() = default;

	RFUNCTION()
		void PrintTest()
	{
		LOG("called printTest on TestStr");
	}
	REFLECT_BODY()

public:

	RCONSTRUCTOR()
	TestStr(int a, float b, double c, long d) {}

	RPROPERTY()
	int32_t intProp = -2;

	RPROPERTY()
	uint32_t propD = 6;

	RPROPERTY()
	const char* C_Prop_Test = nullptr;

	RPROPERTY()
	const unsigned long* auto_VaR = nullptr;
};

REFLECT()
class TestClass : public TestStr
{
	REFLECT_BODY()
public:
	TestClass() = default;

	RFUNCTION()
		void PrintTestClass()
	{
		LOG("called printTest on TestClass");
	}

	RCONSTRUCTOR()
		TestClass(int a, float b, double c, long d)
		: testProp1(a), testProp2(b), testProp3(c), testProp4(d), TestStr()
	{
		LOG("building test class with params : " + String(a) + " " + String(b) + " " + String(c) + " " + String(d));
		PrintTest();
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

	RPROPERTY()
		String testString = "toto";

};


REFLECT()
struct FooStruct {
	REFLECT_BODY()

		RCONSTRUCTOR()
		FooStruct(int test) { 
		LOG("instancied obj with arg : ");
	}

	FooStruct() { 
		LOG("instancied obj with default ctor"); 
	}

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


REFLECT()
class OObjectTest : public OObject
{
	REFLECT_BODY()

public:

	RPROPERTY()
		int testProperty1 = 2;

	RPROPERTY()
		double testProperty2 = 2;

	RPROPERTY()
		int testProperty3 = 2;
};
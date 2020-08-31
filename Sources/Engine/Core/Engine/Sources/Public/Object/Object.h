#pragma once

#include <memory>
#include "Serializer.h"
#include "Types\String.h"
#include "Object\Object.refl.h" // automatically generated reflection header




REFLECT()
class OObject : public Serializable
{
	REFLECT_BODY()

public:
	RCONSTRUCTOR()
	OObject() {}

	RPROPERTY()
		int var1 = 2;

	RPROPERTY()
		int var2 = 3;

	RPROPERTY()
		int var3 = 4;
};

REFLECT()
class TestClassA {
	REFLECT_BODY()

public:

	RPROPERTY()
		int var1 = 2;
};



REFLECT()
class TestClassB : public TestClassA {
	REFLECT_BODY()

public:

	RPROPERTY()
		int var2 = 2;
};

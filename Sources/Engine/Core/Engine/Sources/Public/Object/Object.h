#pragma once

#include "EngineMinimal.h"
#include "Types\Array.h"
#include "Object\Object.refl.h" // automatically generated reflection header


REFLECT()
class OObject
{
	REFLECT_BODY()

public:
	RCONSTRUCTOR()
	OObject() {}

	RPROPERTY()
		float var1 = 2;

	RPROPERTY()
		int32_t var2 = 3;

	RPROPERTY()
		float var3 = 4;

	RPROPERTY()
		TVector<float> var4 = { 5.f, 8.2f, 539.f,4.7f, 2.8f };
};

REFLECT()
class TestClassA {
	REFLECT_BODY()

public:

	RPROPERTY()
		int32_t var1 = 2;
};



REFLECT()
class TestClassB : public TestClassA {
	REFLECT_BODY()

public:

	RPROPERTY()
		int32_t var2 = 2;
};

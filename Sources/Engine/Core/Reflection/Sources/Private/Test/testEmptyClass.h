#pragma once

#include "Private\Test\testEmptyClass.refl.h"


REFLECT()
struct StrEmpTest
{

	REFLECT_BODY()

public:
	
	RCONSTRUCTOR()
	StrEmpTest()
	{
		std::cout << "test" << std::endl;
	}
};
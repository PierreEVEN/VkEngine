#include "ReflectionMacro.h"
#include <iostream>
#include "RProperty.h"
#include <functional>
#include "RStruct.h"


REFL_DECLARE_TYPE(bool);
REFL_DECLARE_TYPE(float);
REFL_DECLARE_TYPE(double);

REFL_DECLARE_TYPE(uint8_t);
REFL_DECLARE_TYPE(uint16_t);
REFL_DECLARE_TYPE(uint32_t);
REFL_DECLARE_TYPE(uint64_t);

REFL_DECLARE_TYPE(int8_t);
REFL_DECLARE_TYPE(int16_t);
REFL_DECLARE_TYPE(int32_t);
REFL_DECLARE_TYPE(int64_t);

struct Foo
{
	Foo()
	{
		std::cout << "create obj " << std::endl;
	}
	REFL_DECLARE_STRUCT(Foo)

		int val;
};


int main(int argc, char* argv[]) {
	REFL_REGISTER_TYPE(bool);
	REFL_REGISTER_TYPE(float);
	REFL_REGISTER_TYPE(double);

	REFL_REGISTER_TYPE(uint8_t);
	REFL_REGISTER_TYPE(uint16_t);
	REFL_REGISTER_TYPE(uint32_t);
	REFL_REGISTER_TYPE(uint64_t);

	REFL_REGISTER_TYPE(int8_t);
	REFL_REGISTER_TYPE(int16_t);
	REFL_REGISTER_TYPE(int32_t);
	REFL_REGISTER_TYPE(int64_t);


	RStructBuilder<Foo>* builder = new RStructBuilder<Foo>("Foo");
	builder->Ctor<>();
	//builder.Struct->NewStruct<Foo>();
}
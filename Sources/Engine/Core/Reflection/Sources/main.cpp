#include "ReflectionMacro.h"
#include <iostream>
#include "RStruct.h"
#include <vector>


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

struct Foo {
	REFL_DECLARE_STRUCT(Foo)
	Foo() { }
	Foo(int test) {	}
};
struct Foo1 : public Foo { REFL_DECLARE_STRUCT(Foo1) };
struct Foo2 : public Foo { REFL_DECLARE_STRUCT(Foo2) };
struct Foo3 : public Foo { REFL_DECLARE_STRUCT(Foo3) };
struct Foo4 : public Foo { REFL_DECLARE_STRUCT(Foo4) };

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

	/* Create new type named "Foo" */
	RStruct* fooStr = static_cast<RStruct*>(RType::RegisterType<RStruct>("Foo"));
	/* Assign Foo constructor */
	fooStr->SetCtor<>(std::function<void* ()>(&Foo::MakeStruct<>));

	/* Instantiate a new Foo */
	void* test = fooStr->Instantiate<void*>(10);
}
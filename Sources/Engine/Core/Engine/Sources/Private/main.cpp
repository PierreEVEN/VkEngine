
#include "EngineTypes.h"
#include "Object/Object.h"
#include "IO/EngineIO.h"
#include "IO/Log.h"
#include "Test/RTestReflection.h"


template<typename T, typename U> constexpr size_t offsetOf(U T::* member)
{
	return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}

struct A { int valBidon; };
struct B { };
struct C : public A, B { int valBidon2 = 12; };

int main(int argc, char* argv[]) {

	Serializable* obj = new OObject("TestOutFIle.Asset");
	obj->Save();

// 	RClass* cl = OObject::GetStaticClass();
// 
// 	std::cout << obj << std::endl;
// 	std::cout << reinterpret_cast<OObject*>(obj)->var << std::endl;
// 	std::cout << static_cast<OObject*>(obj)->var << std::endl;


// 	OA* test = new OB();
// 
// 	size_t VarOffset = (char*)&(((OB*)nullptr)->*&OB::b1) - (char*)nullptr; // Retrieve var offset
// 
// 	std::cout << sizeof(OC) << std::endl;
// 	std::cout << (size_t)test << std::endl;
// 	std::cout << (size_t)(OB*)test << std::endl << std::endl;
// 	std::cout << "dif : " << (size_t)test - (size_t)(OB*)test << std::endl;
// 
// 	std::cout << sizeof(OC) + sizeof(OA) << std::endl;
// 	std::cout << (size_t)&((OB*)test)->b1 << std::endl;
// 	std::cout << (size_t)test + (size_t)VarOffset << std::endl;
// 	std::cout << "dif : " << (size_t) & ((OB*)test)->b1 - (size_t)test + (size_t)VarOffset << std::endl;
}
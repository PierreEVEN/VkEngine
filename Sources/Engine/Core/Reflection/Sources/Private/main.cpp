#include "Test/RTestReflection.h"
#include "Reflection.h"


int main(int argc, char* argv[]) {
	RClass* cl = TestClass::GetStaticClass();

	TestClass* objTest = cl->Instantiate<TestClass, int, float, double, long>(1, 2, 3, 4);

	std::cout << "Properties : " << std::endl;
	for (const auto& prop : cl->GetProperties())
	{
		if (prop->GetType())
			std::cout << "	" << prop->GetType()->GetName() << " ";
		else
			std::cout << "	unknown ";

		std::cout << prop->GetName() << " = ";
		if (prop->Value<void>(objTest))
		{
			if (prop->GetType() == RType::GetType<int>()) std::cout << *prop->Value<int>(objTest) << ";" << std::endl;
			else if (prop->GetType() == RType::GetType<float>()) std::cout << *prop->Value<float>(objTest) << ";" << std::endl;
			else if (prop->GetType() == RType::GetType<int64_t>()) std::cout << *prop->Value<int64_t>(objTest) << ";" << std::endl;
			else if (prop->GetType() == RType::GetType<double>()) std::cout << *prop->Value<double>(objTest) << ";" << std::endl;
			else if (prop->GetType() == RType::GetType<TestStr>()) prop->Value<TestStr>(objTest)->PrintTest();
			else std::cout << prop->Value(objTest) << ";" << std::endl;
		}
		else
			std::cout << "null" << std::endl;
	}
}
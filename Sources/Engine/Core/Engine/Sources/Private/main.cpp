#include "Rendering.h"
#include "IO/IniLoader.h"
#include "IO/Log.h"


IniLoader TestIni("Config/test.ini");


struct testStrObj
{
	testStrObj(const char* testPar) : val(testPar) 
	{

		std::cout << val << std::endl;
	}

	const char* val;


	void test()
	{
		std::cout << val << std::endl;
	}

};


int main(int argc, char* argv[])
{
	Rendering::InitializeWindow();
	Rendering::InitializeRendering();

	Rendering::ExecuteRenderLoop();

	Rendering::CleanupRendering();
	Rendering::CleaneupWindow();
}

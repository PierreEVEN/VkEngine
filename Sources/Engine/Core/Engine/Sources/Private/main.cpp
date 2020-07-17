

#include "EngineTypes.h"
#include "Object/Object.h"
#include "IO/EngineIO.h"
#include "IO/Log.h"
#include "Test/RTestReflection.h"
#include "Events/EventManager.h"

int main(int argc, char* argv[])
{
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)

	static_assert(false, "ca marche");
#else
	static_assert(false, "merde");
#endif
}
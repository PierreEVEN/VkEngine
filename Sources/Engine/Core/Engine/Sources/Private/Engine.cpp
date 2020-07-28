#include "Engine.h"
#include "IO/Log.h"

void Engine::Initialize()
{
	int64_t i = 0;
	int64_t j = 5;
	for (i = 0; i < 5000000000; ++i)
	{
		j += 2;
	}
	LOG(String("complete ") << String::ToString(j));
}


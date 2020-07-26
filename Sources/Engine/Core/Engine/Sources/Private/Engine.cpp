#include "Engine.h"
#include "IO/Log.h"

void Engine::Initialize()
{
	for (int i = 0; i < 100000000; ++i)
	{
		i++;
	}
	LOG("complete");
}


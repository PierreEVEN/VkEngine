
#include "EngineMinimal.h"

#include "Rendering.h"
#include "Object/Object.h"
#include "Types/AssetPtr.h"


std::mutex testMutex;
std::condition_variable testCond;

int main(int argc, char* argv[])
{
	TestStruct test;

// 	OObject* obj = new OObject();
// 	OObject* obj2 = new OObject();
// 
// 	std::vector<Serialization::SerializedProperty> props = Serialization::SerializeObject(obj);
// 	Serialization::DeserializeObject(obj2, props);
// 
// 	for (const auto& prop : props) {
// 		LOG(String("Successfully serialized property '") + prop.GetName().data() + "' - dataSize : " + ToString(prop.GetSize()) + " (value = " + ToString(*static_cast<float*>(prop.GetData())) + ")");
// 	}
// 
// 	Serialization::DeserializeObject(obj2, props);
// 
// 
// 
// 	LOG("Deserialized values : var1 = " + ToString(obj2->var1) + " - var2 = " + ToString(obj2->var2) + " - val3 = " + ToString(obj->var3) + " - val4 = {" + String::ConcatenateArray(obj->var4) + "}");
// 
// 
// 
// 	return 0;


	JobSystem::Initialize();


	JobSystem::NewJob([] {
		Rendering::InitializeWindow();
		Rendering::InitializeRendering();

		Rendering::ExecuteRenderLoop();

		Rendering::CleanupRendering();
		Rendering::CleaneupWindow();
		testCond.notify_all();
		});


	std::unique_lock<std::mutex> ReleaseThreadsLock(testMutex);
	testCond.wait(ReleaseThreadsLock);
}

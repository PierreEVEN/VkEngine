#include "Types\AsyncProperty.refl.h" // automatically generated reflection header
#pragma once

REFLECT(options = { CreateSerializer }, type = TAsyncProperty<>);

struct IAsyncProperty {
	virtual void Load() = 0;
};

template<typename T>
struct TAsyncProperty : public IAsyncProperty {

	void Load() {}


private:

	bool bIsLoad = false;
	T value;
};

namespace Serialization {
	template<typename T>
	struct TSerializer<TAsyncProperty<T>>
	{
		static bool Serialize(const TAsyncProperty<T>* value, SerializedProperty& outValue) {
			static_assert(false, "not implemented yet");
			return false;
		}

		static bool Deserialize(const SerializedProperty& inProp, TAsyncProperty<T>* result) {
			static_assert(false, "not implemented yet");
			return false;
		}
	};
}
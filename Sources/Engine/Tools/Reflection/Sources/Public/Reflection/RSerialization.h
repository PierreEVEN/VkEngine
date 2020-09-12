#pragma once

#include "RClass.h"
#include "RProperty.h"
#include "RType.h"
#include <map>


namespace Serialization {

	struct SerializedProperty final {
		friend inline bool SerializeProperty(RProperty* inProperty, void* objectPointer, SerializedProperty& result);

		SerializedProperty() = default;

		SerializedProperty(void* dataPtr, const size_t dataSize) {
			data.resize(dataSize);
			memcpy(data.data(), dataPtr, dataSize);
		}

		inline size_t GetSize() const { return data.size(); }
		inline void* GetData() const { return (void*)data.data(); }
		inline const std::string& GetName() const { return propertyName; }

	private:

		std::string propertyName;
		std::vector<uint8_t> data;
	};

	template<typename T>
	struct TSerializer {
		static bool Serialize(const T* value, SerializedProperty& outValue) {
			return false;
		}

		static bool Deserialize(const SerializedProperty& inProp, T* result) {
			return false;
		}
	};

	template<typename T>
	struct IsSerializable
	{
	private:
		template<typename U> static auto MakeTest() -> decltype(U::Serialize(std::declval<const T*>(), std::declval<SerializedProperty&>()), std::true_type());
		template<typename> static std::false_type MakeTest(...);
	public:
		static constexpr bool value = std::is_same<decltype(MakeTest<TSerializer<T>>()), std::true_type>::value;
	};

	template<typename T>
	struct IsDeserializable
	{
	private:
		template<typename U> static auto MakeTest() -> decltype(U::Deserialize(std::declval<const SerializedProperty&>(), std::declval<T*>()), std::true_type());
		template<typename> static std::false_type MakeTest(...);
	public:
		static constexpr bool value = std::is_same<decltype(MakeTest<TSerializer<T>>()), std::true_type>::value;
	};

	struct ISerializer {

		friend ISerializer* GetSerializer(const std::string& inType);

		ISerializer(std::string inTypename)
		{
			serializers[inTypename] = this;
		}

		virtual bool SerializeProperty(RProperty* inProperty, void* objectPointer, SerializedProperty& outValue) = 0;
		virtual bool DeserializeProperty(RProperty* inProperty, void* objectPointer, const SerializedProperty& inValue) = 0;
	private:
		inline static std::map<const std::string, ISerializer*> serializers;
	};

	inline ISerializer* GetSerializer(const std::string& inType) {
		const auto& it = ISerializer::serializers.find(inType);
		if (it != ISerializer::serializers.end()) {
			return it->second;
		}
		return nullptr;
	}

	inline bool SerializeProperty(RProperty* inProperty, void* objectPointer, SerializedProperty& result) {
		if (inProperty->GetType()) {
			if (ISerializer* serializer = GetSerializer(inProperty->GetType()->GetName())) {
				if (serializer->SerializeProperty(inProperty, objectPointer, result)) {
					result.propertyName = inProperty->GetName();
					return true;
				}
			}
		}
		return false;
	}

	inline bool DeserializeProperty(RProperty* inProperty, void* objectPointer, const SerializedProperty& inValue) {
		if (inProperty->GetType()) {
			if (ISerializer* serializer = GetSerializer(inProperty->GetType()->GetName())) {
				return serializer->DeserializeProperty(inProperty, objectPointer, inValue);
			}
		}
		return false;
	}

	inline std::vector<SerializedProperty> SerializeObject(RClass* objectClass, void* objectPointer) {
		std::vector<SerializedProperty> properties;
		for (const auto& prop : objectClass->GetPropertiesRecursive()) {
			SerializedProperty result;
			if (SerializeProperty(prop.second, objectPointer, result)) {
				properties.push_back(result);
			}
		}
		return properties;
	}

	inline void DeserializeObject(RClass* objectClass, void* objectPointer, const std::vector<SerializedProperty>& inProperties) {
		for (const auto& prop : inProperties) {
			if (RProperty* foundProp = objectClass->GetProperty(prop.GetName())) {
				DeserializeProperty(foundProp, objectPointer, prop);
			}
		}
	}

	template <typename T>
	inline std::vector<SerializedProperty> SerializeObject(T* object) {
		return SerializeObject(T::GetStaticClass(), object);
	}

	template <typename T>
	inline void DeserializeObject(T* object, const std::vector<SerializedProperty>& inProperties) {
		DeserializeObject(T::GetStaticClass(), object, inProperties);
	}
}
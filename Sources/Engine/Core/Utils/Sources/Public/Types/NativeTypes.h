#pragma once

#include "Reflection.h"
#include "Types\NativeTypes.refl.h" // automatically generated reflection header


REFLECT()
class TestStruct {
	REFLECT_BODY()
public:
	TestStruct() = default;
};

REFLECT(options = { CreateSerializer }, type = bool)
REFLECT(options = { CreateSerializer }, type = float)
REFLECT(options = { CreateSerializer }, type = double)

REFLECT(options = { CreateSerializer }, type = uint8_t);
REFLECT(options = { CreateSerializer }, type = uint16_t);
REFLECT(options = { CreateSerializer }, type = uint32_t);
REFLECT(options = { CreateSerializer }, type = uint64_t);

REFLECT(options = { CreateSerializer }, type = int8_t);
REFLECT(options = { CreateSerializer }, type = int16_t);
REFLECT(options = { CreateSerializer }, type = int32_t);
REFLECT(options = { CreateSerializer }, type = int64_t);


namespace Serialization {

	template<>
	struct TSerializer<bool>
	{
		static bool Serialize(const bool* value, SerializedProperty& outValue) {
			outValue = SerializedProperty((void*)value, sizeof(bool));
			return true;
		}

		static bool Deserialize(const SerializedProperty& inProp, bool* result) {
			memcpy(result, inProp.GetData(), inProp.GetSize());
			return false;
		}
	};

	template<>
	struct TSerializer<float>
	{
		static bool Serialize(const float* value, SerializedProperty& outValue) {
			outValue = SerializedProperty((void*)value, sizeof(float));
			return true;
		}

		static bool Deserialize(const SerializedProperty& inProp, float* result) {
			memcpy(result, inProp.GetData(), inProp.GetSize());
			return false;
		}
	};

	template<>
	struct TSerializer<int8_t>
	{
		static bool Serialize(const int8_t* value, SerializedProperty& outValue) {
			outValue = SerializedProperty((void*)value, sizeof(int8_t));
			return true;
		}

		static bool Deserialize(const SerializedProperty& inProp, int8_t* result) {
			memcpy(result, inProp.GetData(), inProp.GetSize());
			return false;
		}
	};

	template<>
	struct TSerializer<int16_t>
	{
		static bool Serialize(const int16_t* value, SerializedProperty& outValue) {
			outValue = SerializedProperty((void*)value, sizeof(int16_t));
			return true;
		}

		static bool Deserialize(const SerializedProperty& inProp, int16_t* result) {
			memcpy(result, inProp.GetData(), inProp.GetSize());
			return false;
		}
	};

	template<>
	struct TSerializer<int32_t>
	{
		static bool Serialize(const int32_t* value, SerializedProperty& outValue) {
			outValue = SerializedProperty((void*)value, sizeof(int32_t));
			return true;
		}

		static bool Deserialize(const SerializedProperty& inProp, int32_t* result) {
			memcpy(result, inProp.GetData(), inProp.GetSize());
			return false;
		}
	};

	template<>
	struct TSerializer<int64_t>
	{
		static bool Serialize(const int64_t* value, SerializedProperty& outValue) {
			outValue = SerializedProperty((void*)value, sizeof(int64_t));
			return true;
		}

		static bool Deserialize(const SerializedProperty& inProp, int64_t* result) {
			memcpy(result, inProp.GetData(), inProp.GetSize());
			return false;
		}
	};

	template<>
	struct TSerializer<uint8_t>
	{
		static bool Serialize(const uint8_t* value, SerializedProperty& outValue) {
			outValue = SerializedProperty((void*)value, sizeof(uint8_t));
			return true;
		}

		static bool Deserialize(const SerializedProperty& inProp, uint8_t* result) {
			memcpy(result, inProp.GetData(), inProp.GetSize());
			return false;
		}
	};

	template<>
	struct TSerializer<uint16_t>
	{
		static bool Serialize(const uint16_t* value, SerializedProperty& outValue) {
			outValue = SerializedProperty((void*)value, sizeof(uint16_t));
			return true;
		}

		static bool Deserialize(const SerializedProperty& inProp, uint16_t* result) {
			memcpy(result, inProp.GetData(), inProp.GetSize());
			return false;
		}
	};

	template<>
	struct TSerializer<uint32_t>
	{
		static bool Serialize(const uint32_t* value, SerializedProperty& outValue) {
			outValue = SerializedProperty((void*)value, sizeof(uint32_t));
			return true;
		}

		static bool Deserialize(const SerializedProperty& inProp, uint32_t* result) {
			memcpy(result, inProp.GetData(), inProp.GetSize());
			return false;
		}
	};

	template<>
	struct TSerializer<uint64_t>
	{
		static bool Serialize(const uint64_t* value, SerializedProperty& outValue) {
			outValue = SerializedProperty((void*)value, sizeof(uint64_t));
			return true;
		}

		static bool Deserialize(const SerializedProperty& inProp, uint64_t* result) {
			memcpy(result, inProp.GetData(), inProp.GetSize());
			return false;
		}
	};

	template<>
	struct TSerializer<double>
	{
		static double Serialize(const double* value, SerializedProperty& outValue) {
			outValue = SerializedProperty((void*)value, sizeof(double));
			return true;
		}

		static double Deserialize(const SerializedProperty& inProp, double* result) {
			memcpy(result, inProp.GetData(), inProp.GetSize());
			return false;
		}
	};
}

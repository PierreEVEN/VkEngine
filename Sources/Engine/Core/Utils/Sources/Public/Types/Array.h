#pragma once

#include <initializer_list>
#include <xutility>
#include "Types\Array.refl.h" // automatically generated reflection header

REFLECT(options = { CreateSerializer }, type = TVector<>);

template<typename T>
class TVector
{
public:

	/** Default ctor */
	TVector() = default;
	~TVector() { if (data) delete data; }
	
	/** size ctor */
	TVector(const std::vector<T>& right) {
		Resize(right.size());
		memcpy(data, &right.begin(), right.size() * sizeof(T));
	}

	/** initializer ctor */
	TVector(std::initializer_list<T> iniList) {
		Resize(iniList.size());
		memcpy(data, iniList.begin(), iniList.size() * sizeof(T));
	}

	/** Copy ctor */
	TVector(TVector<T>& right) {
		Resize(right.dataLength);
		memcpy(data, right.data, right.dataLength * sizeof(T));
	}

	/** Move ctor */
	TVector(TVector<T>&& right) {
		data = std::move(right.data);
		dataLength = std::move(right.dataLength);
	}

	TVector& operator=(TVector&& right) {
		Resize(right.dataLength);
		memcpy(data, right.data, right.dataLength * sizeof(T));
		return *this;
	}

	T* GetData() { return data; }
	const T* GetData() const { return data; }

	const size_t& Length() const { return dataLength; }

	T& operator[](const size_t& index) { return data[index]; }
	const T& operator[](const size_t& index) const { return data[index]; }

	/** Add */
	void Add(const T& elem) {
		Resize(dataLength + 1);
		data[dataLength - 1] = elem;
	}

	/** Pop */
	T Pop() {
		T res = data[dataLength - 1];
		Resize(dataLength - 1);
		return res;
	}

	/** Iterators */
	typedef T* iterator;
	typedef const T* const_iterator;
	iterator begin() { return &data[0]; }
	const_iterator begin() const { return &data[0]; }
	const_iterator cbegin() const { return &data[0]; }
	iterator end() { return &data[dataLength]; }
	const_iterator end() const { return &data[dataLength]; }
	const_iterator cend() const { return &data[dataLength]; }

	/** Vector operations */
	void Resize(const size_t& inSize) {
		if (inSize <= 0 && data) {
			delete data;
			data = nullptr;
			dataLength = 0;
		}
		else {
			dataLength = inSize;
			data = static_cast<T*>(realloc(data, dataLength * sizeof(T)));
		}
	}

private:

	size_t dataLength = 0;
	T* data = nullptr;
};

namespace Serialization {

	template<typename T>
	struct TSerializer<TVector<T>>
	{
		static double Serialize(const TVector<T>* value, SerializedProperty& outValue) {
			outValue = SerializedProperty((void*)value->GetData(), (size_t)(value->Length() * sizeof(T)));
			return true;
		}

		static double Deserialize(const SerializedProperty& inProp, TVector<T>* result) {
			result->Resize(inProp.GetSize() / sizeof(T));
			memcpy(result->GetData(), inProp.GetData(), inProp.GetSize());
			return true;
		}
	};
}

#pragma once
#include "IniLoader.h"
#include "Log.h"

template<typename T>
struct TConVar {
	inline TConVar(IniLoader& iniFileSource, const String& inCategoryName, const String& inPropertyName, const T& inDefaultValue, const T& inMin, const T& inMax, const String& inDescription = "")
		: fileSource(iniFileSource), propertyName(inPropertyName), CategoryName(inCategoryName), description(inDescription), bHasBeenChanged(false), DefaultValue(inDefaultValue), min(inMin), max(inMax)
	{
		Value = (fileSource.*TIniGetSetLink<T>::GetFuncPtr())(CategoryName, propertyName, DefaultValue);
	}

	inline TConVar(IniLoader& iniFileSource, const String& inCategoryName, const String& inPropertyName, const T& inDefaultValue, const String& inDescription = "")
		: fileSource(iniFileSource), propertyName(inPropertyName), CategoryName(inCategoryName), description(inDescription), bHasBeenChanged(false), DefaultValue(inDefaultValue)
	{
		Value = (fileSource.*TIniGetSetLink<T>::GetFuncPtr())(CategoryName, propertyName, DefaultValue);
	}

	inline ~TConVar() {
		if (bHasBeenChanged) {
			if (DefaultValue == Value) {
				fileSource.ClearProperty(CategoryName, propertyName);
			}
			else {
				(fileSource.*TIniGetSetLink<T>::SetFuncPtr())(CategoryName, propertyName, Value);
			}
		}
	}

	inline const T& GetValue() const { return Value; }
	inline void SetValue(const T& inValue) {
		if (Value != inValue)
		{
			Value = inValue;
			bHasBeenChanged = true;
			OnPropertyChanged.Execute(Value);
		}
	}

	DelegateMultiCast<const T&> OnPropertyChanged;

	inline const String& GetCategory() const { return CategoryName; }
	inline const String& GetName() const { return propertyName; }
	inline const T& GetDefault() const { return DefaultValue; }
	inline const T& GetMin() const { return min; }
	inline const T& GetMax() const { return max; }

private:

	T Value;
	T DefaultValue;
	T min;
	T max;

	bool bHasBeenChanged;

	String propertyName;
	String CategoryName;
	String description;
	IniLoader& fileSource;
};

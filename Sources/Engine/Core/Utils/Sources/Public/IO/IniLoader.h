#pragma once

#include"Types/String.h"

/**
 * @IniLoader - Read and write ini files
 *
 * 1) Create a new IniLoader with desired file path
 *			IniLoader myLoader("path/config.ini");
 *
 * 2) Read and write property from your loader
 *			String value = myLoader.GetPropertyAsString("categoryName", "propertyName", "defaultValue");
 *			myLoader.SetPropertyAsString("categoryName", "propertyName", "value");
 *
 * 3) Clear a property value by writing an empty string
 *			myLoader.SetPropertyAsString("categoryName", "propertyName", "");
 *
 * 3) Don't forget to save modifications
 *			myLoader.Save();
 *
 */


class IniLoader
{
public:

	/** Create a new ini loader - automatically load file at designed path or create path if file doesn't exist. Ini file must have '.ini extension' */
	IniLoader(String filePath);

	/** save ini modifications, then delete resources */
	~IniLoader();

	/** Get ini property from category and property name. Default value is returned if we can't find any occurrence */
	const String GetPropertyAsString(const String& categoryName, const String& propertyName, const String& defaultValue = "");
	const double GetPropertyAsDouble(const String& categoryName, const String& propertyName, const double& defaultValue = 0.f);
	const int32_t GetPropertyAsInt(const String& categoryName, const String& propertyName, const int32_t& defaultValue = 0);
	const bool GetPropertyAsBool(const String& categoryName, const String& propertyName, const bool& defaultValue = false);

	/** Set ini property (Save() must be called to save modifications on disk) */
	void SetPropertyAsString(const String& categoryName, const String& propertyName, const String& propertyValue) { SetProperty(categoryName, propertyName, '"' + propertyValue + '"'); }
	void SetPropertyAsDouble(const String& categoryName, const String& propertyName, const double& propertyValue) { SetProperty(categoryName, propertyName, ToString(propertyValue)); }
	void SetPropertyAsInt(const String& categoryName, const String& propertyName, const int32_t& propertyValue) { SetProperty(categoryName, propertyName, ToString(propertyValue)); }
	void SetPropertyAsBool(const String& categoryName, const String& propertyName, const bool& propertyValue) { SetProperty(categoryName, propertyName, propertyValue ? "true" : "false"); }

	/** Create or update ini file on disk */
	void Save();

	/** Remove property from file */
	void ClearProperty(const String& categoryName, const String& propertyName);

private:

	/** Ini structures */
	struct IniProperty
	{
		IniProperty(const String& propertyName, const String& propertyValue);

		String propertyName;
		String value;

		String WriteLine() const;
		static void GetPropertyNameAndValueFromString(const String& line, String& name, String& value);
		static bool IsPropertyLine(const String& line);
	};

	struct IniCategory
	{
		IniCategory(const String& categoryname);

		String categoryName;
		std::vector<IniProperty> properties;

		void AddProperty(const String& propertyName, const String& propertyValue);
		String WriteCategories() const;

		bool DoesPropertyExist(const String& propertyName) const;
		static const String GetCategoryNameFromString(const String& line);
		static bool IsCategoryLine(const String& line);
	};

	/** Internal methods */
	const String GetProperty(const String& categoryName, const String& propertyName) const;
	void SetProperty(const String& categoryName, const String& propertyName, const String& propertyValue);
	bool DoesCategoryExist(const String& propertyName) const;

	void LinkOrCreate();

	/** ini file path */
	String sourceFile;

	/** does ini be saved */
	bool bDoesBeSaved;

	/** Ini categories (each category contains a property vector) */
	std::vector<IniCategory*> iniCategories;
};

/** Get function ptr */
template<typename T>
using IniPropertyGetterFunc = const T(IniLoader::*)(const String&, const String&, const T&);

/** Set function ptr */
template<typename T>
using IniPropertySetterFunc = void (IniLoader::*)(const String&, const String&, const T&);

/** Handle get / set function ptr */
template<typename T>
struct TIniGetSetLink { };

/** Bool specialization */
template<>
struct TIniGetSetLink<bool> {
	inline static IniPropertyGetterFunc<bool> GetFuncPtr() { return &IniLoader::GetPropertyAsBool;	}
	inline static IniPropertySetterFunc<bool> SetFuncPtr() { return &IniLoader::SetPropertyAsBool;	}
};

/** Double specialization */
template<>
struct TIniGetSetLink<double> {
	inline static IniPropertyGetterFunc<double> GetFuncPtr() { return &IniLoader::GetPropertyAsDouble; }
	inline static IniPropertySetterFunc<double> SetFuncPtr() { return &IniLoader::SetPropertyAsDouble; }
};

/** Int specialization */
template<>
struct TIniGetSetLink<int32_t> {
	inline static IniPropertyGetterFunc<int32_t> GetFuncPtr() { return &IniLoader::GetPropertyAsInt; }
	inline static IniPropertySetterFunc<int32_t> SetFuncPtr() { return &IniLoader::SetPropertyAsInt; }
};

/** String specialization */
template<>
struct TIniGetSetLink<String> {
	inline static IniPropertyGetterFunc<String> GetFuncPtr() { return &IniLoader::GetPropertyAsString; }
	inline static IniPropertySetterFunc<String> SetFuncPtr() { return &IniLoader::SetPropertyAsString; }
};


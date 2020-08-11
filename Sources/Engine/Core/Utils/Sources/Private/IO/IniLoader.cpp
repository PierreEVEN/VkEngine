#include "IO/IniLoader.h"
#include <filesystem>
#include <iostream>
#include <fstream>

IniLoader::IniLoader(const char* filePath)
	: sourceFile(filePath) {
	LinkOrCreate();
}

IniLoader::~IniLoader() {
	Save();
	for (const auto& cat : iniCategories)
	{
		delete cat;
	}
	iniCategories.clear();
}

const String IniLoader::GetPropertyAsString(const char* categoryName, const char* propertyName, const char* defaultValue) {
	String props = GetProperty(categoryName, propertyName);
	if (props != "")
	{
		String left, center, right, finalS;
		if (String::SplitString(props, { '"' }, left, center, true))
		{
			if (String::SplitString(center, { '"' }, finalS, right, false))
			{
				return finalS.GetData();
			}
		}
	}
	return defaultValue;
}

const int IniLoader::GetPropertyAsInt(const char* categoryName, const char* propertyName, const int& defaultValue)
{
	String props = GetProperty(categoryName, propertyName);
	if (props != "")
	{
		return Atoi(props);
	}
	return defaultValue;
}

const double IniLoader::GetPropertyAsDouble(const char* categoryName, const char* propertyName, const double& defaultValue)
{
	String props = GetProperty(categoryName, propertyName);
	if (props != "")
	{
		return Atof(props);
	}
	return defaultValue;
}

const bool IniLoader::GetPropertyAsBool(const char* categoryName, const char* propertyName, const bool& defaultValue)
{
	String props = GetProperty(categoryName, propertyName);
	if (props == "true") return true;
	else if (props == "false") return false;
	return defaultValue;
}

void IniLoader::Save()
{
	std::ofstream of(sourceFile.GetData());

	for (const auto& cat : iniCategories)
	{
		String catString = cat->WriteCategories();
		of.write(catString.GetData(), catString.Length());
	}
	of.close();
}

const String IniLoader::GetProperty(const String& categoryName, const String& propertyName) const
{
	for (const auto& cat : iniCategories)
	{
		if (cat->categoryName == categoryName)
		{
			for (const auto& prop : cat->properties)
			{
				if (prop.propertyName == propertyName)
				{
					return prop.value;
				}
			}
		}
	}
	return "";
}

void IniLoader::SetProperty(const String& categoryName, const String& propertyName, const String& propertyValue)
{
	if (propertyValue == "")
	{
		ClearProperty(categoryName, propertyName);
	}
	else
	{
		for (auto& category : iniCategories)
		{
			if (category->categoryName == categoryName)
			{
				for (auto& prop : category->properties)
				{
					if (prop.propertyName == propertyName)
					{
						prop.value = propertyValue;
						return;
					}
				}
				category->properties.push_back(IniProperty(propertyName, propertyValue));
				return;
			}
		}
		IniCategory* newCat = new IniCategory(categoryName);
		newCat->properties.push_back(IniProperty(propertyName, propertyValue));
		iniCategories.push_back(newCat);
		return;
	}
}

void IniLoader::ClearProperty(const String& categoryName, const String& propertyName)
{
	for (int i = (int)iniCategories.size() - 1; i >= 0; --i)
	{
		if (iniCategories[i]->categoryName == categoryName)
		{
			for (int j = (int)iniCategories[i]->properties.size() - 1; j >= 0; --j)
			{
				if (iniCategories[i]->properties[j].propertyName == propertyName)
				{
					iniCategories[i]->properties.erase(iniCategories[i]->properties.begin() + j);
				}
			}
			if (iniCategories[i]->properties.size() == 0)
			{
				delete iniCategories[i];
				iniCategories.erase(iniCategories.begin() + i);
			}
			return;
		}
	}
}

bool IniLoader::DoesCategoryExist(const String& propertyName) const
{
	for (const auto& cat : iniCategories)
	{
		if (cat->categoryName == propertyName) return true;
	}
	return false;
}

void IniLoader::LinkOrCreate()
{
	if (!String::IsEndingWith(sourceFile, ".ini")) { std::cerr << "Ini files must have '.ini' extension" << std::endl; }

	if (!std::filesystem::exists(sourceFile.GetData()))
	{
		String left, right;
		String::SplitString(sourceFile, { '/', '\'' }, left, right, false);
		std::filesystem::create_directories(left.GetData());
	}

	std::ifstream fs(sourceFile.GetData());
	char* line = new char[1000];
	IniCategory* currentCategory = nullptr;
	while (fs.getline(line, 1000, '\n'))
	{
		String resultLine, right;
		if (!String::SplitString(line, { ';', '#' }, resultLine, right))
		{
			resultLine = line;
		}


		if (IniCategory::IsCategoryLine(resultLine))
		{
			String catName = IniCategory::GetCategoryNameFromString(resultLine);
			if (!DoesCategoryExist(catName))
			{
				currentCategory = new IniCategory(catName);
				iniCategories.push_back(currentCategory);
			}
			else
			{
				for (const auto& cat : iniCategories)
				{
					if (cat->categoryName == catName) currentCategory = cat;
				}
			}
		}
		else
		{
			if (currentCategory && IniProperty::IsPropertyLine(resultLine))
			{
				String name, value;
				IniProperty::GetPropertyNameAndValueFromString(line, name, value);
				if (!currentCategory->DoesPropertyExist(name))
				{
					currentCategory->AddProperty(name, value);
				}
			}
		}
	}
	delete line;
	fs.close();
}

IniLoader::IniProperty::IniProperty(const String& inPropertyName, const String& inPropertyValue) {
	propertyName = inPropertyName;
	value = inPropertyValue;
}

String IniLoader::IniProperty::WriteLine() const {
	return propertyName+ '=' + value + '\n';
}

void IniLoader::IniProperty::GetPropertyNameAndValueFromString(const String& line, String& name, String& value)
{
	String::SplitString(line, { '=' }, name, value);
	name = String::RemoveBorderSpaces(name);
	value = String::RemoveBorderSpaces(value);
}

bool IniLoader::IniProperty::IsPropertyLine(const String& line)
{
	String left, right;
	return (String::SplitString(line, { '=' }, left, right, true) && right != "");
}

IniLoader::IniCategory::IniCategory(const String& inCategoryName)
{
	categoryName = inCategoryName;
}

void IniLoader::IniCategory::AddProperty(const String& propertyName, const String& propertyValue)
{
	properties.push_back(IniProperty(propertyName, propertyValue));
}

String IniLoader::IniCategory::WriteCategories() const
{
	String outString = '[' + categoryName + "]\n";
	for (const auto& prop : properties)
	{
		outString += prop.WriteLine();
	}
	return outString + "\n";
}

bool IniLoader::IniCategory::DoesPropertyExist(const String& propertyName) const
{
	for (const auto& prop : properties)
	{
		if (prop.propertyName == propertyName) return true;
	}
	return false;
}

const String IniLoader::IniCategory::GetCategoryNameFromString(const String& line)
{
	String left, center, right, categoryName;
	String::SplitString(line, { '[' }, left, center);
	String::SplitString(center, { ']' }, categoryName, right);
	return categoryName;
}

bool IniLoader::IniCategory::IsCategoryLine(const String& line)
{
	return String::IsStartingWith(line, '[') && String::IsEndingWith(line, ']');
}
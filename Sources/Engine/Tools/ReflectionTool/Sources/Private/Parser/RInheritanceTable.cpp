#include "Parser/RInheritanceTable.h"
#include <iostream>

struct InheritanceTableElement;
bool GetElem(std::string className, InheritanceTableElement& elem);

struct InheritanceTableElement {
	InheritanceTableElement() = default;
	InheritanceTableElement(const std::string& inName, const std::vector<std::string>& inParents)
		: parents(inParents), className(inName) {}
	std::string className;
	std::vector<std::string> parents;

	std::vector<std::string> GetParents() const
	{
		std::vector<std::string> parentArray;
		for (const auto& elem : parents)
		{
			parentArray.push_back(elem);
			InheritanceTableElement parentElem;
			if (GetElem(elem, parentElem))
			{
				for (const auto& parent : parentElem.GetParents())
				{
					parentArray.push_back(parent);
				}
			}			
		}
		return parentArray;
	}
};

std::vector<InheritanceTableElement> table;


bool GetElem(std::string className, InheritanceTableElement& elem)
{
	for (auto& element : table)
	{
		if (className == element.className)
		{
			elem = element;
			return true;
		}
	}
	return false;
}


void InheritanceTable::RegisterClass(const std::string& className, const std::vector<std::string>& parents)
{
	for (const auto& elem : table)
	{
		if (className == elem.className) return;
	}
	table.push_back(InheritanceTableElement(className, parents));
}

bool InheritanceTable::IsChildOf(const std::string& child, const std::string& parent)
{
	for (const InheritanceTableElement& elem : table)
	{
		if (child == elem.className)
		{
			for (const std::string& foundParent : elem.GetParents())
			{
				if (foundParent == parent) return true;
			}
			return false;
		}
	}
	return false;
}

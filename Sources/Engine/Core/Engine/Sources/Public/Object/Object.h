#pragma once

#include <memory>
#include "Serializer.h"
#include "Types\String.h"
#include "Object\Object.refl.h" // automatically generated reflection header


REFLECT()
class OObject : public IStringable, public Serializable
{
	REFLECT_BODY()

public:
	OObject() {}
	OObject(const String& serialisationPath) : Serializable(serialisationPath) {}

	virtual String ToString() const override
	{
		return GetName();
	}

	virtual String GetName() const
	{
		return "not implemented yet";
	}

	RPROPERTY()
		int var = 2;
};


#include "Utils/FileLibrary.h"
#include "Utils/FileWriter.h"
#include <memory>
#include "Utils/StringLibrary.h"
#include "Parser/RClassParser.h"
#include "Parser/RInheritanceTable.h"

RClassParser::RClassParser(const LineReader& inData, uint32_t startLine, const std::string& path, const uint64_t& uniqueID)
	: data(inData), RBodyLine(startLine), classpath(path), fileUniqueID(uniqueID)
{
	if (StringLibrary::GetStringField(data.GetLine(1), {' ', '\t'}, 0) == "class")
	{
		bisStruct = false;
		bIsClass = true;
	}
	else if (StringLibrary::GetStringField(data.GetLine(1), { ' ', '\t' }, 0) == "struct")
	{
		bisStruct = true;
		bIsClass = false;
	}
	else
	{
		bIsClass = false;
		bisStruct = false;
	}
	if (bisStruct || bIsClass)
	{
		if (!StringLibrary::GetStringField(data.GetLine(1), { ' ', '\t' }, 1, className)) { std::cerr << "failed to get class name : " << data.GetLine(1) << std::endl; }
		
		std::string left, pars, right;
		if (StringLibrary::SplitLine(data.GetLine(1), { ':' }, left, pars))
		{
			if (StringLibrary::SplitLine(pars, { '{' }, left, right)) pars = left;
			for (const std::string& field : StringLibrary::GetStringFields(pars, { ',' }))
			{
				std::vector<std::string> cont = StringLibrary::GetStringFields(field, { ' ', '\t' });
				if (cont.size() <= 0) std::cerr << "failed to parse parent" << std::endl;
				else
				{
					parents.push_back(StringLibrary::CleanupLine(cont.size() == 2 ? cont[1] : cont[0]));
				}
			}
			InheritanceTable::RegisterClass(className, parents);
		}
	}
	else
	{
		std::string left, right;
		StringLibrary::SplitLine(data.GetLine(0), { '(' }, left, right, true);
		std::string newRight = right;
		StringLibrary::SplitLine(newRight, { ')' }, left, right, false);
		className = StringLibrary::CleanupLine(left);

	}

	ExtractContent();
}

void RClassParser::WriteHeader(OSWriter& writer)
{
	if (!IsValid()) return;
	writer.WriteLine("\n\n\n\n/* ##############################  Reflection for " + className + "  ############################## */\n");
	if (bIsClass || bisStruct) writer.WriteLine(std::string(bisStruct ? "struct " : "class ") + className + "; // Forward declaration");
	if (bIsClass || bisStruct)
	{
		writer.WriteLine("\n#define _REFLECTION_BODY_RUID_" + std::to_string(fileUniqueID) + "_LINE_" + std::to_string(RBodyLine) + std::string(" ") + (bIsClass ? "REFL_DECLARE_CLASS(" + className + ")" : "REFL_DECLARE_STRUCT(" + className + ") // Declare REFLECT_BODY() macro"));
	}
	writer.WriteLine("\nREFL_DECLARE_TYPENAME(" + className + "); // Declare typename");
	writer.WriteLine("\ntemplate<> struct RIsReflected<" + className + "> { static constexpr bool Reflect = true; }; // Used to detect if class is reflected or not");

}

void RClassParser::WriteSource(OSWriter& writer)
{
	writer.WriteLine("\n\n\n\n/* ##############################  Reflection for " + className + "  ############################## */\n");
	if (bisStruct || bIsClass)
	{
		if (bisStruct)
		{
			writer.WriteLine("static RStruct* _static_Item_Class_" + className + " = nullptr; //Static struct reference");
			writer.WriteLine("RStruct* " + className + "::GetStaticStruct() { return _static_Item_Class_" + className + "; } //Static struct getter\n");
			writer.WriteLine("RStruct* " + className + "::GetStruct() const { return _static_Item_Class_" + className + "; } //struct getter\n");
		}
		if (bIsClass)
		{
			writer.WriteLine("static RClass* _static_Item_Class_" + className + " = nullptr; //Static class reference");
			writer.WriteLine("RClass* " + className + "::GetStaticClass() { return _static_Item_Class_" + className + "; } //Static class getter\n");
			writer.WriteLine("RClass* " + className + "::GetClass() const { return _static_Item_Class_" + className + "; } //class getter\n");
		}
		writer.WriteLine("void _Refl_Register_Item_" + className + "() { // Register function");
		if (bisStruct) writer.WriteLine("		_static_Item_Class_" + className + " = RStruct::RegisterStruct<" + className + (ctor && ctor->args.size() > 0 ? "," + StringLibrary::ConcatString(ctor->args, ", ") : "") + ">(\"" + className + "\"); //Register Struct");
		if (bIsClass) writer.WriteLine("		_static_Item_Class_" + className + " = RClass::RegisterClass<" + className + (ctor && ctor->args.size() > 0 ? "," + StringLibrary::ConcatString(ctor->args, ", ") : "") + ">(\"" + className + "\"); //Register Class");
		for (const std::string& parent : parents)
		{
			writer.WriteLine("		if (RIsReflected<" + parent + ">::Reflect) // Is parent reflected");
			writer.WriteLine("			_static_Item_Class_" + className + "->AddParent(\"" + parent + "\"); // register parent");
		}

		if (properties.size() > 0) writer.WriteLine("		size_t VarOffset; // Var offset");
		for (const RPropertyParser& prop : properties)
		{
			writer.WriteLine("		VarOffset = (char*)&((" + className + "*)nullptr->* & " + className + "::" + prop.propertyName + ") - (char*)nullptr; // Retrieve var offset");
			writer.WriteLine("		_static_Item_Class_" + className + "->RegisterProperty(new RProperty(RType::GetType(\"" + prop.propertyType + "\"), VarOffset, sizeof(" + prop.propertyType + "), \"" + prop.propertyName + "\")); // Register property");
		}
		for (const RFunctionParser& func : functions)
		{
			std::string paramString = StringLibrary::ConcatString(func.params, { ", " });
			writer.WriteLine("		_static_Item_Class_" + className + "->RegisterFunction(RFunction<" + func.returnType + (paramString != "" ? ", " + paramString : "") + (func.bIsStatic ? "" : "," + className) + ">(\"" + func.functionName + "\", std::function<" + func.returnType + " (" + (func.bIsStatic ? paramString : className + "&" + (paramString == "" ? "" : ", " + paramString)) + ")>(&" + className + "::" + func.functionName + (paramString != "" ? "<" + paramString  + ">" : "") + "))); // Register function");
		}
		writer.WriteLine("}\n");
		writer.WriteLine("struct _Refl_Static_Item_Builder_" + className + "{ // Item builder - Build reflection data");
		writer.WriteLine("	_Refl_Static_Item_Builder_" + className + "() { // Builder constructor");
		writer.WriteLine("		_Refl_Register_Item_" + className + "(); // Call to builder function");
		writer.WriteLine("	}");
		writer.WriteLine("};\n");
		writer.WriteLine("static _Refl_Static_Item_Builder_" + className + " _Refl_Static_Item_Builder_Var_" + className + "; //Build item when compiled");
	}
	else
	{
		writer.WriteLine("struct _Refl_static_type_builder_" + StringLibrary::ReplaceCharWith(className, ':', '_') + " { // Type builder - Build reflection data");
		writer.WriteLine("\t_Refl_static_type_builder_" + StringLibrary::ReplaceCharWith(className, ':', '_') + "() { // Builder constructor");
		writer.WriteLine("\t\tREFL_REGISTER_TYPE(" + className + "); // Call to builder function");
		writer.WriteLine("\t}");
		writer.WriteLine("};");
		writer.WriteLine("static _Refl_static_type_builder_" + StringLibrary::ReplaceCharWith(className, ':', '_') + " _Refl_static_type_builder_var_" + StringLibrary::ReplaceCharWith(className, ':', '_') + "; //Build item when compiled");
	}

}

void RClassParser::ExtractContent()
{
	bool bFoundBody = !(bIsClass || bisStruct);
	for (int i = 0; i < data.Lines(); ++i)
	{
		if (StringLibrary::IsStartingWith(data.GetLine(i), "RCONSTRUCTOR("))
		{
			if (ctor.get())
			{
				std::cerr << "cannot have multiple reflected constructor for one class" << std::endl;
			}
			else
			{
				ctor = std::make_unique<RCtorParser>(RCtorParser(data.GetLine(i + 1)));
			}
		}
		if (StringLibrary::IsStartingWith(data.GetLine(i), "RPROPERTY"))
		{
			properties.push_back(RPropertyParser(data.GetLine(i + 1)));
		}
		if (StringLibrary::IsStartingWith(data.GetLine(i), "RFUNCTION("))
		{
			functions.push_back(RFunctionParser(data.GetLine(i + 1)));
		}
		if (StringLibrary::IsStartingWith(data.GetLine(i), "REFLECT_BODY("))
		{
			RBodyLine += i;
		}
	}
}

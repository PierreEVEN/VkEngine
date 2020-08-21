
#include "Utils/FileLibrary.h"
#include "Utils/FileWriter.h"
#include <memory>
#include "Utils/StringLibrary.h"
#include "Parser/RClassParser.h"
#include "Parser/RInheritanceTable.h"

RClassParser::RClassParser(const LineReader& inData, uint32_t startLine, const std::string& path, const uint64_t& uniqueID, const std::string& inClassNamespace)
	: data(inData), RBodyLine(startLine), classpath(path), fileUniqueID(uniqueID), classNamespace(inClassNamespace)
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
	if (classNamespace != "")
	{
		writer.WriteLine("namespace " + classNamespace);
		writer.WriteLine("{");
	}
	if (bIsClass || bisStruct) writer.WriteLine('\t' + std::string(bisStruct ? "struct " : "class ") + className + "; // Forward declaration");
	if (bIsClass || bisStruct)
	{
		writer.WriteLine("\n\t#define _REFLECTION_BODY_RUID_" + std::to_string(fileUniqueID) + "_LINE_" + std::to_string(RBodyLine) + std::string(" ") + (bIsClass ? "REFL_DECLARE_CLASS(" + className + ")" : "REFL_DECLARE_STRUCT(" + className + ") // Declare REFLECT_BODY() macro"));
	}
	writer.WriteLine("\n\tREFL_DECLARE_TYPENAME(" + className + "); // Declare typename");
	writer.WriteLine("\n\ttemplate<> struct RIsReflected<" + className + "> { static constexpr bool Reflect = true; }; // Used to detect if class is reflected or not");

	if (classNamespace != "") writer.WriteLine("}");
}

void RClassParser::WriteSource(OSWriter& writer)
{
	writer.WriteLine("\n\n\n\n/* ##############################  Reflection for " + className + "  ############################## */\n");
	if (classNamespace != "")
	{
		writer.WriteLine("namespace " + classNamespace);
		writer.WriteLine("{");
	}
	if (bisStruct || bIsClass)
	{
		if (bisStruct)
		{
			writer.WriteLine("\tstatic RStruct* _static_Item_Class_" + className + " = nullptr; //Static struct reference");
			writer.WriteLine("\tRStruct* " + className + "::GetStaticStruct() { return _static_Item_Class_" + className + "; } //Static struct getter\n");
			writer.WriteLine("\tRStruct* " + className + "::GetStruct() const { return _static_Item_Class_" + className + "; } //struct getter\n");
		}
		if (bIsClass)
		{
			writer.WriteLine("\tstatic RClass* _static_Item_Class_" + className + " = nullptr; //Static class reference");
			writer.WriteLine("\tRClass* " + className + "::GetStaticClass() { return _static_Item_Class_" + className + "; } //Static class getter\n");
			writer.WriteLine("\tRClass* " + className + "::GetClass() const { return _static_Item_Class_" + className + "; } //class getter\n");
		}
		writer.WriteLine("\tvoid _Refl_Register_Item_" + className + "() { // Register function");
		if (bisStruct) writer.WriteLine("\t		_static_Item_Class_" + className + " = RStruct::RegisterStruct<" + className + (ctor && ctor->args.size() > 0 ? "," + StringLibrary::ConcatString(ctor->args, ", ") : "") + ">(\"" + className + "\"); //Register Struct");
		if (bIsClass) writer.WriteLine("\t		_static_Item_Class_" + className + " = RClass::RegisterClass<" + className + (ctor && ctor->args.size() > 0 ? "," + StringLibrary::ConcatString(ctor->args, ", ") : "") + ">(\"" + className + "\"); //Register Class");
		for (const std::string& parent : parents)
		{
			writer.WriteLine("\t		if (RIsReflected<" + parent + ">::Reflect) // Is parent reflected");
			writer.WriteLine("\t			_static_Item_Class_" + className + "->AddParent(\"" + parent + "\"); // register parent");
		}

		if (properties.size() > 0) writer.WriteLine("\t		size_t VarOffset; // Var offset");
		for (const RPropertyParser& prop : properties)
		{
			writer.WriteLine("\t		VarOffset = (char*)&((" + className + "*)nullptr->* & " + className + "::" + prop.propertyName + ") - (char*)nullptr; // Retrieve var offset");
			writer.WriteLine("\t		_static_Item_Class_" + className + "->RegisterProperty(new RProperty(RType::GetType(\"" + prop.propertyType + "\"), VarOffset, sizeof(" + prop.propertyType + "), \"" + prop.propertyName + "\")); // Register property");
		}
		for (const RFunctionParser& func : functions)
		{
			std::string paramString = StringLibrary::ConcatString(func.params, { ", " });
			writer.WriteLine("\t		_static_Item_Class_" + className + "->RegisterFunction(RFunction<" + func.returnType + (paramString != "" ? ", " + paramString : "") + (func.bIsStatic ? "" : "," + className) + ">(\"" + func.functionName + "\", std::function<" + func.returnType + " (" + (func.bIsStatic ? paramString : className + "&" + (paramString == "" ? "" : ", " + paramString)) + ")>(&" + className + "::" + func.functionName + (paramString != "" ? "<" + paramString  + ">" : "") + "))); // Register function");
		}
		writer.WriteLine("\t}\n");
		writer.WriteLine("\tstruct _Refl_Static_Item_Builder_" + className + "{ // Item builder - Build reflection data");
		writer.WriteLine("\t	_Refl_Static_Item_Builder_" + className + "() { // Builder constructor");
		writer.WriteLine("\t		_Refl_Register_Item_" + className + "(); // Call to builder function");
		writer.WriteLine("\t	}");
		writer.WriteLine("\t};\n");
		writer.WriteLine("\tstatic _Refl_Static_Item_Builder_" + className + " _Refl_Static_Item_Builder_Var_" + className + "; //Build item when compiled");
	}
	else
	{
		writer.WriteLine("\tstruct _Refl_static_type_builder_" + StringLibrary::ReplaceCharWith(className, ':', '_') + " { // Type builder - Build reflection data");
		writer.WriteLine("\t\t_Refl_static_type_builder_" + StringLibrary::ReplaceCharWith(className, ':', '_') + "() { // Builder constructor");
		writer.WriteLine("\t\t\tREFL_REGISTER_TYPE(" + className + "); // Call to builder function");
		writer.WriteLine("\t\t}");
		writer.WriteLine("\t};");
		writer.WriteLine("\tstatic _Refl_static_type_builder_" + StringLibrary::ReplaceCharWith(className, ':', '_') + " _Refl_static_type_builder_var_" + StringLibrary::ReplaceCharWith(className, ':', '_') + "; //Build item when compiled");
	}
	if (classNamespace != "") writer.WriteLine("}");

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

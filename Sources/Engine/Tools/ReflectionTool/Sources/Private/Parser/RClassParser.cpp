
#include "Utils/FileLibrary.h"
#include "Utils/FileWriter.h"
#include <memory>
#include "Utils/StringLibrary.h"
#include "Parser/RClassParser.h"
#include "Parser/RInheritanceTable.h"

RClassParser::RClassParser(const LineReader& inData, uint32_t startLine, const std::string& path, const uint64_t& uniqueID, const std::string& inClassNamespace, const std::string& inClassName, const EClassType& inClassType, const RClassTemplateData& inTemplateData, bool bInIsFinal, const RClassOptions& inOptions)
	: data(inData), RBodyLine(startLine), filePath(path), fileUniqueID(uniqueID), classNamespace(inClassNamespace), className(inClassName), classType(inClassType), templateData(inTemplateData), bIsFinal(bInIsFinal), options(inOptions)
{
	if (classType == EClassType::ECT_CLASS || classType == EClassType::ECT_STRUCT)
	{
		std::string left, pars, right;
		if (StringLibrary::SplitLine(data.GetLine(0), { ':' }, left, pars))
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
	ExtractContent();
	if (classType == EClassType::ECT_TEMPLATE_TYPE) {
		templateDeclarationFilePaths[className] = filePath;
	}
}

void RClassParser::WriteHeader(OSWriter& writer)
{
	if (classType != EClassType::ECT_TEMPLATE_TYPE) {

		if (!IsValid()) return;
		writer.WriteBreaks(4);
		writer.WriteLine("/* ##############################  Reflection for " + className + "  ############################## */");
		writer.indentation++;
		writer.WriteBreaks(1);
		if (classNamespace != "")
		{
			writer.WriteLine("namespace " + classNamespace);
			writer.WriteLine("{");
			writer.indentation++;
		}

		/** Template declaration */
		std::string templateArgsString = "";
		for (int i = 0; i < templateData.templateParameters.size(); ++i) templateArgsString += "typename " + templateData.templateParameters[i] + (i == templateData.templateParameters.size() - 1 ? "" : ", ");
		if (templateData.bIsTemplateClass)  writer.WriteLine(std::string("template<") + templateArgsString + "> // template declaration");
		if (classType == EClassType::ECT_CLASS || classType == EClassType::ECT_STRUCT) writer.WriteLine(std::string(classType == EClassType::ECT_STRUCT ? "struct " : "class ") + className + "; // Forward declaration\n");

		/** template specialization */
		for (const auto& tempSpec : templateData.specializations) {
			writer.WriteLine(std::string("typedef ") + className + "<" + tempSpec.tempType + "> " + tempSpec.tempName + "; // template specialization");
		}


		if (classType == EClassType::ECT_CLASS || classType == EClassType::ECT_STRUCT)
		{
			writer.WriteBreaks(1);
			if (bIsFinal) {
				writer.WriteLine("#define _REFLECTION_BODY_RUID_" + std::to_string(fileUniqueID) + "_LINE_" + std::to_string(RBodyLine) + std::string(" ") + (classType == EClassType::ECT_CLASS ? "REFL_DECLARE_CLASS_NON_VIRTUAL(" + className + ")" : "REFL_DECLARE_STRUCT_NON_VIRTUAL(" + className + ")") + " // Declare REFLECT_BODY() macro");
			}
			else {
				writer.WriteLine("#define _REFLECTION_BODY_RUID_" + std::to_string(fileUniqueID) + "_LINE_" + std::to_string(RBodyLine) + std::string(" ") + (classType == EClassType::ECT_CLASS ? "REFL_DECLARE_CLASS(" + className + ")" : "REFL_DECLARE_STRUCT(" + className + ")") + " // Declare REFLECT_BODY() macro");
			}
			writer.WriteBreaks(1);
		}

		if (templateData.bIsTemplateClass) {
			for (const auto& tempCl : templateData.specializations) {
				writer.WriteLine("REFL_DECLARE_TYPENAME(" + tempCl.tempName + "); // Declare typename");
			}
		}
		else {
			writer.WriteLine("REFL_DECLARE_TYPENAME(" + className + "); // Declare typename");
		}
		if (templateData.bIsTemplateClass) {
			writer.WriteLine("");
			for (const auto& tempCl : templateData.specializations) {
				writer.WriteLine("template<> struct RIsReflected<" + tempCl.tempName + "> { static constexpr bool Reflect = true; }; // Used to detect if class is reflected or not");
			}
		}
		else {
			writer.WriteBreaks(1);
			writer.WriteLine("template<> struct RIsReflected<" + className + "> { static constexpr bool Reflect = true; }; // Used to detect if class is reflected or not");
		}

		if (classNamespace != "") {
			writer.indentation--;
			writer.WriteLine("}");
		}
		writer.indentation--;
	}
}

void RClassParser::WriteSource(OSWriter& writer)
{
	std::vector<std::string> reflectedClasses;
	if (templateData.bIsTemplateClass) {
		for (const auto& cl : templateData.specializations) {
			reflectedClasses.push_back(cl.tempName);
		}
	}
	else {
		reflectedClasses = { className };
	}

	writer.WriteBreaks(5);

	if (templateBodies.size() != 0) {


		for (const auto& body : templateBodies) {
			if (body.bCreate) {


				std::string customname = StringLibrary::ReplaceCharWith(body.baseType + '_' + body.newContent, ':', '_');
				std::string customClassType = body.baseType + '<' + body.newContent + ">";
				writer.WriteLine("/* ##############################  Template bodies " + customname + "  ############################## */");
				writer.WriteBreaks(1);

				if (classNamespace != "")
				{
					writer.WriteLine("namespace " + classNamespace);
					writer.WriteLine("{");
					writer.indentation++;
				}

				writer.indentation++;

				writer.WriteLine("struct _Refl_static_type_builder_" + customname + " { // Type builder - Build reflection data");
				writer.indentation++;
				writer.WriteLine("_Refl_static_type_builder_" + customname + "() { // Builder constructor");
				writer.indentation++;
				writer.WriteLine("REFL_REGISTER_TYPE(" + customClassType + "); // Call to builder function");
				writer.indentation--;
				writer.WriteLine("}");
				writer.indentation--;
				writer.WriteLine("} _Refl_static_type_builder_var_" + customname + "; //Build item when compiled");


				writer.WriteBreaks(2);
				writer.WriteLine("/** Create serializer for " + customClassType + " */");
				writer.WriteLine("struct " + customname + " : public Serialization::ISerializer {");
				writer.indentation++;
				writer.WriteLine("using Serialization::ISerializer::ISerializer; // Implement constructor");
				writer.WriteBreaks(1);
				writer.WriteLine("bool SerializeProperty(RProperty* inProperty, void* objectPointer, Serialization::SerializedProperty& outValue) {");
				writer.indentation++;
				writer.WriteLine("return Serialization::TSerializer<" + customClassType + ">::Serialize(inProperty->Value<" + customClassType + ">(objectPointer), outValue); // Call to serializer");
				writer.indentation--;
				writer.WriteLine("}");
				writer.WriteBreaks(1);
				writer.WriteLine("bool DeserializeProperty(RProperty* inProperty, void* objectPointer, const Serialization::SerializedProperty& inValue) {");
				writer.indentation++;
				writer.WriteLine("return Serialization::TSerializer<" + customClassType + ">::Deserialize(inValue, inProperty->Value<" + customClassType + ">(objectPointer)); // Call to deserializer");
				writer.indentation--;
				writer.WriteLine("}");
				writer.WriteBreaks(1);
				writer.indentation--;
				writer.WriteLine("} inline _" + customname + "_Value(\"" + customClassType + "\"); // structure registration");
				writer.indentation--;

				if (classNamespace != "") {
					writer.indentation--;
					writer.WriteLine("}");
				}

				writer.WriteBreaks(2);

				writer.indentation--;
			}
		}
	}


	writer.WriteLine("/* ##############################  Reflection for " + className + "  ############################## */\n");
	if (classNamespace != "")
	{
		writer.WriteLine("namespace " + classNamespace);
		writer.WriteLine("{");
	}
	for (const auto& cl : reflectedClasses) {
		if (classType == EClassType::ECT_CLASS || classType == EClassType::ECT_STRUCT)
		{
			if (classType == EClassType::ECT_STRUCT)
			{
				writer.WriteLine("\tstatic RStruct* _static_Item_Class_" + cl + " = nullptr; //Static struct reference");
				writer.WriteLine("\tRStruct* " + cl + "::GetStaticStruct() { return _static_Item_Class_" + cl + "; } //Static struct getter\n");
				writer.WriteLine("\tRStruct* " + cl + "::GetStruct() const { return _static_Item_Class_" + cl + "; } //struct getter\n");
			}
			if (classType == EClassType::ECT_CLASS)
			{
				writer.WriteLine("\tstatic RClass* _static_Item_Class_" + cl + " = nullptr; //Static class reference");
				writer.WriteLine("\tRClass* " + cl + "::GetStaticClass() { return _static_Item_Class_" + cl + "; } //Static class getter\n");
				writer.WriteLine("\tRClass* " + cl + "::GetClass() const { return _static_Item_Class_" + cl + "; } //class getter\n");
			}
			writer.WriteLine("\tvoid _Refl_Register_Item_" + cl + "() { // Register function");
			if (classType == EClassType::ECT_STRUCT) writer.WriteLine("\t		_static_Item_Class_" + cl + " = RStruct::RegisterStruct<" + cl + (ctor && ctor->args.size() > 0 ? "," + StringLibrary::ConcatString(ctor->args, ", ") : "") + ">(\"" + cl + "\"); //Register Struct");
			if (classType == EClassType::ECT_CLASS) writer.WriteLine("\t		_static_Item_Class_" + cl + " = RClass::RegisterClass<" + cl + (ctor && ctor->args.size() > 0 ? "," + StringLibrary::ConcatString(ctor->args, ", ") : "") + ">(\"" + cl + "\"); //Register Class");
			for (const std::string& parent : parents)
			{
				writer.WriteLine("\t		if (RIsReflected<" + parent + ">::Reflect) // Is parent reflected");
				writer.WriteLine("\t			_static_Item_Class_" + cl + "->AddParent(\"" + parent + "\"); // register parent");
			}

			if (properties.size() > 0) writer.WriteLine("\t		size_t VarOffset; // Var offset");
			for (const RPropertyParser& prop : properties)
			{
				writer.WriteLine("\t		VarOffset = (char*)&((" + cl + "*)nullptr->* & " + cl + "::" + prop.propertyName + ") - (char*)nullptr; // Retrieve var offset");
				writer.WriteLine("\t		_static_Item_Class_" + cl + "->RegisterProperty(new RProperty(\"" + prop.propertyType + "\", VarOffset, sizeof(" + prop.propertyType + "), \"" + prop.propertyName + "\")); // Register property");
			}
			for (const RFunctionParser& func : functions)
			{
				std::string paramString = StringLibrary::ConcatString(func.params, { ", " });
				writer.WriteLine("\t		_static_Item_Class_" + cl + "->RegisterFunction(RFunction<" + func.returnType + (paramString != "" ? ", " + paramString : "") + (func.bIsStatic ? "" : "," + cl) + ">(\"" + func.functionName + "\", std::function<" + func.returnType + " (" + (func.bIsStatic ? paramString : cl + "&" + (paramString == "" ? "" : ", " + paramString)) + ")>(&" + cl + "::" + func.functionName + (paramString != "" ? "<" + paramString + ">" : "") + "))); // Register function");
			}
			writer.WriteLine("\t}\n");
			writer.WriteLine("\tstruct _Refl_Static_Item_Builder_" + cl + "{ // Item builder - Build reflection data");
			writer.WriteLine("\t	_Refl_Static_Item_Builder_" + cl + "() { // Builder constructor");
			writer.WriteLine("\t		_Refl_Register_Item_" + cl + "(); // Call to builder function");
			writer.WriteLine("\t	}");
			writer.WriteLine("\t};\n");
			writer.WriteLine("\tstatic _Refl_Static_Item_Builder_" + cl + " _Refl_Static_Item_Builder_Var_" + cl + "; //Build item when compiled");
		}
		else
		{
			if (classType != EClassType::ECT_TEMPLATE_TYPE) {
				writer.WriteLine("\tstruct _Refl_static_type_builder_" + StringLibrary::ReplaceCharWith(cl, ':', '_') + " { // Type builder - Build reflection data");
				writer.WriteLine("\t\t_Refl_static_type_builder_" + StringLibrary::ReplaceCharWith(cl, ':', '_') + "() { // Builder constructor");
				writer.WriteLine("\t\t\tREFL_REGISTER_TYPE(" + cl + "); // Call to builder function");
				writer.WriteLine("\t\t}");
				writer.WriteLine("\t} _Refl_static_type_builder_var_" + StringLibrary::ReplaceCharWith(cl, ':', '_') + "; //Build item when compiled");
			}
		}
	}

	/** Serializer */
	if (classType != EClassType::ECT_TEMPLATE_TYPE) {
		writer.indentation++;
		if (options.bCreateSerializer) {
			std::string customname = StringLibrary::ReplaceCharWith(className, ':', '_') + "_Serializer";
			customname = StringLibrary::ReplaceCharWith(customname, '<', '_');
			customname = StringLibrary::ReplaceCharWith(customname, '>', '_');
			writer.WriteBreaks(2);
			writer.WriteLine("/** Create serializer for " + className + " */");
			writer.WriteLine("struct " + customname + " : public Serialization::ISerializer {");
			writer.indentation++;
			writer.WriteLine("using Serialization::ISerializer::ISerializer; // Implement constructor");
			writer.WriteBreaks(1);
			writer.WriteLine("bool SerializeProperty(RProperty* inProperty, void* objectPointer, Serialization::SerializedProperty& outValue) {");
			writer.indentation++;
			writer.WriteLine("return Serialization::TSerializer<" + className + ">::Serialize(inProperty->Value<" + className + ">(objectPointer), outValue); // Call to serializer");
			writer.indentation--;
			writer.WriteLine("}");
			writer.WriteBreaks(1);
			writer.WriteLine("bool DeserializeProperty(RProperty* inProperty, void* objectPointer, const Serialization::SerializedProperty& inValue) {");
			writer.indentation++;
			writer.WriteLine("return Serialization::TSerializer<" + className + ">::Deserialize(inValue, inProperty->Value<" + className + ">(objectPointer)); // Call to deserializer");
			writer.indentation--;
			writer.WriteLine("}");
			writer.WriteBreaks(1);
			writer.indentation--;
			writer.WriteLine("} inline _" + customname + "_Value(\"" + className + "\"); // structure registration");
		}
		writer.indentation--;
	}

	if (classNamespace != "") writer.WriteLine("}");
}

void RClassParser::ExtractContent()
{
	bool bFoundBody = !(classType == EClassType::ECT_CLASS || classType == EClassType::ECT_STRUCT);
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
			bool bIsTemplateMember;
			properties.push_back(RPropertyParser(data.GetLine(i + 1), bIsTemplateMember));
			if (bIsTemplateMember) {
				templateBodies.push_back(RTemplateBody(data.GetLine(i + 1)));
			}
		}
		if (StringLibrary::IsStartingWith(data.GetLine(i), "RFUNCTION("))
		{
			functions.push_back(RFunctionParser(data.GetLine(i + 1)));
		}
	}
}

RTemplateBody::RTemplateBody(const std::string& inLineData)
{
	std::string TypeName, center, right, final;
	StringLibrary::SplitLine(inLineData, { '<' }, TypeName, center);
	StringLibrary::SplitLine(center, { '>' }, final, right);
	bCreate = false;
	TypeName = StringLibrary::CleanupLine(TypeName);
	final = StringLibrary::CleanupLine(final);
	baseType = TypeName;
	newContent = final;
	auto& ite = existingTemplateBodies.find(TypeName);

	if (ite != existingTemplateBodies.end()) {
		for (const auto& type : existingTemplateBodies[TypeName]) {
			if (type == final) {
				bCreate = false;
				return;
			}
		}
		bCreate = true;
		existingTemplateBodies[TypeName].push_back(final);
	}
	else {
		bCreate = true;
		existingTemplateBodies[TypeName] = { final };
	}
}

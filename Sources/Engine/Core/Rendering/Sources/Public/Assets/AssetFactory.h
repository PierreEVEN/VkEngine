#pragma once


#define MAKE_FACTORY_METHOD_CHECK_DECL_VAL_0()
#define MAKE_FACTORY_METHOD_CHECK_DECL_VAL_1(val) std::declval<val>()
#define MAKE_FACTORY_METHOD_CHECK_DECL_VAL_2(v1, ...) std::declval<val>(), MAKE_FACTORY_METHOD_CHECK_DECL_VAL_1(__VA_ARGS__)
#define MAKE_FACTORY_METHOD_CHECK_DECL_VAL_3(val, ...) std::declval<val>(), MAKE_FACTORY_METHOD_CHECK_DECL_VAL_2(__VA_ARGS__)

#define MAKE_FACTORY_METHOD_CHECK(checKname, funcName, argc, ...) \
	template<typename T> \
	struct checKname \
	{ \
	private: \
		template<typename U> static auto MakeTest() -> decltype(U::funcName(MAKE_FACTORY_METHOD_CHECK_DECL_VAL_ ## argc(__VA_ARGS__)), std::true_type()); \
		template<typename> static std::false_type MakeTest(...); \
		public: \
		static constexpr bool value = std::is_same<decltype(MakeTest<TAssetFactory<T>>()), std::true_type>::value; \
	}


namespace Rendering {

	template<typename T>
	struct TAssetFactory {};

	namespace AssetFactory {
		MAKE_FACTORY_METHOD_CHECK(HasCustomFileExtension, GetCustomFileExtension, 0);
		MAKE_FACTORY_METHOD_CHECK(HasDesiredImportFormats, GetDesiredImportFormats, 0);
		MAKE_FACTORY_METHOD_CHECK(CanCreateFromDatas, CreateFromData, 0);
		MAKE_FACTORY_METHOD_CHECK(CanCreateFromAsset, CreateFromAsset, 1, String);
		MAKE_FACTORY_METHOD_CHECK(CanImportFromPath, ImportFromPath, 1, String);
		MAKE_FACTORY_METHOD_CHECK(CanBeTransient, MakeTransient, 1, T*);
	}





}
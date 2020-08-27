@ECHO off
SETLOCAL enabledelayedexpansion
COLOR B

REM CHECK ADMIN MODE
net session >NUL 2>&1
IF NOT %errorLevel% == 0 (
	REM REQUEST ADMIN MODE IF NEEDED
	if not "%1"=="am_admin" (powershell start -verb runas '%0' am_admin & exit /b)
)

REM SET GLOBALS
FOR /F "tokens=* USEBACKQ" %%F IN (`where cmake`) DO SET CMAKEPATH=%%F
CD /D %~dp0

SET ROOT=%cd%
SET LIBPATH=%cd%\Sources\ThirdParty\Libs
FOR /F "usebackq tokens=*" %%i IN (`Tools\vswhere -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) DO SET VSPATH=%%i

REM CHECK PATHS
IF NOT DEFINED VSPATH (
	COLOR 4
	ECHO "failed to locate msbuild tools."
	PAUSE
	EXIT 2
)

IF NOT DEFINED CMAKEPATH (
	COLOR 4
	echo "failed to locate cmake. Please set cmake path in environment variable."
	PAUSE
	EXIT 3
)

SET GLFWPATH=%cd%\Sources\ThirdParty\glfw\
SET SHADERCPATH=%cd%\Sources\ThirdParty\shaderc\
SET SPIRVCROSSPATH=%SHADERCPATH%\third_party\spirv_cross

REM BUILD SHADERC
echo Downloading shaderc dependencies...
python %SHADERCPATH%\utils\git-sync-deps

echo Building SHADERC
mkdir %SHADERCPATH%\build > NUL 2>&1
CD %SHADERCPATH%\build

echo building SHADERC x64...
cmake -DSHADERC_ENABLE_SHARED_CRT=true -DSHADERC_ENABLE_SPVC=ON %SHADERCPATH%
IF NOT %errorLevel% == 0 ECHO failed to generate vs files for SHADERC.

echo compiling SHADERC for Release x64...
"%VSPATH%" libshaderc/shaderc.vcxproj /t:build /p:Configuration="Release" /p:Platform="x64" /p:BuildInParallel=true /p:OutDir=%LIBPATH%\Shaderc\Release
IF NOT %errorLevel% == 0 ECHO failed to compile SHADERC.

echo compiling SPVC for Release x64...
"%VSPATH%" spvc/spvc_exe.vcxproj /t:build /p:Configuration="Release" /p:Platform="x64" /p:BuildInParallel=true /p:OutDir=%LIBPATH%\Shaderc\Release
IF NOT %errorLevel% == 0 ECHO failed to compile SPVC.

echo compiling SHADERC for Debug x64...
"%VSPATH%" libshaderc/shaderc.vcxproj /t:build /p:Configuration="Debug" /p:Platform="x64" /p:BuildInParallel=true /p:OutDir=%LIBPATH%\Shaderc\Debug
IF NOT %errorLevel% == 0 ECHO failed to compile SHADERC.

echo compiling SPVC for Debug x64...
"%VSPATH%" spvc/spvc_exe.vcxproj /t:build /p:Configuration="Debug" /p:Platform="x64" /p:BuildInParallel=true /p:OutDir=%LIBPATH%\Shaderc\Debug
IF NOT %errorLevel% == 0 ECHO failed to compile SPVC.

REM BUILD GLFW
echo Building GLFW
mkdir %GLFWPATH%\build > NUL 2>&1
CD %GLFWPATH%\build

echo building GLFW x64...
cmake -G "Visual Studio 16 2019" -A x64 %GLFWPATH%
IF NOT %errorLevel% == 0 ECHO failed to generate vs files for GLFW.

echo compiling GLFW for Release x64...
"%VSPATH%" src/glfw.vcxproj /t:build /p:Configuration="Release" /p:Platform="x64" /p:BuildInParallel=true /p:OutDir=%LIBPATH%\glfw\Release
IF NOT %errorLevel% == 0 ECHO failed to compile GLFW.

echo compiling GLFW for Debug x64...
"%VSPATH%" src/glfw.vcxproj /t:build /p:Configuration="Debug" /p:Platform="x64" /p:BuildInParallel=true /p:OutDir=%LIBPATH%\glfw\Debug
IF NOT %errorLevel% == 0 ECHO failed to compile GLFW.
pause

exit 0
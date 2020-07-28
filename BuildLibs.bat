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

SET GAINPUTPATH=%cd%\Sources\ThirdParty\gainput\
SET GLFWPATH=%cd%\Sources\ThirdParty\glfw\


REM BUILD GAINPUT
echo Building GAINPUT
mkdir %GAINPUTPATH%\build > NUL 2>&1
CD %GAINPUTPATH%\build

echo building GAINPUT x64...
cmake -G "Visual Studio 16 2019" -A x64 %GAINPUTPATH%
IF NOT %errorLevel% == 0 ECHO failed to generate vs files for GAINPUT.

echo compiling GAINPUT for Release x64...
"%VSPATH%" lib/gainputstatic.vcxproj /t:build /p:Configuration="Release" /p:Platform="x64" /p:BuildInParallel=true /p:OutDir=%LIBPATH%\GaInput\Release
IF NOT %errorLevel% == 0 ECHO failed to compile GAINPUT.

pause

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

pause

exit 0
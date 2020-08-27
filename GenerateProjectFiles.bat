@ECHO off
SETLOCAL enabledelayedexpansion

echo "Running premake for vs2019"
"Tools/premake5.exe" --file=Sources/Engine/Engine.lua vs2019

pause
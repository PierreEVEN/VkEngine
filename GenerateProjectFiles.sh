
if [[ "$OSTYPE" == msys ]]; then
	echo "Running premake for vs2019"
	"Tools/premake5.exe" --file=Sources/Engine/Engine.lua vs2019
else
	echo "Running premake for CMake"
	chmod u+x ./Tools/premake5.lin
	"Tools/premake5.lin" --file=Sources/Engine/Engine.lua gmake2
fi

read -rsn1 -p"Press any key to continue" variable;echo
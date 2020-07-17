
if [[ "$OSTYPE" == msys ]]; then
  echo "Running premake for vs2019"
"Tools/premake5.exe" --file=Sources/Engine/Engine.lua vs2019
else
  echo "Running premake for CMake"
"Tools/premake5.exe" --file=Sources/Engine/Engine.lua gmake2
fi

sleep 1
@echo off

REM Add MinGW path.
path=C:\MinGW\bin;%path%

if not exist Release mkdir Release
cd Release
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if not exist Debug mkdir Debug
cd ..\Debug
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug

# if not exist build-VS10 mkdir build-VS10
# cd ..\build-VS10
# cmake .. -G "Visual Studio 10"

cd ..

cd Release
mingw32-make -j6 install

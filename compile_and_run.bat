@echo off
echo Compiling 

:: Create build and bin directories if they don't exist
if not exist build mkdir build
if not exist bin mkdir bin

:: Compile the source files
g++ -std=c++17 -Wall -Wextra -Icode/include src/main.cpp src/noise.cpp src/voxel_renderer.cpp -o bin/app3D.exe -Lcode/lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32

:: Check if compilation succeeded
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b 1
)

echo Compilation successful!
echo Running application...

:: Copy necessary DLLs to the bin directory if they don't exist
if not exist bin\sfml-graphics-2.dll copy code\lib\sfml-graphics-2.dll bin\
if not exist bin\sfml-window-2.dll copy code\lib\sfml-window-2.dll bin\
if not exist bin\sfml-system-2.dll copy code\lib\sfml-system-2.dll bin\

:: Run the application
cd bin
app3D.exe

:: Return to the original directory
cd ..

echo Done!
pause 
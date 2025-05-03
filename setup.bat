@echo off
REM Setup batch file for SFML project

echo Checking for SFML in code directory...

if not exist code\include\SFML (
  echo SFML include directory not found!
  echo Please copy SFML header files to code\include\SFML\
  pause
  exit /b 1
)

if not exist code\lib (
  echo SFML lib directory not found!
  echo Please copy SFML library files to code\lib\
  pause
  exit /b 1
)

echo Building project...
make

if %ERRORLEVEL% NEQ 0 (
  echo Build failed!
  pause
  exit /b 1
)

echo.
echo Build successful!
echo To run the application, type: bin\app.exe
echo or type: make run
echo.

REM Ask if user wants to run the app
set /p run_app=Run the application now? (y/n): 

if /i "%run_app%"=="y" (
  echo Running application...
  bin\app.exe
)

echo.
echo Done!
pause 
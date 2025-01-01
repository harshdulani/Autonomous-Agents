@echo off

REM Move to a build directory so that cleaning up is simple
if exist build (
    if exist build\NUL (
        echo Build is a directory; deleting it
		echo.
		echo.
        rmdir /s /q build
    ) else (
        echo Build exists but is not a directory ^,likely a file ^- deleting it
		echo.
		echo.
        del /q build
    )
)
mkdir build
cd build

REM Run CMake and generate configs
echo.
echo.
echo Start: Configure the project and generates build files:
echo.
echo.
cmake ..
echo.
echo.
echo Start: Build the project using the generated build files:
echo.
echo.
cmake --build .

REM Pause the script to see any output (optional)
echo.
echo.
echo If there are no errors, Executable will be generated at build\src\CalcMod\Driver\Debug\GameTarget.exe
echo.
echo.
pause
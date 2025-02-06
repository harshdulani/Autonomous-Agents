@echo off

where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo Error: CMake is not installed or not in PATH.
    pause
    exit /b 1
)

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
echo Start: Configure the project and generate build files:
echo.
echo.
cmake ..
echo.
echo.
echo Start: Build the project using the generated build files:
echo.
echo.
cmake --build . --config Release
::start "" .\Source\Core\Driver\Release\Asteroids.exe

REM Pause the script to see any output (optional)
echo.
echo.
echo Executable should be generated at "\build\Source\Core\Driver\Release\Asteroids.exe"
echo.
echo.
pause
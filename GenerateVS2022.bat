@echo off
REM Change directory to the location of the batch file
cd /d %~dp0

REM Run premake5 with the specified script and target
Vendor\premake\premake5.exe --file=Scripts/build.lua vs2022

REM Pause the script to see any output (optional)
pause
@echo off
g++ -std=c++20 mainfile.cpp -o main2.exe
if %errorlevel% neq 0 (
    echo Compilation failed.
    pause
    exit /b %errorlevel%
)
echo Compilation successful. Running program...
main2.exe
pause
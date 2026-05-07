@echo off
echo Building Bazarly Backend...
g++ -std=c++14 -Iinlcude src/main.cpp src/core/*.cpp -o BazarlyBackend.exe -lws2_32 -lmswsock -ladvapi32
if %ERRORLEVEL% EQU 0 (
    echo Build successful!
) else (
    echo Build failed!
)

@echo off
echo Building Bazarly Backend...
g++ -std=c++14 src/main.cpp src/core/Product.cpp src/core/User.cpp -o BazarlyBackend.exe -lws2_32 -lmswsock -ladvapi32
if %ERRORLEVEL% EQU 0 (
    echo Build successful!
) else (
    echo Build failed!
)

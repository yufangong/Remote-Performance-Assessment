:run.bat
@echo off
cd ./Debug
start "Server" Server.exe
start "Client GUI" WPF-Interop.exe
start "Client Tester" Tester.exe 5
cd ../
pause
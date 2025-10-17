@echo off
mkdir Build
cd Build
cmake ../ -G "Visual Studio 17 2022" -A x64
echo Launch VS project? (y/n)
set /p answer=
if %answer% == y goto launch
pause
exit
:launch
PlotWriter.sln
pause
exit
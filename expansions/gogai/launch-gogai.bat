@echo off
cd /d "%~dp0"
if not exist th095.dat goto missing
if not exist thbgm.dat goto missing
start "" th095-gogai.exe
exit /b 0
:missing
echo Copy your original Japanese th095.dat and thbgm.dat into this folder first.
pause
exit /b 1

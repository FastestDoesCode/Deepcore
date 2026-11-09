@echo off
cd /d "%~dp0"
where g++ >nul 2>nul
if %errorlevel%==0 (
  echo Building with g++...
  g++ -std=c++17 -O2 -static -o deepcore.exe src\*.cpp
  goto check
)
where cl >nul 2>nul
if %errorlevel%==0 (
  echo Building with Visual Studio...
  cl /nologo /std:c++17 /O2 /EHsc /utf-8 /Fe:deepcore.exe src\*.cpp
  del *.obj 2>nul
  goto check
)
echo.
echo No C++ compiler found.
echo Install one by running this in PowerShell, then reopen it and run build.bat again:
echo   winget install -e --id BrechtSanders.WinLibs.POSIX.UCRT
echo.
pause
exit /b 1
:check
if exist deepcore.exe (
  echo.
  echo Built deepcore.exe
) else (
  echo.
  echo Build failed. Copy the errors above and send them over.
)
pause

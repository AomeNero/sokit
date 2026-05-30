@echo off
call "D:\Program Files\Microsoft Visual Studio\18\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
set QTDIR=D:\Qt5.14.2\5.14.2\msvc2017_64
set PATH=D:\Qt5.14.2\5.14.2\msvc2017_64\bin;%PATH%
cd /d "D:\git\sokit-master\build\general\qmake"

echo.
echo === Cleaning previous build ===
if exist Makefile del /q Makefile Makefile.Debug Makefile.Release 2>nul
if exist ..\..\..\tmp\release rmdir /s /q ..\..\..\tmp\release 2>nul
if exist ..\..\..\tmp\debug rmdir /s /q ..\..\..\tmp\debug 2>nul

echo.
echo === Running qmake ===
"D:\Qt5.14.2\5.14.2\msvc2017_64\bin\qmake.exe" sokit.pro
if %ERRORLEVEL% NEQ 0 (
    echo qmake failed with error code %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)

echo.
echo === Running nmake (Release) ===
nmake /f Makefile.Release 2>&1

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo === Build failed with error code %ERRORLEVEL% ===
    exit /b %ERRORLEVEL%
)
echo.
echo === Build succeeded ===
dir ..\..\..\bin\release\*.exe 2>nul

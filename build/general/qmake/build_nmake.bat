@echo off
REM ----------------------------------------------------
REM sokit build script for nmake (MSVC + Qt5)
REM ----------------------------------------------------
REM Usage:
REM   build.bat          - Build release version
REM   build.bat debug    - Build debug version
REM   build.bat clean    - Clean all generated files
REM   build.bat all      - Build both debug and release
REM ----------------------------------------------------

setlocal

REM Qt installation path - modify this to match your Qt installation
set QTDIR=D:\Qt\Static\5.15.2_v142

REM Setup MSVC environment (Visual Studio 2019/2022)
if exist "D:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    call "D:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    call "D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "D:\Program Files\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    call "D:\Program Files\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "D:\Program Files\Microsoft Visual Studio\18\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    call "D:\Program Files\Microsoft Visual Studio\18\Professional\VC\Auxiliary\Build\vcvars64.bat"
) else (
    echo ERROR: Visual Studio not found. Please edit build.bat to set up MSVC environment.
    exit /b 1
)

REM Add Qt to PATH
set PATH=%QTDIR%\bin;%PATH%

REM Run nmake
if "%1"=="" (
    echo Building release version...
    nmake /f Makefile.nmake release
) else (
    nmake /f Makefile.nmake %*
)

endlocal

@echo off
setlocal
chcp 65001 >nul 2>&1
rem Procedural Warfare - Tactical Edition. C23 + ARM64 NEON + Vulkan, arm64-v8a.
rem Phases live in scripts\ ; each stays under the 300 word ceiling.

cd /d "%~dp0"
set "TDM_DEBUG=0"
set "DEPLOY=1"
set "CLEAN=0"

for %%A in (%*) do (
  if /i "%%A"=="--debug"     set "TDM_DEBUG=1"
  if /i "%%A"=="--no-deploy" set "DEPLOY=0"
  if /i "%%A"=="--clean"     set "CLEAN=1"
  if /i "%%A"=="--help"      goto :usage
  if /i "%%A"=="-h"          goto :usage
)

if "%CLEAN%"=="1" (
  echo [clean] wiping build\
  if exist "build" rmdir /s /q "build"
)

echo [1/6] word limit
call "scripts\words.bat" || goto :fail
echo [2/6] shaders
call "scripts\shaders.bat" || goto :fail
echo [3/6] compile
call "scripts\compile.bat" || goto :fail
echo [4/6] link
call "scripts\link.bat" || goto :fail
echo [5/6] package
call "scripts\package.bat" || goto :fail
if "%DEPLOY%"=="1" (
  echo [6/6] deploy
  call "scripts\deploy.bat" || goto :fail
) else (
  echo [6/6] deploy skipped
)
echo BUILD OK -^> RAMAI.apk
exit /b 0

:usage
echo build.bat [--debug] [--clean] [--no-deploy] [--help]
echo   --debug      -O0 -g, symbols kept
echo   --clean      wipe build\ first
echo   --no-deploy  skip adb install and launch
exit /b 0

:fail
echo BUILD FAILED
exit /b 1

@echo off
setlocal enabledelayedexpansion
rem Shared library. -u keeps ANativeActivity_onCreate despite gc-sections.
call "%~dp0env.bat" || exit /b 1

set "TGT=--target=aarch64-linux-android24"
set "SO=%LIB%\libramai.so"
if "%TDM_DEBUG%"=="1" (set "OPT=-O0 -g") else (set "OPT=-O3")

"%CC%" %TGT% -shared %OPT% -fPIC -o "%SO%" "%OBJ%\*.o" ^
  -Wl,-soname,libramai.so -Wl,-u,ANativeActivity_onCreate ^
  -Wl,--gc-sections -Wl,--exclude-libs,ALL ^
  -Wl,-z,relro -Wl,-z,now -Wl,-z,max-page-size=16384 ^
  -landroid -llog -lvulkan -lm || exit /b 1

echo [link] %SO%
exit /b 0

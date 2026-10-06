@echo off
setlocal enabledelayedexpansion
rem Every translation unit with -Werror, plus the ARM64 kernels and the glue.
call "%~dp0env.bat" || exit /b 1

set "TGT=--target=aarch64-linux-android24"
set "WARN=-Wall -Wextra -Werror"
set "DEFS=-DTDM_ENABLE_ASM=1 -DVK_USE_PLATFORM_ANDROID_KHR"
set "INC=-I%ROOT%\core -I%ROOT%\game -I%ROOT%\render -I%ROOT%\android -I%GEN% -I%GLUE%"

if "%TDM_DEBUG%"=="1" (set "OPT=-O0 -g") else (set "OPT=-O3")

set "FAIL=0"

for %%F in ("%ROOT%\core\*.c" "%ROOT%\game\*.c" "%ROOT%\render\*.c" "%ROOT%\android\*.c") do (
  "%CC%" %TGT% -std=c23 %OPT% %WARN% %DEFS% %INC% -fPIC -fvisibility=hidden ^
    -c "%%~fF" -o "%OBJ%\%%~nF.o" || set /a FAIL+=1
)
if not "!FAIL!"=="0" echo [compile] !FAIL! C unit(s) failed & exit /b 1

for %%F in ("%ROOT%\asm\arm64\*.S") do (
  "%CC%" %TGT% %OPT% %WARN% -fPIC -c "%%~fF" -o "%OBJ%\%%~nF.o" || set /a FAIL+=1
)
if not "!FAIL!"=="0" echo [compile] !FAIL! asm unit(s) failed & exit /b 1

"%CC%" %TGT% -std=c23 %OPT% -I"%GLUE%" -c "%GLUE%\android_native_app_glue.c" ^
  -o "%OBJ%\native_app_glue.o" || exit /b 1

echo [compile] objects in build\obj
exit /b 0

@echo off
setlocal
rem Installs and launches on the first attached device, if any.
call "%~dp0env.bat" || exit /b 1

set "DEV="
for /f "skip=1 tokens=1,2" %%A in ('"%ADB%" devices') do (
  if "%%B"=="device" if not defined DEV set "DEV=%%A"
)
if not defined DEV echo [deploy] no device attached & exit /b 0

"%ADB%" -s %DEV% install -r "%ROOT%\RAMAI.apk" || exit /b 1
"%ADB%" -s %DEV% shell am start -n com.ramai.engine/android.app.NativeActivity ^
  >nul || exit /b 1
echo [deploy] launched on %DEV%
exit /b 0

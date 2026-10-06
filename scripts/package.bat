@echo off
setlocal enabledelayedexpansion
rem aapt2 link, add the .so, align, sign, and drop RAMAI.apk at the root.
call "%~dp0env.bat" || exit /b 1

set "UNSIGNED=%APKDIR%\app.apk"
set "ALIGNED=%APKDIR%\aligned.apk"
set "FINAL=%ROOT%\RAMAI.apk"

if not exist "%LIB%\libramai.so" echo [package] link first & exit /b 1

mkdir "%APKDIR%\lib\arm64-v8a" 2>nul
copy /y "%LIB%\libramai.so" "%APKDIR%\lib\arm64-v8a\libramai.so" >nul || exit /b 1

"%AAPT2%" link -o "%UNSIGNED%" --manifest "%ROOT%\android\AndroidManifest.xml" ^
  -I "%PLATFORM%" --min-sdk-version 24 --target-sdk-version 34 ^
  --version-code 1 --version-name 1.0 || exit /b 1

pushd "%APKDIR%"
"%JAR%" uf "app.apk" "lib\arm64-v8a\libramai.so" || (popd & exit /b 1)
popd

"%ZIPALIGN%" -f -p 4 "%UNSIGNED%" "%ALIGNED%" || exit /b 1

call "%APKSIGNER%" sign --ks "%KEYSTORE%" --ks-pass pass:android ^
  --key-pass pass:android --ks-key-alias androiddebugkey ^
  --out "%FINAL%" "%ALIGNED%" || exit /b 1

echo [package] %FINAL%
exit /b 0

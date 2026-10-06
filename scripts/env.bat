@echo off
rem Locates the toolchain. Called by build.bat. Deliberately no setlocal:
rem the variables it sets must stay visible to the later phases.

set "ROOT=%~dp0.."
pushd "%ROOT%" >nul
set "ROOT=%CD%"
popd >nul

set "SDK=%LOCALAPPDATA%\Android\Sdk"
if defined ANDROID_SDK_ROOT set "SDK=%ANDROID_SDK_ROOT%"
if defined ANDROID_HOME set "SDK=%ANDROID_HOME%"

set "NDK=%SDK%\ndk\30.0.16248370"
if not exist "%NDK%" for /d %%D in ("%SDK%\ndk\*") do set "NDK=%%~fD"
if not exist "%NDK%\toolchains" echo [env] no NDK under %SDK%\ndk & exit /b 1

set "PRE=%NDK%\toolchains\llvm\prebuilt\windows-x86_64"
set "CC=%PRE%\bin\clang.exe"
set "GLUE=%NDK%\sources\android\native_app_glue"

set "BT=%SDK%\build-tools\34.0.0"
if not exist "%BT%\aapt2.exe" for /d %%D in ("%SDK%\build-tools\*") do set "BT=%%~fD"
if not exist "%BT%\aapt2.exe" echo [env] no aapt2 in %BT% & exit /b 1
set "AAPT2=%BT%\aapt2.exe"
set "ZIPALIGN=%BT%\zipalign.exe"
set "APKSIGNER=%BT%\apksigner.bat"

set "PLATFORM=%SDK%\platforms\android-34\android.jar"
if not exist "%PLATFORM%" for /d %%D in ("%SDK%\platforms\android-*") do set "PLATFORM=%%~fD\android.jar"

set "JDK=%ProgramFiles%\Eclipse Adoptium\jdk-21.0.10.7-hotspot"
if not exist "%JDK%\bin\jar.exe" for /d %%D in ("%ProgramFiles%\Eclipse Adoptium\jdk-*") do set "JDK=%%~fD"
set "JAR=%JDK%\bin\jar.exe"

set "GLSLC=C:\VulkanSDK\1.4.328.1\Bin\glslc.exe"
if defined VULKAN_SDK if exist "%VULKAN_SDK%\Bin\glslc.exe" set "GLSLC=%VULKAN_SDK%\Bin\glslc.exe"

set "PY=python"
where /q python || set "PY=%LOCALAPPDATA%\Programs\Python\Python311\python.exe"

set "ADB=%SDK%\platform-tools\adb.exe"
set "KEYSTORE=%USERPROFILE%\.android\debug.keystore"
set "OBJ=%ROOT%\build\obj"
set "GEN=%ROOT%\build\gen"
set "SPV=%ROOT%\build\shaders"
set "LIB=%ROOT%\build\lib\arm64-v8a"
set "APKDIR=%ROOT%\build\apk"
mkdir "%OBJ%" "%GEN%" "%SPV%" "%LIB%" "%APKDIR%" 2>nul
exit /b 0

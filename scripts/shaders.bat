@echo off
setlocal enabledelayedexpansion
rem GLSL to SPIR-V with glslc, then SPIR-V to C arrays under build/gen.
call "%~dp0env.bat" || exit /b 1

for %%S in ("%ROOT%\shaders\*.vert" "%ROOT%\shaders\*.frag") do (
  "%GLSLC%" --target-env=vulkan1.1 -O -o "%SPV%\%%~nxS.spv" "%%~fS" || exit /b 1
  echo [shaders] %%~nxS
)

"%PY%" "%ROOT%\tools\spirv_to_c_header.py" "%SPV%\tdm_scene.vert.spv" "%GEN%\tdm_scene_vert.h" tdm_scene_vert_spv || exit /b 1
"%PY%" "%ROOT%\tools\spirv_to_c_header.py" "%SPV%\tdm_scene.frag.spv" "%GEN%\tdm_scene_frag.h" tdm_scene_frag_spv || exit /b 1
"%PY%" "%ROOT%\tools\spirv_to_c_header.py" "%SPV%\tdm_hud.vert.spv"   "%GEN%\tdm_hud_vert.h"   tdm_hud_vert_spv   || exit /b 1
"%PY%" "%ROOT%\tools\spirv_to_c_header.py" "%SPV%\tdm_hud.frag.spv"   "%GEN%\tdm_hud_frag.h"   tdm_hud_frag_spv   || exit /b 1

echo [shaders] headers in build\gen
exit /b 0

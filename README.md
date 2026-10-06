# Procedural Warfare — Team Deathmatch (Android)

3D team deathmatch shooter built in **C23 + ARM64 assembly + Vulkan**,
Android only. Port of the reference HTML prototype: 6 bots per team, three
procedural maps, five weapons, grenades, vehicles, turrets, killstreaks.

## Build
```
build.bat                # compile, package and sign RAMAI.apk
build.bat --no-deploy    # stop before adb
build.bat --debug        # unoptimized with symbols
```
Requires: Android NDK, SDK build-tools, `glslc` (Vulkan SDK or NDK
shader-tools), a JDK, and `python` for tools. See [rules/build.md](rules/build.md).

## Controls (touch)
- Left half: drag — virtual move stick.
- Right half: drag — look around, auto-fires while held.
- Buttons: `FIRE`, `JUMP`, `RELOAD`, `WPN`, `GREN`, `USE`.
- `USE` enters/exits vehicles and turrets when the prompt shows.

## Rules of the road
Read [project.md](project.md), [system.md](system.md),
[negative.md](negative.md) and [rules/](rules/README.md) first.
**Every file must stay under 300 words** — check with
`python tools/check_word_limits.py`.

## Layout
```
core/ game/ render/ android/   C23 sources
asm/arm64/                     NEON kernels
shaders/                       GLSL -> SPIR-V (build/gen)
scripts/                       build stages, each < 300 words
tools/                         word checker, SPIR-V header generator
```

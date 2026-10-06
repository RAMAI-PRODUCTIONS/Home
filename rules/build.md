# Build

## Commands
```
build.bat                  # compile, package, sign, install, launch
build.bat --debug          # -O0 -g, symbols kept
build.bat --clean          # wipe build\ first
build.bat --no-deploy      # stop before adb
build.bat --help
```
Only `arm64-v8a` is built; there is no other ABI target.

## Phases
`build.bat` runs six phases; each lives in `scripts/` and stays under 300
words. `env.bat` is not a phase — it is the shared toolchain probe every
phase calls. Phase order:

1. `words.bat` — word-limit gate; aborts if any file is >= 300 words.
2. `shaders.bat` — `glslc` GLSL to SPIR-V, then C headers in `build/gen/`.
3. `compile.bat` — clang C23 objects + ARM64 `.S` objects into `build/obj`.
4. `link.bat` — shared `libramai.so` with `-landroid -llog -lvulkan -lm`.
5. `package.bat` — aapt2, jar, zipalign, apksigner -> `build/apk`.
6. `deploy.bat` — `adb install -r` and `am start` when a device is present.

## Toolchain facts
- Target: `aarch64-linux-android24`, `minSdk 24`, `targetSdk 34`.
- Shaders: `--target-env=vulkan1.1 -O`.
- Compile flags: `-std=c23 -O3 -Wall -Wextra -Werror -fPIC -fvisibility=hidden`.
- Linker: `lld`, `--gc-sections`, `-z relro -z now`, 16 KiB page alignment.

## Outputs
Everything generated lives under `build/`. `build.bat` also copies the
finished package to `RAMAI.apk` at the repo root.

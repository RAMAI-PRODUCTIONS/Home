# Files

## Word limit
Count = whitespace-separated tokens of the file. Limit = **299 max**.
Run `python tools/check_word_limits.py` before finishing. The build script
calls it and aborts when any authored file is too large.

## What counts
- Counts: `.c`, `.h`, `.S`, `.md`, `.bat`, `.py`, `.vert`, `.frag`, `.xml`,
  `.json`, `.txt`.
- Exempt: everything under `build/` (generated), binaries, `.git/`.

## Splitting strategy
A file approaching 300 words is a design smell. Split by responsibility:
- `foo.c` too big -> `foo_<part>.c` in the same directory, named for the
  stage it holds (`tdm_vk_swap_alloc.c`, `tdm_player_mount.c`).
- Data tables -> own `*_data.c` file.
- Anything two parts share stops being `static`: promote it and declare it
  once, in the module's own header.
- Declarations -> `foo.h`; never declare in two headers.

## Layout
```
core/     math, config, terrain, collision, pathfind, font, simd
game/     weapons, entity, player, bot, map, match, hud, input
render/   tdm_vk_* Vulkan wrappers, scene emission
android/  android_main.c, AndroidManifest.xml
asm/arm64 NEON .S sources
shaders/  GLSL sources only
tools/    python helpers
scripts/  batch build stages, each under 300 words
docs live at the repo root: project.md, system.md, negative.md, rules/
```

## Naming
Files mirror their single public symbol: `tdm_player.c` defines
`tdm_player_*`. One underscore-separated stem, no camelCase filenames.

# System — architecture

## Layers (bottom to top)

```
android/     NativeActivity glue, input events, lifecycle, main loop
render/      Vulkan instance/device/swapchain/pipelines/buffers/frame
game/        match rules, entities, AI, weapons, maps, HUD composition
core/        math, RNG, terrain noise, collision, A*, font, SIMD dispatch
asm/arm64/   NEON kernels (compiled only for arm64-v8a)
shaders/     GLSL sources -> SPIR-V headers generated at build time
```

## Frame flow

1. Glue pumps Android events into `tdm_input_*` (touch state machine).
2. `tdm_game_tick()` advances match time, player, bots, grenades, pickups.
3. `tdm_scene_build_static()` fills the static buffer once per map.
4. `tdm_scene_render()` emits world/entity/effect vertices per frame.
5. `tdm_hud_build()` appends UI quads and text to the same dynamic buffer.
6. `tdm_vk_frame()` records both render passes and presents.

## Memory rules
- Fixed static arrays sized in `tdm_config.h`. No per-frame allocation.
- World geometry is built once at map load and stays in a vertex buffer.
- Per-frame geometry is rewritten into a host-visible mapped ring buffer.

## Conventions
- `tdm_` prefix for all non-Vulkan symbols. `vk` names follow Vulkan style.
- One struct per module, passed explicitly as the first argument.
- Structs are plain C; no hidden globals except the two singletons:
  the game (behind `g_tdm_game()`) and `s_ren`, both in `android/tdm_android_main.c`.

## Scaling out
Add a feature by: config constants -> game module -> emit geometry in a
`tdm_scene_*.c` -> draw text in a `tdm_hud_*.c`. Never grow a file past 300
words; split it and promote what the halves share.

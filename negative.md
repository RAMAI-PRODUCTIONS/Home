# Negative — do NOT do these

## Platform
- No desktop, Windows, Linux, macOS, web/WASM, iOS, or console code.
- No Java, Kotlin, Swift, Objective-C, or Dart. NativeActivity only.
- No third-party engines or frameworks (no SDL, GLFW, Oboe, FMOD, glm).
- No NDK modules beyond `android`, `log`, `vulkan`, `math`, `aaudio` (unused).

## Language
- No C++ (`extern "C"` headers must not be compiled as C++).
- No compiler extensions except `__attribute__((packed))` where required.
- No dynamic allocation in the frame loop: no `malloc`/`free` per frame.
- No recursion deeper than A*'s bounded search.
- No `assert` in release paths; return errors and log with `__android_log_print`.

## Files
- **No file may reach 300 words.** Split instead of growing.
- No generated file committed into `src/`; outputs go to `build/`.
- No blank filler, no commented-out code, no TODO stubs left behind.

## Rendering
- No pipeline caches of unbounded size, no per-frame `vkCreate*`.
- No swapchain image read-back, no `vkQueueWaitIdle` in the hot path.
- No texture streaming; the only texture is the runtime font atlas.
- Do not assume a present mode other than `FIFO` (always available).

## Gameplay
- Do not invent mechanics absent from the reference prototype.
- Do not raise score limit, timers, or bot counts without a config change.
- Team damage stays off; friendly fire is not part of the reference.

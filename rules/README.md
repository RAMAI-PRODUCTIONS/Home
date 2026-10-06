# Rules index

Read these before editing. They are binding, not advisory.

| File | Governs |
|------|---------|
| [files.md](files.md) | 300-word limit, naming, layout, splitting |
| [style.md](style.md) | C23 style, types, headers, error handling |
| [build.md](build.md) | build commands, targets, outputs |
| [android.md](android.md) | NativeActivity, lifecycle, input, manifest |
| [vulkan.md](vulkan.md) | device, swapchain, passes, buffers, sync |
| [asm.md](asm.md) | NEON kernels, dispatch, fallbacks |

Root-level companions: [project.md](../project.md) (what we build),
[system.md](../system.md) (how it is shaped), [negative.md](../negative.md)
(hard prohibitions).

## Order of precedence
1. `negative.md` — never violate.
2. This `rules/` directory.
3. `project.md` / `system.md`.
4. The reference HTML prototype (behaviour source of truth).

## Review checklist
- File under 300 words? `python tools/check_word_limits.py`
- Builds? `build.bat --no-deploy`
- New symbol prefixed `tdm_` or `TDM_`?
- No new dependency, no platform creep, no C++.

# Android

## Entry
`android_main(struct android_app *)` in `android/tdm_android_main.c`, provided
by the NDK `android_native_app_glue.c` (compiled from the NDK tree, linked with
`-Wl,-u,ANativeActivity_onCreate`).

## Lifecycle
Handle these app commands:
- `APP_CMD_INIT_WINDOW` — create surface, swapchain, pipelines.
- `APP_CMD_TERM_WINDOW` — drop to a null surface and stop drawing.
- `APP_CMD_WINDOW_RESIZED`, `APP_CMD_WINDOW_REDRAW_NEEDED` and
  `APP_CMD_CONFIG_CHANGED` — re-create or resize the swapchain.
- `APP_CMD_GAINED_FOCUS` — re-seed the frame clock so `dt` cannot jump.
Never render while `app->window == NULL`.

## Input
- Single source of truth: the `InputState` built from `AInputEvent` motion
  events, owned by `game/tdm_input.h`.
- Pointer ids are tracked in a fixed table: left half = move stick,
  right half = look stick (auto-fire), otherwise button hit-test.
- Buttons are laid out once by `tdm_hud_layout_touch()` / `tdm_hud_layout_menu()`
  so hit-tests and drawing cannot drift apart.
- Key events only handle BACK to leave the match.

## Manifest
- `android.app.NativeActivity`, `android:hasCode="false"`.
- meta-data `android.app.lib_name` = `ramai`.
- `screenOrientation="landscape"`, fullscreen, `minSdkVersion 24`.
- Declares `android.hardware.vulkan.version` as required.

## Rules
- No Java. No activity subclasses. No permissions requested.
- Touch is the only input; keyboard/mouse are not assumed.
- Respect `onPause`: stop the loop, do not keep presenting.

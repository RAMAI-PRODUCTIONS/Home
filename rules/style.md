# Style

## Language
C23 only. Compile with `-std=c23 -Wall -Wextra`; warnings are fixed, never
suppressed with pragmas.

## Naming
- Functions and types: `tdm_` prefix, snake_case (`tdm_player_shoot`).
- Struct tags: no suffix (`typedef struct Entity { ... } Entity;`).
- Macros/enum constants: `TDM_` prefix, SCREAMING_SNAKE.
- Locals: short, no Hungarian. `i`, `n`, `dt`, `pos`, `hp`.

## Types
- `<stdint.h>`: `int32_t`, `uint32_t`; `float` for all simulation values.
- `bool` from `<stdbool.h>` for flags. No `int` booleans.
- Arrays use `TDM_MAX_*` constants from `tdm_config.h`, never literals.

## Headers
- Every header starts with `#pragma once`.
- Include own header first, then project headers, then system headers.
- Headers declare; `.c` files define. No functions defined in headers
  except `static inline` one-liners in `tdm_math.h`.

## Functions
- Prefer many small functions; the 300-word rule enforces this.
- Early `return` on error. No deep nesting.
- Out-parameters for more than one return value.

## Errors
- Vulkan calls go through `VK_CHECK(expr)` from `tdm_render.h`.
- Log with `__android_log_print(ANDROID_LOG_ERROR, "TDM", ...)` and bail
  out of the frame; never continue with a broken device/swapchain.

## Comments
Explain *why*, not *what*. One line above a tricky block. No banner
comments, no file-level copyright blocks inside source.

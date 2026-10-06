# Assembly

## Scope
Only ARM64 (`aarch64`) NEON. Assembly is allowed for three kernels:

| File | Symbol | Used by |
|------|--------|---------|
| `asm/arm64/mat4_mul_neon.S` | `tdm_mat4_mul_neon` | camera/matrix math |
| `asm/arm64/vec3_transform_neon.S` | `tdm_vec3_transform_batch_neon` | box/mesh emission |
| `asm/arm64/particle_step_neon.S` | `tdm_particle_step_neon` | tracer/spark particles |

## Rules
- Each `.S` file stays under 300 words: keep loops tight, no banner comments.
- Matrix rows come from `trn1/trn2` stage 2 in `.2d` form, never `.4s`.
- Batch transforms take strides >= 4 floats so `ldr q`/`st1 q` are legal.
- AAPCS64 calling convention: args in `x0..x7`, results in `x0`/`v0`.
- Float args in `v0..v7`, return in `v0`. Preserve `x19..x28` if used.
- Symbols are global with C linkage; no name mangling, no local aliases.
- File-scope: `.text` for code, `.section .note.GNU-stack,"",@progbits`.

## Dispatch
`core/tdm_simd.c` exposes function pointers. `tdm_simd_init()` selects the
NEON implementation when `TDM_ENABLE_ASM=1` **and** `__aarch64__`, otherwise
the scalar C version from `core/tdm_simd_scalar.c`.

## Correctness
- Scalar and NEON paths must agree; the scalar path is the reference.
- Shipped builds are `arm64-v8a` only; a host build compiles no `.S` and
  always takes the scalar path.
- Never call an NEON symbol directly from game code — use the pointer.

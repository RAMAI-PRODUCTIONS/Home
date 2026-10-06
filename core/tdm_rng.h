#pragma once

#include <stdint.h>

/* Deterministic xorshift RNG. Seed must never be zero. */

static inline uint32_t tdm_rng_next(uint32_t *s)
{
    uint32_t x = *s;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *s = x ? x : 0x9E3779B9u;
    return *s;
}

static inline float tdm_rng_f(uint32_t *s)
{
    return (tdm_rng_next(s) & 0xFFFFFFu) / 16777216.0f;
}

static inline float tdm_rng_range(uint32_t *s, float a, float b)
{
    return a + tdm_rng_f(s) * (b - a);
}

static inline int tdm_rng_int(uint32_t *s, int n)
{
    if (n <= 0) return 0;
    return (int)(tdm_rng_next(s) % (uint32_t)n);
}

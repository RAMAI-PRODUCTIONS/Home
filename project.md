# Project — Procedural Warfare TDM

## What this is
A 3D team deathmatch shooter for **Android only**, ported from the reference
HTML5/Three.js prototype (`Procedural Warfare - Tactical Edition`).

Blue vs Red. First team to **50 kills** or best score after **10 minutes** wins.

## Stack (fixed)
- **C23** for all game/engine code (`-std=c23`).
- **ARM64 assembly** (NEON) for hot math kernels, with scalar fallbacks.
- **Vulkan 1.1** for all rendering (`VK_KHR_swapchain`, `VK_KHR_android_surface`).
- **NDK NativeActivity** — no Java/Kotlin, `android:hasCode="false"`.

## Feature set (ported from reference)
- Three procedural maps: `DUSTY OUTPOST`, `URBAN RUINS`, `FOREST COMPOUND`.
- 6 bots per team, A* pathfinding, patrol/chase/attack AI.
- 5 weapons: pistol, SMG, rifle, shotgun, sniper (ADS zoom, reload, spread).
- Grenades, health/ammo pickups, drivable vehicles, emplaced turrets.
- Killstreaks: UAV radar at 3, airstrike at 5 and 8.
- HUD: score, timer, minimap, compass-free radar, health, ammo, killfeed,
  hitmarker, damage vignette, XP and streak popups.
- Touch controls: dual virtual sticks, fire/jump/reload/weapon/grenade/use.
- Screens: title, map select, in-match, death/respawn, match over.

## Success criteria
1. `build.bat` produces a signed `RAMAI.apk` for `arm64-v8a`.
2. Every source/doc file in the tree is **under 300 words**.
3. No desktop, web, iOS, or Java code paths anywhere.

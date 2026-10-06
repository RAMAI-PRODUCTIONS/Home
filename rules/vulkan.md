# Vulkan

## Instance / device
- Instance extensions: `VK_KHR_surface`, `VK_KHR_android_surface`.
- One physical device: the first that exposes a graphics+present queue.
- Device extensions: `VK_KHR_swapchain` only.
- Enable no features. Validation layers only in debug builds.

## Memory
- All buffers are `HOST_VISIBLE | HOST_COHERENT` (mobile unified memory).
- No allocation in the frame loop; create resources once, reuse forever.
- One depth image `D32_SFLOAT` (fallback `D24_UNORM_S8_UINT`).

## Passes
Two render passes per frame:
1. `pass3d` — clear color+depth, draw world/entities, store color.
2. `passHud` — load color, draw UI quads, final layout `PRESENT_SRC_KHR`.

## Pipelines
- `pipe3d`: depth test/write on, cull off (box winding is not uniform),
  push constants `mat4 mvp + vec4 fogRange + vec4 fogColor`.
- `pipeHud`: no depth, alpha blend, push constants `vec4 screen`.
- Vertex layout is shared: `pos vec3, uv vec2, color vec4` (36 bytes).

## Synchronization
2 frames in flight: fence + imageAvailable + renderFinished per slot.
Wait the fence before reusing a command buffer. `FIFO` present mode only.

## Rules
- Every `vk*` call goes through `VK_CHECK`.
- Recreate only swapchain-dependent objects on resize; never the pipelines.
- No `vkQueueWaitIdle` inside the frame; fences carry the waits.

#pragma once

#include <android/native_window.h>
#ifndef VK_USE_PLATFORM_ANDROID_KHR
#define VK_USE_PLATFORM_ANDROID_KHR
#endif
#include <vulkan/vulkan.h>

#include "tdm_types.h"

typedef struct Game Game;

#define TDM_MAX_SWAP 8
#define TDM_FRAMES 2

#define VK_CHECK(expr)                                                     \
    do {                                                                   \
        VkResult vkcheck_r = (expr);                                       \
        if (vkcheck_r != VK_SUCCESS)                                       \
            tdm_vk_fail(__FILE__, __LINE__, #expr, (int)vkcheck_r);        \
    } while (0)

typedef struct ScenePush {
    float mvp[16];
    float fog[4];
    float fogCol[4];
} ScenePush;

typedef struct HudPush {
    float screen[4];
} HudPush;

typedef struct Renderer {
    VkInstance inst;
    ANativeWindow *window;
    VkSurfaceKHR surface;
    VkPhysicalDevice phys;
    VkDevice dev;
    uint32_t qfam;
    VkQueue queue;

    VkSwapchainKHR swap;
    VkFormat fmt, depthFmt;
    VkExtent2D ext;
    uint32_t imgN;
    VkImage img[TDM_MAX_SWAP];
    VkImageView view[TDM_MAX_SWAP];
    VkFramebuffer fb3d[TDM_MAX_SWAP];
    VkFramebuffer fbHud[TDM_MAX_SWAP];

    VkImage depthImg;
    VkDeviceMemory depthMem;
    VkImageView depthView;

    VkRenderPass pass3d, passHud;
    VkPipelineLayout lay3d, layHud;
    VkPipeline pipe3d, pipeHud;
    VkDescriptorSetLayout dsl;
    VkDescriptorPool dpool;
    VkDescriptorSet dset;
    VkSampler sampler;
    VkImage fontImg;
    VkDeviceMemory fontMem;
    VkImageView fontView;

    VkBuffer vStatic, vDyn;
    VkDeviceMemory mStatic, mDyn;
    void *stc, *dyn;
    int stcN, dynN, hudN;
    int builtGen;

    VkCommandPool cmdPool;
    VkCommandBuffer cmd[TDM_FRAMES];
    VkSemaphore avail[TDM_FRAMES], done[TDM_FRAMES];
    VkFence fence[TDM_FRAMES];
    uint32_t frame;
    int valid;
} Renderer;

void tdm_vk_fail(const char *file, int line, const char *expr, int code);

int tdm_vk_create(Renderer *r, ANativeWindow *window);
void tdm_vk_destroy(Renderer *r);
int tdm_vk_resize(Renderer *r, ANativeWindow *window);
int tdm_vk_frame(Renderer *r, Game *game);

/* internal helpers shared between tdm_vk_*.c files */
int tdm_vk_device(Renderer *r);
int tdm_vk_swap_images(Renderer *r);
int tdm_vk_swapchain(Renderer *r);
void tdm_vk_swap_free(Renderer *r);
int tdm_vk_pipelines(Renderer *r);
int tdm_font_create(Renderer *r);
int tdm_vk_buffers(Renderer *r);
uint32_t tdm_vk_mem_type(Renderer *r, uint32_t bits, VkMemoryPropertyFlags props);
VkCommandBuffer tdm_vk_begin_oneoff(Renderer *r);
void tdm_vk_end_oneoff(Renderer *r, VkCommandBuffer cmd);

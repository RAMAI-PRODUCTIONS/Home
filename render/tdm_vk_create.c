#include "tdm_vk_int.h"

#include <string.h>

int tdm_vk_create(Renderer *r, ANativeWindow *window)
{
    VkCommandBufferAllocateInfo cai;
    memset(r, 0, sizeof *r);
    r->window = window;
    if (!tdm_vk_make_instance(r)) return 0;
    if (!tdm_vk_make_surface(r, window)) return 0;
    if (!tdm_vk_device(r)) return 0;
    if (!tdm_vk_make_pool(r)) return 0;
    if (!tdm_vk_swapchain(r)) return 0;
    if (!tdm_vk_pipelines(r)) return 0;
    if (!tdm_vk_buffers(r)) return 0;

    memset(&cai, 0, sizeof cai);
    cai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cai.commandPool = r->cmdPool;
    cai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cai.commandBufferCount = TDM_FRAMES;
    if (vkAllocateCommandBuffers(r->dev, &cai, r->cmd) != VK_SUCCESS) return 0;
    tdm_vk_make_sync(r);
    r->frame = 0;
    r->valid = 1;
    return 1;
}

void tdm_vk_destroy(Renderer *r)
{
    int i;
    r->valid = 0;
    if (!r->dev) return;
    vkDeviceWaitIdle(r->dev);
    tdm_vk_swap_free(r);
    for (i = 0; i < TDM_FRAMES; i++) {
        if (r->avail[i]) vkDestroySemaphore(r->dev, r->avail[i], NULL);
        if (r->done[i]) vkDestroySemaphore(r->dev, r->done[i], NULL);
        if (r->fence[i]) vkDestroyFence(r->dev, r->fence[i], NULL);
    }
    if (r->cmdPool) vkDestroyCommandPool(r->dev, r->cmdPool, NULL);
    if (r->pipe3d) vkDestroyPipeline(r->dev, r->pipe3d, NULL);
    if (r->pipeHud) vkDestroyPipeline(r->dev, r->pipeHud, NULL);
    if (r->lay3d) vkDestroyPipelineLayout(r->dev, r->lay3d, NULL);
    if (r->layHud) vkDestroyPipelineLayout(r->dev, r->layHud, NULL);
    if (r->dpool) vkDestroyDescriptorPool(r->dev, r->dpool, NULL);
    if (r->dsl) vkDestroyDescriptorSetLayout(r->dev, r->dsl, NULL);
    if (r->sampler) vkDestroySampler(r->dev, r->sampler, NULL);
    if (r->fontView) vkDestroyImageView(r->dev, r->fontView, NULL);
    if (r->fontImg) vkDestroyImage(r->dev, r->fontImg, NULL);
    if (r->fontMem) vkFreeMemory(r->dev, r->fontMem, NULL);
    if (r->vStatic) vkDestroyBuffer(r->dev, r->vStatic, NULL);
    if (r->vDyn) vkDestroyBuffer(r->dev, r->vDyn, NULL);
    if (r->mStatic) vkFreeMemory(r->dev, r->mStatic, NULL);
    if (r->mDyn) vkFreeMemory(r->dev, r->mDyn, NULL);
    vkDestroyDevice(r->dev, NULL);
    r->dev = VK_NULL_HANDLE;
    if (r->surface) vkDestroySurfaceKHR(r->inst, r->surface, NULL);
    if (r->inst) vkDestroyInstance(r->inst, NULL);
    r->surface = VK_NULL_HANDLE;
    r->inst = VK_NULL_HANDLE;
    r->window = NULL;
}

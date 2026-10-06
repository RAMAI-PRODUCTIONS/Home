#include "tdm_vk_int.h"

#include <string.h>

static int make_fbs(Renderer *r)
{
    uint32_t i;
    for (i = 0; i < r->imgN; i++) {
        VkImageView atts[2];
        VkFramebufferCreateInfo fi;
        atts[0] = r->view[i];
        atts[1] = r->depthView;
        memset(&fi, 0, sizeof fi);
        fi.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fi.renderPass = r->pass3d;
        fi.attachmentCount = 2;
        fi.pAttachments = atts;
        fi.width = r->ext.width;
        fi.height = r->ext.height;
        fi.layers = 1;
        if (vkCreateFramebuffer(r->dev, &fi, NULL, &r->fb3d[i]) != VK_SUCCESS)
            return 0;
        fi.renderPass = r->passHud;
        fi.attachmentCount = 1;
        if (vkCreateFramebuffer(r->dev, &fi, NULL, &r->fbHud[i]) != VK_SUCCESS)
            return 0;
    }
    return 1;
}

int tdm_vk_swapchain(Renderer *r)
{
    if (!tdm_vk_swap_images(r)) return 0;
    if (!tdm_vk_make_passes(r)) return 0;
    return make_fbs(r);
}

void tdm_vk_swap_free(Renderer *r)
{
    uint32_t i;
    if (!r->dev) return;
    for (i = 0; i < r->imgN; i++) {
        if (r->fb3d[i]) vkDestroyFramebuffer(r->dev, r->fb3d[i], NULL);
        if (r->fbHud[i]) vkDestroyFramebuffer(r->dev, r->fbHud[i], NULL);
        if (r->view[i]) vkDestroyImageView(r->dev, r->view[i], NULL);
        r->fb3d[i] = r->fbHud[i] = VK_NULL_HANDLE;
        r->view[i] = VK_NULL_HANDLE;
    }
    if (r->depthView) vkDestroyImageView(r->dev, r->depthView, NULL);
    if (r->depthImg) vkDestroyImage(r->dev, r->depthImg, NULL);
    if (r->depthMem) vkFreeMemory(r->dev, r->depthMem, NULL);
    r->depthView = VK_NULL_HANDLE;
    r->depthImg = VK_NULL_HANDLE;
    r->depthMem = VK_NULL_HANDLE;
    if (r->pass3d) vkDestroyRenderPass(r->dev, r->pass3d, NULL);
    if (r->passHud) vkDestroyRenderPass(r->dev, r->passHud, NULL);
    r->pass3d = r->passHud = VK_NULL_HANDLE;
    if (r->swap) vkDestroySwapchainKHR(r->dev, r->swap, NULL);
    r->swap = VK_NULL_HANDLE;
    r->imgN = 0;
}

int tdm_vk_resize(Renderer *r, ANativeWindow *window)
{
    vkDeviceWaitIdle(r->dev);
    tdm_vk_swap_free(r);
    if (r->surface) {
        vkDestroySurfaceKHR(r->inst, r->surface, NULL);
        r->surface = VK_NULL_HANDLE;
    }
    r->valid = 0;
    r->window = window;
    if (!window) return 1;
    {
        VkAndroidSurfaceCreateInfoKHR ci;
        memset(&ci, 0, sizeof ci);
        ci.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
        ci.window = window;
        if (vkCreateAndroidSurfaceKHR(r->inst, &ci, NULL, &r->surface) != VK_SUCCESS)
            return 0;
    }
    if (!tdm_vk_swapchain(r)) return 0;
    r->valid = 1;
    return 1;
}

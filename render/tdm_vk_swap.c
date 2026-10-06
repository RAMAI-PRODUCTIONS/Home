#include "tdm_vk_int.h"

#include <string.h>

VkFormat tdm_vk_pick_format(VkPhysicalDevice pd, VkSurfaceKHR surf)
{
    VkFormat want[2] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM };
    VkSurfaceFormatKHR fmts[16];
    uint32_t n = 0, i, j;
    vkGetPhysicalDeviceSurfaceFormatsKHR(pd, surf, &n, NULL);
    if (n > 16) n = 16;
    if (!n) return VK_FORMAT_B8G8R8A8_UNORM;
    vkGetPhysicalDeviceSurfaceFormatsKHR(pd, surf, &n, fmts);
    for (j = 0; j < 2; j++)
        for (i = 0; i < n; i++)
            if (fmts[i].format == want[j] &&
                fmts[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return fmts[i].format;
    return fmts[0].format;
}

VkFormat tdm_vk_pick_depth(VkPhysicalDevice pd)
{
    static const VkFormat cands[3] = { VK_FORMAT_D32_SFLOAT,
                                       VK_FORMAT_D24_UNORM_S8_UINT,
                                       VK_FORMAT_D16_UNORM };
    int i;
    for (i = 0; i < 3; i++) {
        VkFormatProperties p;
        vkGetPhysicalDeviceFormatProperties(pd, cands[i], &p);
        if (p.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
            return cands[i];
    }
    return VK_FORMAT_D16_UNORM;
}

int tdm_vk_pick_extent(Renderer *r, const VkSurfaceCapabilitiesKHR *caps)
{
    VkExtent2D e;
    if (caps->currentExtent.width != UINT32_MAX &&
        caps->currentExtent.width > 1 && caps->currentExtent.height > 1) {
        e = caps->currentExtent;
    } else {
        if (!r->window) return 0;
        e.width = (uint32_t)ANativeWindow_getWidth(r->window);
        e.height = (uint32_t)ANativeWindow_getHeight(r->window);
        if (e.width < caps->minImageExtent.width) e.width = caps->minImageExtent.width;
        if (e.height < caps->minImageExtent.height) e.height = caps->minImageExtent.height;
        if (e.width > caps->maxImageExtent.width) e.width = caps->maxImageExtent.width;
        if (e.height > caps->maxImageExtent.height) e.height = caps->maxImageExtent.height;
    }
    if (e.width < 2 || e.height < 2) return 0;
    r->ext = e;
    return 1;
}

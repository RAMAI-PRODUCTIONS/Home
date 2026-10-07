#include "tdm_vk_int.h"

#include <string.h>

int tdm_vk_make_swap(Renderer *r)
{
    VkSurfaceCapabilitiesKHR caps;
    VkSwapchainCreateInfoKHR sc;
    VkSurfaceFormatKHR chosen;
    uint32_t n = 0, i;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(r->phys, r->surface, &caps);
    if (!tdm_vk_pick_extent(r, &caps)) return 0;
    chosen.format = r->fmt;
    chosen.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    if (!vkGetPhysicalDeviceSurfaceFormatsKHR(r->phys, r->surface, &n, NULL) && n) {
        VkSurfaceFormatKHR fmts[16];
        if (n > 16) n = 16;
        vkGetPhysicalDeviceSurfaceFormatsKHR(r->phys, r->surface, &n, fmts);
        for (i = 0; i < n; i++)
            if (fmts[i].format == r->fmt &&
                fmts[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                chosen = fmts[i];
    }

    memset(&sc, 0, sizeof sc);
    sc.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    sc.surface = r->surface;
    sc.minImageCount = caps.minImageCount + 1;
    if (caps.maxImageCount > 0 && sc.minImageCount > caps.maxImageCount)
        sc.minImageCount = caps.maxImageCount;
    if (sc.minImageCount > TDM_MAX_SWAP) sc.minImageCount = TDM_MAX_SWAP;
    if (sc.minImageCount < caps.minImageCount) return 0;
    sc.imageFormat = chosen.format;
    sc.imageColorSpace = chosen.colorSpace;
    sc.imageExtent = r->ext;
    sc.imageArrayLayers = 1;
    sc.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    sc.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    /* The buffer is already in window coordinates; let the presentation
       engine apply the display rotation itself. */
    sc.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    (void)caps.currentTransform;
    sc.compositeAlpha = (caps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
                        ? VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
                        : VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
    sc.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    sc.clipped = VK_TRUE;
    if (vkCreateSwapchainKHR(r->dev, &sc, NULL, &r->swap) != VK_SUCCESS) return 0;

    vkGetSwapchainImagesKHR(r->dev, r->swap, &n, NULL);
    if (n == 0 || n > TDM_MAX_SWAP) return 0;
    r->imgN = n;
    vkGetSwapchainImagesKHR(r->dev, r->swap, &n, r->img);
    return n == r->imgN;
}

int tdm_vk_swap_images(Renderer *r)
{
    r->fmt = tdm_vk_pick_format(r->phys, r->surface);
    r->depthFmt = tdm_vk_pick_depth(r->phys);
    if (!tdm_vk_make_swap(r)) return 0;
    if (!tdm_vk_make_views(r)) return 0;
    return tdm_vk_make_depth(r);
}

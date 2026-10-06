#include "tdm_vk_int.h"

#include <string.h>

int tdm_vk_make_views(Renderer *r)
{
    uint32_t i;
    for (i = 0; i < r->imgN; i++) {
        VkImageViewCreateInfo vi;
        memset(&vi, 0, sizeof vi);
        vi.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        vi.image = r->img[i];
        vi.viewType = VK_IMAGE_VIEW_TYPE_2D;
        vi.format = r->fmt;
        vi.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        vi.subresourceRange.levelCount = 1;
        vi.subresourceRange.layerCount = 1;
        if (vkCreateImageView(r->dev, &vi, NULL, &r->view[i]) != VK_SUCCESS)
            return 0;
    }
    return 1;
}

int tdm_vk_make_depth(Renderer *r)
{
    VkImageCreateInfo ii;
    VkImageViewCreateInfo vi;
    VkMemoryRequirements mr;
    VkMemoryAllocateInfo ai;
    r->depthFmt = tdm_vk_pick_depth(r->phys);
    memset(&ii, 0, sizeof ii);
    ii.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    ii.imageType = VK_IMAGE_TYPE_2D;
    ii.format = r->depthFmt;
    ii.extent.width = r->ext.width;
    ii.extent.height = r->ext.height;
    ii.extent.depth = 1;
    ii.mipLevels = 1;
    ii.arrayLayers = 1;
    ii.samples = VK_SAMPLE_COUNT_1_BIT;
    ii.tiling = VK_IMAGE_TILING_OPTIMAL;
    ii.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    if (vkCreateImage(r->dev, &ii, NULL, &r->depthImg) != VK_SUCCESS) return 0;
    vkGetImageMemoryRequirements(r->dev, r->depthImg, &mr);
    memset(&ai, 0, sizeof ai);
    ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    ai.allocationSize = mr.size;
    ai.memoryTypeIndex = tdm_vk_mem_type(r, mr.memoryTypeBits,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (vkAllocateMemory(r->dev, &ai, NULL, &r->depthMem) != VK_SUCCESS) return 0;
    vkBindImageMemory(r->dev, r->depthImg, r->depthMem, 0);
    memset(&vi, 0, sizeof vi);
    vi.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    vi.image = r->depthImg;
    vi.viewType = VK_IMAGE_VIEW_TYPE_2D;
    vi.format = r->depthFmt;
    vi.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    vi.subresourceRange.levelCount = 1;
    vi.subresourceRange.layerCount = 1;
    return vkCreateImageView(r->dev, &vi, NULL, &r->depthView) == VK_SUCCESS;
}

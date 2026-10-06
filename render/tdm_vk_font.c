#include "tdm_vk_int.h"

#include <string.h>

#include "tdm_font.h"

void tdm_vk_image_barrier(VkCommandBuffer cmd, VkImage img, VkImageLayout oldL,
                          VkImageLayout newL, VkPipelineStageFlags srcStage,
                          VkPipelineStageFlags dstStage, VkAccessFlags srcAcc,
                          VkAccessFlags dstAcc)
{
    VkImageMemoryBarrier b;
    memset(&b, 0, sizeof b);
    b.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    b.oldLayout = oldL;
    b.newLayout = newL;
    b.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    b.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    b.image = img;
    b.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    b.subresourceRange.levelCount = 1;
    b.subresourceRange.layerCount = 1;
    b.srcAccessMask = srcAcc;
    b.dstAccessMask = dstAcc;
    vkCmdPipelineBarrier(cmd, srcStage, dstStage, 0, 0, NULL, 0, NULL, 1, &b);
}

static int make_sampler(Renderer *r)
{
    VkSamplerCreateInfo s;
    memset(&s, 0, sizeof s);
    s.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    s.magFilter = VK_FILTER_NEAREST;
    s.minFilter = VK_FILTER_NEAREST;
    s.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    s.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    s.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    s.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    s.maxLod = 0.0f;
    return vkCreateSampler(r->dev, &s, NULL, &r->sampler) == VK_SUCCESS;
}

static int make_view(Renderer *r)
{
    VkImageViewCreateInfo vi;
    memset(&vi, 0, sizeof vi);
    vi.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    vi.image = r->fontImg;
    vi.viewType = VK_IMAGE_VIEW_TYPE_2D;
    vi.format = VK_FORMAT_R8_UNORM;
    vi.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    vi.subresourceRange.levelCount = 1;
    vi.subresourceRange.layerCount = 1;
    return vkCreateImageView(r->dev, &vi, NULL, &r->fontView) == VK_SUCCESS;
}

int tdm_font_create(Renderer *r)
{
    uint8_t px[TDM_FONT_W * TDM_FONT_H];
    tdm_font_build(px);
    if (!make_sampler(r)) return 0;
    if (!tdm_vk_font_upload(r, px, sizeof px)) return 0;
    if (!make_view(r)) return 0;
    return tdm_vk_font_descriptor(r);
}

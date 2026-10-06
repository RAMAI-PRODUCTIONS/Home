#include "tdm_vk_int.h"

#include <string.h>

#include "tdm_font.h"

int tdm_vk_font_upload(Renderer *r, const uint8_t *px, VkDeviceSize size)
{
    VkBuffer staging;
    VkDeviceMemory smem;
    void *map = NULL;
    VkBufferCreateInfo bi;
    VkMemoryRequirements mr;
    VkMemoryAllocateInfo ai;
    VkImageCreateInfo ii;
    VkCommandBuffer cmd;
    VkBufferImageCopy copy;

    memset(&bi, 0, sizeof bi);
    bi.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bi.size = size;
    bi.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    if (vkCreateBuffer(r->dev, &bi, NULL, &staging) != VK_SUCCESS) return 0;
    vkGetBufferMemoryRequirements(r->dev, staging, &mr);
    memset(&ai, 0, sizeof ai);
    ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    ai.allocationSize = mr.size;
    ai.memoryTypeIndex = tdm_vk_mem_type(r, mr.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (vkAllocateMemory(r->dev, &ai, NULL, &smem) != VK_SUCCESS) return 0;
    vkBindBufferMemory(r->dev, staging, smem, 0);
    vkMapMemory(r->dev, smem, 0, VK_WHOLE_SIZE, 0, &map);
    memcpy(map, px, (size_t)size);
    vkUnmapMemory(r->dev, smem);

    memset(&ii, 0, sizeof ii);
    ii.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    ii.imageType = VK_IMAGE_TYPE_2D;
    ii.format = VK_FORMAT_R8_UNORM;
    ii.extent.width = TDM_FONT_W;
    ii.extent.height = TDM_FONT_H;
    ii.extent.depth = 1;
    ii.mipLevels = 1;
    ii.arrayLayers = 1;
    ii.samples = VK_SAMPLE_COUNT_1_BIT;
    ii.tiling = VK_IMAGE_TILING_OPTIMAL;
    ii.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    ii.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    if (vkCreateImage(r->dev, &ii, NULL, &r->fontImg) != VK_SUCCESS) return 0;
    vkGetImageMemoryRequirements(r->dev, r->fontImg, &mr);
    ai.allocationSize = mr.size;
    ai.memoryTypeIndex = tdm_vk_mem_type(r, mr.memoryTypeBits,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (vkAllocateMemory(r->dev, &ai, NULL, &r->fontMem) != VK_SUCCESS) return 0;
    vkBindImageMemory(r->dev, r->fontImg, r->fontMem, 0);

    cmd = tdm_vk_begin_oneoff(r);
    if (!cmd) return 0;
    tdm_vk_image_barrier(cmd, r->fontImg, VK_IMAGE_LAYOUT_UNDEFINED,
                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                  VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
                  0, VK_ACCESS_TRANSFER_WRITE_BIT);
    memset(&copy, 0, sizeof copy);
    copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy.imageSubresource.layerCount = 1;
    copy.imageExtent.width = TDM_FONT_W;
    copy.imageExtent.height = TDM_FONT_H;
    copy.imageExtent.depth = 1;
    vkCmdCopyBufferToImage(cmd, staging, r->fontImg,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy);
    tdm_vk_image_barrier(cmd, r->fontImg, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                  VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                  VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT);
    tdm_vk_end_oneoff(r, cmd);
    vkDestroyBuffer(r->dev, staging, NULL);
    vkFreeMemory(r->dev, smem, NULL);
    return 1;
}

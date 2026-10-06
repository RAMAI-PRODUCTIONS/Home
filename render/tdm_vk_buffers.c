#include "tdm_render.h"

#include <string.h>

#include "tdm_config.h"

uint32_t tdm_vk_mem_type(Renderer *r, uint32_t bits, VkMemoryPropertyFlags props)
{
    VkPhysicalDeviceMemoryProperties mp;
    uint32_t i;
    vkGetPhysicalDeviceMemoryProperties(r->phys, &mp);
    for (i = 0; i < mp.memoryTypeCount; i++)
        if ((bits & (1u << i)) && (mp.memoryTypes[i].propertyFlags & props) == props)
            return i;
    for (i = 0; i < mp.memoryTypeCount; i++)
        if (bits & (1u << i)) return i;
    return 0;
}

static int make_vbuf(Renderer *r, VkDeviceSize size, VkBuffer *buf,
                     VkDeviceMemory *mem, void **map)
{
    VkBufferCreateInfo bi;
    VkMemoryRequirements mr;
    VkMemoryAllocateInfo ai;
    memset(&bi, 0, sizeof bi);
    bi.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bi.size = size;
    bi.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bi.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateBuffer(r->dev, &bi, NULL, buf) != VK_SUCCESS) return 0;
    vkGetBufferMemoryRequirements(r->dev, *buf, &mr);
    memset(&ai, 0, sizeof ai);
    ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    ai.allocationSize = mr.size;
    ai.memoryTypeIndex = tdm_vk_mem_type(r, mr.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (vkAllocateMemory(r->dev, &ai, NULL, mem) != VK_SUCCESS) return 0;
    vkBindBufferMemory(r->dev, *buf, *mem, 0);
    return vkMapMemory(r->dev, *mem, 0, VK_WHOLE_SIZE, 0, map) == VK_SUCCESS;
}

int tdm_vk_buffers(Renderer *r)
{
    if (!make_vbuf(r, (VkDeviceSize)TDM_STATIC_VERTS * sizeof(Vtx),
                   &r->vStatic, &r->mStatic, &r->stc)) return 0;
    if (!make_vbuf(r, (VkDeviceSize)(TDM_DYN_VERTS + TDM_HUD_VERTS) * sizeof(Vtx),
                   &r->vDyn, &r->mDyn, &r->dyn)) return 0;
    r->stcN = r->dynN = r->hudN = 0;
    return 1;
}

VkCommandBuffer tdm_vk_begin_oneoff(Renderer *r)
{
    VkCommandBufferAllocateInfo ai;
    VkCommandBufferBeginInfo bi;
    VkCommandBuffer cmd;
    memset(&ai, 0, sizeof ai);
    ai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    ai.commandPool = r->cmdPool;
    ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    ai.commandBufferCount = 1;
    if (vkAllocateCommandBuffers(r->dev, &ai, &cmd) != VK_SUCCESS)
        return VK_NULL_HANDLE;
    memset(&bi, 0, sizeof bi);
    bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmd, &bi);
    return cmd;
}

void tdm_vk_end_oneoff(Renderer *r, VkCommandBuffer cmd)
{
    VkSubmitInfo si;
    vkEndCommandBuffer(cmd);
    memset(&si, 0, sizeof si);
    si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    si.commandBufferCount = 1;
    si.pCommandBuffers = &cmd;
    vkQueueSubmit(r->queue, 1, &si, VK_NULL_HANDLE);
    vkQueueWaitIdle(r->queue);
    vkFreeCommandBuffers(r->dev, r->cmdPool, 1, &cmd);
}

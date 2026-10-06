#include "tdm_vk_int.h"

#include <string.h>

int tdm_vk_frame(Renderer *r, Game *game)
{
    uint32_t slot, idx = 0;
    VkResult res;
    VkSubmitInfo si;
    VkPresentInfoKHR pi;
    VkSemaphore avail, done;

    if (!r->valid) return 0;
    slot = r->frame % TDM_FRAMES;
    VK_CHECK(vkWaitForFences(r->dev, 1, &r->fence[slot], VK_TRUE, UINT64_MAX));
    res = vkAcquireNextImageKHR(r->dev, r->swap, UINT64_MAX, r->avail[slot],
                                VK_NULL_HANDLE, &idx);
    if (res == VK_ERROR_OUT_OF_DATE_KHR)
        return tdm_vk_resize(r, r->window);
    VK_CHECK(vkResetFences(r->dev, 1, &r->fence[slot]));
    VK_CHECK(vkResetCommandBuffer(r->cmd[slot], 0));
    {
        VkCommandBufferBeginInfo bi;
        memset(&bi, 0, sizeof bi);
        bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        VK_CHECK(vkBeginCommandBuffer(r->cmd[slot], &bi));
    }
    tdm_vk_record(r, game, r->cmd[slot], idx);
    VK_CHECK(vkEndCommandBuffer(r->cmd[slot]));

    avail = r->avail[slot];
    done = r->done[slot];
    memset(&si, 0, sizeof si);
    si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    si.waitSemaphoreCount = 1;
    si.pWaitSemaphores = &avail;
    si.pWaitDstStageMask = (VkPipelineStageFlags[]){ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    si.commandBufferCount = 1;
    si.pCommandBuffers = &r->cmd[slot];
    si.signalSemaphoreCount = 1;
    si.pSignalSemaphores = &done;
    VK_CHECK(vkQueueSubmit(r->queue, 1, &si, r->fence[slot]));

    memset(&pi, 0, sizeof pi);
    pi.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    pi.waitSemaphoreCount = 1;
    pi.pWaitSemaphores = &done;
    pi.swapchainCount = 1;
    pi.pSwapchains = &r->swap;
    pi.pImageIndices = &idx;
    res = vkQueuePresentKHR(r->queue, &pi);
    r->frame++;
    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
        return tdm_vk_resize(r, r->window);
    return 1;
}

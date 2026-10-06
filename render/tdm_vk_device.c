#include "tdm_vk_int.h"

#include <string.h>

int tdm_vk_device(Renderer *r)
{
    VkPhysicalDevice devs[8];
    uint32_t n = 0, i;
    float prio = 1.0f;
    VkDeviceQueueCreateInfo qci;
    VkDeviceCreateInfo dci;
    static const char *ext = "VK_KHR_swapchain";

    vkEnumeratePhysicalDevices(r->inst, &n, NULL);
    if (n > 8) n = 8;
    if (!n) return 0;
    vkEnumeratePhysicalDevices(r->inst, &n, devs);
    for (i = 0; i < n; i++) {
        if (tdm_vk_pick_queue(r, devs[i], &r->qfam)) {
            r->phys = devs[i];
            break;
        }
    }
    if (!r->phys) return 0;

    memset(&qci, 0, sizeof qci);
    qci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    qci.queueFamilyIndex = r->qfam;
    qci.queueCount = 1;
    qci.pQueuePriorities = &prio;
    memset(&dci, 0, sizeof dci);
    dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    dci.queueCreateInfoCount = 1;
    dci.pQueueCreateInfos = &qci;
    dci.enabledExtensionCount = 1;
    dci.ppEnabledExtensionNames = &ext;
    if (vkCreateDevice(r->phys, &dci, NULL, &r->dev) != VK_SUCCESS) return 0;
    vkGetDeviceQueue(r->dev, r->qfam, 0, &r->queue);
    return 1;
}

int tdm_vk_make_pool(Renderer *r)
{
    VkCommandPoolCreateInfo pci;
    memset(&pci, 0, sizeof pci);
    pci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pci.queueFamilyIndex = r->qfam;
    return vkCreateCommandPool(r->dev, &pci, NULL, &r->cmdPool) == VK_SUCCESS;
}

void tdm_vk_make_sync(Renderer *r)
{
    int i;
    VkSemaphoreCreateInfo sci;
    VkFenceCreateInfo fci;
    memset(&sci, 0, sizeof sci);
    sci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    memset(&fci, 0, sizeof fci);
    fci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fci.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    for (i = 0; i < TDM_FRAMES; i++) {
        vkCreateSemaphore(r->dev, &sci, NULL, &r->avail[i]);
        vkCreateSemaphore(r->dev, &sci, NULL, &r->done[i]);
        vkCreateFence(r->dev, &fci, NULL, &r->fence[i]);
    }
}

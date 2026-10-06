#include "tdm_vk_int.h"

#include <android/log.h>
#include <string.h>

void tdm_vk_fail(const char *file, int line, const char *expr, int code)
{
    __android_log_print(ANDROID_LOG_ERROR, "TDM", "%s:%d %s failed (%d)",
                        file, line, expr, code);
}

int tdm_vk_make_instance(Renderer *r)
{
    VkApplicationInfo app;
    VkInstanceCreateInfo ci;
    static const char *exts[2] = { "VK_KHR_surface", "VK_KHR_android_surface" };
    memset(&app, 0, sizeof app);
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pApplicationName = "Procedural Warfare";
    app.pEngineName = "SSE";
    app.apiVersion = VK_API_VERSION_1_1;
    memset(&ci, 0, sizeof ci);
    ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ci.pApplicationInfo = &app;
    ci.enabledExtensionCount = 2;
    ci.ppEnabledExtensionNames = exts;
    return vkCreateInstance(&ci, NULL, &r->inst) == VK_SUCCESS;
}

int tdm_vk_make_surface(Renderer *r, ANativeWindow *window)
{
    VkAndroidSurfaceCreateInfoKHR ci;
    memset(&ci, 0, sizeof ci);
    ci.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
    ci.window = window;
    return vkCreateAndroidSurfaceKHR(r->inst, &ci, NULL, &r->surface) == VK_SUCCESS;
}

int tdm_vk_pick_queue(Renderer *r, VkPhysicalDevice pd, uint32_t *out)
{
    VkQueueFamilyProperties q[16];
    uint32_t n = 0, i;
    vkGetPhysicalDeviceQueueFamilyProperties(pd, &n, NULL);
    if (n > 16) n = 16;
    vkGetPhysicalDeviceQueueFamilyProperties(pd, &n, q);
    for (i = 0; i < n; i++) {
        VkBool32 present = VK_FALSE;
        if (!(q[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) continue;
        vkGetPhysicalDeviceSurfaceSupportKHR(pd, i, r->surface, &present);
        if (present) {
            *out = i;
            return 1;
        }
    }
    return 0;
}

#include "tdm_vk_int.h"

#include <string.h>

int tdm_vk_make_passes(Renderer *r)
{
    VkAttachmentDescription at[2];
    VkAttachmentReference colRef, depRef;
    VkSubpassDescription sub;
    VkSubpassDependency dep;
    VkRenderPassCreateInfo rc;

    memset(at, 0, sizeof at);
    at[0].format = r->fmt;
    at[0].samples = VK_SAMPLE_COUNT_1_BIT;
    at[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    at[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    at[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    at[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    at[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    at[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    at[1] = at[0];
    at[1].format = r->depthFmt;
    at[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    at[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    colRef.attachment = 0;
    colRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    depRef.attachment = 1;
    depRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    memset(&sub, 0, sizeof sub);
    sub.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    sub.colorAttachmentCount = 1;
    sub.pColorAttachments = &colRef;
    sub.pDepthStencilAttachment = &depRef;
    memset(&dep, 0, sizeof dep);
    dep.srcSubpass = VK_SUBPASS_EXTERNAL;
    dep.dstSubpass = 0;
    dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                       VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                       VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                        VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    memset(&rc, 0, sizeof rc);
    rc.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rc.attachmentCount = 2;
    rc.pAttachments = at;
    rc.subpassCount = 1;
    rc.pSubpasses = &sub;
    rc.dependencyCount = 1;
    rc.pDependencies = &dep;
    if (vkCreateRenderPass(r->dev, &rc, NULL, &r->pass3d) != VK_SUCCESS) return 0;

    at[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    at[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    at[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    sub.pDepthStencilAttachment = NULL;
    rc.attachmentCount = 1;
    return vkCreateRenderPass(r->dev, &rc, NULL, &r->passHud) == VK_SUCCESS;
}

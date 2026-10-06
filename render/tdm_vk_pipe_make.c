#include "tdm_vk_int.h"

#include <string.h>

VkPipeline tdm_vk_make_pipe(Renderer *r, VkRenderPass pass, VkPipelineLayout lay,
                            const uint32_t *vs, VkDeviceSize vsSize,
                            const uint32_t *fs, VkDeviceSize fsSize,
                            int hud)
{
    VkPipelineShaderStageCreateInfo stage[2];
    VkVertexInputBindingDescription bind;
    VkVertexInputAttributeDescription attr[3];
    VkPipelineVertexInputStateCreateInfo vin;
    VkPipelineInputAssemblyStateCreateInfo ia;
    VkPipelineViewportStateCreateInfo vp;
    VkPipelineRasterizationStateCreateInfo rs;
    VkPipelineMultisampleStateCreateInfo ms;
    VkPipelineDepthStencilStateCreateInfo ds;
    VkPipelineColorBlendAttachmentState blendAtt;
    VkPipelineColorBlendStateCreateInfo cb;
    VkDynamicState dyn[2];
    VkPipelineDynamicStateCreateInfo dsv;
    VkGraphicsPipelineCreateInfo pi;
    VkPipeline p = VK_NULL_HANDLE;
    int i;

    if (!tdm_vk_pipe_stages(r, stage, vs, vsSize, fs, fsSize))
        return VK_NULL_HANDLE;
    tdm_vk_pipe_vin(&vin, &bind, attr);

    memset(&ia, 0, sizeof ia);
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    memset(&vp, 0, sizeof vp);
    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vp.viewportCount = 1;
    vp.scissorCount = 1;
    memset(&rs, 0, sizeof rs);
    rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rs.polygonMode = VK_POLYGON_MODE_FILL;
    rs.cullMode = VK_CULL_MODE_NONE;
    rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rs.lineWidth = 1.0f;
    memset(&ms, 0, sizeof ms);
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    memset(&ds, 0, sizeof ds);
    ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    ds.depthTestEnable = hud ? VK_FALSE : VK_TRUE;
    ds.depthWriteEnable = hud ? VK_FALSE : VK_TRUE;
    ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    memset(&blendAtt, 0, sizeof blendAtt);
    blendAtt.colorWriteMask = 0xF;
    if (hud) {
        blendAtt.blendEnable = VK_TRUE;
        blendAtt.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        blendAtt.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        blendAtt.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        blendAtt.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    }
    memset(&cb, 0, sizeof cb);
    cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    cb.attachmentCount = 1;
    cb.pAttachments = &blendAtt;
    dyn[0] = VK_DYNAMIC_STATE_VIEWPORT;
    dyn[1] = VK_DYNAMIC_STATE_SCISSOR;
    memset(&dsv, 0, sizeof dsv);
    dsv.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dsv.dynamicStateCount = 2;
    dsv.pDynamicStates = dyn;
    memset(&pi, 0, sizeof pi);
    pi.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pi.stageCount = 2;
    pi.pStages = stage;
    pi.pVertexInputState = &vin;
    pi.pInputAssemblyState = &ia;
    pi.pViewportState = &vp;
    pi.pRasterizationState = &rs;
    pi.pMultisampleState = &ms;
    pi.pDepthStencilState = &ds;
    pi.pColorBlendState = &cb;
    pi.pDynamicState = &dsv;
    pi.layout = lay;
    pi.renderPass = pass;
    pi.subpass = 0;
    i = vkCreateGraphicsPipelines(r->dev, VK_NULL_HANDLE, 1, &pi, NULL, &p);
    vkDestroyShaderModule(r->dev, stage[0].module, NULL);
    vkDestroyShaderModule(r->dev, stage[1].module, NULL);
    return i == VK_SUCCESS ? p : VK_NULL_HANDLE;
}

#include "tdm_vk_int.h"

#include <string.h>

int tdm_vk_pipe_stages(Renderer *r, VkPipelineShaderStageCreateInfo stage[2],
                       const uint32_t *vs, VkDeviceSize vsSize,
                       const uint32_t *fs, VkDeviceSize fsSize)
{
    memset(stage, 0, sizeof(VkPipelineShaderStageCreateInfo) * 2);
    stage[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stage[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stage[0].module = tdm_vk_shader(r, vs, vsSize);
    stage[0].pName = "main";
    stage[1] = stage[0];
    stage[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stage[1].module = tdm_vk_shader(r, fs, fsSize);
    return stage[0].module && stage[1].module;
}

void tdm_vk_pipe_vin(VkPipelineVertexInputStateCreateInfo *vin,
                     VkVertexInputBindingDescription *bind,
                     VkVertexInputAttributeDescription *attr)
{
    bind->binding = 0;
    bind->stride = sizeof(Vtx);
    bind->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    attr[0] = (VkVertexInputAttributeDescription){ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0 };
    attr[1] = (VkVertexInputAttributeDescription){ 1, 0, VK_FORMAT_R32G32_SFLOAT, 12 };
    attr[2] = (VkVertexInputAttributeDescription){ 2, 0, VK_FORMAT_R32G32B32A32_SFLOAT, 20 };
    memset(vin, 0, sizeof *vin);
    vin->sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vin->vertexBindingDescriptionCount = 1;
    vin->pVertexBindingDescriptions = bind;
    vin->vertexAttributeDescriptionCount = 3;
    vin->pVertexAttributeDescriptions = attr;
}

#include "tdm_vk_int.h"

#include <string.h>

#include "tdm_hud_frag.h"
#include "tdm_hud_vert.h"
#include "tdm_scene_frag.h"
#include "tdm_scene_vert.h"

VkShaderModule tdm_vk_shader(Renderer *r, const uint32_t *code, VkDeviceSize size)
{
    VkShaderModuleCreateInfo ci;
    VkShaderModule m = VK_NULL_HANDLE;
    memset(&ci, 0, sizeof ci);
    ci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    ci.codeSize = size;
    ci.pCode = code;
    if (vkCreateShaderModule(r->dev, &ci, NULL, &m) != VK_SUCCESS)
        return VK_NULL_HANDLE;
    return m;
}

static int make_layouts(Renderer *r)
{
    VkPushConstantRange pr;
    VkPipelineLayoutCreateInfo li;
    VkDescriptorSetLayoutBinding b;
    VkDescriptorSetLayoutCreateInfo dl;

    memset(&pr, 0, sizeof pr);
    pr.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pr.size = sizeof(ScenePush);
    memset(&li, 0, sizeof li);
    li.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    li.pushConstantRangeCount = 1;
    li.pPushConstantRanges = &pr;
    if (vkCreatePipelineLayout(r->dev, &li, NULL, &r->lay3d) != VK_SUCCESS) return 0;

    memset(&b, 0, sizeof b);
    b.binding = 0;
    b.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    b.descriptorCount = 1;
    b.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    memset(&dl, 0, sizeof dl);
    dl.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    dl.bindingCount = 1;
    dl.pBindings = &b;
    if (vkCreateDescriptorSetLayout(r->dev, &dl, NULL, &r->dsl) != VK_SUCCESS) return 0;

    pr.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pr.size = sizeof(HudPush);
    li.setLayoutCount = 1;
    li.pSetLayouts = &r->dsl;
    return vkCreatePipelineLayout(r->dev, &li, NULL, &r->layHud) == VK_SUCCESS;
}

int tdm_vk_pipelines(Renderer *r)
{
    if (!make_layouts(r)) return 0;
    if (!tdm_font_create(r)) return 0;
    r->pipe3d = tdm_vk_make_pipe(r, r->pass3d, r->lay3d,
                                 tdm_scene_vert_spv, sizeof tdm_scene_vert_spv,
                                 tdm_scene_frag_spv, sizeof tdm_scene_frag_spv, 0);
    r->pipeHud = tdm_vk_make_pipe(r, r->passHud, r->layHud,
                                  tdm_hud_vert_spv, sizeof tdm_hud_vert_spv,
                                  tdm_hud_frag_spv, sizeof tdm_hud_frag_spv, 1);
    return r->pipe3d && r->pipeHud;
}

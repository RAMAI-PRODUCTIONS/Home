#include "tdm_vk_int.h"

#include <string.h>

int tdm_vk_font_descriptor(Renderer *r)
{
    VkDescriptorPoolSize ps;
    VkDescriptorPoolCreateInfo pi;
    VkDescriptorSetAllocateInfo ai;
    VkDescriptorImageInfo ii;
    VkWriteDescriptorSet w;

    memset(&ps, 0, sizeof ps);
    ps.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    ps.descriptorCount = 1;
    memset(&pi, 0, sizeof pi);
    pi.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pi.maxSets = 1;
    pi.poolSizeCount = 1;
    pi.pPoolSizes = &ps;
    if (vkCreateDescriptorPool(r->dev, &pi, NULL, &r->dpool) != VK_SUCCESS) return 0;
    memset(&ai, 0, sizeof ai);
    ai.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    ai.descriptorPool = r->dpool;
    ai.descriptorSetCount = 1;
    ai.pSetLayouts = &r->dsl;
    if (vkAllocateDescriptorSets(r->dev, &ai, &r->dset) != VK_SUCCESS) return 0;

    memset(&ii, 0, sizeof ii);
    ii.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    ii.imageView = r->fontView;
    ii.sampler = r->sampler;
    memset(&w, 0, sizeof w);
    w.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    w.dstSet = r->dset;
    w.dstBinding = 0;
    w.descriptorCount = 1;
    w.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    w.pImageInfo = &ii;
    vkUpdateDescriptorSets(r->dev, 1, &w, 0, NULL);
    return 1;
}

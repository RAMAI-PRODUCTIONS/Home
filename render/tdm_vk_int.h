#pragma once

#include "tdm_render.h"

/* Internals shared between the tdm_vk_* translation units. Every one of
   these used to be static; they are promoted here so the pieces of a single
   driver stage can live in separate <300-word files. */

int tdm_vk_make_instance(Renderer *r);
int tdm_vk_make_surface(Renderer *r, ANativeWindow *window);
int tdm_vk_pick_queue(Renderer *r, VkPhysicalDevice pd, uint32_t *out);
int tdm_vk_make_pool(Renderer *r);
void tdm_vk_make_sync(Renderer *r);

VkFormat tdm_vk_pick_format(VkPhysicalDevice pd, VkSurfaceKHR surf);
VkFormat tdm_vk_pick_depth(VkPhysicalDevice pd);
int tdm_vk_pick_extent(Renderer *r, const VkSurfaceCapabilitiesKHR *caps);
int tdm_vk_make_views(Renderer *r);
int tdm_vk_make_depth(Renderer *r);
int tdm_vk_make_swap(Renderer *r);

VkShaderModule tdm_vk_shader(Renderer *r, const uint32_t *code, VkDeviceSize size);
int tdm_vk_pipe_stages(Renderer *r, VkPipelineShaderStageCreateInfo stage[2],
                       const uint32_t *vs, VkDeviceSize vsSize,
                       const uint32_t *fs, VkDeviceSize fsSize);
void tdm_vk_pipe_vin(VkPipelineVertexInputStateCreateInfo *vin,
                     VkVertexInputBindingDescription *bind,
                     VkVertexInputAttributeDescription *attr);
VkPipeline tdm_vk_make_pipe(Renderer *r, VkRenderPass pass, VkPipelineLayout lay,
                            const uint32_t *vs, VkDeviceSize vsSize,
                            const uint32_t *fs, VkDeviceSize fsSize, int hud);

void tdm_vk_image_barrier(VkCommandBuffer cmd, VkImage img, VkImageLayout oldL,
                          VkImageLayout newL, VkPipelineStageFlags srcStage,
                          VkPipelineStageFlags dstStage, VkAccessFlags srcAcc,
                          VkAccessFlags dstAcc);
int tdm_vk_font_upload(Renderer *r, const uint8_t *px, VkDeviceSize size);
int tdm_vk_font_descriptor(Renderer *r);

int tdm_vk_make_passes(Renderer *r);

void tdm_vk_build_push(const Renderer *r, const Game *game, ScenePush *sp);
void tdm_vk_set_viewport(VkCommandBuffer cmd, uint32_t w, uint32_t h);
void tdm_vk_record(Renderer *r, Game *game, VkCommandBuffer cmd, uint32_t idx);

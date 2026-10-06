#include "tdm_vk_int.h"

#include <string.h>

#include "tdm_game.h"

void tdm_vk_record(Renderer *r, Game *game, VkCommandBuffer cmd, uint32_t idx)
{
    VkRenderPassBeginInfo rp;
    VkClearValue clear[2];
    VkDeviceSize off = 0;
    ScenePush sp;
    HudPush hp;
    uint32_t w, h;

    if (idx >= r->imgN) return;
    w = r->ext.width;
    h = r->ext.height;

    tdm_vk_build_push(r, game, &sp);
    memset(clear, 0, sizeof clear);
    clear[0].color.float32[0] = game->clearR;
    clear[0].color.float32[1] = game->clearG;
    clear[0].color.float32[2] = game->clearB;
    clear[0].color.float32[3] = 1.0f;
    clear[1].depthStencil.depth = 1.0f;

    memset(&rp, 0, sizeof rp);
    rp.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp.renderPass = r->pass3d;
    rp.framebuffer = r->fb3d[idx];
    rp.renderArea.extent.width = w;
    rp.renderArea.extent.height = h;
    rp.clearValueCount = 2;
    rp.pClearValues = clear;
    vkCmdBeginRenderPass(cmd, &rp, VK_SUBPASS_CONTENTS_INLINE);
    tdm_vk_set_viewport(cmd, w, h);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, r->pipe3d);
    vkCmdPushConstants(cmd, r->lay3d,
                       VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof sp, &sp);
    if (r->stcN > 0) {
        vkCmdBindVertexBuffers(cmd, 0, 1, &r->vStatic, &off);
        vkCmdDraw(cmd, (uint32_t)r->stcN, 1, 0, 0);
    }
    if (r->dynN > 0) {
        vkCmdBindVertexBuffers(cmd, 0, 1, &r->vDyn, &off);
        vkCmdDraw(cmd, (uint32_t)r->dynN, 1, 0, 0);
    }
    vkCmdEndRenderPass(cmd);

    rp.renderPass = r->passHud;
    rp.framebuffer = r->fbHud[idx];
    rp.clearValueCount = 1;
    vkCmdBeginRenderPass(cmd, &rp, VK_SUBPASS_CONTENTS_INLINE);
    tdm_vk_set_viewport(cmd, w, h);
    if (r->hudN > 0) {
        VkDeviceSize hudOff = (VkDeviceSize)r->dynN * sizeof(Vtx);
        hp.screen[0] = (float)w;
        hp.screen[1] = (float)h;
        hp.screen[2] = 0.0f;
        hp.screen[3] = 0.0f;
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, r->pipeHud);
        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, r->layHud,
                                0, 1, &r->dset, 0, NULL);
        vkCmdPushConstants(cmd, r->layHud, VK_SHADER_STAGE_VERTEX_BIT,
                           0, sizeof hp, &hp);
        vkCmdBindVertexBuffers(cmd, 0, 1, &r->vDyn, &hudOff);
        vkCmdDraw(cmd, (uint32_t)r->hudN, 1, 0, 0);
    }
    vkCmdEndRenderPass(cmd);
}

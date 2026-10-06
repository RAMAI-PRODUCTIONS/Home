#include "tdm_vk_int.h"

#include <string.h>

#include "tdm_game.h"
#include "tdm_math.h"

#define DEG2RAD 0.0174532925f

void tdm_vk_build_push(const Renderer *r, const Game *game, ScenePush *sp)
{
    Mat4 proj = m4_perspective(game->camFov * DEG2RAD,
                               (float)r->ext.width / (float)r->ext.height,
                               0.1f, 600.0f);
    Vec3 eye = v3_add(game->camPos, game->camShake);
    Mat4 view = m4_view(eye, game->camYaw, game->camPitch, game->camRoll);
    Mat4 mvp = m4_mul(&proj, &view);
    memcpy(sp->mvp, mvp.m, sizeof mvp.m);
    sp->fog[0] = game->fogStart;
    sp->fog[1] = game->fogEnd;
    sp->fog[2] = 0.0f;
    sp->fog[3] = 0.0f;
    sp->fogCol[0] = game->fogR;
    sp->fogCol[1] = game->fogG;
    sp->fogCol[2] = game->fogB;
    sp->fogCol[3] = 1.0f;
}

void tdm_vk_set_viewport(VkCommandBuffer cmd, uint32_t w, uint32_t h)
{
    VkViewport vp;
    VkRect2D sc;
    memset(&vp, 0, sizeof vp);
    vp.width = (float)w;
    vp.height = (float)h;
    vp.maxDepth = 1.0f;
    memset(&sc, 0, sizeof sc);
    sc.extent.width = w;
    sc.extent.height = h;
    vkCmdSetViewport(cmd, 0, 1, &vp);
    vkCmdSetScissor(cmd, 0, 1, &sc);
}

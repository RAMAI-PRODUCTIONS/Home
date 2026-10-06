#include "tdm_scene.h"

#include <math.h>

#include "tdm_math.h"

Mat4 tdm_scene_rot_y(float a)
{
    float c = cosf(a), s = sinf(a);
    Mat4 m = {{ c, 0, -s, 0, 0, 1, 0, 0, s, 0, c, 0, 0, 0, 0, 1 }};
    return m;
}

Mat4 tdm_scene_rot_x(float a)
{
    float c = cosf(a), s = sinf(a);
    Mat4 m = {{ 1, 0, 0, 0, 0, c, s, 0, 0, -s, c, 0, 0, 0, 0, 1 }};
    return m;
}

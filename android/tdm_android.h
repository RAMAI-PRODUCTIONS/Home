#pragma once

#include <android/input.h>
#include <android_native_app_glue.h>

#include "tdm_input.h"

struct Game;

extern InputState tdm_in;

/* touch state shared by the input split files */
#define TDM_MAX_PTR 8
#define TDM_STICK_R 55.0f
extern int s_pid[TDM_MAX_PTR], s_role[TDM_MAX_PTR], s_bid[TDM_MAX_PTR];
extern float s_lookRX, s_lookRY;
int tdm_slot_free(void);
int tdm_slot_of(int id);
void tdm_ptr_press(int id);
void tdm_ptr_release(int id);
void tdm_stick_move(float px, float py, int role, float ox, float oy,
                    float *kx, float *ky);
void tdm_stick_end(int role);

struct Game *g_tdm_game(void);
void tdm_input_look_step(float dt);
void tdm_input_reset(void);
int32_t tdm_input_event(struct android_app *app, AInputEvent *ev);
int32_t tdm_android_key(struct android_app *app, AInputEvent *ev);
int32_t tdm_android_input(struct android_app *app, AInputEvent *ev);

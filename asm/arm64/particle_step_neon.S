// void tdm_particle_step_neon(float *posVel, unsigned count,
//     float dt, float gravity)
// Record = 6 floats: px py pz vx vy vz. Semi-implicit Euler, matching the
// scalar reference exactly: velocity first, then position.
    .text
    .globl tdm_particle_step_neon
    .type tdm_particle_step_neon, %function
tdm_particle_step_neon:
    cbz w1, .Ldone
    fmul s2, s0, s1
    fneg s2, s2
    dup v30.4s, v0.s[0]
    movi v29.4s, 0
    fmov w2, s2
    ins v29.s[1], w2
.Lloop:
    ldr q0, [x0]
    ldr q1, [x0, #12]
    ins v1.s[3], wzr
    fadd v1.4s, v1.4s, v29.4s
    fmul v2.4s, v1.4s, v30.4s
    fadd v0.4s, v0.4s, v2.4s
    str q0, [x0]                // px py pz vx
    umov w4, v1.s[1]
    umov w5, v1.s[2]
    str w4, [x0, #16]           // vy
    str w5, [x0, #20]           // vz
    add x0, x0, #24
    subs w1, w1, #1
    b.ne .Lloop
.Ldone:
    ret
    .size tdm_particle_step_neon, .-tdm_particle_step_neon

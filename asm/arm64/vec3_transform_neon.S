// void tdm_vec3_transform_batch_neon(const float *m, const float *in,
//     float *out, unsigned count, unsigned inStride, unsigned outStride)
// Transforms xyz with w = 1; lane 3 of each record is copied untouched so
// vertex streams can be transformed in place. Strides are in floats >= 4.
    .text
    .globl tdm_vec3_transform_batch_neon
    .type tdm_vec3_transform_batch_neon, %function
tdm_vec3_transform_batch_neon:
    cbz w3, .Ldone
    ld1 {v0.4s, v1.4s, v2.4s, v3.4s}, [x0]
    trn1 v4.4s, v0.4s, v1.4s
    trn2 v5.4s, v0.4s, v1.4s
    trn1 v6.4s, v2.4s, v3.4s
    trn2 v7.4s, v2.4s, v3.4s
    trn1 v16.2d, v4.2d, v6.2d   // row 0 -> d[0]
    trn1 v17.2d, v5.2d, v7.2d   // row 1 -> d[1]
    trn2 v18.2d, v4.2d, v6.2d   // row 2 -> d[2]
    movz w6, #0x3f80, lsl #16
    ins v24.s[3], w6
.Lloop:
    ldr q0, [x1]
    ins v27.s[0], v0.s[3]
    ins v0.s[3], v24.s[3]
    fmul v2.4s, v16.4s, v0.4s     // row0 * v
    fmul v3.4s, v17.4s, v0.4s     // row1 * v
    fmul v25.4s, v18.4s, v0.4s    // row2 * v
    faddp v2.4s, v2.4s, v2.4s
    faddp v3.4s, v3.4s, v3.4s
    faddp v25.4s, v25.4s, v25.4s
    faddp v2.4s, v2.4s, v2.4s     // lane0 = row0 . v
    faddp v3.4s, v3.4s, v3.4s     // lane0 = row1 . v
    faddp v25.4s, v25.4s, v25.4s  // lane0 = row2 . v
    ins v2.s[1], v3.s[0]
    ins v2.s[2], v25.s[0]
    ins v2.s[3], v27.s[0]
    st1 {v2.4s}, [x2]
    add x1, x1, w4, uxtw #2
    add x2, x2, w5, uxtw #2
    subs w3, w3, #1
    b.ne .Lloop
.Ldone:
    ret
    .size tdm_vec3_transform_batch_neon, .-tdm_vec3_transform_batch_neon

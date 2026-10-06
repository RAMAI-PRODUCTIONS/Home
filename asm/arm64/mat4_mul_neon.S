// void tdm_mat4_mul_neon(const float *a, const float *b, float *out)
// out = a * b, column-major. Uses only caller-saved V registers.
//
// Column-major means a[i][k] == a[k*4 + i] and b[k][j] == b[j*4 + k], so
// out[i][j] = sum_k a[i][k] * b[k][j].  Accumulating over k therefore needs
// the k-th column of A scaled by b[k][j], which is exactly what MCOL does:
// lane i of the destination is out[i][j], the row index.
    .text
    .globl tdm_mat4_mul_neon
    .type tdm_mat4_mul_neon, %function

// Multiplies A (columns in v16..v19) by B column \BREG, result into \OUT.
    .macro MCOL OUT, BREG
    dup v20.4s, \BREG\().s[0]
    dup v21.4s, \BREG\().s[1]
    dup v22.4s, \BREG\().s[2]
    dup v23.4s, \BREG\().s[3]
    fmul \OUT\().4s, v16.4s, v20.4s
    fmla \OUT\().4s, v17.4s, v21.4s
    fmla \OUT\().4s, v18.4s, v22.4s
    fmla \OUT\().4s, v19.4s, v23.4s
    .endm

tdm_mat4_mul_neon:
    ld1 {v16.4s, v17.4s, v18.4s, v19.4s}, [x0]
    ld1 {v0.4s, v1.4s, v2.4s, v3.4s}, [x1]
    MCOL v12, v0
    MCOL v13, v1
    MCOL v14, v2
    MCOL v15, v3
    st1 {v12.4s}, [x2], #16
    st1 {v13.4s}, [x2], #16
    st1 {v14.4s}, [x2], #16
    st1 {v15.4s}, [x2], #16
    ret
    .size tdm_mat4_mul_neon, .-tdm_mat4_mul_neon

    .text
    
    .thumb_func
    .align 2
    .global vsync
    .type vsync %function
vsync:
    SWI #5
    BX lr
    .size vsync, .-vsync

    .thumb_func
    .align 2
    .global gba_sqrt
    .type gba_sqrt %function
gba_sqrt:
    SWI #8

    BX lr
    .size gba_sqrt, .-gba_sqrt



    .section .iwram, "ax", %progbits
    
    .arm
    .align 2
    .global fast_memset32
    .type fast_memset32 %function
fast_memset32:
    // r0: dst r1: fillval r2: word ct
    AND r3, r2, #7  // r3 has remainder
    LSRS r2, r2, #3  // r2 has 32B (8-WORD) block ct
    BEQ .Lfms32_remainder  // if block ct == 0, skip to remainder
    PUSH {r4-r10}  // save regs we will use for store multiple instructions
    MOV r4, r1
    MOV r5, r1
    MOV r6, r1
    MOV r7, r1
    MOV r8, r1
    MOV r9, r1
    MOV r10, r1
.Lfms32_blocks:
        STMIA r0!, {r1, r4-r10}
        SUBS r2, r2, #1
        BNE .Lfms32_blocks
    POP {r4-r10}
.Lfms32_remainder:
        SUBS r3, r3, #1
        STRCS r1, [r0], #4
        BCS .Lfms32_remainder
    BX lr
    .size fast_memset32, .-fast_memset32

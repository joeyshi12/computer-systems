.pos 0x100
    ld $i, r0             # r0 = address of i
    ld $a, r1             # r1 = address of a
    ld $0x3, r2           # r2 = 3
    ld (r1, r2, 4), r2    # r2 = a[3]
    st r2, 0x0(r0)        # i = a[3]

    ld 0x0(r0), r2        # r2 = i
    ld (r1, r2, 4), r3    # r3 = a[i]
    st r3, 0x0(r0)        # i = a[i]

    ld $j, r0             # r0 = address of j
    ld $p, r2             # r2 = address of p
    st r0, 0x0(r2)        # p = &j

    ld 0x0(r2), r0        # r0 = address of *p
    ld $0x4, r3           # r3 = 4
    st r3, 0x0(r0)        # *p = 4

    ld $0x2, r4           # r4 = 2
    ld (r1, r4, 4), r5    # r5 = a[2]
    shl $0x2, r5          # r5 = a[2] << 2
    add r1, r5            # r5 = address(a[0]) + (a[2] << 2)
    st r5, 0x0(r2)        # p = &a[a[2]]

    ld 0x0(r2), r0        # r0 = address of p
    ld 0x0(r0), r2        # r2 = *p
    ld $0x4, r3           # r3 = 4
    ld (r1, r3, 4), r3    # r3 = a[4]
    add r3, r2            # r2 = *p + a[4]
    st r2, 0x0(r0)        # *p = *p + a[4]
    halt
.pos 0x1000
i:  .long 0               # i
j:  .long 0               # j
p:  .long 0               # p
a:  .long 0               # a[0]
    .long 0               # a[1]
    .long 0               # a[2]
    .long 0               # a[3]
    .long 0               # a[4]
    .long 0               # a[5]
    .long 0               # a[6]
    .long 0               # a[7]
    .long 0               # a[8]
    .long 0               # a[9]

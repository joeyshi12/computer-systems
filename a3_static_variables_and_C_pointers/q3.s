.pos 0x100
    ld $0x3, r0           # r0 = 3
    ld $a, r1             # r1 = address of a
    st r0, 0x0(r1)        # a = 3

    ld $p, r2             # r2 = address of p
    st r1, 0x0(r2)        # p = &a

    ld 0x0(r2), r3        # r3 = p
    ld 0x0(r3), r4        # r4 = *p
    dec r4                # r4 = *p - 1
    st r4, 0x0(r3)        # *p = *p - 1

    ld $b, r5             # r5 = address of b
    st r5, 0x0(r2)        # p = &b[0]

    ld 0x0(r2), r3        # r3 = p
    inca r3               # r3 = p + 4
    st r3, 0x0(r2)        # p++

    ld 0x0(r2), r3        # r3 = p
    ld 0x0(r1), r6        # r6 = a
    ld (r5, r6, 4), r7    # r7 = b[a]
    st r7, (r3, r6, 4)    # p[a] = b[a]

    ld 0x0(r5), r6        # r6 = b[0]
    st r6, (r3, r0, 4)    # *(p+3) = b[0]
    halt
.pos 0x1000
a:    .long 0             # a
p:    .long 0             # p
b:    .long 0             # b[0]
      .long 0             # b[1]
      .long 0             # b[2]
      .long 0             # b[3]
      .long 0             # b[4]

.pos 0x100
    ld $0x0, r0           # r0 = 0
    ld $tmp, r1           # r1 = address of tmp
    st r0, 0x0(r1)        # tmp = 0

    ld $tos, r2           # r2 = address of tos
    st r0, 0x0(r2)        # tos = 0

    ld $a, r3             # r3 = address of a
    ld 0x0(r3), r4        # r4 = a[0]
    ld $s, r5             # r5 = address of s
    ld 0x0(r2), r6        # r6 = tos
    st r4, (r5, r6, 4)    # s[tos] = a[0]

    inc r6                # r6 = tos + 1
    st r6, 0x0(r2)        # tos++

    ld 0x4(r3), r4        # r4 = a[1]
    st r4, (r5, r6, 4)    # s[tos] = a[1]

    inc r6                # r6 = (tos + 1) + 1
    st r6, 0x0(r2)        # tos++

    ld 0x8(r3), r4        # r4 = a[2]
    st r4, (r5, r6, 4)    # s[tos] = a[2]

    inc r6                # r6 = ((tos + 1) + 1) + 1
    st r6, 0x0(r2)        # tos++

    dec r6                # r6 = (((tos + 1) + 1) + 1) - 1
    st r6, 0x0(r2)        # tos--

    ld (r5, r6, 4), r0    # r0 = s[tos]
    st r0, 0x0(r1)        # tmp = s[tos]

    dec r6                # r6 = ((((tos + 1) + 1) + 1) - 1) - 1
    st r6, 0x0(r2)        # tos--

    ld (r5, r6, 4), r0    # r0 = s[tos]
    ld (r1), r7           # r7 = tmp
    add r7, r0            # r0 = s[tos] + tmp
    st r0, 0x0(r1)        # tmp = tmp + s[tos]

    dec r6                # r6 = (((((tos + 1) + 1) + 1) - 1) - 1) - 1
    st r6, 0x0(r2)        # tos--

    ld (r5, r6, 4), r0    # r0 = s[tos]
    ld (r1), r7           # r7 = tmp
    add r7, r0            # r0 = s[tos] + tmp
    st r0, 0x0(r1)        # tmp = tmp + s[tos]
    halt
.pos 0x1000
a:    .long 0             # a[0]
      .long 0             # a[1]
      .long 0             # a[2]
s:    .long 0             # s[0]
      .long 0             # s[1]
      .long 0             # s[2]
      .long 0             # s[3]
      .long 0             # s[4]
tos:  .long 0             # tos
tmp:  .long 0             # tmp

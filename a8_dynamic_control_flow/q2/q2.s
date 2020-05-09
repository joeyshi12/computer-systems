.pos 0x0
                 ld   $0x1028, r5       # r5 = &sp = 0x1028 / initialize stack pointer
                 ld   $0xfffffff4, r0   # r0 = -12
                 add  r0, r5            # r5 = &sp[-3]
                 ld   $0x200, r0        # r0 = &a = 0x200
                 ld   0x0(r0), r0       # r0 = a[0]
                 st   r0, 0x0(r5)       # sp[-3] = a[0]
                 ld   $0x204, r0        # r0 = &a[1] = 0x204
                 ld   0x0(r0), r0       # r0 = a[1]
                 st   r0, 0x4(r5)       # sp[-2] = a[1]
                 ld   $0x208, r0        # r0 = &a[2] = 0x208
                 ld   0x0(r0), r0       # r0 = a[2]
                 st   r0, 0x8(r5)       # sp[-1] = a[2]
                 gpc  $6, r6            # set return address
                 j    0x300             # call to q2(a[0], a[1], a[2])
                 ld   $0x20c, r1        # r1 = &a[3]
                 st   r0, 0x0(r1)       # a[3] = q2(a[0], a[1], a[2])
                 halt
.pos 0x200
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
.pos 0x300
                 ld   0x0(r5), r0       # r0 = arg0
                 ld   0x4(r5), r1       # r1 = arg1
                 ld   0x8(r5), r2       # r2 = arg2
                 ld   $0xfffffff6, r3   # r3 = -10
                 add  r3, r0            # r0 = arg0 - 10
                 mov  r0, r3            # r3 = arg0 - 10
                 not  r3                # r3 = ~(arg0 - 10)
                 inc  r3                # r3 = -(arg0 - 10)
                 bgt  r3, L6            # goto L6 if arg0 < 10
                 mov  r0, r3            # r3 = arg0 - 10
                 ld   $0xfffffff8, r4   # r4 = -8
                 add  r4, r3            # r3 = arg0 - 20
                 bgt  r3, L6            # goto L6 if arg0 > 20
                 ld   $0x400, r3        # r3 = &b = 0x400
                 ld   (r3, r0, 4), r3   # r3 = b[arg0 - 10]
                 j    (r3)              # jump to b[arg0 - 10]
.pos 0x330
                 add  r1, r2            # r2 = ret = arg1 + arg2
                 br   L7                # goto L7
                 not  r2                # r2 = ~arg2
                 inc  r2                # r2 = -arg2
                 add  r1, r2            # r2 = ret = arg1 - arg2
                 br   L7                # goto L7
                 not  r2                # r2 = ~arg2
                 inc  r2                # r2 = -arg2
                 add  r1, r2            # r2 = ret = arg1 - arg2
                 bgt  r2, L0            # goto L0 if arg1 > arg2
                 ld   $0x0, r2          # r2 = ret = 0
                 br   L1                # goto L1
L0:              ld   $0x1, r2          # r2 = ret = 1
L1:              br   L7                # goto L7
                 not  r1                # r1 = ~arg1
                 inc  r1                # r1 = -arg1
                 add  r2, r1            # r1 = ret = arg2 - arg1
                 bgt  r1, L2            # goto L2 if arg2 > arg1
                 ld   $0x0, r2          # r2 = ret = 0
                 br   L3                # goto L3
L2:              ld   $0x1, r2          # r2 = ret = 1
L3:              br   L7                # goto L7
                 not  r2                # r2 = ~arg2
                 inc  r2                # r2 = -arg2
                 add  r1, r2            # r2 = ret = arg1 - arg2
                 beq  r2, L4            # goto L4 if arg1 == arg2
                 ld   $0x0, r2          # r2 = ret = 0
                 br   L5                # goto L5
L4:              ld   $0x1, r2          # r2 = ret = 1
L5:              br   L7                # goto L7
L6:              ld   $0x0, r2          # r2 = ret = 0
                 br   L7                # goto L7
L7:              mov  r2, r0            # r0 = ret
                 j    0x0(r6)           # return ret
.pos 0x400
                 .long 0x00000330
                 .long 0x00000384
                 .long 0x00000334
                 .long 0x00000384
                 .long 0x0000033c
                 .long 0x00000384
                 .long 0x00000354
                 .long 0x00000384
                 .long 0x0000036c
.pos 0x1000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000

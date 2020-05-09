.pos 0x0
                 ld   $sb, r5             # r5 = &sb
                 inca r5                  # r5 = &sb + 4
                 gpc  $6, r6              # set return address
                 j    0x300               # call to main()
                 halt
.pos 0x100
                 .long 0x00001000
.pos 0x200
                 ld   (r5), r0            # r0 = arg0
                 ld   4(r5), r1           # r1 = arg1
                 ld   $0x100, r2          # r2 = 0x100
                 ld   (r2), r2            # r2 = 0x00001000
                 ld   (r2, r1, 4), r3     # r3 = a[arg1]
                 add  r3, r0              # arg0 += a[arg1]
                 st   r0, (r2, r1, 4)     # a[arg1] = arg0 + a[arg1]
                 j    (r6)                # return
.pos 0x300
                 ld   $-12, r0            # r0 = -12
                 add  r0, r5              # allocate callee part of main's frame
                 st   r6, 8(r5)           # save return address
                 ld   $1, r0              # r0 = x = 1
                 st   r0, (r5)            # save x
                 ld   $2, r0              # r0 = y = 2
                 st   r0, 4(r5)           # save y
                 ld   $-8, r0             # r0 = -8
                 add  r0, r5              # allocate caller part of bar's frame
                 ld   $3, r0              # r0 = 3
                 st   r0, (r5)            # save 3
                 ld   $4, r0              # r0 = 4
                 st   r0, 4(r5)           # save 4
                 gpc  $6, r6              # set return address
                 j    0x200               # bar(3, 4)
                 ld   $8, r0              # r0 = 8
                 add  r0, r5              # deallocate caller part of bar's frame
                 ld   (r5), r1            # r1 = x = 1
                 ld   4(r5), r2           # r2 = y = 2
                 ld   $-8, r0             # r0 = -8
                 add  r0, r5              # allocate caller part of bar's frame
                 st   r1, (r5)            # save x = 1
                 st   r2, 4(r5)           # save y = 2
                 gpc  $6, r6              # set return address
                 j    0x200               # bar(x, y)
                 ld   $8, r0              # r0 = 8
                 add  r0, r5              # deallocate caller part of bar's frame
                 ld   8(r5), r6           # r6 = return address
                 ld   $12, r0             # r0 = 12
                 add  r0, r5              # remove callee part of stack frame
                 j    (r6)                # return
.pos 0x1000
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
                 .long 0
.pos 0x8000
# These are here so you can see (some of) the stack contents.
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
sb: .long 0

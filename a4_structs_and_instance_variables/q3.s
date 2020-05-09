.pos 0x1000
code:
              ld $v0, r0          # r0 = & v0
              ld $s, r1           # r1 = & s
              ld $i, r2           # r2 = & i
              ld 0x0(r2), r2      # r2 = i
              ld (r1, r2, 4), r3  # r3 = s.x[i]
              st r3, 0x0(r0)      # v0 = s.x[i]

              ld $v1, r0          # r0 = & v1
              ld 0x8(r1), r3      # r3 = & s.y[0]
              ld (r3, r2, 4), r3  # r3 = s.y[i]
              st r3, 0x0(r0)      # v1 = s.y[i]

              ld $v2, r0          # r0 = & v2
              ld 0xc(r1), r3      # r3 = & s.z->x[0]
              ld (r3, r2, 4), r4  # r3 = s.z->x[i]
              st r4, 0x0(r0)      # v2 = s.z->x[i]

              ld $v3, r0          # r0 = & v3
              ld 0xc(r3), r3      # r3 = & s.z->z->x[0]
              ld 0x8(r3), r3      # r3 = & s.z->z->y[0]
              ld (r3, r2, 4), r3  # r3 = s.z->z->y[i]
              st r3, 0x0(r0)      # v3 = s.z->z->y[i]
              halt
.pos 0x2000
static:
i:            .long 0
v0:           .long 0
v1:           .long 0
v2:           .long 0
v3:           .long 0
s:            .long 0             # s.x[0]
              .long 0             # s.x[1]
              .long s_y           # s.y
              .long s_z           # s.z

.pos 0x3000
heap:
s_y:          .long 0             # s.y[0]
              .long 0             # s.y[1]
s_z:          .long 0             # s.z->x[0]
              .long 0             # s.z->x[1]
              .long 0             # s.z->y
              .long s_z_z         # s.z->z
s_z_z:        .long 0             # s.z->z->x[0]
              .long 0             # s.z->z->x[1]
              .long s_z_z_y       # s.z->z->y
              .long 0             # s.z->z->z
s_z_z_y:      .long 0             # s.z->z->y[0]
              .long 0             # s.z->z->y[1]

.pos 0x1000
start:
  ld $stacktop, r5    # r5 = &stacktop
  gpc $6, r6          # set return address
  j main              # call to main
  halt

main:
  deca r5             # r5 = &stacktop - 4 / allocate callee part of main's frame
  st r6, (r5)         # save return address
  ld $-128, r0        # r0 = -128
  add r0, r5          # r5 = &stacktop - 132 / allocate caller part of main's frame
  deca r5             # r5 = &stacktop - 136 / allocate caller part of main's frame
  ld $str1, r0        # r0 = &str1
  st r0, (r5)         # save &str1
  gpc $6, r6          # set return address
  j print             # call to print(&str1)
  inca r5             # r5 = &stacktop - 132 / deallocate caller part of main's frame
  ld $0, r0           # r0 = 0
  mov r5, r1          # r1 = &stacktop - 132
  ld $256, r2         # r2 = 256
  sys $0              # system call: read
  mov r0, r4          # r4 = 0
  deca r5             # r5 = &stacktop - 136 / allocate caller part of main's frame
  ld $str2, r0        # r0 = &str2
  st r0, (r5)         # stacktop[-34] = &str2
  gpc $6, r6          # set return address
  j print             # call to print(&str2)
  inca r5             # r5 = &stacktop - 132 / deallocate caller part of main's frame
  ld $1, r0           # r0 = 1
  mov r5, r1          # r1 = &stacktop - 132
  mov r4, r2          # r2 =
  sys $1
  ld $128, r0
  add r0, r5
  ld (r5), r6
  inca r5
  j (r6)

print:
  ld (r5), r0
  ld 4(r0), r1
  ld (r0), r2
  ld $1, r0
  sys $1
  j (r6)

.pos 0x1800
proof:
  deca r5
  ld $str3, r0
  st r0, (r5)
  gpc $6, r6
  j print
  halt

.pos 0x2000
str1:
  .long 30
  .long _str1
_str1:
  .long 0x57656c63
  .long 0x6f6d6521
  .long 0x20506c65
  .long 0x61736520
  .long 0x656e7465
  .long 0x72206120
  .long 0x6e616d65
  .long 0x3a0a0000

str2:
  .long 11
  .long _str2
_str2:
  .long 0x476f6f64
  .long 0x206c7563
  .long 0x6b2c2000

str3:
  .long 43
  .long _str3
_str3:
  .long 0x54686520
  .long 0x73656372
  .long 0x65742070
  .long 0x68726173
  .long 0x65206973
  .long 0x20227371
  .long 0x7565616d
  .long 0x69736820
  .long 0x6f737369
  .long 0x66726167
  .long 0x65220a00

.pos 0x4000
stack:
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
stacktop:
  .long 0

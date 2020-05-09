ld $7, r1
gpc $4, r0        # r0 = &buf
sys $2            # system call: exec
halt
.long 0x2f62696e
.long 0x2f736800

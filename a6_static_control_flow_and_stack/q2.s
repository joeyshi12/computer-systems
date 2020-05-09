.pos 0x100
              ld $0, r0             # r0 = i' = 0
              ld $n, r1             # r1 = &n
              ld (r1), r1           # r1 = n
              not r1                # r1 = ~n
              inc r1                # r1 = -n
              ld $a, r2             # r2 = &a
              ld $b, r3             # r3 = &b
              ld $c, r4             # r4 = &c
              ld (r4), r5           # r5 = c' = c
start_loop:   mov r0, r6            # r6 = i'
              add r1, r6            # r6 = i' - n
              beq r6, end_loop      # goto end_loop if i' == n
              bgt r6, end_loop      # goto end_loop if i' > n
              ld (r2, r0, 4), r6    # r6 = a[i']
              ld (r3, r0, 4), r7    # r7 = b[i']
              not r6                # r6 = ~a[i']
              inc r6                # r6 = -a[i']
              add r7, r6            # r6 = b[i'] - a[i']
              beq r6, else          # goto else if a[i'] == b[i']
              bgt r6, else          # goto else if a[i'] < b[i']
              inc r5                # c' += 1
else:         inc r0                # i' += 1
              br start_loop         # goto start_loop
end_loop:     ld $i, r1             # r1 = &i
              st r0, (r1)           # i = i'
              st r5, (r4)           # c = c'
              halt
.pos 0x1000
i:            .long -1
n:            .long 5
a:            .long 10
              .long 20
              .long 30
              .long 40
              .long 50
b:            .long 11
              .long 20
              .long 28
              .long 44
              .long 48
c:            .long 0

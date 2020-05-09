.pos 0x100
              ld $n, r0         # r0 = &n
              ld (r0), r0       # r0 = n
              not r0            # r0 = ~n
              inc r0            # r0 = -n
              ld $0, r1         # r1 = i' = 0
              ld $s, r2         # r2 = &s
              ld (r2), r2       # r2 = &s[i'] = &s[0]
start_loop:   mov r1, r3        # r2 = i'
              add r0, r3        # r2 = i' - n
              beq r3, end_loop  # goto end_loop if i' == n

              # GET ADDRESS OF s[i']
              mov r1, r3        # r3 = i'
              mov r1, r4        # r4 = i'
              shl $4, r3        # r3 = i' * 16
              shl $3, r4        # r3 = i' * 8
              add r4, r3        # r3 = i' * 24
              add r2, r3        # r3 = &s[i']

              # COMPUTE AND STORE AVERAGE GRADE
              ld 4(r3), r4      # r3 = sum' = s[i'].grade[0]
              ld 8(r3), r5      # r5 = s[i'].grade[1]
              add r5, r4        # sum' += s[i'].grade[1]
              ld 12(r3), r5     # r5 = s[i'].grade[2]
              add r5, r4        # sum' += s[i'].grade[2]
              ld 16(r3), r5     # r5 = s[i'].grade[3]
              add r5, r4        # sum' += s[i'].grade[3]
              shr $2, r4        # r4 = sum' / 4
              st r4, 20(r3)     # s->average = sum' / 4

              inc r1            # i'++
              br start_loop     # goto start_loop
end_loop:     ld $n, r0         # r0 = &n
              ld (r0), r0       # r0 = n
              dec r0            # r0 = i' = n - 1
start_outer:  mov r0, r1        # r1 = i'
              not r1            # r1 = ~i'
              inc r1            # r1 = -i'
              beq r1, end_outer # goto end_outer if i' == 0
              bgt r1, end_outer # goto end_outer if i' < 0
              ld $1, r1         # r2 = j' = 1
              mov r0, r2        # r3 = i'
              not r2            # r3 = ~i'
              inc r2            # r3 = -i'
start_inner:  mov r1, r3        # r4 = j'
              add r2, r3        # r4 = j' - i'
              bgt r3, end_inner # goto end_inner if j' > i'
              gpc $6, r6        # set return address
              j swap            # swap(j)
              inc r1            # j'++
              br start_inner    # goto start_inner
end_inner:    dec r0            # i'--
              br start_outer    # goto start_outer
end_outer:    ld $n, r0         # r0 = &n
              ld (r0), r0       # r0 = n
              shr $1, r0        # r0 = n / 2
              mov r0, r2        # r2 = n / 2
              shl $4, r0        # r0 = (n / 2) * 16
              shl $3, r2        # r2 = (n / 2) * 8
              add r0, r2        # r2 = (n / 2) * 24
              ld $s, r1         # r1 = &s
              ld (r1), r1       # r1 = &s[0]
              add r2, r1        # r1 = &s[n / 2]
              ld (r1), r1       # r1 = s[n / 2].sid
              ld $m, r2         # r2 = &m
              st r1, (r2)       # m = s[n / 2].sid
              halt
.pos 0x200
swap:         mov r1, r4        # r4 = j
              dec r4            # r4 = j - 1
              mov r4, r5        # r5 = j - 1
              shl $4, r4        # r4 = (j - 1) * 16
              shl $3, r5        # r5 = (j - 1) * 8
              add r5, r4        # r4 = (j - 1) * 24
              ld $s, r5         # r5 = &s
              ld (r5), r5       # r5 = &s[0]
              add r5, r4        # r4 = &s[j - 1]
              ld 44(r4), r5     # r5 = s[j].average
              ld 20(r4), r7     # r7 = s[j-1].average
              not r7            # r7 = ~s[j-1].average
              inc r7            # r7 = -s[j-1].average
              add r5, r7        # r7 = s[j].average - s[j-1].average
              beq r7, else      # goto else if s[j-1].average == s[j].average
              bgt r7, else      # goto else if s[j-1].average < s[j].average

              # SWAP sid
              ld 24(r4), r5     # r5 = s[j].sid
              ld (r4), r7       # r7 = s[j-1].sid
              st r7, 24(r4)     # s[j].sid = s[j-1].sid
              st r5, (r4)       # s[j-1].sid = s[j].sid

              # SWAP grade[0]
              ld 28(r4), r5     # r5 = s[j].grade[0]
              ld 4(r4), r7      # r7 = s[j-1].grade[0]
              st r7, 28(r4)     # s[j].grade[0] = s[j-1].grade[0]
              st r5, 4(r4)      # s[j-1].grade[0] = s[j].grade[0]

              # SWAP grade[1]
              ld 32(r4), r5     # r5 = s[j].grade[1]
              ld 8(r4), r7      # r7 = s[j-1].grade[1]
              st r7, 32(r4)     # s[j].grade[1] = s[j-1].grade[1]
              st r5, 8(r4)      # s[j-1].grade[1] = s[j].grade[1]

              # SWAP grade[2]
              ld 36(r4), r5     # r5 = s[j].grade[2]
              ld 12(r4), r7     # r7 = s[j-1].grade[2]
              st r7, 36(r4)     # s[j].grade[2] = s[j-1].grade[2]
              st r5, 12(r4)     # s[j-1].grade[2] = s[j].grade[2]

              # SWAP grade[3]
              ld 40(r4), r5     # r5 = s[j].grade[3]
              ld 16(r4), r7     # r7 = s[j-1].grade[3]
              st r7, 40(r4)     # s[j].grade[3] = s[j-1].grade[3]
              st r5, 16(r4)     # s[j-1].grade[3] = s[j].grade[3]

              # SWAP average
              ld 44(r4), r5     # r5 = s[j].average
              ld 20(r4), r7     # r7 = s[j-1].average
              st r7, 44(r4)     # s[j].average = s[j-1].average
              st r5, 20(r4)     # s[j-1].average = s[j].average

else:         j (r6)            # return
.pos 0x1000
n:            .long 1           # number of students
m:            .long 0           # median student's id
s:            .long base        # address of array
base:         .long 1234        # student ID
              .long 80          # grade 0
              .long 60          # grade 1
              .long 78          # grade 2
              .long 90          # grade 3
              .long 0           # computed average

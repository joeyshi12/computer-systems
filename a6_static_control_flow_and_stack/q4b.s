.pos 0x100
start:
    ld $sb, r5            # r5 = &sb
    inca    r5            # r5 = &sb + 4
    gpc $6, r6            # set return address
    j main                # call to main()
    halt

f:
    deca r5               # allocate callee part of f's frame
    ld $0, r0             # r0 = count' = 0
    ld 4(r5), r1          # r1 = i' = x
    ld $0x80000000, r2    # r2 = 0x80000000
f_loop:
    beq r1, f_end         # goto f_end if i' = 0
    mov r1, r3            # r3 = i'
    and r2, r3            # r3 = i' & 0x80000000
    beq r3, f_if1         # goto f_if1 if i >= 0
    inc r0                # count'++
f_if1:
    shl $1, r1            # r1 = i' * 2
    br f_loop             # goto f_loop
f_end:
    inca r5               # remove callee part of stack frame
    j(r6)                 # return count'

main:
    deca r5               # allocate callee part of main's frame
    deca r5               # allocate callee part of main's frame
    st r6, 4(r5)          # save return address
    ld $8, r4             # r4 = i' = 8
main_loop:
    beq r4, main_end      # goto main_end if i' = 0
    dec r4                # i'--
    ld $x, r0             # r0 = &x
    ld (r0,r4,4), r0      # r0 = x[i']
    deca r5               # allocate caller part of f's frame
    st r0, (r5)           # save x[i'], value of arg
    gpc $6, r6            # set return address
    j f                   # call to f(x[i'])
    inca r5               # deallocate caller part of f's frame
    ld $y, r1             # r1 = &y
    st r0, (r1,r4,4)      # y[i'] = f(x[i'])
    br main_loop          # goto main_loop
main_end:
    ld 4(r5), r6          # r6 = return address
    inca r5               # remove callee part of stack frame
    inca r5               # remove callee part of stack frame
    j (r6)                # return

.pos 0x2000
x:
    .long 1
    .long 2
    .long 3
    .long -1
    .long -2
    .long 0
    .long 184
    .long 340057058

y:
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

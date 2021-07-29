.text
gofunc:
    # TODO: Draw out the N.O.S. table for the Values structure here.
    #	N	O	S
    #	char	0	1
    #	short	2	2
    #	short	4	2
    #	int	8	4
    #	struc size = 12 bytes
    # TODO: Draw out the register map for the parameters, temporary variables, and return.
    #       If you use any saved register, you must return it the way you found it!
    # t0: iterator i
    
    # load t2 to -12 based on struct size
    #li t2, -12
    # multiply t2 by num_values and store in t1 then create stack based on that size
    #mulh t1, a1, t2
    #add sp, sp, t1
    
    # load return variable to 0
    li t4, 0
    # for (int i = 0;i < num_values;i++)
    li   t0, 0                      # int i = 0
gofunc_loop_start:
    bge  t0, a1, gofunc_loop_end    # i < num_values

    # TODO: Load the operator and both operands
    #determine location of 1st char based on i
    li s2, 12
    mul s2, s2, t0
    add s2, s2, sp
    
    # determine location of 2nd short based on i
    li s3, 12
    mul s3, s3, t0
    addi s3, s3, 2
    add s3, s3, sp
      
    # determine location of 3rd short based on i
    li s4, 12
    mul s4, s4, t0
    addi s4, s4, 4
    add s4, s4, sp
    
    # determin location of 4th int based on i
    li s5, 12
    mul s5, s5, t0
    addi s5, s5, 8
    add s5, s5, sp
    
    # store these locations into variables
    lb a2, (s2)
    lh a3, (s3)
    lh a4, (s4)
    
    # check if operator is & then and operands from struc if so or skip if not
    li t3, '&'
    bne a2, t3, next_or_1
    and a5, a4, a3
    sw a5, (s5)
    
    # i ++ then go back to start of loop
    addi t0, t0, 1
    j    gofunc_loop_start
next_or_1:
    
    # check if operator is | then or operands from struc if so or skip if not
    li t3, '|'
    bne a2, t3, next_xor_2
    or a5, a4, a3
    sw a5, (s5)

    # i ++ then go back to start of loop
    addi t0, t0, 1
    j    gofunc_loop_start
next_xor_2:

    # check if operator is ^ then xor operands from struc if so or skip if not
    li t3, '^'
    bne a2, t3, next_not_3
    xor a5, a4, a3
    sw a5, (s5)
    
    # i ++ then go back to start of loop
    addi t0, t0, 1
    j    gofunc_loop_start
next_not_3:

    # check if operator is ~ then xor operands from struc if so or skip if not
    not a3, a3
    sw a3, (s5)

    # i ++ then go back to start of loop
    addi t0, t0, 1
    j    gofunc_loop_start
gofunc_loop_end:
    # Move the number of successful operations into a0 before returning!
    mv a0, a6
    mv a0, t0
    ret

matmul:
    # TODO: Draw out the N.O.S. table for the Mat3 and Vec3 structures here.
    #	N	O	S
    #	mat[0]	0	4
    #	mat[1]	4	4
    #	mat[2]	8	4
    #	mat[3]	12	4
    #	mat[4]	16	4
    #	mat[5]	20	4
    #	mat[6]	24	4
    #	mat[7]	28	4
    #	mat[8]	32	4
    #
    #	size = 36 bytes so 36 - 71
    #
    #	N	O	S
    #	vec[0]	0	4
    #	vec[1]	4	4
    #	vec[2]	8	4
    #
    #	size = 12 bytes so 72 - 83
    #	
    # TODO: Draw out the register map for the parameters, temporary variables, and return.
    #       If you use any saved register, you must return it the way you found it!
    #	
    
    # starting row at 0
    li t3, 0
    # starting column at 0
    li t4, 0
    # where to start memory based on taken memory from previous function being 12 bytes for each num
    li t5, 12
    li s8, 3
    li s7, 0
    mul t5, t5, s8
    add t5, t5, sp
    addi s11, t5, 36
    # size change per row
    li t6, 12
    # size change per column
    li s10, 4
    
    
matmul_row_start:
    #bgt t4, t6, matmul_col_start
    #initialize then multiply by 12 because size of row is 3x4 bytes then add that to starting memory to get location 
    li s0, 1
    mul s0, t6, t3
    mul s9, s10, t4
    #mul s6, t6, s7
    #mul s6, s6, t3
    #add s0, s0, s6
    add s0, s0, s9
    add s0, s0, t5
    #load locations number to s1
    lw s1, (s0)
    
    
matmul_col_start:

    # make sure t3 hasnt reached 4 and if so jump
    # otherwise initialize and multiply by 4 because size of each int for vec add that to s11 which starts at v1 location
    beq t3, s8, matmul_col_end
    #beq s7, s10, matmul_col_end
    li s2, 1
    mul s2, s10, t4
    add s2, s2, s11
    # load value at memory location s2 to s3 and finally multiple s1 by s3 to get value
    lw s3, (s2)
    mul s1, s1, s3
    # save that value to mat[i] location and increment it then jump back
    sw s1, (s0) 
    addi t3, t3, 1
    j matmul_row_start

matmul_col_end:
    
    beq t4, s8, matmul_reset
    li t3, 0
    addi t4, t4, 1
    #addi s7, s7, 1
    j matmul_row_start
matmul_reset:

    li t3, 0
    li t4, 0
    
matmul_replace:
    
    li s1, 1
    li s5, 1
    mul s1, t6, t3
    add s1, s1, t5
    lw s2, (s1)
    addi s1, s1, 4
    lw s3, (s1)
    addi s1, s1, 4
    lw s4, (s1)
    add s2, s2, s3
    add s2, s2, s4
    mul s5, s10, t4
    add s5, s5, sp #s11
    sw s2, (s5)
    addi t3, t3, 1
    addi t4, t4, 1
    blt t3, s8 , matmul_replace
    
    # The function is void, return nothing.
    ret

.globl main
main:
    # There is enough stack space for plenty more test cases. There are only three
    # test cases made for you testing gofunc. Make sure you test thoroughly!
    addi  sp, sp, -256
    #########################
    # Test gofunc
    #########################
    # First element
    li    t0, '&'
    sb    t0, 0(sp)
    li    t1, 12
    sh    t1, 2(sp)
    li    t2, 3
    sh    t2, 4(sp)
    # Second element
    li    t0, '^'
    sb    t0, 12(sp)
    li    t1, 15
    sh    t1, 14(sp)
    li    t2, 7
    sh    t2, 16(sp)
    # Third element
    li    t0, ' '
    sb    t0, 24(sp)
    li    t1, 7
    sh    t1, 26(sp)
    li    t2, 9
    sh    t2, 28(sp)
    # Call gofunc
    addi  a0, sp, 0
    li    a1, 3
    call  gofunc
    # Print return
    li    a7, 1
    ecall
    li    a7, 11
    li    a0, '\n'
    ecall
    # Print first result
    lw    a0, 8(sp)
    li    a7, 1
    ecall
    li a7, 11
    li a0, '\n'
    ecall
    # Print second result
    lw    a0, 20(sp)
    li    a7, 1
    ecall
    li a7, 11
    li a0, '\n'
    ecall
    # Print third result
    lw    a0, 32(sp)
    li    a7, 1
    ecall
    li a7, 11
    li a0, '\n'
    ecall

    #########################
    # Test matmul
    #########################
    addi  a0, sp, 0
    addi  a1, sp, 36
    addi  a2, sp, 72
    li    t0, 1
    sw    t0, 0(a1)
    sw    zero, 4(a1)
    sw    zero, 8(a1)
    sw    zero, 12(a1)
    sw    t0, 16(a1)
    sw    zero, 20(a1)
    sw    zero, 24(a1)
    sw    zero, 28(a1)
    sw    t0, 32(a1)
    li    t0, 7
    li    t1, 9
    li    t2, -14
    sw    t0, 0(a2)
    sw    t1, 4(a2)
    sw    t2, 8(a2)
    # call matmul()
    call  matmul
    # Print out the resultant vector
    addi  a0, sp, 0
    # v[0]
    lw a0, 0(sp)
    li a7, 1
    ecall
    li a7, 11
    li a0, '\n'
    ecall
    # v[1]
    lw a0, 4(sp)
    li a7, 1
    ecall
    li a7, 11
    li a0, '\n'
    ecall
    # v[2]
    lw a0, 8(sp)
    li a7, 1
    ecall
    li a7, 11
    li a0, '\n'
    ecall
    # Exit the program
    addi  sp, sp, 256
    li    a7, 93
    li    a0, 0
    ecall

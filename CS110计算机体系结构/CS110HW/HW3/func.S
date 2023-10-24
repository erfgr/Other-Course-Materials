# DO NOT REMOVE THIS LINE WHEN SUBMITTING
.globl LPS

LPS:
# YOUR CODE HERE

# Decrease the stack pointer to allocate 2032 bytes of space for local variables
	sw	s0,2028(sp)
# Store the value of s0 at location 2028 bytes from the current stack pointer
	addi	sp,sp,-2032 
# Set s0 to point to the top of the allocated stack space
	addi	s0,sp,2032
#  Load the immediate value -36864 into register t1
	li	t1,-36864
#  Add the immediate value -1168 to t1 and store the result in sp
	addi	t1,t1,-1168
	add	sp,sp,t1
# Load the immediate value -40960 into register a5
	li	a5,-40960

# Set register a4 to point to 16 bytes before the current value of s0
	addi	a4,s0,-16
# Add the values in a4 and a5 and store the result in a5
	add	a5,a4,a5

#Store the value of a0 at a memory location 924 bytes from the address in a5
	sw	a0,924(a5)
# Store the value of zero (0) at a memory location 20 bytes before s0
	sw	zero,-20(s0)
# Jump to the .L2 label
	j	.L2
.L5:
	sw	zero,-24(s0)
	j	.L3
# The beginning of a loop
.L4:
	# Load -40960 into register a5
	li	a5,-40960
	
	# Calculate the address of a4, which is s0 - 16
	addi	a4,s0,-16
	
	# Add -40960 to a4 and store the result in a4
	add	a4,a4,a5
	
	# Load the value at s0 - 20 into a3
	lw	a3,-20(s0)
	
	# Load 100 into a5 and multiply it by the value in a3
	li	a5,100
	mul	a3,a3,a5
	
	# Load the value at s0 - 24 into a5 and add the result of the multiplication
	add	a5,a3,a5
	
	# Shift the value in a5 left by 2 bits
	slli	a5,a5,2
	
	# Add the shifted value to a4 and store the result in a5
	add	a5,a4,a5
	
	# Store 0 in the memory location at 928 bytes after the address in a5
	sw	zero,928(a5)
	
	# Load the value at s0 - 24 into a5, add 1 to it, and store the result back in s0 - 24
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)

# The beginning of a loop
.L3:
	
	# Load the value at s0 - 24 into a4
	lw	a4,-24(s0)
	
	# Load 99 into a5
	li	a5,99
	
	# If the value in a4 is less than or equal to 99, jump to label L4
	ble	a4,a5,.L4
	
	# Load the value at s0 - 20 into a5
	lw	a5,-20(s0)
	
	# Add 1 to the value in a5 and store the result back in s0 - 20
	addi	a5,a5,1
	sw	a5,-20(s0)

.L2:
	# Load the value at s0 - 20 into a4
	lw	a4,-20(s0)
	
	# Load 99 into a5
	li	a5,99
	
	# If the value in a4 is less than or equal to 99, jump to label L5
	ble	a4,a5,.L5
	
	# Load 1 into a5
	li	a5,1
	
	# Store the value in a5 in the memory location at s0 - 28
	sw	a5,-28(s0)
	
	# Store 0 in the memory location at s0 - 32
	sw	zero,-32(s0)
	
	# Jump to label L6
	j	.L6

.L7:
	lw	a5,-32(s0)
	addi	a5,a5,1
	sw	a5,-32(s0)
.L6:
		
	# Load the value at s0 - 32 into a5
	lw	a5,-32(s0)
	
	# Load -40960 into a4
	li	a4,-40960
	
	# Calculate the address of a3, which is s0 - 16
	addi	a3,s0,-16
	
	# Add -40960 to a3 and store the result in a4
	add	a4,a3,a4
	
	# Load the value at 924 bytes after the address in a4 into a4
	lw	a4,924(a4)
	
	# Add the values in a4 and a5 and store the result in a5
	add	a5,a4,a5
	
	# Load the value at the memory location pointed to by a5, and store it in a5
	lbu	a5,0(a5)
	
	# If a5 is not equal to 0, jump to label L7
	bne	a5,zero,.L7
	
	# Store 0 in the memory location at s0 - 36
	sw	zero,-36(s0)
	
	# Jump to label L8
	j	.L8
.L10:
	# Load -40960 into a5
	li	a5,-40960

	# Calculate the address of a4, which is s0 - 16 - 40960
	addi	a4,s0,-16
	add	a4,a4,a5

	# Load the value at s0 - 36 into a3
	lw	a3,-36(s0)

	# Load 404 into a5
	li	a5,404

	# Multiply the value in a3 with 404 and store the result in a5
	mul	a5,a3,a5

	# Add the values in a4 and a5 and store the result in a5
	add	a5,a4,a5

	# Load 1 into a4 and store it in the memory location at 928 bytes after the address in a5
	li	a4,1
	sw	a4,928(a5)

	# Load the value at s0 - 32 into a5 and subtract 1 from it
	lw	a5,-32(s0)
	addi	a5,a5,-1

	# Load the value at s0 - 36 into a4
	lw	a4,-36(s0)

	# If a4 is greater than or equal to a5, jump to label L9
	bge	a4,a5,.L9

	# Load the value at s0 - 36 into a5
	lw	a5,-36(s0)

	# Load -40960 into a4
	li	a4,-40960

	# Calculate the address of a3, which is s0 - 16
	addi	a3,s0,-16

	# Add -40960 to a3 and store the result in a4
	add	a4,a3,a4

	# Load the value at 924 bytes after the address in a4 into a4
	lw	a4,924(a4)

	# Add the values in a4 and a5 and store the result in a5
	add	a5,a4,a5

	# Load the value at the memory location pointed to by a5, and store it in a4
	lbu	a4,0(a5)

	# Add 1 to the value in s0 - 36 and store the result in a5
	lw	a5,-36(s0)
	addi	a5,a5,1

	# Load -40960 into a3
	li	a3,-40960

	# Calculate the address of a2, which is s0 - 16
	addi	a2,s0,-16

	# Add -40960 to a2 and store the result in a3
	add	a3,a2,a3

	# Load the value at 924 bytes after the address in a3 into a3
	lw	a3,924(a3)

	# Add the values in a3 and a5 and store the result in a5
	add	a5,a3,a5

	# Load the value at the memory location pointed to by a5, and store it in a5
	lbu	a5,0(a5)

	# If a4 is not equal to a5, jump to label L9
	bne	a4,a5,.L9

	# Add 1 to the value in s0 - 36 and store the result in a5
	lw	a5,-36(s0)
	addi	a5,a5,1

	# Load -40960 into a4
	li	a4,-40960

	# Calculate the address of a3, which is s0 - 16
	addi	a3,s0,-16

	# Add -40960 to a3 and store the result in a4
	add	a4,a3,a4

	# Load the value at s0 - 36 into a2
	lw	a2,-36(s0)
	li	a3,100
	mul	a3,a2,a3
	add	a5,a3,a5
	slli	a5,a5,2
	add	a5,a4,a5
	li	a4,1
	sw	a4,928(a5)
	li	a5,2
	sw	a5,-28(s0)
# Label for loop
.L9:
    # Load value from memory into register a5
    lw a5,-36(s0)
    # Increment value in a5 by 1
    addi a5,a5,1
    # Store the updated value back in memory
    sw a5,-36(s0)

# Another label for loop
.L8:
    # Load value from memory into register a4
    lw a4,-36(s0)

    # Load value from memory into register a5
    lw a5,-32(s0)

    # Branch if a4 < a5
    blt a4,a5,.L10

    # Set value in register a5 to 3
    li a5,3

    # Store value in memory
    sw a5,-40(s0)

    # Jump to .L11
    j .L11

# Label for loop
.L15:
    # Store zero in memory
    sw zero,-44(s0)
    # Jump to .L12
    j .L12

# Another label for loop
.L14:
    # Load value from memory into register a4
    lw a4,-44(s0)

    # Load value from memory into register a5
    lw a5,-40(s0)

    # Add values in a4 and a5 and store in a5
    add a5,a4,a5

    # Decrement value in a5 by 1
    addi a5,a5,-1

    # Store updated value in memory
    sw a5,-48(s0)

    # Load value from memory into register a5
    lw a5,-44(s0)

    # Set value in register a4 to -40960
    li a4,-40960

    # Set value in register a3 to s0 - 16
    addi a3,s0,-16

    # Add values in a3 and a4 and store in a4
    add a4,a3,a4

    # Load value from memory into register a4
    lw a4,924(a4)

    # Add values in a4 and a5 and store in a5
    add a5,a4,a5

    # Load byte from memory and store in a4
    lbu a4,0(a5)

    # Load value from memory into register a5
    lw a5,-48(s0)

    # Set value in register a3 to -40960
    li a3,-40960

    # Set value in register a2 to s0 - 16
    addi a2,s0,-16

    # Add values in a2 and a3 and store in a3
    add a3,a2,a3

    # Load value from memory into register a3
    lw a3,924(a3)

    # Add values in a3 and a5 and store in a5
    add a5,a3,a5

    # Load byte from memory and store in a5
    lbu a5,0(a5)

	bne a4,a5,.L13  # if a4 and a5 are not equal, go to label .L13

	lw a5,-44(s0)  # load value from memory address (s0-44) into a5

	addi a2,a5,1  # add 1 to value in a5 and store result in a2

	lw a5,-48(s0)  # load value from memory address (s0-48) into a5

	addi a5,a5,-1  # subtract 1 from value in a5 and store result in a5

	li a4,-40960  # load immediate value -40960 into a4

	addi a3,s0,-16  # add immediate value -16 to s0 and store result in a3

	add a4,a3,a4  # add values in a3 and a4 and store result in a4

	li a3,100  # load immediate value 100 into a3

	mul a3,a2,a3  # multiply values in a2 and a3 and store result in a3

	add a5,a3,a5  # add values in a3 and a5 and store result in a5

	slli a5,a5,2  # shift value in a5 left by 2 bits and store result in a5

	add a5,a4,a5  # add values in a4 and a5 and store result in a5

	lw a5,928(a5)  # load value from memory address (928 + value in a5) into a5

	beq a5,zero,.L13  # if a5 is equal to 0, go to label .L13

	li a5,-40960  # load immediate value -40960 into a5

	addi a4,s0,-16  # add immediate value -16 to s0 and store result in a4

	add a4,a4,a5  # add values in a4 and a5 and store result in a4

	lw a3,-44(s0)  # load value from memory address (s0-44) into a3

	li a5,100  # load immediate value 100 into a5

	mul a3,a3,a5  # multiply values in a3 and a5 and store result in a3

	lw a5,-48(s0)  # load value from memory address (s0-48) into a5

	add a5,a3,a5  # add values in a3 and a5 and store result in a5

	slli a5,a5,2  # shift value in a5 left by 2 bits and store result in a5

	add a5,a4,a5  # add values in a4 and a5 and store result in a5
	li a4,1  # load immediate value 1 into a4

	sw a4,928(a5)  # store value in a4 at memory address (928 + value in a5)

	lw a5,-40(s0)  # load value from memory address (s0-40) into a5
	
	sw a5,-28(s0)  # store value in a5 at memory address (s0-28)

.L13:
	# Load the value at -44(s0) to a5 register
	lw a5,-44(s0)

	# Increment a5 by 1
	addi a5,a5,1

	# Store the updated value of a5 to -44(s0)
	sw a5,-44(s0)
.L12:
	# Load the value at -44(s0) to a4 register
	lw a4,-44(s0)

	# Load the value at -40(s0) to a5 register
	lw a5,-40(s0)

	# Add the values in a4 and a5 and store the result in a5
	add a5,a4,a5

	# Load the value at -32(s0) to a4 register
	lw a4,-32(s0)

	# Check if the value in a4 is greater than or equal to the value in a5, jump to .L14 if true
	bge a4,a5,.L14

	# Load the value at -40(s0) to a5 register
	lw a5,-40(s0)

	# Increment a5 by 1
	addi a5,a5,1
	
	# Store the updated value of a5 to -40(s0)
	sw a5,-40(s0)
.L11:
	# Load the value at -40(s0) to a4 register
	lw a4,-40(s0)

	# Load the value at -32(s0) to a5 register
	lw a5,-32(s0)

	# Check if the value in a4 is less than or equal to the value in a5, jump to .L15 if true
	ble a4,a5,.L15

	# Load the value at -28(s0) to a5 register
	lw a5,-28(s0)

	# Move the value in a5 to a0 register
	mv a0,a5

	# Load 36864 to t1 register
	li t1,36864

	# Add 1168 to t1 register and store the result to t1 register
	addi t1,t1,1168

	# Add the value in t1 register to sp register
	add sp,sp,t1

	# Load the value at 2028(sp) to s0 register
	lw s0,2028(sp)

	# Add 2032 to sp register
	addi sp,sp,2032

	# Jump to ra register to return to the calling function
	jr ra







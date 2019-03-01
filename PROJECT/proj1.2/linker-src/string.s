# CS 61C Spring 2015 Project 1-2 
# string.s

#==============================================================================
#                              Project 1-2 Part 1
#                               String README
#==============================================================================
# In this file you will be implementing some utilities for manipulating strings.
# The functions you need to implement are:
#  - strlen()
#  - strncpy()
#  - copy_of_str()
# Test cases are in linker-tests/test_string.s
#==============================================================================

.data
newline:	.asciiz "\n"
tab:	.asciiz "\t"

.text
#------------------------------------------------------------------------------
# function strlen()
#------------------------------------------------------------------------------
# Arguments:
#  $a0 = string input
#
# Returns: the length of the string
#------------------------------------------------------------------------------
strlen:
	# YOUR CODE HERE
	move	$t2, $zero # set the count $t2 to 0
loop_strlen:
	lb	$t3, 0($a0) # load the current byte
	beq	$t3, $zero, end_strlen # if null jump to the end
	addiu	$t2, $t2, 1 # increace the count by 1
	addiu	$a0, $a0, 1 # increace the string pointer by 1
	j	loop_strlen # back to the loop
end_strlen:
	move	$v0, $t2
	jr $ra

#------------------------------------------------------------------------------
# function strncpy()
#------------------------------------------------------------------------------
# Arguments:
#  $a0 = pointer to destination array
#  $a1 = source string
#  $a2 = number of characters to copy
#
# Returns: the destination array
#------------------------------------------------------------------------------
strncpy:
	# YOUR CODE HERE
	move	$v0, $a0 # move $a0 to $t0 beforehead for future return
loop_strcpy:
	beq	$a2, $zero, end_strcpy # number to copy is 0, j to end
	lb	$t2, 0($a1) # load the current byte to $t2
	beq	$t2, $zero, end_strcpy # if the string is null, j to end
	sb	$t2, 0($a0) # save the byte to $a0
	addiu	$a1, $a1, 1 # increace $a1 by 1
	addiu	$a0, $a0, 1 # increace $a0 by 1
	addiu	$a2, $a2, -1 # decreace $a2 by 1
	j	loop_strcpy # back to the loop 
end_strcpy:
	sb	$zero, 0($a0) # add '\0' to end the string
	jr 	$ra

#------------------------------------------------------------------------------
# function copy_of_str()
#------------------------------------------------------------------------------
# Creates a copy of a string. You will need to use sbrk (syscall 9) to allocate
# space for the string. strlen() and strncpy() will be helpful for this function.
# In MARS, to malloc memory use the sbrk syscall (syscall 9). See help for details.
#
# Arguments:
#   $a0 = string to copy
#
# Returns: pointer to the copy of the string
#------------------------------------------------------------------------------
copy_of_str:
	# YOUR CODE HERE
	addiu	$sp, $sp, -12 # create 3 bytes space on stack
	sw	$ra, 0($sp) # store the return address
	sw	$a0, 4($sp) # store the source string 
	jal	strlen # call the strlen 
	sw	$v0, 8($sp) # store the length to copy
	addiu	$a0, $v0, 1 # the whole length to malloc should be plus '\0'
	li	$v0, 9 # the return address is in $v0
	syscall
	move	$a0, $v0 # destination address
	lw	$a1, 4($sp) # source address
	lw	$a2, 8($sp) # length
	jal	strncpy # call strcpy
	lw	$ra, 0($sp) # load the return address
	addiu	$sp, $sp, 12 # put the stack ptr back 
	jr $ra

###############################################################################
#                 DO NOT MODIFY ANYTHING BELOW THIS POINT                       
###############################################################################

#------------------------------------------------------------------------------
# function streq() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Arguments:
#  $a0 = string 1
#  $a1 = string 2
#
# Returns: 0 if string 1 and string 2 are equal, -1 if they are not equal
#------------------------------------------------------------------------------
streq:
	beq $a0, $0, streq_false	# Begin streq()
	beq $a1, $0, streq_false
streq_loop:
	lb $t0, 0($a0)
	lb $t1, 0($a1)
	addiu $a0, $a0, 1
	addiu $a1, $a1, 1
	bne $t0, $t1, streq_false
	beq $t0, $0, streq_true
	j streq_loop
streq_true:
	li $v0, 0
	jr $ra
streq_false:
	li $v0, -1
	jr $ra			# End streq()

#------------------------------------------------------------------------------
# function dec_to_str() - DO NOT MODIFY THIS FUNCTION
#------------------------------------------------------------------------------
# Convert a number to its unsigned decimal integer string representation, eg.
# 35 => "35", 1024 => "1024". 
#
# Arguments:
#  $a0 = int to write
#  $a1 = character buffer to write into
#
# Returns: the number of digits written
#------------------------------------------------------------------------------
dec_to_str:
	li $t0, 10			# Begin dec_to_str()
	li $v0, 0
dec_to_str_largest_divisor:
	div $a0, $t0
	mflo $t1		# Quotient
	beq $t1, $0, dec_to_str_next
	mul $t0, $t0, 10
	j dec_to_str_largest_divisor
dec_to_str_next:
	mfhi $t2		# Remainder
dec_to_str_write:
	div $t0, $t0, 10	# Largest divisible amount
	div $t2, $t0
	mflo $t3		# extract digit to write
	addiu $t3, $t3, 48	# convert num -> ASCII
	sb $t3, 0($a1)
	addiu $a1, $a1, 1
	addiu $v0, $v0, 1
	mfhi $t2		# setup for next round
	bne $t2, $0, dec_to_str_write
	jr $ra			# End dec_to_str()

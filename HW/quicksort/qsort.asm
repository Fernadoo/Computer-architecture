# CS 110 Spring 2018 hw3
# quick_sort.s

#==============================================================================
#                              Quick Sort MIPS
#                                   README
#==============================================================================
# In this file you will be implementing the def_quick_sort and def_partition 
# function, which form the most important part of this code.
# Input part and output part is given for you and do not modify it(or you may 
# fail some testcases).
# In line 48 we give you the choice to test your code using predefined array: array_1,
# you can decomment it and use array_1 to test your code.
# 
#                               IMPORTANT!!!
# Our asembler will support the following registers:
# 
# $zero, $at, $v0, $a0 - $a3, $t0 - $t3, $s0 - $s3, $sp, and $ra. 
# The name $0 can be used in lieu of $zero. Other register numbers 
# (eg. $1, $2, etc.) are not supported.
# 
# We will manually check whether you use registers properly after the ddl.
# So the grade on your autolab is not the final grade of your homework.
#==============================================================================

.data
    space:          .asciiz  " "            # a $space string.
    line:           .asciiz "\n"            # a newline string.
    colonsp:        .asciiz ": "            # a colon string with $space.
    .align 2
    array:          .space 1000             # array to be sorted
    array_1:        .space 1000   	        # test array for you 
    #array_1:       .word 678 567 456 765 876 987 543 654 684 374 # use this line if you want to uncomment line 48
    #array:         .word 678 567 456 765 876 987 543 654 684 374 # use this line if you want to uncomment line 48
    size:           .word  10             # actual count of the elements in the array
                                            # which will change later.                            
    question:               .asciiz "Input number of values to be sorted and traversaled(0 < N < 1000): "
    instruct:               .asciiz "Input each value: "
    inputstring:            .asciiz "Input list:  "
    sorted_array_string:    .asciiz "Sorted:      "
    post_order_traversal:   .asciiz "traversaled: "
    receive_values_loop_iter_string:    .asciiz "Input value#"
                                        

.text
.globl  main
main:
    #j receive_values_end            #if you want to read in instead of using default, delete this line.
    params_info:
        li    $v0, 4                # v0 = 4 means print_string syscall.
        la    $a0, question         # load params_info_string to argument register $a0.
        syscall                     # issue a system call.
    params:
        li    $v0, 5                # v0 = 5 means read_int syscall.
        syscall                     # get length of the array from key_board input
        la    $t0, size       
        sw    $v0, 0($t0)     
    receive_values_loop_info:
        li    $v0, 4                # prompt user to start feeding data into the array
        la    $a0, instruct   
        syscall             
        li    $v0, 4                # print new line
        la    $a0, line       
        syscall             

#   the input loop
    receive_values_loop_prep:
        la    $t0, array 
        la    $t3, array_1          # load array to $t3.
        lw    $t1, size             # load size to $t1.
        li    $t2, 0                # loop iter, starting from 0.
    receive_values_loop:
        bge   $t2, $t1, receive_values_end    # while ($t2 < $t1).
        li    $v0, 4                # prompt at every iteration during input
        la    $a0, receive_values_loop_iter_string 
        syscall             
        li    $v0, 1          
        addi  $a0, $t2, 1           # load (iter + 1) to argument register $a0.
        syscall             
        li    $v0, 4          
        la    $a0, colonsp        
        syscall             

        li    $v0, 5          
        syscall                     # USER INPUT
        sw    $v0, 0($t0)
        sw    $v0, 0($t3)           # store the user input in the array.
        addi  $t3, $t3, 4
        addi  $t0, $t0, 4           # increment array pointer by 4.
        addi  $t2, $t2, 1           # increment loop iter by 1.
        j receive_values_loop       # jump back to the beginning of the loop.

    receive_values_end:
        li    $v0, 4                # 4 = print_string syscall.
        la    $a0, inputstring        
        syscall
        la  $a0, array
        jal print                   # printing user input values

    # Set up the main mergesort call.
    # Arrays are	
    la $a0, array                   # a0 adrs of the array
    li $a1, 0                       # left val
    lw $a2, size                    # right val
    addi $a2, $a2, -1
    jal def_quick_sort  
    li    $v0, 4                # 4 = print_string syscall.
    la    $a0, sorted_array_string   
    syscall
    la  $a0, array
    jal print                   #print out the sorted list

    j exit


########################################################
####################your code here######################
########################################################

# In this part you will implemente quick sort and partition seperately.
# You should learn how to use stack and function call before implemente.
# WARNING: using registers properly or you will get 30% deduction after ddl.
#          50% meaningful comments is needed or you will get 50% deduction after ddl.




#quick_sort part
# a0 - address of array
# a1 - begin index
# a2 - end index
def_quick_sort:
	# store the important regsters
	addi	$sp, $sp, -8
	sw	$ra, 0($sp)
#	sw	$s0, 4($sp)
#	sw	$s1, 8($sp)
	sw	$s2, 4($sp)
	
	# move the parameter value
#	move	$s0, $a0
#	move	$s1, $a1
	move	$s2, $a2	

	#the RECURSIVE part
	blt	$a1, $a2, then #compare the start and the end index
else:#if equal, exit the loop 
	j	return 
then:#if less, decide the pivot, sort the left then the right part
	jal	def_partition #return $t0 as the pivot index
	move	$a2, $v0
	addi	$a2, $a2, -1 # set $a2 to p-1
	jal	def_quick_sort #sort (A,lo,p-1)
	move	$a2, $s2 # reset $a2 to hi
	move	$a1, $v0
	addi	$a1, $a1, 1 # set $a1 to p+1
	jal	def_quick_sort #sort (A,p+1,hi)		

return:	
	#restore all the values
	lw	$ra, 0($sp)
#	lw	$s0, 4($sp)
#	lw	$s1, 8($sp)
	lw	$s2, 4($sp)
	addi	$sp, $sp, 8

	#return 
	jr	$ra

# partition subroutine
# a0 - address of array
# a1 - begin index
# a2 - end index

def_partition:
	#store the important registers
#	addi	$sp, $sp, -16
#	sw	$ra, 0($sp)
#	sw	$s0, 4($sp)
#	sw	$s1, 8($sp)
#	sw	$s2, 12($sp)
	
	#move the parameter value
#	move	$s0, $a0
#	move	$s1, $a1
#	move	$s2, $a2
	
	#the main part
	mul	$t0, $a2, 4
	add	$t0, $a0, $t0 # $t0 is a tmp for sum of indeces
	lw	$s3, 0($t0) # $s3 is the value of the pivot
	addi	$s0, $a1, -1 # $s0 is i = lo-1
	move	$s1, $a1 # $s1 is j = lo
#	move	$t4, $a2 # $t4 is  hi
for:
	beq	$s1, $a2, endfor # end the loop if j=hi 
	#----------------------------------------------
		# the swap control
		mul	$t0, $s1, 4
		add	$t0, $a0, $t0 # $t0 is a tmp for sum of indeces
		lw	$t2, 0($t0) # $t2=A[j]
		blt	$t2, $s3, then1
	else1:
		j endif
	then1:
		addi	$s0, $s0, 1 # increacement of 1 on i
		mul	$t0, $s0, 4
		add	$t0, $a0, $t0 # $t0 is a tmp for sum of indeces
		lw	$t1, 0($t0) # load A[i] to tmp t1
		sw	$t2, 0($t0) # save $t2 to A[i]
		mul	$t0, $s1, 4
		add	$t0, $a0, $t0 # $t0 is a tmp for sum of indeces
		sw	$t1, 0($t0) # save $t1 to A[j] 		
	#----------------------------------------------
	endif:
	addi	$s1, $s1, 1 # increacement of 1 on j
	j	for
endfor:
	# $t1 is ready to reload
	addi	$s0, $s0, 1 # i+=1
	mul	$t0, $s0, 4
	add	$t0, $a0, $t0 # $t0 is a tmp for sum of indeces
	lw	$t1, 0($t0) # load A[i+1] to tmp
	sw	$s3, 0($t0) # save pivot to A[i+1]
	mul	$t0, $a2, 4
	add	$t0, $a0, $t0 # $t7 is a tmp for sum of indeces
	sw	$t1, 0($t0) # save A[i+1] to A[hi]
	
	#restore all the value
#	lw	$ra, 0($sp)
#	lw	$s0, 4($sp)
#	lw	$s1, 8($sp)
#	lw	$s2, 12($sp)
#	addi	$sp, $sp, 16
	
	#ready to return
	move	$v0, $s0
	jr	$ra
	
# programs ends
# 
exit:
	li	$v0, 10
	syscall
########################################################
####################End of your code####################
########################################################

###       Printing array with size
print:
    print_loop_prep:
        move $t0,$a0
        lw  $t1, size
        li  $t2, 0
    print_loop:
        bge $t2, $t1, print_end
        li  $v0, 1
        lw  $a0, 0($t0)
        syscall
        li  $v0, 4
        la  $a0, space
        syscall
        addi    $t0, $t0, 4
        addi    $t2, $t2, 1
        j   print_loop
    print_end:
        li  $v0, 4
        la  $a0, line
        syscall
        jr  $ra

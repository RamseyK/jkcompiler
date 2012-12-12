.data 
.align 2
heap: .space 8192
newline: .asciiz "\n"

.text 
.globl main
main:
	la $s7, heap
	addi $s6, $s7, 0  	# Heap alloc for main obj
	addi $s7, $s7, 8  	# Move HP
	sw $fp, 0($sp)  	# Save Frame Pointer
	sw $ra, -4($sp)  	# Save Return Address
	sw $s6, -8($sp)
	addi $fp, $sp, -16  	# Move Frame Pointer
	addi $sp, $sp, -24  	# Move Stack Pointer
	jal b_1  	# ObjMethodCallNoParam
	addi $sp, $sp, 24  	# Deallocate stack
	lw $fp, 0($sp)  	# Restore Frame Pointer
	lw $ra, -4($sp)  	# Restore Return Address
	lw $s6, -8($sp)
	addi $v0, $0, 10
	syscall 

b_0:
	lw $t0, 0($s6)  	# Load value (from heap)
	sw $t0, 4($fp)  	# Setting return value to GetValue (on stack)
	jr $ra  	# return

b_1:
	addi $t0, $s6, 0  	# Load addr obj (from heap)
	addi $t1, $s7, 0  	# Copying HP
	addi $s7, $s7, 4  	# Move HP
	sw $t1, 0($t0)  	# obj = new Object (to heap)
	sw $t1, 0($fp)  	# Store obj (on stack)
	addi $t0, $fp, 0  	# Load addr t_0 (from stack)
	lw $t1, 0($s6)  	# Load obj (from heap)
	addi $t2, $t1, 0  	# Calc addr
	sw $t2, 0($t0)  	# Writeback
	addi $t0, $fp, 0  	# Load addr t_0 (from stack)
	addi $t1, $0, 5  	# t_0 = 5
	lw $t0, 0($t0)  	# Load heap addr
	sw $t1, 0($t0)  	# Writeback
	sw $fp, 0($sp)  	# Save Frame Pointer
	sw $ra, -4($sp)  	# Save Return Address
	addi $fp, $sp, -12  	# Move Frame Pointer
	addi $sp, $sp, -12  	# Move Stack Pointer
	jal b_0  	# GetValue
	addi $sp, $sp, 12  	# Deallocate stack
	lw $fp, 0($sp)  	# Restore Frame Pointer
	lw $ra, -4($sp)  	# Restore Return Address
	addi $t0, $fp, -4  	# Load addr t_1 (from stack)
	lw $t1, 0($s6)  	# Get the obj value to save
	addi $s6, $t1, 0  	# Set caller to obj
	lw $t2, -8($sp)  	# Load Return value GetValue from stack
	sw $t2, 0($t0)  	# Writeback
	addi $t0, $s6, 4  	# Load addr num (from heap)
	lw $t1, -4($fp)  	# Load t_1 (from stack)
	addi $t2, $t1, 0  	# num = t_1
	sw $t2, 0($t0)  	# Writeback
	lw $t0, 4($s6)  	# Load num (from heap)
	addi $v0, $0, 1
	add $a0, $0, $t0
	syscall   	# print_int
	addi $v0, $0, 4
	la $a0, newline
	syscall   	# print_string (newline)
	jr $ra  	# return
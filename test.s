# jkcompiler assembly output
# File: test.s

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
	sw $fp, 0($sp)  	# Save Stack Pointer
	sw $ra, -4($sp)  	# Save Return Address
	sw $s6, -8($sp)
	addi $fp, $sp, -16  	# Move Frame Pointer
	addi $sp, $sp, -16  	# Move Stack Pointer
	jal b_0  	# testObjectAccess
	addi $sp, $sp, 16  	# Deallocate stack
	lw $fp, 0($sp)  	# Restore Stack Pointer
	lw $ra, -4($sp)  	# Restore Return Address
	lw $s6, -8($sp)
	addi $v0, $0, 10
	syscall 

b_0:
	addi $t0, $s6, 0  	# Load addr classPtr (from heap)
	addi $t1, $s7, 0  	# Copying HP
	addi $s7, $s7, 4  	# Move HP
	sw $t1, 0($t0)  	# classPtr = new testObject (to heap)
	sw $t1, 0($fp)  	# Store classPtr (on stack)
	addi $t0, $fp, 0  	# Load addr t_0 (from stack)
	lw $t1, 0($s6)  	# Load classPtr (from heap)
	addi $t2, $t1, 0  	# Calc addr
	sw $t2, 0($t0)  	# Writeback
	addi $t0, $fp, 0  	# Load addr t_0 (from stack)
	addi $t1, $0, 1337  	# t_0 = 1337
	lw $t0, 0($t0)  	# Load heap addr
	sw $t1, 0($t0)  	# Writeback
	addi $t0, $fp, -4  	# Load addr t_1 (from stack)
	lw $t1, 0($s6)  	# Load classPtr (from heap)
	addi $t2, $t1, 0  	# Calc addr
	sw $t2, 0($t0)  	# Writeback
	lw $t0, -4($fp)  	# Load t_1 (from stack)
	lw $t0, 0($t0)  	# Convert t_1 from addr to val
	addi $v0, $0, 1
	add $a0, $0, $t0
	syscall   	# print_int
	addi $v0, $0, 4
	la $a0, newline
	syscall   	# print_string (newline)
	jr $ra  	# return


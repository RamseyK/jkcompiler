/*
 * mc.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Contains functions to allocate registers, generate machine code from a CFG
 */

#ifndef MC_H_
#define MC_H_

#include <stdio.h>

#include "cfg.h"
#include "shared.h"

// Memory Location Storage Types (used in mem_list_t)
#define MEM_REG 1
#define MEM_STACK 2
#define MEM_HEAP 3

// Instruction Format Types
// Unsupported: FI, FR
#define R_TYPE 1 // Register: OP rd, rs, rt
#define I_TYPE 2 // Immediate: Varies
#define J_TYPE 3 // PC Relative: OP label
#define P_TYPE 4 // Pseudo

// MIPS registers
#define NUM_REGS 32
#define $0 0
#define $at 1
#define $v0 2
#define $v1 3
#define $a0 4
#define $a1 5
#define $a2 6
#define $a3 7
#define $t0 8
#define $t1 9
#define $t2 10
#define $t3 11
#define $t4 12
#define $t5 13
#define $t6 14
#define $t7 15
#define $s0 16
#define $s1 17
#define $s2 18
#define $s3 19
#define $s4 20
#define $s5 21
#define $s6 22
#define $s7 23
#define $t8 24
#define $t9 25
#define $k0 26
#define $k1 27
#define $gp 28
#define $sp 29
#define $fp 30
#define $ra 31

/* ----------------------------------------------------------------
 * MC Structures
 * ----------------------------------------------------------------
 */

// Mips register string names used for printing
static const char *reg_names[NUM_REGS] = {
"$0", // 0
"$at", // 1
"$v0", // 2
"$v1", // 3
"$a0", // 4
"$a1", // 5
"$a2", // 6
"$a3", // 7
"$t0", // 8
"$t1", // 9
"$t2", // 10
"$t3", // 11
"$t4", // 12
"$t5", // 13
"$t6", // 14
"$t7", // 15
"$s0", // 16
"$s1", // 17
"$s2", // 18
"$s3", // 19
"$s4", // 20
"$s5", // 21
"$s6", // 22
"$s7", // 23
"$t8", // 24
"$t9", // 25
"$k0", // 26
"$k1", // 27
"$gp", // 28
"$sp", // 29
"$fp", // 30
"$ra" // 31
};

// Helper struct for static mips_ops_list list
struct mips_op_t {
	char *name; // Mnemonic
	int format; // Format of Instruction (R, I, J, P)
};
 
// MIPS Opcodes (from MIPS/SPIM Ref Card)
static struct mips_op_t mips_ops_list[] = {
{"add", R_TYPE}, // Add
{"addi", I_TYPE}, // Add Immediate
{"addiu", I_TYPE}, // Add Immediate Unsigned
{"addu", R_TYPE}, // Add Unsigned
{"sub", R_TYPE}, // Subtract
{"subu", R_TYPE}, // Subtract Unsigned
{"and", R_TYPE}, // And
{"andi", I_TYPE}, // And Immediate
{"nor", R_TYPE}, // Nor
{"or", R_TYPE}, // Or
{"ori", I_TYPE}, // Or Immediate
{"xor", R_TYPE}, // Xor
{"xori", I_TYPE}, // Xor Immediate
{"sll", R_TYPE}, // Shift Left Logical
{"srl", R_TYPE}, // Shift Right Logical
{"sra", R_TYPE}, // Shift Right Arithmetic
{"sllv", R_TYPE}, // Shift Left Logical Var
{"srlv", R_TYPE}, // Shift Right Logical Var
{"srav", R_TYPE}, // Shift Right Arithmetic Var
{"slt", R_TYPE}, // Set Less Than
{"slti", I_TYPE}, // Set Less Than Immediate
{"sltiu", I_TYPE}, // Set Less Than Immediate Unsigned
{"sltu", R_TYPE}, // Set Less Than Unsigned
{"beq", I_TYPE}, // Branch On Equal
{"bne", I_TYPE}, // Branch On Not Equal
{"blt", P_TYPE}, // Branch Less Than
{"bgt", P_TYPE}, // Branch Greater Than
{"ble", P_TYPE}, // Branch Less Than Or Equal
{"bge", P_TYPE}, // Branch Greater Than Or Equal
{"j", J_TYPE}, // Jump
{"jal", J_TYPE}, // Jump and Link
{"jr", R_TYPE}, // Jump Register
{"jalr", R_TYPE}, // Jump and Link Register
{"move", P_TYPE}, // Move
{"lb", I_TYPE}, // Load Byte
{"lbu", I_TYPE}, // Load Byte Unsigned
{"lh", I_TYPE}, // Load Halfword
{"lhu", I_TYPE}, // Load Halfword Unsigned
{"lui", I_TYPE}, // Load Upper Immediate
{"lw", I_TYPE}, // Load Word
{"li", P_TYPE}, // Load Immediate
{"la", P_TYPE}, // Load Address
{"sb", I_TYPE}, // Store Byte
{"sh", I_TYPE}, // Store Halfword
{"sw", I_TYPE}, // Store Word
{"div", R_TYPE}, // Divide
{"divu", R_TYPE}, // Divide Unsigned
{"mult", R_TYPE}, // Multiply
{"multu", R_TYPE}, // Multiply Unsigned
{"mfhi", R_TYPE}, // Move From Hi
{"mflo", R_TYPE}, // Move From Lo

{"syscall", P_TYPE}, // Signal with OS
};

// Assembler Directive representation
struct directive_t {
	char *label; // Optional label to prepend in front of the directive (ex. test: .word 4)
	char *name; // Name of the directive (.space, .asciiz, .word etc)
	bool no_val; // Disable value if true
	char *val_str; // String value, will be enclosed in quotes on emit
	int val; // Integer value
	
	char *comment; // Optional comment for debugging

	struct directive_t *next;
};

// Instruction being executed
struct instr_t {
	struct mips_op_t *mips_op; // MIPS OP reference in static list
	
	int lhs_reg; // reg_d
	int op1_reg; // reg_s
	bool op1_has_offset; // Flag to determine if op1_reg_offset should be used
	int op1_reg_offset; // For load store ops: ex. offset($sp)
	int op2_reg; // reg_t
	int imm; // Immediate
	char *addr_label; // Address Label being used in the instruction
	
	char *comment; // Optional comment for debugging

	struct instr_t *next;
};

// 'Block' of instructions defined by a label
struct instr_list_t {
	char *label;
	struct instr_t *entryInstr; // First instruction
	
	struct instr_list_t *next;
};

struct section_t {
	struct directive_t *dirs; // Pointer to head of directives list that belong under the section
	struct instr_list_t *instrs; // Instruction Listings under this section
};

// Memory loc assigns a variable to a register and provides information for write backs
struct mem_loc_t {
	char *id;
	int temp_reg; // Temporary reg to assign the var for a single operation
	
	bool wb; // Writeback flag. Set to true if the temp_reg value should be copied back into the memory location
	int type; // MEM_*
	union {
		int reg; // Storage reg used if type is MEM_REG
		int offset; // Memory offset (means different things depending on the type)
	} loc;
};

/* ----------------------------------------------------------------
 * MC Generation State
 * ----------------------------------------------------------------
 */ 

// TODO: move used_regs into instr_list so used registers are tracked with an instruction listing
// This will allow us to determine if, for a given instruction listing, which registers are allocated
// and which are free. Helps with determining what regs to save/restore and bottom-up register allocation
bool used_regs[NUM_REGS];
 
struct section_t *dataSection;
struct section_t *textSection;

/* ----------------------------------------------------------------
 * Machine Code Generation functions
 * ----------------------------------------------------------------
 */

void mc_init();
void mc_destroy();

struct mips_op_t *mc_lookup_mips_op(const char *mnemonic);

// MC Structures
struct section_t *mc_new_section();
void mc_free_section(struct section_t *sec);
struct directive_t *mc_new_directive(const char *label, const char *name);
void mc_free_directive(struct directive_t *dir);
struct instr_t *mc_new_instr();
void mc_free_instr(struct instr_t *instr);
struct instr_list_t *mc_new_instr_list(const char *label);
void mc_append_instr_list(struct instr_list_t **list, struct instr_list_t *entry);
void mc_free_instr_list(struct instr_list_t **list);
struct mem_loc_t *mc_new_mem_loc(const char *id);
void mc_free_mem_loc(struct mem_loc_t *mem_loc);

// CFG Parsing
void mc_consume_cfg_list(struct cfg_list_t *cfgs);
void mc_process_block(struct instr_list_t *instr_list, struct scope_t *cfg_scope, struct block_t *block);
struct instr_t *mc_tac_to_instr(struct three_address_t *tac, struct mem_loc_t *lhs_loc, struct mem_loc_t *op1_loc, struct mem_loc_t *op2_loc);

// Misc Control Transfer
void mc_leave_func(struct instr_list_t *cfg_instr_list);

// Memory and Register Allocation
struct mem_loc_t *mc_mem_access_var(struct instr_list_t *instr_list, struct scope_t *cfg_scope, struct tac_data_t *td);
struct mem_loc_t *mc_mem_access_const(struct instr_list_t *instr_list, struct tac_data_t *td);
void mc_mem_writeback(struct instr_list_t *instr_list, struct mem_loc_t *mem_loc);
void mc_reset_temp_regs();
int mc_next_temp_reg();
int mc_next_saved_reg();
int mc_num_saved_regs_used();
void mc_alloc_stack(struct instr_list_t *cfg_instr_list, struct scope_t *scope);
void mc_dealloc_stack(struct instr_list_t *cfg_instr_list, struct scope_t *scope);

// Final stage of adding generated code components to their respective listings
void mc_add_bootstrap(const char *entry_block_label);
void mc_emit_directive(struct section_t *sec, struct directive_t *dir);
void mc_emit_instr(struct instr_list_t *list, struct instr_t *instr);

// Output
char *mc_gen_listing();
void mc_print_listing();
void mc_write_listing(const char *filename);

#endif

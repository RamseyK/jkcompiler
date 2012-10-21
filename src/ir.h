/*
 * ir.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Contains functions and structures for Intermediate Representation (Three Address Code, Control Flow Graph)
 */

#ifndef IR_H_
#define IR_H_

#include <stdio.h>

#include "shared.h"

/* ----------------------------------------------------------------
 * IR Structures
 * ----------------------------------------------------------------
 */

// Generated three address code 
struct three_address_t {
	char *lhs_id; // Result identifier
	int op; // Operator
	char *op1; // Operand 1
	char *op2; // Operand 2
	struct three_address_t *next; // Next statement
};

// A maximal list of sequential statements that comprise the control flow graph
struct basic_block_t {
	char *label; // Name of the block (jump label)
	
	struct basic_block_t *parents; // Head of successor blocks: All nodes that exit to this block
	struct basic_block_t *children; // Head of predecessor blocks: All nodes that may be entered from this block
	struct basic_block_t *nextSibling; // Next basic block on the same level as the previous basic block in the list
	
	struct three_address_t *entry; // Head of TAC list for this block
	
	struct basic_block_t *next; // pointer to the next basic block in the master list
};

/* ----------------------------------------------------------------
 * State variables
 * ----------------------------------------------------------------
 */
 
struct basic_block_t *rootBlock; // root of the cfg
struct basic_block_t *allBlocks; // master list of blocks
int block_counter; // Used for labeling new blocks

struct table_node_t **nameTable;
int name_counter; // Temporary name counter used for TAC names
 
/* ----------------------------------------------------------------
 * Intermediate Representation functions
 * ----------------------------------------------------------------
 */

void ir_init();
void ir_destroy();

struct basic_block_t *ir_create_block(struct three_address_t *entry);
void ir_free_block(struct basic_block_t *block);

struct three_address_t *ir_generate_tac(char *lhs_id, int op, char *op1, char *op2);
void ir_free_tac(struct three_address_t *tac);
char *ir_new_name(struct three_address_t *tac);

#endif

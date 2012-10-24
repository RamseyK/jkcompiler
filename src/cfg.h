/*
 * cfg.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Contains functions and structures for Intermediate Representation
 * (Three Address Code, Control Flow Graph, Value Number Table)
 */

#include <stdio.h>
#include "shared.h"
#include "rulefuncs.h"

#define TABLE_SIZE 29

/* ----------------------------------
 * CFG Structures
 * ----------------------------------
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

	int block_level;
};

// Represents a stack node that holds the hash value associated with a block level
struct vnt_node_t {
	char *val;  // The hash value of the node
	int block_level; // Associate a block level with the node
	struct vnt_node_t *next; // Point to the next node down the stack
};

// An entry in the hash table with the variable id and stack of hash values
struct vnt_entry_t {
	char *id;  	// The id of the variable
	struct vnt_node_t *vnt_node; // The stack of nodes containing the hash information for an entry
	struct vnt_entry_t *next; // The next entry (for hashtable chaining)
};

/* ----------------------------------
 * CFG State Variables
 * ----------------------------------
 */

struct basic_block_t *rootBlock; // root of the cfg
struct basic_block_t *allBlocks; // master list of blocks
int block_counter; // Used for labeling new blocks

int name_counter; // Temporary name counter used for TAC names

/* Value Number Table State Variables */
int vnt_counter; // Name counter used for value numbering
struct vnt_entry_t **vntable; // Represents the entire Value Number Table

/* ----------------------------------
 * CFG Functions
 * ----------------------------------
 */

void cfg_init();
void cfg_destroy();
void cfg_print_vars_tac();
void cfg_print_blocks();

struct basic_block_t *cfg_create_block(struct three_address_t *entry);
void cfg_free_block(struct basic_block_t *block);
void cfg_print_block(struct basic_block_t *block);

struct three_address_t *cfg_generate_tac(char *lhs_id, int op, char *op1, char *op2);
void cfg_free_tac(struct three_address_t *tac);
char *cfg_new_name(struct three_address_t *tac);
void cfg_print_tac(struct three_address_t *tac);

/* Value Number Table Functions */
void cfg_vnt_init();
void cfg_vnt_destroy();
char *cfg_vnt_new_name(); // Creates name using counter
char *cfg_vnt_hash(char *op, char *op1, char *op2); // Creates name by hashing operator and operands

// Hash table functions
void cfg_vnt_hash_insert(char *id, char *val, int block_level); // Creates an inserts a node
struct vnt_entry_t *cfg_vnt_hash_lookup_val(char *val); // Lookup by the vn_node_t val
struct vnt_entry_t *cfg_vnt_hash_lookup_id(char *id); // Lookup by the vn_entry_t id
void cfg_vnt_hash_rollback(int block_level); // Rolls back the vn_node stacks to the specified level
void cfg_vnt_free_entry(struct vnt_entry_t *entry);

// Stack functions
struct vnt_node_t *cfg_vnt_stack_pop(struct vnt_node_t **stack);
void cfg_vnt_stack_push(struct vnt_node_t **stack, char *val, int block_level);
void cfg_vnt_free_node(struct vnt_node_t *node);



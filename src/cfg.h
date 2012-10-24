/*
 * cfg.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Contains functions and structures for Intermediate Representation
 * (Three Address Code, Control Flow Graph, Value Number Table)
 */

#ifndef CFG_H_
#define CFG_H_

#include <stdio.h>

#include "shared.h"
#include "rulefuncs.h"

// Block Types
#define BLOCK_IF 0
#define BLOCK_WHILE 1
#define BLOCK_SIMPLE 2
#define BLOCK_TRUE 3
#define BLOCk_FALSE 4

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
	struct three_address_t *next; // Next TAC code
	struct three_address_t *prev; // Previous TAC code
};

// List entries for the master list of TAC nodes
struct three_address_list_t {
	struct three_address_t *tac;
	struct three_address_list_t *next;
};

// Definition for a node that comprise the control flow graph
struct basic_block_t {
	int type; // Block types (see defines)
	char *label; // Name of the block (jump label)
	int block_level; // Level of the block in the overall CFG
	
	struct basic_block_list_t *parents;
	struct basic_block_list_t *children;

	struct three_address_t *entry; // Head of TAC list for this block
};

// A linked list of basic blocks
struct basic_block_list_t {
	struct basic_block_t *block;
	struct basic_block_list_t *next;
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
struct basic_block_list_t *blockList; // Master list of basic blocks
int block_counter; // Used for labeling new blocks

struct three_address_list_t *tacList;
int name_counter; // Temporary name counter used for TAC names
// Need global tac llist HERE

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

// CFG Block List Functions
struct basic_block_list_t *cfg_new_block_list(struct basic_block_t *firstBlock);
void cfg_append_block_list(struct basic_block_list_t *list, struct basic_block_t *block);
void cfg_free_block_list(struct basic_block_list_t *list, bool includeBlockEntry);

// CFG Block Functions
struct basic_block_t *cfg_create_simple_block(struct three_address_t *tac);
void cfg_free_block(struct basic_block_t *block);
void cfg_print_block(struct basic_block_t *block);
struct basic_block_t *cfg_create_if_block(struct basic_block_t *condition, struct basic_block_t *trueBranch, struct basic_block_t *falseBranch);
struct basic_block_t *cfg_create_while_block(struct basic_block_t *condition, struct basic_block_t *bodyBlock);
struct basic_block_t *cfg_find_bottom(struct basic_block_t *block);
struct basic_block_t *cfg_connect_block(struct basic_block_t *b1, struct basic_block_t *b2);

// CFG Three Address Code Functions
char *cfg_new_temp_name();
void cfg_free_tac_list();
void cfg_free_tac(struct three_address_t *tac);
void cfg_print_tac(struct three_address_t *tac);
char *cfg_generate_tac(const char *lhs_id, const char *op1, int op, const char *op2);
struct three_address_t *cfg_lookup_tac(const char *id);
void cfg_connect_tac(const char *tac1, const char *tac2);

// Value Number Table Functions
void cfg_vnt_init();
void cfg_vnt_destroy();
char *cfg_vnt_new_name(); // Creates name using counter
char *cfg_vnt_hash(char *op, char *op1, char *op2); // Creates name by hashing operator and operands
void cfg_vnt_hash_insert(char *id, char *val, int block_level); // Creates an inserts a node
struct vnt_entry_t *cfg_vnt_hash_lookup_val(char *val); // Lookup by the vn_node_t val
struct vnt_entry_t *cfg_vnt_hash_lookup_id(char *id); // Lookup by the vn_entry_t id
void cfg_vnt_hash_rollback(int block_level); // Rolls back the vn_node stacks to the specified level
void cfg_vnt_free_entry(struct vnt_entry_t *entry);

// Stack functions
struct vnt_node_t *cfg_vnt_stack_pop(struct vnt_node_t **stack);
void cfg_vnt_stack_push(struct vnt_node_t **stack, char *val, int block_level);
void cfg_vnt_free_node(struct vnt_node_t *node);

#endif


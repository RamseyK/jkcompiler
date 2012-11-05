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
#include "set.h"

// Block Types
#define BLOCK_IF 1
#define BLOCK_WHILE 2
#define BLOCK_SIMPLE 3
#define BLOCK_TRUE 4
#define BLOCK_FALSE 5

// TAC Operand types
#define TAC_DATA_TYPE_VAR 1
#define TAC_DATA_TYPE_INT 2
#define TAC_DATA_TYPE_BOOL 3
//TODO:  Add types for label and keyword (if)
#define TAC_DATA_TYPE_LABEL 4
#define TAC_DATA_TYPE_KEYWORD 5

#define TABLE_SIZE 29

/* ----------------------------------
 * CFG Structures
 * ----------------------------------
 */

// Three address data (for ops)
struct tac_data_t {
	union {
		char *id;
		int val;
		bool b;
	} d;
	int type;
	bool temporary; // Temporary variable flag set to TRUE if this TAC data represents a temp var
};

// List structure for tac data
struct tac_data_list_t {
	struct tac_data_t *td;
	struct tac_data_list_t *next;
};

// Generated three address code
struct three_address_t {
	struct tac_data_t *lhs; // Result identifier
	int op; // Operator
	struct tac_data_t *op1; // Operand 1
	struct tac_data_t *op2; // Operand 2
	struct three_address_t *next; // Next TAC code
	struct three_address_t *prev; // Previous TAC code
};

// List entries for the master list of TAC nodes
struct three_address_list_t {
	struct three_address_t *tac;
	struct three_address_list_t *next;
};

// Definition for a node that comprise the control flow graph
struct block_t {
	int type; // Block types (see defines)
	char *label; // Name of the block (jump label)
	
	struct block_list_t *parents;
	struct block_list_t *children;

	struct three_address_t *entry; // Head of TAC list for this block
};

// A linked list of basic blocks
struct block_list_t {
	struct block_t *block;
	struct block_list_t *next;
};

// Represents a stack node that holds the hash value associated with a block level
struct vnt_node_t {
	char *val;  // The hash value of the node
	int block_level; // Associate a block level with the node
	char *pretty_name; // Human readable numbering for debug and output
	struct tac_data_t *val_td; // Associates tac data with the value on the stack
	struct vnt_node_t *next; // Point to the next node down the stack
};

// An entry in the hash table with the variable id and stack of hash values
struct vnt_entry_t {
	struct tac_data_t *var_td;  	// The tacData of the variable
	struct vnt_node_t *vnt_node; // The stack of nodes containing the hash information for an entry
	struct vnt_entry_t *next; // The next entry (for hashtable chaining)
};

struct set_list_t {
	struct set_t *set;
	struct set_list_t *next;
};

/* ----------------------------------
 * CFG State Variables
 * ----------------------------------
 */

struct block_t *rootBlock; // root of the cfg
struct block_list_t *blockList; // Master list of basic blocks
int block_counter; // Used for labeling new blocks

struct three_address_list_t *tacList; // Master list of all the tac
struct three_address_t *lastConnectedTac; // Pointer to the last tac list that was added to a block
int name_counter; // Temporary name counter used for TAC names
struct set_t *varList; // Holds all of the declared and used variables and constants

struct tac_data_list_t *tacDataList; // Master list of all tac data
struct tac_data_t *TAC_DATA_BOOL_TRUE; // tac data for constant value "true"
struct tac_data_t *TAC_DATA_BOOL_FALSE; // tac data for constant value "false"
struct tac_data_t *TAC_DATA_KEYWORD_IF; // tac data for the keyword "if"

int vnt_counter; // Name counter used for value numbering
int vnt_pretty_counter; // Pretty name counter used for outputting and debug
struct vnt_entry_t **vntable; // Represents the entire Value Number Table

struct set_list_t *label_aliases; // Represents labels that are aliases to fixup after CFG is built

/* ----------------------------------
 * CFG Functions
 * ----------------------------------
 */

void cfg_init();
struct block_t *cfg_get_root();
void cfg_destroy();
void cfg_print_vars_tac();
void cfg_print_blocks();

// CFG Block List Functions
struct block_list_t *cfg_new_block_list(struct block_t *firstBlock);
int cfg_block_list_size(struct block_list_t **list);
bool cfg_exists_in_block_list(struct block_list_t **list, struct block_t *block);
void cfg_append_block_list(struct block_list_t **list, struct block_t *block);
void cfg_drop_block_list(struct block_list_t **list, struct block_t *block);
void cfg_free_block_list(struct block_list_t **list, bool includeBlockEntry);

// CFG Block Functions
struct block_t *cfg_create_simple_block();
void cfg_free_block(struct block_t *block);
void cfg_print_block(struct block_t *block);
struct block_t *cfg_create_if_block(struct block_t *condition, struct block_t *trueBranch, struct block_t *falseBranch);
struct block_t *cfg_create_while_block(struct block_t *condition, struct block_t *bodyBlock);
struct block_t *cfg_find_bottom(struct block_t *block);
void cfg_connect_block(struct block_t *b1, struct block_t *b2);
void cfg_add_label_alias(char *label1, char *label2);

// Three Address Code Functions
char *cfg_new_temp_name();
void cfg_free_tac_list();
void cfg_free_tac(struct three_address_t *tac);
void cfg_free_tac_data_list();
void cfg_print_tac(struct three_address_t *tac);
struct tac_data_t *cfg_generate_tac(const char *lhs_id, struct tac_data_t *op1, int op, struct tac_data_t *op2);
void cfg_add_to_varlist(const char *id);
struct three_address_t *cfg_lookup_tac(const char *id);
void cfg_connect_tac(struct three_address_t *tac1, struct three_address_t *tac2);
struct three_address_t *cfg_generate_goto_tac(struct tac_data_t *cond, const char *label); // Creates tac for branching
char *cfg_tac_data_to_str(struct tac_data_t *td);
struct tac_data_t *cfg_new_tac_data();

// Value Number Table Functions
void cfg_vnt_init();
void cfg_vnt_destroy();
char *cfg_vnt_new_name(); // Creates name using counter
char *cfg_vnt_hash(const char *op1, int op, const char *op2); // Creates name by hashing operator and operands
struct vnt_entry_t *cfg_vnt_hash_insert(struct tac_data_t *var_td, char *val, struct tac_data_t *val_td, int block_level); // Creates and inserts a node
struct vnt_entry_t *cfg_vnt_hash_lookup_val(char *val); // Lookup by the vn_node_t val
struct vnt_entry_t *cfg_vnt_hash_lookup_td(struct tac_data_t *td); // Lookup by the vn_entry_t tacData
void cfg_vnt_hash_rollback(int block_level); // Rolls back the vn_node stacks to the specified level
void cfg_vnt_free_entry(struct vnt_entry_t *entry);

// VNT Stack functions
struct vnt_node_t **cfg_vnt_stack_pop(struct vnt_node_t **stack);
void cfg_vnt_stack_push(struct vnt_node_t **stack, char *val, struct tac_data_t *val_td, int block_level);
void cfg_vnt_free_node(struct vnt_node_t *node);

#endif


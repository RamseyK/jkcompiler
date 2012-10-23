/*
 * vntable.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Defines the Value Numbering table as a hash table
 * with collision resolution by chaining
 */


#include "shared.h"
#include "rulefuncs.h"

#define TABLE_SIZE 29

int counter;

struct vn_node_t {
	char *val;  // The hash value of the node
	int block_level; // Associate a block level with the node
	struct vn_node_t *next; // Point to the next node down the stack
};

struct vn_entry_t {
	char *id;  	// The id of the variable
	struct vn_node_t *vn_node; // The stack of nodes containing the hash information for an entry
	struct vn_entry_t *next; // The next entry (for hashtable chaining)
};

struct vn_entry_t *table[TABLE_SIZE];

void vntable_init();
void vntable_destroy();
// Both must give unique values
char *vntable_new_hash();
char *vntable_hash(char *op, char *op1, char *op2);

// Hash table functions
void vntable_hash_insert(char *id, char *val, int block_level); // Creates an inserts a node
struct vn_entry_t *vntable_hash_lookup_val(char *val); // Lookup by the vn_node_t val
struct vn_entry_t *vntable_hash_lookup_id(char *id); // Lookup by the vn_entry_t id
void vntable_hash_rollback(int block_level); // Rolls back the vn_node stacks to the specified level
void vntable_free_entry(struct vn_entry_t *entry);

// Stack functions
struct vn_node_t *vntable_stack_pop(struct vn_node_t **stack);
void vntable_stack_push(struct vn_node_t **stack, char *val, int block_level);
void vntable_free_node(struct vn_node_t *node);



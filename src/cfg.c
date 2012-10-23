/*
 * cfg.c
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Implements functions for Intermediate Representation
 * Three Address Code, Control Flow Graph, and Value Number Table
 */

#include "cfg.h"

/*
 * Control Flow Graph Functions
 */

// Initialize the control flow graph and any state variables
void cfg_init() {
	allBlocks = NULL;
	block_counter = 0;
	name_counter = 0;
	rootBlock = cfg_create_block(NULL);
}

// Destroy any objects used for intermediate representation
void cfg_destroy() {
	// Loop through the master list and free each basic block
	struct basic_block_t *it = allBlocks;
	struct basic_block_t *cur = allBlocks;
	while(it != NULL) {
		cur = it;
		it = it->next;
		cfg_free_block(cur);
	}

	allBlocks = NULL;
	block_counter = 0;
	name_counter = 0;
	rootBlock = NULL;
}

// Create a new basic block node and set it's TAC entry point
struct basic_block_t *cfg_create_block(struct three_address_t *entry) {
	struct basic_block_t *temp_block = (struct basic_block_t *)malloc(sizeof(struct basic_block_t));
	CHECK_MEM_ERROR(temp_block);

	// Create a new block label
	char buffer[8];
	int chars_written = sprintf(buffer, "b_%i", block_counter);
	temp_block->label = (char*)malloc(chars_written + 1);
	strncpy(temp_block->label, buffer, chars_written+1);
	block_counter++;

	temp_block->parents = NULL;
	temp_block->children = NULL;

	temp_block->entry = entry;

	temp_block->block_level = 0;

	// Add to the master list
	if(allBlocks == NULL) {
		allBlocks = temp_block;
	} else {
		// Add to end of master list
		struct basic_block_t *end = allBlocks;
		while(end->next != NULL)
			end = end->next;

		end->next = temp_block;
	}

	return temp_block;
}

// Free a basic block node, it's children, and associated TAC
void cfg_free_block(struct basic_block_t *block) {
	// Free TAC list
	struct three_address_t *it = block->entry;
	struct three_address_t *cur = block->entry;
	while(it != NULL) {
		cur = it;
		it = it->next;
		cfg_free_tac(cur);
	}

	// Free the basic block
	free(block);
}

// Generate a TAC node
struct three_address_t *cfg_generate_tac(char *lhs_id, int op, char *op1, char *op2) {
	struct three_address_t *temp_tac = (struct three_address_t *)malloc(sizeof(struct three_address_t));
	CHECK_MEM_ERROR(temp_tac);

	temp_tac->lhs_id = lhs_id;
	temp_tac->op = op;
	temp_tac->op1 = op1;
	temp_tac->op2 = op2;
	temp_tac->next = NULL;

	return temp_tac;
}

// Free TAC node
void cfg_free_tac(struct three_address_t *tac) {
	// Free any non null names / operand names
	if(tac->lhs_id != NULL)
		free(tac->lhs_id);

	if(tac->op1 != NULL)
		free(tac->op1);

	if(tac->op2 != NULL)
		free(tac->op2);

	// Free the actual TAC node
	free(tac);
}

// Generate a temporary name for TAC
char *cfg_new_temp_name() {
	char *temp_name = NULL;
	char buffer[8];

	// Create a name
	int chars_written = sprintf(buffer, "t_%i", name_counter);
	temp_name = (char*)malloc(chars_written + 1);
	strncpy(temp_name, buffer, chars_written+1);
	name_counter++;

	return temp_name;
}


/* ----------------------------------
 * Value Number Table Functions
 * ----------------------------------
 */

// Initialize the table
void cfg_vnt_init() {
	vnt_counter = 0;
	vntable = (struct vnt_entry_t **) calloc(TABLE_SIZE,sizeof(struct vnt_entry_t *));

	// Initialize all entries in the table to null
	int i;
	for(i = 0; i< TABLE_SIZE; i++) {
		vntable[i] = NULL;
	}
}

// Frees up memory allocated for the value number table
void cfg_vnt_destroy() {
	// Free every entry list in the table
	for(int i = 0; i<TABLE_SIZE; i++) {
		// Free every entry in the list
		struct vnt_entry_t *it = vntable[i];
		struct vnt_entry_t *cur = vntable[i];
		while(it != NULL) {
			cur = it;
			it = it->next;
			cfg_vnt_free_entry(cur);
		}

	}

	// Free the table pointer
	free(vntable);
}

// Returns a new name using the vnt_counter
char *cfg_vnt_new_name() {
	char *temp_name = NULL;
	char buffer[8];

	// Create a name
	int chars_written = sprintf(buffer, "#%i", vnt_counter);
	temp_name = (char*)malloc(chars_written + 1);
	strncpy(temp_name, buffer, chars_written+1);
	vnt_counter++;

	return temp_name;
}

// Returns a hash of the operator and operands
char *cfg_vnt_hash(char *op, char *op1, char *op2) {
	// Concatenate the strings together with #
	char *temp_name = NULL;
	char buffer[1024];

	// Concatenate the strings to make the hash
	int chars_written = sprintf(buffer, "%s%s%s", op, op1, op2);
	temp_name = (char *)malloc(chars_written +1);
	strncpy(temp_name, buffer, chars_written+1);

	return temp_name;
}

/* Hash Table manipulation */
// Does an update/insert on an entry for hash table
void cfg_vnt_hash_insert(char *id, char *val, int block_level) {
	// See if there is already an entry for this node by id
	struct vnt_entry_t *found_entry = cfg_vnt_hash_lookup_id(id);
	if(found_entry != NULL) {
		// See if the value is different from what is already on the stack
		if(strcmp(found_entry->vnt_node->val,val) != 0) {
			// Push onto the stack
			cfg_vnt_stack_push(&(found_entry->vnt_node), val, block_level);
		}
	} else {
		// Create the vnt_entry node
		struct vnt_entry_t *temp_entry = (struct vnt_entry_t *)malloc(sizeof(struct vnt_entry_t));
		CHECK_MEM_ERROR(temp_entry);
		temp_entry->id = new_identifier(id);
		temp_entry->vnt_node = NULL;
		// Push the vnt_node onto the empty stack
		cfg_vnt_stack_push(&(temp_entry->vnt_node), val, block_level);

		// Determine the hash key for this node
		int key = makekey(id, TABLE_SIZE);

		// Check the spot in the table
		if(vntable[key] == NULL) {
			vntable[key] = temp_entry;
		} else {
			// Resolve with chaining
			struct vnt_entry_t *vnt_entry_it = vntable[key];
			GOTO_END_OF_LIST(vnt_entry_it);
			vnt_entry_it->next = temp_entry;
		}
	}

}

// Looks up a vnt_entry_t by its current value using top of the vnt_node stack
struct vnt_entry_t *cfg_vnt_hash_lookup_val(char *val) {
	int i;
	for(i = 0; i<TABLE_SIZE; i++) {
		// Go through each entry that is chained at this table entry
		struct vnt_entry_t *vnt_entry_it = vntable[i];
		while(vnt_entry_it != NULL) {
			if(strcmp(vnt_entry_it->vnt_node->val,val) == 0)
				return vntable[i];
			vnt_entry_it = vnt_entry_it->next;
		}
	}
	return NULL;
}

// Looks up a vnt_entry_t by its id
struct vnt_entry_t *cfg_vnt_hash_lookup_id(char *id) {
	// Get the key that corresponds to this id
	int key = makekey(id, TABLE_SIZE);

	// Iterate through the elements at vntable[key] until found
	struct vnt_entry_t *vnt_entry_it = vntable[key];
	while(vnt_entry_it != NULL) {
		if(strcmp(vnt_entry_it->id,id) == 0)
			return vnt_entry_it;
		vnt_entry_it = vnt_entry_it->next;
	}
	return NULL;
}

// Rolls back each entry of the hash table to entries with a block_level
// less than or equal to that specified
void cfg_vnt_hash_rollback(int block_level) {
	int i;
	for(i = 0; i<TABLE_SIZE; i++) {
		// Iterate through each chained element
		struct vnt_entry_t *vnt_entry_it = vntable[i];
		while(vnt_entry_it != NULL) {
			// Pop from the stack until the block level is right
			while(vnt_entry_it->vnt_node->block_level > block_level) {
				cfg_vnt_stack_pop(&(vnt_entry_it->vnt_node));
			}
			vnt_entry_it = vnt_entry_it->next;
		}
	}
}

// Free up memory allocated for a hash entry
void cfg_vnt_free_entry(struct vnt_entry_t *entry) {
	// Free the id
	if(entry->id != NULL)
		free(entry->id);

	// Free each node in the node stack
	struct vnt_node_t *it = entry->vnt_node;
	struct vnt_node_t *cur = entry->vnt_node;
	while(it != NULL) {
		cur = it;
		it = it->next;
		cfg_vnt_free_node(cur);
	}

	// Free the entry
	free(entry);
}

/* Stack Manipulation */
// Pops the top element from the stack and returns it;
struct vnt_node_t *cfg_vnt_stack_pop(struct vnt_node_t **stack) {
	struct vnt_node_t *stack_top = *stack;
	if(stack_top == NULL)
		return NULL;
	*stack = stack_top->next;
	return stack_top;
}

// Pushes a new element to the top of the stack
void cfg_vnt_stack_push(struct vnt_node_t **stack, char *val, int block_level) {
	// Keep track of the top of the stack
	struct vnt_node_t *stack_top = *stack;

	// Make the node
	struct vnt_node_t *temp_node = (struct vnt_node_t *) malloc(sizeof(struct vnt_node_t));
	CHECK_MEM_ERROR(temp_node);
	temp_node->val = new_identifier(val);
	temp_node->block_level = block_level;

	// Attach the rest of the stack to this node
	temp_node->next = stack_top;

	// Move the top of the stack pointer to the new node
	stack_top = temp_node;
}

// Frees the memory allocated for a node
void cfg_vnt_free_node(struct vnt_node_t *node) {
	// Free the val
	if (node->val != NULL)
		free(node->val);

	// Free the pointer
	free(node);
}



/*
 * vntable.c
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Defines the Value Numbering table as a hash table
 * with collision resolution by chaining
 */

#include "vntable.h"

// Initialize the table
void vntable_init() {
	counter = 0;

	// Initialize all entries in the table to null
	int i;
	for(i = 0; i< TABLE_SIZE; i++) {
		table[i] = NULL;
	}
}

// Returns a new hash using the counter
char *vntable_new_hash() {
	char *temp_name = NULL;
	char buffer[8];

	// Create a name
	int chars_written = sprintf(buffer, "#%i", counter);
	temp_name = (char*)malloc(chars_written + 1);
	strncpy(temp_name, buffer, chars_written+1);
	counter++;

	return temp_name;
}

// Returns a hash of the operator and operands
char *vntable_hash(char *op, char *op1, char *op2) {
	// Concatenate the strings together with #
	char *temp_name = NULL;
	char buffer[1024];

	// Concatenate the strings to make the hash
	int chars_written = sprintf(buffer, "%s%s%s", op, op1, op2);
	temp_name = (char *)malloc(chars_written +1);
	strncpy(temp_name, buffer, chars_written+1);

	return temp_name;
}

/*
 * Hash table manipulation
 */
// Does an update/insert on an entry for hash table
void vntable_hash_insert(char *id, char *val, int block_level) {

	// See if there is already an entry for this node by id
	struct vn_entry_t *found_entry = vntable_hash_lookup_id(id);
	if(found_entry != NULL) {
		// See if the value is different from what is already on the stack
		if(strcmp(found_entry->vn_node->val,val) != 0) {
			// Push onto the stack
			vntable_stack_push(&(found_entry->vn_node), val, block_level);
		}
	} else {
		// Create the vn_entry node
		struct vn_entry_t *temp_entry = (struct vn_entry_t *)malloc(sizeof(struct vn_entry_t));
		CHECK_MEM_ERROR(temp_entry);
		temp_entry->id = new_identifier(id);
		temp_entry->vn_node = NULL;
		// Push the vn_node onto the empty stack
		vntable_stack_push(&(temp_entry->vn_node), val, block_level);

		// Determine the hash key for this node
		int key = make_key(id,TABLE_SIZE);

		// Check the spot in the table
		if(table[key] == NULL) {
			table[key] = temp_entry;
		} else {
			// Resolve with chaining
			struct vn_entry_t *vn_entry_it = table[key];
			GOTO_END_OF_LIST(vn_entry_it);
			vn_entry_it->next = temp_entry;
		}
	}

}

// Looks up a vn_entry_t by its current value using top of the vn_node stack
struct vn_entry_t *vntable_hash_lookup_val(char *val) {
	int i;
	for(i = 0; i<TABLE_SIZE; i++) {
		// Go through each entry that is chained at this table entry
		struct vn_entry_t *vn_entry_it = table[i];
		while(vn_entry_it != NULL) {
			if(strcmp(vn_entry_it->vn_node->val,val) == 0)
				return table[i];
			vn_entry_it = vn_entry_it->next;
		}
	}
	return NULL;
}

// Looks up a vn_entry_t by its id
struct vn_entry_t *vntable_hash_lookup_id(char *id) {
	// Get the key that corresponds to this id
	int key = make_key(id);

	// Iterate through the elements at table[key] until found
	struct vn_entry_t *vn_entry_it = table[key];
	while(vn_entry_it != NULL) {
		if(strcmp(vn_entry_it->id,id) == 0)
			return vn_entry_it;
		vn_entry_it = vn_entry_it->next;
	}
	return NULL;
}

// Rolls back each entry of the hash table to entries with a block_level
// less than or equal to that specified
void vntable_hash_rollback(int block_level) {
	int i;
	for(i = 0; i<TABLE_SIZE; i++) {
		// Iterate through each chained element
		struct vn_entry_t *vn_entry_it = table[i];
		while(vn_entry_it != NULL) {
			// Pop from the stack until the block level is right
			while(vn_entry_it->vn_node->block_level > block_level) {
				vntable_stack_pop(&(vn_entry_it->vn_node));
			}
			vn_entry_it = vn_entry_it->next;
		}
	}
}

/*
 * Stack manipulation
 */
// Pops the top element from the stack and returns it;
struct vn_node_t *vntable_stack_pop(struct vn_node_t **stack) {
	struct vn_node_t *stack_top = stack;
	*stack = *(stack)->next;
	return stack_top;
}

// Pushes a new element to the top of the stack
void vntable_stack_push(struct vn_node_t **stack, char *val, int block_level) {
	// Keep track of the top of the stack
	struct vn_node_t *stack_top = *stack;

	// Make the node
	struct vn_node_t *temp_node = (struct vn_node_t *) malloc(sizeof(struct vn_node_t));
	CHECK_MEM_ERROR(temp_node);
	temp_node->val = new_identifier(val);
	temp_node->block_level = block_level;

	// Attach the rest of the stack to this node
	temp_node->next = stack_top;

	// Move the top of the stack pointer to the new node
	stack_top = temp_node;
}



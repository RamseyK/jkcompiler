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
	rootBlock = NULL;
	blockList = NULL;
	block_counter = 0;
	name_counter = 0;
}

// Destroy any objects used for intermediate representation
void cfg_destroy() {
	// Free master list of blocks
	cfg_free_block_list();
		
	// Free the master list of TAC nodes
	cfg_free_tac_list();

	rootBlock = NULL;
	blockList = NULL;
	block_counter = 0;
	name_counter = 0;
}
// Prints A list of all variables including all temporary variables produced
// followed by the body of the method in 3-address code.
void cfg_print_vars_tac() {
	// Print all variables?
	// Supposed to print all of the variables along with temp variables
	// Sounds like we just print a list of all the lhs, op1, and op2 of the tacs
	// In a giant list but without repeating

	// Print tac for every block
	// How to traverse the cfg and print things sensibly?
	// Possibly use pre order traversal and the "marking" of blocks
}

// Prints the basic blocks of the cfg
void cfg_print_blocks() {
	struct basic_block_t it = rootBlock;
	while(it != NULL) {
		cfg_print_block(it);
		printf("\n");
		it = it->next;
	}
}

// Create a new simple basic block node with a TAC name (that will be resolved to the TAC node)
struct basic_block_t *cfg_create_simple_block(const char *tacName) {
	struct basic_block_t *temp_block = (struct basic_block_t *)malloc(sizeof(struct basic_block_t));
	CHECK_MEM_ERROR(temp_block);

	// Create a new block label
	char buffer[8];
	int chars_written = sprintf(buffer, "b_%i", block_counter);
	temp_block->label = (char*)malloc(chars_written + 1);
	strncpy(temp_block->label, buffer, chars_written+1);
	block_counter++;

	temp_block->type = BLOCK_SIMPLE;
	temp_block->parents = NULL;
	temp_block->children = NULL;

	// Lookup tac node and set it as the entry point
	temp_block->entry = cfg_lookup_tac(tacName);

	temp_block->block_level = 0;

	// Add to the master list
	if(blockList == NULL) {
		blockList = (struct basic_block_list_t *)malloc(sizeof(struct basic_block_list_t));
		CHECK_MEM_ERROR(blockList);
		blockList->block = temp_block;
		blockList->next = NULL;
	} else {
		// Find the end of the TAC list
		struct basic_block_list_t *end = blockList;
		while(end->next != NULL)
			end = end->next;
		
		// Link to the end of the list
		end->next = (struct basic_block_list_t *)malloc(sizeof(struct basic_block_list_t));
		CHECK_MEM_ERROR(end->next);
		end->next->block = temp_block;
		end->next->next = NULL;
	}

	return temp_block;
}

// Free the master list of blocks
void cfg_free_block_list() {
	struct basic_block_list_t *it = blockList;
	struct basic_block_list_t *cur = blockList;
	
	while(it != NULL) {
		cur = it;
		it = it->next;
		cfg_free_block(cur->block);
		free(cur);
	}
	
	rootBlock = NULL;
	blockList = NULL;
}

// Free a basic block node and other components ??
void cfg_free_block(struct basic_block_t *block) {
	// Free the basic block
	free(block->label);
	free(block);
}

// Prints the label of the block, and the labels of its parents and children
void cfg_print_block(struct basic_block_t *block) {
	printf("--------------------\n");
	printf("Block: %s\n",block->label);

	// Print parents
	printf("Parents: ");
	struct basic_block_t *pars = block->parents;
	while(pars != NULL) {
		printf("%s",pars->label);
		if(pars->nextSibling != NULL)
			printf(", ");
		pars = pars->nextSibling;
	}
	printf("\n");

	// Print children
	printf("Children: ");
	struct basic_block_t *chil = block->children;
	while(chil != NULL) {
		printf("%s", chil->label);
		if(chil->nextSibling != NULL)
			printf(", ");
		chil = chil->nextSibling;
	}
	printf("\n");
	printf("--------------------\n");
}

struct basic_block_t *cfg_create_if_block(struct basic_block_t *condition, struct basic_block_t *trueBranch, struct basic_block_t *falseBranch) {
	//if(condition == NULL)
		return NULL;
	// Link branches to a dummy node at the bottom
}

struct basic_block_t *cfg_create_while_block(struct basic_block_t *condition, struct basic_block_t *bodyBlock) {
	//if(condition == NULL)
		return NULL;
	// Dummy child is the false branch
	// Link branches to a dummy node at the bottom
}

// Find the bottom of the control flow graph given a starting block
// Return's NULL if it couldn't be found
struct basic_block_t *cfg_find_bottom(struct basic_block_t *block) {
	if(block == NULL)
		return NULL;

	struct basic_block_list_t *childList = block->children;
	
	// Bottom doesn't have children. Found it!
	if(childList == NULL)
		return block;
	
	// Traverse differently depending on the block type
	if(block->type == BLOCK_WHILE) {
		// Find a child that isn't a TRUE child - otherwise we'll be in an infinite loop
		struct basic_block_list_t *it = childList;
		struct basic_block_t *child = NULL;
		while(it != NULL && child == NULL) {
			if(it->block->type != BLOCK_TRUE) {
				child = it->block;
			}
			it = it->next;
		}
		
		// Not found??
		if(child == NULL)
			return NULL;
		
		return cfg_find_bottom(child);
	} else { // IF, SIMPLE, TRUE, FALSE
		// Any child will work. First child in the list
		return cfg_find_bottom(childList->block);
	}
}

struct basic_block_t *cfg_connect_block(struct basic_block_t *b1, struct basic_block_t *b2) {
	//if(b1 == NULL || b2 == NULL)
		return NULL;
	// First block is the pointer to the HEAD of a list of blocks
	// Second block is 
	// Check types of b1 and b2
}

// Prints the three address code recursively in the format:
// LHS = OP1 op OP2
void cfg_print_tac(struct three_address_t *tac) {
	if(tac == NULL) return;
	printf("%s = %s %s %s\n", tac->lhs_id, tac->op1, tac->op, tac->op2);
	cfg_print_tac(tac->next);
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

// Generate a TAC node and add it to the master list. Returns the name of the tac
// If lhs_id is NULL, a temporary name is generated
char *cfg_generate_tac(const char *lhs_id, const char *op1, int op, const char *op2) {
	struct three_address_t *temp_tac = (struct three_address_t *)malloc(sizeof(struct three_address_t));
	CHECK_MEM_ERROR(temp_tac);
	
	if(lhs_id == NULL)
		temp_tac->lhs_id = cfg_new_temp_name();
	else
		temp_tac->lhs_id = new_identifier(lhs_id);
	temp_tac->op = op;
	temp_tac->op1 = new_identifier(op1);
	temp_tac->op2 = new_identifier(op2);
	temp_tac->next = NULL;
	temp_tac->prev = NULL;
	
	// Insert the TAC node into the master list
	if(tacList == NULL) {
		tacList = (struct three_address_list_t *)malloc(sizeof(struct three_address_list_t));
		CHECK_MEM_ERROR(tacList);
		tacList->tac = temp_tac;
		tacList->next = NULL;
	} else {
		// Find the end of the TAC list
		struct three_address_list_t *end = tacList;
		while(end->next != NULL)
			end = end->next;
		
		// Link to the end of the list
		end->next = (struct three_address_list_t *)malloc(sizeof(struct three_address_list_t));
		CHECK_MEM_ERROR(end->next);
		end->next->tac = temp_tac;
		end->next->next = NULL;
	}

	return temp_tac->lhs_id;
}

// Frees the master TAC list and TAC nodes
void cfg_free_tac_list() {
	// Free TAC list
	struct three_address_list_t *it = tacList;
	struct three_address_list_t *cur = tacList;
	while(it != NULL) {
		cur = it;
		it = it->next;
		
		// Free the TAC node
		cfg_free_tac(cur->tac);
		
		// Free the list entry
		free(cur);
	}
	
	tacList = NULL;
}

// Frees a TAC node and it's components
void cfg_free_tac(struct three_address_t *tac) {
	// Free any non null names / operand names
	free(tac->lhs_id);
	
	if(tac->op1 != NULL)
		free(tac->op1);

	if(tac->op2 != NULL)
		free(tac->op2);

	// Free the TAC node
	free(tac);
}

// Given a temporary TAC name, lookup the TAC node in the master list
// Returns NULL if not found
struct three_address_t *cfg_lookup_tac(const char *id) {
	struct three_address_list_t *it = tacList;
	while(it != NULL) {
		if(strcmp(it->tac->lhs_id, id) == 0)
			return it->tac;
		it = it->next;
	}
	
	return NULL;
}

// Connect two tacs together. tac1 will precede tac2
void cfg_connect_tac(const char *tac1, const char *tac2) {
	struct three_address_t *node1 = cfg_lookup_tac(tac1);
	struct three_address_t *node2 = cfg_lookup_tac(tac2);
	if(node1 == NULL || node2 == NULL)
		return;
		
	node1->next = node2;
	node2->prev = node1;
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



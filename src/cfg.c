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
	varList = NULL;
}

// Return the root of the CFG
struct basic_block_t *cfg_get_root() {
	return rootBlock;
}

// Destroy any objects used for intermediate representation
void cfg_destroy() {
	// Free master list of blocks and all block objects
	cfg_free_block_list(&blockList, true);
		
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

	printf("All of the vars:\n");
	set_print(varList);

	// Print tac for every block
	// How to traverse the cfg and print things sensibly?
	// Possibly use pre order traversal and the "marking" of blocks
	printf("All of the TAC:\n");
	struct basic_block_list_t *it = blockList;
	while(it != NULL) {
		printf("%s:\n",it->block->label);
		cfg_print_tac(it->block->entry);
		printf("\n");
		it = it->next;
	}

}

// Prints the basic blocks of the cfg
void cfg_print_blocks() {
	struct basic_block_list_t *it = blockList;
	while(it != NULL) {
		cfg_print_block(it->block);
		printf("\n");
		it = it->next;
	}
}

// Create a new CFG block list
struct basic_block_list_t *cfg_new_block_list(struct basic_block_t *firstBlock) {
	struct basic_block_list_t *list = NULL;
	list = (struct basic_block_list_t *)malloc(sizeof(struct basic_block_list_t));
	CHECK_MEM_ERROR(list);
	
	list->block = firstBlock;
	list->next = NULL;
	
	return list;
}

// Determine the size of the list
int cfg_block_list_size(struct basic_block_list_t **list) {
	int count = 0;
	struct basic_block_list_t *cur = *list;
	while(cur != NULL) {
		count++;
		cur = cur->next;
	}
	return count;
}

// Check if a block exists in a list
bool cfg_exists_in_block_list(struct basic_block_list_t **list, struct basic_block_t *block) {
	struct basic_block_list_t *cur = *list;
	while(cur != NULL) {
		if(cur->block == block)
			return true;
		cur = cur->next;
	}
	return false;
}

// Append to an existing CFG block list
void cfg_append_block_list(struct basic_block_list_t **list, struct basic_block_t *block) {
	if(block == NULL)
		return;
	
	if(*list == NULL) {
		*list = cfg_new_block_list(block);
	} else {
		// Find the end of the list
		struct basic_block_list_t *end = *list;
		while(end->next != NULL)
			end = end->next;
		
		// Link to the end of the list
		end->next = cfg_new_block_list(block);
	}
}

// Drop a block from the block_list
void cfg_drop_block_list(struct basic_block_list_t **list, struct basic_block_t *block) {
	if(*list == NULL || block == NULL)
		return;
		
	struct basic_block_list_t *drop = NULL;

	if((*list)->block == block) {
		// b2 is the HEAD
		// Save the 'old' head
		drop = *list;
		// Adjust the head of the list to point to the new head
		*list = (*list)->next;
		// Remove and free the old head from memory
		drop->block = NULL;
		free(drop);
	} else {
		// 'block' is somewhere else in the master list
		struct basic_block_list_t *it = *list;
		while(it != NULL) {
			if(it->next->block == block) {
				// Save the pointer to the node being dropped
				drop = it->next;
				drop->block = NULL;
				
				// Adjust the pointer of the current list node to skip the node being dropped
				it->next = it->next->next;
				
				// Free the found node in memory
				free(drop);
				break;
			}
			it = it->next;	
		}
	}
}

// Free an existing list of blocks
// If includeBlockEntry is true, block pointers within the block_list object will also be freed
void cfg_free_block_list(struct basic_block_list_t **list, bool includeBlockEntry) {
	struct basic_block_list_t *it = *list;
	struct basic_block_list_t *cur = *list;
	
	while(it != NULL) {
		cur = it;
		it = it->next;
		if(includeBlockEntry) {
			cfg_free_block(cur->block);
			cur->block = NULL;
		}
		free(cur);
	}
	
	*list = NULL;
}

// Create a new simple basic block node
struct basic_block_t *cfg_create_simple_block() {
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

	// Connect all the tac
	// Go to the last connected tac
	IRLOG(("Going to the last connected tac\n"));
	struct three_address_list_t *tl_it = tacList;
	if(lastConnectedTac != NULL) {
		while(tl_it->tac != lastConnectedTac) {
			tl_it = tl_it->next;
		}
		IRLOG(("Moving one past it\n"));
		// Move one past the last connected
		tl_it = tl_it->next;
	}
	// Associate with the block
	// If its a dummy node then tl_it should be null because
	// the lastConnectedTac was the end of the list, so skip
	if(tl_it != NULL) {
		temp_block->entry = tl_it->tac;

		IRLOG(("Connecting the tacs\n"));
		while(tl_it->next != NULL) {
			tl_it->tac->next = tl_it->next->tac;
			tl_it->tac->next->prev = tl_it->tac;
			tl_it = tl_it->next;
		}
		// Update the last connected
		lastConnectedTac = tl_it->tac;
	} else {
		temp_block->entry = NULL;
	}



	// Add to the master list
	cfg_append_block_list(&blockList, temp_block);

	//Debug
	IRLOG(("Simple block created\n"));
	cfg_print_block(temp_block);

	return temp_block;
}

// Free a basic block node
// The TAC nodes, parents and children blocks are not released here. They will be freed through the master list
void cfg_free_block(struct basic_block_t *block) {
	// Free the label
	free(block->label);
	block->label = NULL;
	
	// Only free the list objects associated in parents and children, not the actual parents and children block objects
	cfg_free_block_list(&block->parents, false);
	cfg_free_block_list(&block->children, false);
	block->parents = NULL;
	block->children = NULL;

	// Free the basic block
	free(block);
}

// Prints the label of the block, and the labels of its parents and children
void cfg_print_block(struct basic_block_t *block) {
	printf("--------------------\n");
	printf("Block: %s\n",block->label);

	// Print type
	printf("Type: %i\n",block->type);

	// Print parents
	printf("Parents: ");
	struct basic_block_list_t *pars = block->parents;
	while(pars != NULL) {
		printf("%s",pars->block->label);
		if(pars->next != NULL)
			printf(", ");
		pars = pars->next;
	}
	printf("\n");

	// Print children
	printf("Children: ");
	struct basic_block_list_t *chil = block->children;
	while(chil != NULL) {
		printf("%s", chil->block->label);
		if(chil->next != NULL)
			printf(", ");
		chil = chil->next;
	}
	printf("\n");
	printf("TAC:");
	cfg_print_tac(block->entry);
	printf("--------------------\n");
}

// Create an isolated IF block for the CFG
struct basic_block_t *cfg_create_if_block(struct basic_block_t *condition, struct basic_block_t *trueBranch, struct basic_block_t *falseBranch) {
	IRLOG(("Creating if block\n"));

	if(condition == NULL)
		return NULL;
	
	// Set IF, TRUE, and FALSE block properties and children
	trueBranch->type = BLOCK_TRUE;
	falseBranch->type = BLOCK_FALSE;
	condition->type = BLOCK_IF;
	cfg_append_block_list(&condition->children, trueBranch);
	cfg_append_block_list(&condition->children, falseBranch);
	
	// Link True and False branches to a dummy node at the bottom
	struct basic_block_t *dummy = cfg_create_simple_block(NULL);
	cfg_append_block_list(&dummy->parents, trueBranch);
	cfg_append_block_list(&dummy->parents, falseBranch);
	cfg_append_block_list(&trueBranch->children, dummy);
	cfg_append_block_list(&falseBranch->children, dummy);
	
	// Add the branch/goto statement for the condition block and the true/false branches
	struct three_address_t *condTrue = cfg_generate_goto_tac(condition->entry->lhs_id,trueBranch->label);
	struct three_address_t *condFals = cfg_generate_goto_tac("true",falseBranch->label);
	cfg_connect_tac(condition->entry,condTrue);
	cfg_connect_tac(condition->entry,condFals);

	struct three_address_t *checkCond1 = cfg_generate_goto_tac("true",dummy->label);
	cfg_connect_tac(trueBranch->entry,checkCond1);
	struct three_address_t *checkCond2 = cfg_generate_goto_tac("true",dummy->label);
	cfg_connect_tac(falseBranch->entry,checkCond2);

	//Debug
	IRLOG(("If block created\n"));
	cfg_print_block(condition);

	return condition;
}

// Create an isolated WHILE block for the CFG
struct basic_block_t *cfg_create_while_block(struct basic_block_t *condition, struct basic_block_t *bodyBlock) {
	IRLOG(("Creating while block\n"));

	if(condition == NULL)
		return NULL;
	
	// Create a simple block first with the tac nodes of the condition block
	//struct basic_block_t *wh_block = cfg_create_simple_block(condition->entry);
	
	// Set WHILE, TRUE, block properties and children
	bodyBlock->type = BLOCK_TRUE;
	condition->type = BLOCK_WHILE;

	// Link bodyBlock
	cfg_append_block_list(&condition->children, bodyBlock);
	cfg_append_block_list(&bodyBlock->parents,condition);
	
	// Link branches to a dummy node at the bottom
	struct basic_block_t *dummy = cfg_create_simple_block(NULL);
	cfg_append_block_list(&dummy->parents, condition);
	cfg_append_block_list(&condition->children, dummy);
	
	// Add the branch/goto statement for the condition block and the true/false branches
	struct three_address_t *condTrue = cfg_generate_goto_tac(condition->entry->lhs_id,bodyBlock->label);
	//struct three_address_t *condFals = cfg_generate_goto_tac("true",falseBranch->label);
	cfg_connect_tac(condition->entry,condTrue);
	//cfg_connect_tac(condition->entry,condFals);

	struct three_address_t *checkCond1 = cfg_generate_goto_tac("true",condition->label);
	cfg_connect_tac(bodyBlock->entry,checkCond1);
	//struct three_address_t *checkCond2 = cfg_generate_goto_tac("true",condition->label);
	//cfg_connect_tac(falseBranch->entry,checkCond2);

	//Debug
	IRLOG(("While block created\n"));
	cfg_print_block(condition);

	return condition;
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

// Connect CFG b1 to CFG b2
void cfg_connect_block(struct basic_block_t *b1, struct basic_block_t *b2) {
	if(b1 == NULL || b2 == NULL) {
		IRLOG(("b1 or b2 was null\n"));
		return;
	}
	IRLOG(("Connecting blocks\nb1: %s\tb2: %s\n",b1->label,b2->label));

	// First block is the root of a CFG. b2 will be linked to the bottom of b1
	struct basic_block_t *bottom = cfg_find_bottom(b1);
	
	IRLOG(("Bottom block: %s\n",bottom->label));

	// If the bottom is a dummy, replace the dummy block with b2
	if(bottom->entry == NULL) {
		// I hope we aren't using the dummy's label anywhere... How would that affect jumps?
		// TODO:  The dummy node is the target of a jump in the case of while loop and if (false branch)
		// Need to create and maintain a table to traverse later to fix labels that have been changed
		// So that everything is consistent and not mislabeled.  Involves traversing all of the tac nodes
		// In every block and replacing old label with new Label.  LList of Sets.
		bottom->label = new_identifier(b2->label);
		bottom->children = b2->children;
		bottom->entry = b2->entry;
		bottom->type = b2->type;

		// Drop the dummy and destroy it
		cfg_drop_block_list(&blockList, b2);
		cfg_free_block(b2);
	} else { // Bottom isn't a dummy, merge the TACs making a maximal basic block
		// Stick the end of b1's TACs to the beginning of b2's TACs
		struct three_address_t *end = bottom->entry;
		while(end->next != NULL)
			end = end->next;
		end->next = b2->entry;
		

		// If the second block is a simple block, drop it
		if(b2->type == BLOCK_SIMPLE) {
			cfg_drop_block_list(&blockList, b2);
			cfg_free_block(b2);
		} else {
			// Update parent and children pointers for the blocks
			// Add b2 as a child to b1
			cfg_append_block_list(&b1->children,b2);

			// Add b1 as a parent to b2
			cfg_append_block_list(&b2->parents,b1);
		}


	}
	
	//Debug
	IRLOG(("Blocks connected\n\n"));
	cfg_print_blocks();

}

// Prints the three address code recursively in the format:
// LHS = OP1 op OP2
void cfg_print_tac(struct three_address_t *tac) {
	if(tac == NULL) return;
	if(tac == tac->prev) {
		IRLOG(("Infinite loop detected in print tac\n"));
		return;
	}
	
	char *op = op_str(tac->op);

	if(tac->op == OP_NO_OP)
		printf("\t%s := %s\n", tac->lhs_id, tac->op1);
	else if(tac->op == OP_GOTO)
		printf("\t%s %s %s %s\n", tac->lhs_id, tac->op1, "goto", tac->op2);
	else
		printf("\t%s := %s %s %s\n", tac->lhs_id, tac->op1, op, tac->op2);
		

	free(op);
	
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

		// Set this as the lastConnectedTac
		//lastConnectedTac = temp_tac;
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
	IRLOG(("Tac node just created: "));
	cfg_print_tac(temp_tac);

	// Add to the varlist
	cfg_add_to_varlist(temp_tac->lhs_id);

	return temp_tac->lhs_id;
}

struct three_address_t *cfg_generate_goto_tac(const char *cond, const char *label) {
	struct three_address_t *temp_tac = (struct three_address_t *)malloc(sizeof(struct three_address_t));
	CHECK_MEM_ERROR(temp_tac);
	temp_tac->lhs_id = new_identifier("if");
	temp_tac->op = OP_GOTO;
	temp_tac->op1 = new_identifier(cond);
	temp_tac->op2 = new_identifier(label);
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

	return temp_tac;
}

// Adds an element to the varList for tac nodes
void cfg_add_to_varlist(const char *id) {
	varList = set_union(varList,new_set(id));
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
void cfg_connect_tac(struct three_address_t *tac1, struct three_address_t *tac2) {
	/*struct three_address_t *node1 = cfg_lookup_tac(tac1);
	struct three_address_t *node2 = cfg_lookup_tac(tac2);
	if(node1 == NULL || node2 == NULL) {
		IRLOG(("One of them was null\n"));
		return;
	}*/
	struct three_address_t *end1 = tac1;
	GOTO_END_OF_LIST(end1);
	end1->next = tac2;
	tac2->prev = end1;

	IRLOG(("Connected this: \n"));
	cfg_print_tac(tac1);
}

/* ----------------------------------
 * Value Number Table Functions
 * ----------------------------------
 */

// Initialize the table
void cfg_vnt_init() {
	vnt_counter = 0;
	vnt_pretty_counter = 0;
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
char *cfg_vnt_hash(const char *op1, int op, const char *op2) {
	// Concatenate the strings together with #
	char *temp_name = NULL;
	char buffer[1024];

	// Concatenate the strings to make the hash
	int chars_written = sprintf(buffer, "%s%i%s", op1, op, op2);
	temp_name = (char *)malloc(chars_written +1);
	strncpy(temp_name, buffer, chars_written+1);

	return temp_name;
}

/* Hash Table manipulation */
// Does an update/insert on an entry for hash table and returns the newly created entry
struct vnt_entry_t *cfg_vnt_hash_insert(char *key, char *val, int block_level) {
	// See if there is already an entry for this node by id
	struct vnt_entry_t *found_entry = cfg_vnt_hash_lookup_id(key);
	if(found_entry != NULL) {
		// See if the value is different from what is already on the stack
		if(strcmp(found_entry->vnt_node->val,val) != 0) {
			// Push onto the stack
			cfg_vnt_stack_push(&(found_entry->vnt_node), val, block_level);
		}
		return found_entry;
	} else {
		// Create the vnt_entry node
		struct vnt_entry_t *temp_entry = (struct vnt_entry_t *)malloc(sizeof(struct vnt_entry_t));
		CHECK_MEM_ERROR(temp_entry);
		temp_entry->id = new_identifier(key);
		temp_entry->vnt_node = NULL;
		
		// Push the vnt_node onto the empty stack
		cfg_vnt_stack_push(&(temp_entry->vnt_node), val, block_level);

		// Determine the hash key for this node
		int hkey = makekey(key, TABLE_SIZE);

		// Check the spot in the table
		if(vntable[hkey] == NULL) {
			vntable[hkey] = temp_entry;
		} else {
			// Resolve with chaining
			struct vnt_entry_t *vnt_entry_it = vntable[hkey];
			GOTO_END_OF_LIST(vnt_entry_it);
			vnt_entry_it->next = temp_entry;
		}
		return temp_entry;
	}
}

// Looks up a vnt_entry_t by its current value using top of the vnt_node stack
struct vnt_entry_t *cfg_vnt_hash_lookup_val(char *val) {
	int i;
	for(i = 0; i<TABLE_SIZE; i++) {
		// Go through each entry that is chained at this table entry
		struct vnt_entry_t *vnt_entry_it = vntable[i];
		while(vnt_entry_it != NULL) {
			if(vnt_entry_it->vnt_node != NULL) {
				if(strcmp(vnt_entry_it->vnt_node->val,val) == 0)
					return vntable[i];
			}
			vnt_entry_it = vnt_entry_it->next;
		}
	}
	return NULL;
}

// Looks up a vnt_entry_t by its key
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
			while((vnt_entry_it->vnt_node != NULL) && (vnt_entry_it->vnt_node->block_level > block_level)) {
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
	if(*stack == NULL)
		return NULL;
	struct vnt_node_t *stack_top = *stack;
	*stack = (*stack)->next;
	return stack_top;
}

// Pushes a new element to the top of the stack
void cfg_vnt_stack_push(struct vnt_node_t **stack, char *val, int block_level) {
	// Make the node
	struct vnt_node_t *temp_node = (struct vnt_node_t *) malloc(sizeof(struct vnt_node_t));
	CHECK_MEM_ERROR(temp_node);
	temp_node->val = new_identifier(val);
	temp_node->block_level = block_level;
	
	// Create a pretty name for the stack node and set it (for debug)
	char buffer[8];
	int chars_written = sprintf(buffer, "#%i", vnt_pretty_counter);
	temp_node->pretty_name = (char*)malloc(chars_written + 1);
	strncpy(temp_node->pretty_name, buffer, chars_written+1);
	vnt_pretty_counter++;

	// Attach the rest of the stack to this node
	temp_node->next = *stack;

	// Move the top of the stack pointer to the new node
	*stack = temp_node;
}

// Frees the memory allocated for a node
void cfg_vnt_free_node(struct vnt_node_t *node) {
	// Free the val
	if (node->val != NULL)
		free(node->val);
	if (node->pretty_name != NULL)
		free(node->pretty_name);

	// Free the pointer
	free(node);
}



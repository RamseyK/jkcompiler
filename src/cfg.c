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
	currRootBlock = NULL;
	blockList = NULL;
	cfgList = NULL;
	block_counter = 0;
	
	name_counter = 0;
	
	tacDataList = NULL;
	lastConnectedTac = NULL;
	label_aliases = NULL;

	// Initialize the constant true and false tac data nodes
	// And insert into the vnt
	TAC_DATA_BOOL_TRUE = cfg_new_tac_data();
	TAC_DATA_BOOL_TRUE->type = TAC_DATA_TYPE_BOOL;
	TAC_DATA_BOOL_TRUE->d.b = true;

	TAC_DATA_BOOL_FALSE = cfg_new_tac_data();
	TAC_DATA_BOOL_FALSE->type = TAC_DATA_TYPE_BOOL;
	TAC_DATA_BOOL_FALSE->d.b = false;

	// constant tac data for "if"
	TAC_DATA_KEYWORD_IF = cfg_new_tac_data();
	TAC_DATA_KEYWORD_IF->type = TAC_DATA_TYPE_KEYWORD;
	TAC_DATA_KEYWORD_IF->d.id = new_identifier("if");

}

// Destroy any objects used for intermediate representation
void cfg_destroy() {
	// Free master list of blocks and all block objects
	cfg_free_block_list(&blockList, true);
		
	// Free the master list of TAC nodes
	cfg_free_tac_list();
	// Free the master list of tac data
	cfg_free_tac_data_list();
	
	// Free the list of all label alias sets
	cfg_free_label_aliases();

	currRootBlock = NULL;
	blockList = NULL;
	block_counter = 0;
	name_counter = 0;
	lastConnectedTac = NULL;
}

// Prints all tac nodes within the all blocks in the block list
void cfg_print_tacs() {
	// Print tac for every block
	// How to traverse the cfg and print things sensibly?
	// Possibly use pre order traversal and the "marking" of blocks
	printf("All of the TAC:\n");
	struct block_list_t *it = blockList;
	while(it != NULL) {
		printf("%s:\n",it->block->label);
		cfg_print_tac(it->block->entry);
		printf("\n");
		it = it->next;
	}

}

// Prints the basic blocks of the cfg
void cfg_print_blocks() {
	struct block_list_t *it = blockList;
	while(it != NULL) {
		cfg_print_block(it->block);
		printf("\n");
		it = it->next;
	}
}

// Creates a cfg_list_t to represent the functions CFG
struct cfg_list_t *cfg_create_func_cfg(struct scope_t *funcScope) {
	struct cfg_list_t *temp_cfg = (struct cfg_list_t *) malloc(sizeof(struct cfg_list_t));
	CHECK_MEM_ERROR(temp_cfg);
	temp_cfg->scope = funcScope;

	// Assign the root block for the last created section of blocks to this
	temp_cfg->entryBlock = currRootBlock;
	temp_cfg->next = NULL;

	// Set the currRootBlock to null so that the next block created is the root
	// for the next function cfg
	currRootBlock = NULL;

	// Add this cfg to the master list
	if(cfgList == NULL) {
		cfgList = temp_cfg;
	} else {
		struct cfg_list_t *cfg_it = cfgList;
		GOTO_END_OF_LIST(cfg_it);
		cfg_it->next = temp_cfg;
	}

	return temp_cfg;
}

// Create a new CFG block list
struct block_list_t *cfg_new_block_list(struct block_t *firstBlock) {
	struct block_list_t *list = NULL;
	list = (struct block_list_t *)malloc(sizeof(struct block_list_t));
	CHECK_MEM_ERROR(list);
	
	list->block = firstBlock;
	list->next = NULL;
	
	return list;
}

// Determine the size of the list
int cfg_block_list_size(struct block_list_t **list) {
	int count = 0;
	struct block_list_t *cur = *list;
	while(cur != NULL) {
		count++;
		cur = cur->next;
	}
	return count;
}

// Check if a block exists in a list
bool cfg_exists_in_block_list(struct block_list_t **list, struct block_t *block) {
	struct block_list_t *cur = *list;
	while(cur != NULL) {
		if(cur->block == block)
			return true;
		cur = cur->next;
	}
	return false;
}

// Append to an existing CFG block list
void cfg_append_block_list(struct block_list_t **list, struct block_t *block) {
	if(block == NULL)
		return;
	
	if(*list == NULL) {
		*list = cfg_new_block_list(block);
	} else {
		// Find the end of the list
		struct block_list_t *end = *list;
		while(end->next != NULL)
			end = end->next;
		
		// Link to the end of the list
		end->next = cfg_new_block_list(block);
	}
}

// Drop a block from the block_list
void cfg_drop_block_list(struct block_list_t **list, struct block_t *block) {
	if(*list == NULL || block == NULL)
		return;
		
	struct block_list_t *drop = NULL;

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
		struct block_list_t *it = *list;
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
void cfg_free_block_list(struct block_list_t **list, bool includeBlockEntry) {
	struct block_list_t *it = *list;
	struct block_list_t *cur = *list;
	
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
struct block_t *cfg_create_simple_block() {
	struct block_t *temp_block = (struct block_t *)malloc(sizeof(struct block_t));
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

	temp_block->assignVar = NULL;
	temp_block->ueVar = NULL;
	temp_block->killVar = NULL;
	temp_block->liveIn = NULL;
	temp_block->liveOut = NULL;

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

	// Check if the rootBlock is null, which means a function was just
	// finished and the new block here is the root of the next
	if(currRootBlock == NULL) {
		currRootBlock = temp_block;
	}

	// Add to the master list
	cfg_append_block_list(&blockList, temp_block);

	//Debug
	//IRLOG(("Simple block created\n"));
	//cfg_print_block(temp_block);

	return temp_block;
}

// Free a basic block node
// The TAC nodes, parents and children blocks are not released here. They will be freed through the master list
void cfg_free_block(struct block_t *block) {
	// Free the label
	free(block->label);
	block->label = NULL;
	
	// Only free the list objects associated in parents and children, not the actual parents and children block objects
	cfg_free_block_list(&block->parents, false);
	cfg_free_block_list(&block->children, false);
	block->parents = NULL;
	block->children = NULL;
	
	// Free flow sets
	free_set(block->liveIn);
	block->liveIn = NULL;
	free_set(block->assignVar);
	block->assignVar = NULL;
	free_set(block->ueVar);
	block->ueVar = NULL;
	free_set(block->killVar);
	block->killVar = NULL;
	free_set(block->liveOut);
	block->liveOut = NULL;

	// Free the basic block
	free(block);
}

// Prints the label of the block, and the labels of its parents and children
void cfg_print_block(struct block_t *block) {
	printf("--------------------\n");
	printf("Block: %s\n",block->label);

	// Print type
	printf("Type: %i\n",block->type);

	// Print parents
	printf("Parents: ");
	struct block_list_t *pars = block->parents;
	while(pars != NULL) {
		printf("%s",pars->block->label);
		if(pars->next != NULL)
			printf(", ");
		pars = pars->next;
	}
	printf("\n");

	// Print children
	printf("Children: ");
	struct block_list_t *chil = block->children;
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
struct block_t *cfg_create_if_block(struct block_t *condition, struct block_t *trueBranch, struct block_t *falseBranch) {
	IRLOG(("Creating if block\n"));

	if(condition == NULL)
		return NULL;
	
	// Set IF, TRUE, and FALSE block properties and children
	trueBranch->type = BLOCK_TRUE;
	falseBranch->type = BLOCK_FALSE;
	condition->type = BLOCK_IF;
	cfg_append_block_list(&condition->children, trueBranch);
	cfg_append_block_list(&condition->children, falseBranch);
	cfg_append_block_list(&trueBranch->parents, condition);
	cfg_append_block_list(&falseBranch->parents, condition);
	
	// Link True and False branches to a dummy node at the bottom
	struct block_t *dummy = cfg_create_simple_block(NULL);
	cfg_append_block_list(&dummy->parents, trueBranch);
	cfg_append_block_list(&dummy->parents, falseBranch);
	cfg_append_block_list(&trueBranch->children, dummy);
	cfg_append_block_list(&falseBranch->children, dummy);
	
	// Add the branch/goto statement for the condition block and the true/false branches
	struct three_address_t *condTrue = cfg_generate_branch_tac(condition->entry->lhs,trueBranch->label);
	struct three_address_t *condFals = cfg_generate_goto_tac(falseBranch->label);
	cfg_connect_tac(condition->entry,condTrue);
	cfg_connect_tac(condition->entry,condFals);

	struct three_address_t *checkCond1 = cfg_generate_goto_tac(dummy->label);
	cfg_connect_tac(trueBranch->entry,checkCond1);
	struct three_address_t *checkCond2 = cfg_generate_goto_tac(dummy->label);
	cfg_connect_tac(falseBranch->entry,checkCond2);

	//Debug
	IRLOG(("If block created\n"));
	//cfg_print_block(condition);

	return condition;
}

// Create an isolated WHILE block for the CFG
struct block_t *cfg_create_while_block(struct block_t *condition, struct block_t *bodyBlock) {
	IRLOG(("Creating while block\n"));

	if(condition == NULL)
		return NULL;
	
	// Set WHILE, TRUE, block properties and children
	bodyBlock->type = BLOCK_TRUE;
	condition->type = BLOCK_WHILE;

	// Link bodyBlock
	// bodyBlock and condition are parents and children of each other
	cfg_append_block_list(&condition->children, bodyBlock);
	cfg_append_block_list(&condition->parents, bodyBlock);
	cfg_append_block_list(&bodyBlock->parents, condition);
	cfg_append_block_list(&bodyBlock->children, condition);
	
	// Link branches to a dummy node at the bottom
	struct block_t *dummy = cfg_create_simple_block(NULL);
	cfg_append_block_list(&dummy->parents, condition);
	cfg_append_block_list(&condition->children, dummy);
	
	// Add the branch/goto statement for the condition block and the true/false branches
	struct three_address_t *condTrue = cfg_generate_branch_tac(condition->entry->lhs,bodyBlock->label);
	cfg_connect_tac(condition->entry,condTrue);
	// The next tac node should be to jump to the dummy unconditionally
	struct three_address_t *condFals = cfg_generate_goto_tac(dummy->label);
	cfg_connect_tac(condition->entry,condFals);

	struct three_address_t *checkCond1 = cfg_generate_goto_tac(condition->label);
	cfg_connect_tac(bodyBlock->entry,checkCond1);

	//Debug
	IRLOG(("While block created\n"));
	//cfg_print_block(condition);

	return condition;
}

// Find the bottom of the control flow graph given a starting block
// Return's NULL if it couldn't be found
struct block_t *cfg_find_bottom(struct block_t *block) {
	if(block == NULL)
		return NULL;

	struct block_list_t *childList = block->children;
	
	// Bottom doesn't have children. Found it!
	if(childList == NULL)
		return block;
	
	// Traverse differently depending on the block type
	if(block->type == BLOCK_WHILE) {
		// Find a child that isn't a TRUE child - otherwise we'll be in an infinite loop
		struct block_list_t *it = childList;
		struct block_t *child = NULL;
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
void cfg_connect_block(struct block_t *b1, struct block_t *b2) {
	if(b1 == NULL || b2 == NULL) {
		//IRLOG(("b1 or b2 was null\n"));
		return;
	}
	IRLOG(("Connecting blocks\nb1: %s\tb2: %s\n",b1->label,b2->label));

	// First block is the root of a CFG. b2 will be linked to the bottom of b1
	struct block_t *bottom = cfg_find_bottom(b1);
	
	//IRLOG(("Bottom block: %s\n",bottom->label));

	// If the bottom is a dummy, replace the dummy block with b2
	if(bottom->entry == NULL) {
		//IRLOG(("Bottom block is a dummy\n"));
		// Add the two labels as aliases
		cfg_add_label_alias(bottom->label,b2->label);

		bottom->children = b2->children;
		bottom->entry = b2->entry;
		bottom->type = b2->type;

		// Drop the dummy and destroy it
		cfg_drop_block_list(&blockList, b2);
		cfg_free_block(b2);
	} else {
		// If the second block is a simple block, drop it
		if(b2->type == BLOCK_SIMPLE) {
			// Bottom isn't a dummy, and is simple, merge the TACs making a maximal basic block
			// Stick the end of b1's TACs to the beginning of b2's TACs
			struct three_address_t *end = bottom->entry;
			while(end->next != NULL)
				end = end->next;
			end->next = b2->entry;
			end->next->prev = end;

			cfg_drop_block_list(&blockList, b2);
			cfg_free_block(b2);
		} else {
			// Update parent and children pointers for the blocks
			// Add b2 as a child to b1
			cfg_append_block_list(&bottom->children,b2);

			// Add b1 as a parent to b2
			cfg_append_block_list(&b2->parents,b1);

			// Add a goto tac node to jump from b1 to b2
			struct three_address_t *gotoTac = cfg_generate_goto_tac(b2->label);
			cfg_connect_tac(bottom->entry,gotoTac);
		}


	}
	
	//Debug
	IRLOG(("Blocks connected\n\n"));
	//cfg_print_blocks();

}

// Makes label2 an alias of label1
void cfg_add_label_alias(char *label1, char *label2) {
	// Go through the label_aliases list to see if there is an entry with label1
	struct set_list_t *it = label_aliases;
	bool found = false;
	while(it != NULL) {
		if(set_contains(it->set,label1)) {
			set_add(it->set,label2);
			found = true;
			//IRLOG(("Found an entry, added alias %s for %s\n",label2,label1));
		}
		it = it->next;
	}
	// Add a new set_list item for this alias pair
	if(!found) {
		//IRLOG(("Did not find an entry, trying to add it\n"));
		it = label_aliases;
		if(it == NULL) {
			it = (struct set_list_t *) malloc(sizeof(struct set_list_t));
			it->set = new_set(label1);
			set_add(it->set,label2);
		} else {
			GOTO_END_OF_LIST(it);
			it->next = (struct set_list_t *) malloc(sizeof(struct set_list_t));
			CHECK_MEM_ERROR(it->next);
			it->next->set = new_set(label1);
			set_add(it->next->set,label2);
		}
		//IRLOG(("Added %s as an alias for %s\n",label2, label1));
	}
}

// Free the list of all label alias sets
void cfg_free_label_aliases() {
	struct set_list_t *it = label_aliases, *cur = NULL;
	while(it != NULL) {
		cur = it;
		it = it->next;
		free_set(cur->set);
		free(cur);
	}
}

// Return a set of all variables referenced or assigned for this block
// Variables include compiler generated temps
void cfg_find_vars(struct block_t *block, struct set_t **cur_vars) {
	if(block == NULL)
		return;

	struct three_address_t *tac = block->entry;
	while(tac != NULL) {
		if(tac->op1 != NULL && tac->op1->type == TAC_DATA_TYPE_VAR) {
			if(*cur_vars == NULL)
				*cur_vars = new_set(tac->op1->d.id);
			else
				set_add(*cur_vars, tac->op1->d.id);
		}
		if(tac->op2 != NULL && tac->op2->type == TAC_DATA_TYPE_VAR) {
			if(*cur_vars == NULL)
				*cur_vars = new_set(tac->op2->d.id);
			else
				set_add(*cur_vars, tac->op2->d.id);
		}
		if(tac->lhs != NULL && tac->lhs->type == TAC_DATA_TYPE_VAR) {
			if(*cur_vars == NULL)
				*cur_vars = new_set(tac->lhs->d.id);
			else
				set_add(*cur_vars, tac->lhs->d.id);
		}
		
		tac = tac->next;
	}
}

// Prints the three address code recursively in the format:
// LHS = OP1 op OP2
// if COND goto LABEL
// goto LABEL
void cfg_print_tac(struct three_address_t *tac) {
	if(tac == NULL) return;
	if(tac == tac->prev) {
		IRLOG(("Infinite loop detected in print tac\n"));
		return;
	}
	
	char *op = op_str(tac->op);

	// Get string representations of lhs, op1 and op2
	char *lhs_str = cfg_tac_data_to_str(tac->lhs);
	char *op1_str = cfg_tac_data_to_str(tac->op1);
	char *op2_str = cfg_tac_data_to_str(tac->op2);

	if(tac->op == OP_ASSIGN)
		printf("\t%s := %s\n", lhs_str, op1_str);
	else if(tac->op == OP_BRANCH)
		printf("\t%s %s %s %s\n", lhs_str, op1_str, "goto", op2_str);
	else if(tac->op == OP_GOTO)
		printf("\t%s %s\n",op, op2_str);
	else if(tac->op == OP_NEW_OBJ)
		printf("\t%s := %s %s\n", lhs_str, op, op1_str);
	else
		printf("\t%s := %s %s %s\n", lhs_str, op1_str, op, op2_str);
		

	free(op);
	free(lhs_str);
	free(op1_str);
	free(op2_str);
	
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
struct tac_data_t *cfg_generate_tac(const char *lhs_id, struct tac_data_t *op1, int op, struct tac_data_t *op2) {
	struct three_address_t *temp_tac = (struct three_address_t *)malloc(sizeof(struct three_address_t));
	CHECK_MEM_ERROR(temp_tac);
	
	if(lhs_id == NULL) {
		temp_tac->lhs = cfg_new_tac_data();
		temp_tac->lhs->type = TAC_DATA_TYPE_VAR;
		temp_tac->lhs->d.id = cfg_new_temp_name();
		temp_tac->lhs->temporary = true;
	} else {
		temp_tac->lhs = cfg_new_tac_data();
		temp_tac->lhs->type = TAC_DATA_TYPE_VAR;
		temp_tac->lhs->d.id = new_identifier(lhs_id);
		temp_tac->lhs->temporary = false;
	}
	temp_tac->op = op;
	temp_tac->op1 = op1;
	temp_tac->op2 = op2;
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
	IRLOG(("Tac node just created: "));
	//cfg_print_tac(temp_tac);

	return temp_tac->lhs;
}

struct three_address_t *cfg_generate_branch_tac(struct tac_data_t *cond, const char *label) {
	struct three_address_t *temp_tac = (struct three_address_t *)malloc(sizeof(struct three_address_t));
	CHECK_MEM_ERROR(temp_tac);
	temp_tac->lhs = TAC_DATA_KEYWORD_IF;
	temp_tac->op = OP_BRANCH;
	temp_tac->op1 = cond;
	temp_tac->op2 = cfg_new_tac_data();
	temp_tac->op2->type = TAC_DATA_TYPE_LABEL;
	temp_tac->op2->d.id = new_identifier(label);
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

struct three_address_t *cfg_generate_goto_tac(const char *label) {
	struct three_address_t *temp_tac = (struct three_address_t *)malloc(sizeof(struct three_address_t));
	CHECK_MEM_ERROR(temp_tac);
	temp_tac->lhs = NULL;
	temp_tac->op = OP_GOTO;
	temp_tac->op1 = NULL;
	temp_tac->op2 = cfg_new_tac_data();
	temp_tac->op2->type = TAC_DATA_TYPE_LABEL;
	temp_tac->op2->d.id = new_identifier(label);
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

// Converts the tac data to a string representation
char *cfg_tac_data_to_str(struct tac_data_t *td) {
	if(td == NULL) return NULL;
	if(td->type == TAC_DATA_TYPE_VAR) {
		return new_identifier(td->d.id);
	}
	if(td->type == TAC_DATA_TYPE_INT) {
		char buffer[32];
		int chars_written = sprintf(buffer, "%d", td->d.val);
		char *retVal = (char *) malloc(chars_written + 1);
		strncpy(retVal, buffer, chars_written + 1);
		return retVal;
	}
	if(td->type == TAC_DATA_TYPE_BOOL) {
		if(td->d.b)
			return new_identifier(BOOLEAN_VALUE_TRUE);
		else
			return new_identifier(BOOLEAN_VALUE_FALSE);
	}
	if(td->type == TAC_DATA_TYPE_LABEL) {
		return new_identifier(td->d.id);
	}
	if(td->type == TAC_DATA_TYPE_KEYWORD) {
		return new_identifier(td->d.id);
	}

	return NULL;
}

// Allocates memory for a new tac_data_t
struct tac_data_t *cfg_new_tac_data() {
	struct tac_data_t *temp_td = (struct tac_data_t *) malloc(sizeof(struct tac_data_t));
	CHECK_MEM_ERROR(temp_td);
	temp_td->type = 0;
	temp_td->d.id = NULL; // Only set this element in the union to NULL

	// Add the tac data to the master listso we can destroy it all later
	if(tacDataList == NULL) {
		tacDataList = (struct tac_data_list_t *) malloc(sizeof(struct tac_data_list_t));
		tacDataList->td = temp_td;
		tacDataList->next = NULL;
	} else {
		struct tac_data_list_t *end = tacDataList;
		GOTO_END_OF_LIST(end);
		end->next = (struct tac_data_list_t *) malloc(sizeof(struct tac_data_list_t));
		end->next->td = temp_td;
		end->next->next = NULL;
	}

	return temp_td;
}

// Free tac_data_t
void cfg_free_tac_data(struct tac_data_t *td) {
	// Free the tac identifier (if it exists)
	if((td->type != TAC_DATA_TYPE_INT && td->type != TAC_DATA_TYPE_BOOL) && td->d.id != NULL) {
		free(td->d.id);
		td->d.id = NULL;
	}
	
	free(td);
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
	// Free the TAC node
	free(tac);
}

// Frees all of the tac data that has been allocated
void cfg_free_tac_data_list() {
	struct tac_data_list_t *it = tacDataList;
	struct tac_data_list_t *cur = tacDataList;
	while(it != NULL) {
		cur = it;
		it = it->next;
		
		// Free the actual tac data object
		cfg_free_tac_data(cur->td);

		// Free the list entry
		free(cur);
	}
	tacDataList = NULL;
}

// Given a temporary TAC name, lookup the TAC node in the master list
// Returns NULL if not found
struct three_address_t *cfg_lookup_tac(const char *id) {
	struct three_address_list_t *it = tacList;
	while(it != NULL) {
		if(strcmp(it->tac->lhs->d.id, id) == 0)
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

	//Update the last connected tac
	lastConnectedTac = tac2;

	IRLOG(("Connected this: \n"));
	//cfg_print_tac(tac1);
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
	//cfg_vnt_hash_insert(TAC_DATA_BOOL_TRUE,cfg_vnt_new_name(),-1);
	//cfg_vnt_hash_insert(TAC_DATA_BOOL_FALSE,cfg_vnt_new_name(),-1);
	//cfg_vnt_hash_insert(TAC_DATA_KEYWORD_IF,cfg_vnt_new_name(),-1);
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
	int chars_written;
	// If the op has the commutative property (PLUS and STAR) then
	// hash using the lowest on the left of the op
	if(op == OP_PLUS || op == OP_STAR) {
		// Concatenate the strings with lowest lexicographically on left
		if(strcmp(op1,op2) < 0) {
			chars_written = sprintf(buffer, "%s%i%s",op1, op, op2);
		} else {
			chars_written = sprintf(buffer, "%s%i%s", op2, op, op1);
		}
	} else {
		// Concatenate the strings to make the hash
		chars_written = sprintf(buffer, "%s%i%s", op1, op, op2);
	}
	temp_name = (char *)malloc(chars_written +1);
	strncpy(temp_name, buffer, chars_written+1);
	return temp_name;
}

/* Hash Table manipulation */
// Does an update/insert on an entry for hash table and returns the newly created entry
struct vnt_entry_t *cfg_vnt_hash_insert(struct tac_data_t *var_td, char *val, struct tac_data_t *val_td, int block_level) {
	IRLOG(("inserting %s\n", val));
	// See if there is already an entry for this node by id
	struct vnt_entry_t *found_entry = cfg_vnt_hash_lookup_td(var_td);
	if(found_entry != NULL) {
		// See if the value is different from what is already on the stack
		if(strcmp(found_entry->vnt_node->val,val) != 0) {
			// Push onto the stack
			cfg_vnt_stack_push(&(found_entry->vnt_node), val, val_td, block_level);
		}
		return found_entry;
	} else {
		// Create the vnt_entry node
		struct vnt_entry_t *temp_entry = (struct vnt_entry_t *)malloc(sizeof(struct vnt_entry_t));
		CHECK_MEM_ERROR(temp_entry);
		temp_entry->var_td = var_td;
		temp_entry->vnt_node = NULL;
		temp_entry->next = NULL;
		
		// Push the vnt_node onto the empty stack
		cfg_vnt_stack_push(&(temp_entry->vnt_node), val, val_td, block_level);

		// Determine the hash key for this node
		char *tdStr = cfg_tac_data_to_str(var_td);
		int hkey = makekey(tdStr, TABLE_SIZE);
		free(tdStr);

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
	struct vnt_entry_t *vnt_entry_it = NULL;
	for(int i = 0; i<TABLE_SIZE; i++) {
		// Go through each entry that is chained at this table entry
		vnt_entry_it = vntable[i];
		while(vnt_entry_it != NULL) {
			if(vnt_entry_it->vnt_node != NULL) {
				if(strcmp(vnt_entry_it->vnt_node->val,val) == 0) {
					//return vntable[i];
					return vnt_entry_it;
				}
			}
			vnt_entry_it = vnt_entry_it->next;
		}
	}
	return NULL;
}

// Looks up a vnt_entry_t by its key
struct vnt_entry_t *cfg_vnt_hash_lookup_td(struct tac_data_t *td) {
	// Get the key that corresponds to this id
	char *tdStr = cfg_tac_data_to_str(td);
	int key = makekey(tdStr, TABLE_SIZE);
	//IRLOG(("Looking for: %s\n",tdStr));
	free(tdStr);

	// Iterate through the elements at vntable[key] until found
	struct vnt_entry_t *it = vntable[key];
	while(it != NULL) {
		//IRLOG(("Looking at entry in hashtable: %s\n",cfg_tac_data_to_str(it->var_td)));
		// Determine equality using the type and data
		// INT
		if(it->var_td->type == TAC_DATA_TYPE_INT && td->type == TAC_DATA_TYPE_INT && it->var_td->d.val == td->d.val) {
			return it;
		}
		// VAR
		if(it->var_td->type == TAC_DATA_TYPE_VAR && td->type == TAC_DATA_TYPE_VAR && strcmp(it->var_td->d.id,td->d.id) == 0){
			return it;
		}
		// BOOL
		if(it->var_td->type == TAC_DATA_TYPE_BOOL && td->type == TAC_DATA_TYPE_BOOL && it->var_td->d.b == td->d.b) {
			return it;
		}
		it = it->next;
	}
	return NULL;
}

// Rolls back each entry of the hash table to entries with a block_level
// less than or equal to that specified
void cfg_vnt_hash_rollback(int block_level) {
	IRLOG(("Rolling back to level %i\n",block_level));
	int i;
	// Rolls back the stacks
	for(i = 0; i<TABLE_SIZE; i++) {
		// Iterate through each chained element
		struct vnt_entry_t *vnt_entry_it = vntable[i];
		while(vnt_entry_it != NULL) {
			// Only do this for entries that are not constant values (int, bool) or keywords (if)
			if(vnt_entry_it->var_td->type != TAC_DATA_TYPE_INT && vnt_entry_it->var_td->type != TAC_DATA_TYPE_BOOL && vnt_entry_it->var_td->type != TAC_DATA_TYPE_KEYWORD) {
				// Pop and free stack nodes that have a greater block level than the desired rollback block level
				while((vnt_entry_it->vnt_node != NULL) && (vnt_entry_it->vnt_node->block_level > block_level)) {
					cfg_vnt_free_node(cfg_vnt_stack_pop(&(vnt_entry_it->vnt_node)));
				}
			}
			vnt_entry_it = vnt_entry_it->next;
		}
	}
	//IRLOG(("Done popping stacks\n"));
	// Cleans up entries where the stack is empty
	for(i = 0; i<TABLE_SIZE; i++) {
		//Iterate through each chained element
		struct vnt_entry_t *it = vntable[i];
		if(it != NULL) {
			// Prune the front of the list
			while(it != NULL && it->vnt_node == NULL) {
				struct vnt_entry_t **entry = &vntable[i];
				*entry = (*entry)->next;
				it = it->next;
			}
			if(it == NULL) continue;
			// Check the chained elements (front of list has been pruned)
			while(it->next != NULL) {
				// If the vnt_node is null after rollback, the entry should be removed
				if(it->next->vnt_node == NULL) {
					struct vnt_entry_t **entry = &vntable[i];
					*entry = (*entry)->next;
					//IRLOG(("Removed.\n"));
				}
				it = it->next;
			}
		}
	}
	
	IRLOG(("Rolled back to level %i!\n",block_level));
}

// Free up memory allocated for a hash entry
void cfg_vnt_free_entry(struct vnt_entry_t *entry) {
	if(entry == NULL)
		return;

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
void cfg_vnt_stack_push(struct vnt_node_t **stack, char *val, struct tac_data_t *val_td, int block_level) {
	// Make the node
	struct vnt_node_t *temp_node = (struct vnt_node_t *) malloc(sizeof(struct vnt_node_t));
	CHECK_MEM_ERROR(temp_node);
	temp_node->val = new_identifier(val);
	temp_node->val_td = val_td;
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
	if(node == NULL)
		return;

	// Free the val
	if (node->val != NULL)
		free(node->val);
	if (node->pretty_name != NULL)
		free(node->pretty_name);


	// Free the pointer
	free(node);
}



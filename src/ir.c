/*
 * ir.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Implements functions for Intermediate Representation
 */

#include "ir.h"

// Initialize the control flow graph and any state variables
void ir_init() {
	allBlocks = NULL;
	block_counter = 0;
	name_counter = 0;
	rootBlock = ir_create_block(NULL);
}

// Destroy any objects used for intermediate representation
void ir_destroy() {
	// Loop through the master list and free each basic block
	struct basic_block_t *it = allBlocks;
	struct basic_block_t *cur = allBlocks;
	while(it != NULL) {
		cur = it;
		it = it->next;
		ir_free_block(cur);
	}
	
	allBlocks = NULL;
	block_counter = 0;
	name_counter = 0;
	rootBlock = NULL;
}

// Create a new basic block node and set it's TAC entry point
struct basic_block_t *ir_create_block(struct three_address_t *entry) {
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
void ir_free_block(struct basic_block_t *block) {
	// Free TAC list
	struct three_address_t *it = block->entry;
	struct three_address_t *cur = block->entry;
	while(it != NULL) {
		cur = it;
		it = it->next;
		ir_free_tac(cur);
	}
	
	// Free the basic block
	free(block);
}

// Generate a TAC node
struct three_address_t *ir_generate_tac(char *lhs_id, int op, char *op1, char *op2) {
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
void ir_free_tac(struct three_address_t *tac) {
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
char *ir_new_temp_name() {
	char *temp_name = NULL;
	char buffer[8];
	
	// Create a name
	int chars_written = sprintf(buffer, "t_%i", name_counter);
	temp_name = (char*)malloc(chars_written + 1);
	strncpy(temp_name, buffer, chars_written+1);
	name_counter++;
	
	return temp_name;
}


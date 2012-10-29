/*
 * ir.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Implements functions to manipulate Intermediate Representation
 */

#include "ir.h"

// Start value numbering on the CFG
void ir_value_numbering() {
	// Work list contains list of blocks that have more than one parent that needs an additional pass by ir_evn
	workList = cfg_new_block_list(cfg_get_root());
	
	// Recursively perform extended value numbering on all nodes in the workList
	// workList will expand on the first pass through the root node
	struct basic_block_list_t *it = workList;
	while(it != NULL) {
		// Process the current node in the workList
		ir_evn(it);
		
		// Roll back numbering all the way to the start
		cfg_vnt_hash_rollback(-1);
	
		it = it->next;
	}
	
	// Clean up the workList
	cfg_free_block_list(&workList, false);
}

// Recursive worker function to perform extended value numbering (evn)
void ir_evn(struct basic_block_t *block) {
	// Number each TAC and insert it into the VNT
	struct three_address_t *tac = block->entry;
	while(tac != NULL) {
		cfg_vnt_hash_insert(cfg_vnt_new_name(), cfg_vnt_hash(tac->op, tac->op1, tac->op2), block->block_level);
		tac = tac->next;
	}
	
	// Go through the children of this block and perform numbering
	struct basic_block_list_t *child = block->children;
	while(child != NULL) {
		// Value number the current block
		ir_evn(child->block);
		
		// Roll back the numbering before moving to the next sibling
		cfg_vnt_hash_rollback(child->block->block_level);
		
		// If the child has multiple parents, add it the workList for later
		if(cfg_block_list_size(child->parents) > 1)
			cfg_append_block_list(&workList, child->block);
		
		child = child->next;
	}
}

void ir_optimize() {

}


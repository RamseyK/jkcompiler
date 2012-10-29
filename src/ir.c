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
		ir_evn(it->block, 0);
		
		// Roll back numbering all the way to the start
		cfg_vnt_hash_rollback(-1);
	
		it = it->next;
	}
	
	// Clean up the workList
	cfg_free_block_list(&workList, false);
}

// Recursive worker function to perform extended value numbering (evn)
void ir_evn(struct basic_block_t *block, int block_level) {
	// Don't value number for IF and WHILE blocks because their TAC nodes have already been numbered in the parent blocks
	if(block->type != BLOCK_IF && block->type != BLOCK_WHILE) {
		// Number each TAC and insert it into the VNT
		struct three_address_t *tac = block->entry;
		while(tac != NULL) {
			char *k_op1 = cfg_vnt_new_name();
			char *op = op_str(tac->op);
			char *k_op2 = cfg_vnt_new_name();
			char *k_lhs = cfg_vnt_hash(tac->op1, tac->op, tac->op2);
			cfg_vnt_hash_insert(k_op1, tac->op1, block_level);
			cfg_vnt_hash_insert(k_op2, tac->op2, block_level);
			cfg_vnt_hash_insert(k_lhs, tac->lhs_id, block_level);
			
			// Pretty print the value numbered TAC
			struct vnt_entry_t *e_op1 = cfg_vnt_hash_lookup_key(k_op1);
			struct vnt_entry_t *e_op2 = cfg_vnt_hash_lookup_key(k_op2);
			struct vnt_entry_t *e_lhs = cfg_vnt_hash_lookup_key(k_lhs);
			if(e_op1 != NULL && e_op2 != NULL && e_lhs != NULL) {
				IRLOG(("%s = %s %s %s\n", e_lhs->pretty_name, e_op1->pretty_name, op, e_op2->pretty_name));
			}
			free(op);
			
			tac = tac->next;
		}
	}
	
	// Go through the children of this block and perform numbering
	struct basic_block_list_t *child = block->children;
	while(child != NULL) {
		// Value number the current child block
		ir_evn(child->block, block_level+1);
		
		// Roll back the numbering before moving to the next sibling
		cfg_vnt_hash_rollback(block_level);
		
		// If the child has multiple parents, add it the workList for later
		if(cfg_block_list_size(&child->block->parents) > 1)
			cfg_append_block_list(&workList, child->block);
		
		child = child->next;
	}
}

void ir_optimize() {

	// Print the CFG blocks
	printf("\nPrint Blocks:\n");
	cfg_print_blocks();
	printf("\n");
	printf("\nPrint variables and TAC:\n");
	cfg_print_vars_tac();
	printf("\n");

}


/*
 * ir.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Implements functions to manipulate Intermediate Representation
 */

#include "ir.h"

void ir_init() {
	workList = NULL;
	cfg_vnt_init(); // Initialize value numbering table
}

void ir_destroy() {
	cfg_vnt_destroy();
}

// Start value numbering on the CFG
void ir_value_numbering() {
	// Clear the output buffer
	memset(ir_vnt_out_buffer, 0, sizeof(ir_vnt_out_buffer));

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
			char *op = op_str(tac->op);
			
			struct vnt_entry_t *e_op1 = NULL, *e_op2 = NULL, *e_lhs = NULL;
			char *v_op1 = NULL, *v_op2 = NULL, *v_lhs = NULL; // Numbering values
			
			// Lookup op1 in the VNT
			e_op1 = cfg_vnt_hash_lookup_id(tac->op1);
			if(e_op1 == NULL) {
				// op1 doesn't exist, insert it with a numbering value
				v_op1 = cfg_vnt_new_name();
				cfg_vnt_hash_insert(tac->op1, v_op1, block_level);
			} else {
				// op1 is already in the VNT, copy the numbering value
				v_op1 = e_op1->vnt_node->val;
			}
			
			// Case: a = b (no op2)
			if(tac->op == OP_NO_OP) {
				// Copy the value numbering from the right hand side to the left hand side (simple assignment a = b)
				v_lhs = v_op1;
				cfg_vnt_hash_insert(tac->lhs_id, v_lhs, block_level);
			} else { // Case: a = b + c (op2 exists)
				// Lookup op2 in the VNT
				e_op2 = cfg_vnt_hash_lookup_id(tac->op2);
				if(e_op2 == NULL) {
					// op2 doesn't exist, insert it with a numbering value
					v_op2 = cfg_vnt_new_name();
					cfg_vnt_hash_insert(tac->op2, v_op2, block_level);
				} else {
					// op2 is already in the VNT, copy the numbering value
					v_op2 = e_op2->vnt_node->val;
				}
				
				// Lookup the lhs_id in the VNT
				e_lhs = cfg_vnt_hash_lookup_id(tac->lhs_id);
				if(e_lhs == NULL) {
					// lhs_id doesn't exist, insert it with a numbering value
					v_lhs = cfg_vnt_hash(v_op1, tac->op, v_op2);
					cfg_vnt_hash_insert(tac->lhs_id, v_lhs, block_level);
				} else {
					// lhs_id is already in the VNT, copy the numbering value
					v_lhs = e_lhs->vnt_node->val;
				}
			}

			// Lookup the latest entries in the VNT for each component of the TAC if we don't have it already
			if(e_op1 == NULL)
				e_op1 = cfg_vnt_hash_lookup_val(v_op1);
			if(e_op2 == NULL && v_op2 != NULL)
				e_op2 = cfg_vnt_hash_lookup_val(v_op2);
			if(e_lhs == NULL)
				e_lhs = cfg_vnt_hash_lookup_val(v_lhs);
			
			// Pretty print the value numbered TAC
			if(tac->op == OP_NO_OP) {
				sprintf(ir_vnt_out_buffer, "%s%s = %s\n", ir_vnt_out_buffer, e_lhs->vnt_node->pretty_name, e_op1->vnt_node->pretty_name);
			} else {
				sprintf(ir_vnt_out_buffer, "%s%s = %s %s %s\n", ir_vnt_out_buffer, e_lhs->vnt_node->pretty_name, e_op1->vnt_node->pretty_name, op, e_op2->vnt_node->pretty_name);
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

void ir_print_vnt() {
	printf("%s", ir_vnt_out_buffer);
}

void ir_optimize() {
}


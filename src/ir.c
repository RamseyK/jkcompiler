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
	struct block_list_t *it = workList;
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
void ir_evn(struct block_t *block, int block_level) {
	// Don't value number for IF and WHILE blocks because their TAC nodes have already been numbered in the parent blocks
	if(block->type != BLOCK_IF && block->type != BLOCK_WHILE) {
		// Mark the block in the output
		sprintf(ir_vnt_out_buffer, "%s%s:\n", ir_vnt_out_buffer, block->label);
	
		// Number each TAC and insert it into the VNT
		struct three_address_t *tac = block->entry;
		while(tac != NULL) {
			char *op = op_str(tac->op);
			
			struct vnt_entry_t *e_op1 = NULL, *e_op2 = NULL, *e_lhs = NULL;
			char *v_op1 = NULL, *v_op2 = NULL, *v_lhs = NULL; // Numbering values
			
			// Lookup op1 in the VNT
			IRLOG(("tac op1: %s\n", cfg_tac_data_to_str(tac->op1)));
			e_op1 = cfg_vnt_hash_lookup_td(tac->op1);
			if(e_op1 == NULL) {
				// op1 doesn't exist, insert it with a numbering value
				IRLOG(("op1 insert\n"));
				v_op1 = cfg_vnt_new_name();
				e_op1 = cfg_vnt_hash_insert(tac->op1, v_op1, block_level);
			} else {
				// op1 is already in the VNT, copy the numbering value
				IRLOG(("op1 found\n"));
				if(e_op1->vnt_node == NULL) {
					IRLOG(("%s: %s->vnt_node is null\n",block->label,cfg_tac_data_to_str(e_op1->tacData)));
				}
				v_op1 = e_op1->vnt_node->val;
			}
			
			// Case: a = b (no op2)
			if(tac->op == OP_NO_OP) {
				// Copy the value numbering from the right hand side to the left hand side (simple assignment a = b)
				IRLOG(("lhs insert (nop)\n"));
				v_lhs = v_op1;
				e_lhs = cfg_vnt_hash_insert(tac->lhs, v_lhs, block_level);
			} else { // Case: a = b + c (op2 exists)
				// Lookup op2 in the VNT
				IRLOG(("tac op2: %s\n", cfg_tac_data_to_str(tac->op2)));
				e_op2 = cfg_vnt_hash_lookup_td(tac->op2);
				if(e_op2 == NULL) {
					// op2 doesn't exist, insert it with a numbering value
					IRLOG(("op2 insert\n"));
					v_op2 = cfg_vnt_new_name();
					e_op2 = cfg_vnt_hash_insert(tac->op2, v_op2, block_level);
				} else {
					// op2 is already in the VNT, copy the numbering value
					IRLOG(("op2 found\n"));
					v_op2 = e_op2->vnt_node->val;
				}
				
				// Lookup the lhs_id in the VNT
				IRLOG(("tac lhs: %s\n", cfg_tac_data_to_str(tac->lhs)));
				e_lhs = cfg_vnt_hash_lookup_td(tac->lhs);
				if(e_lhs == NULL) {
					// lhs_id doesn't exist, insert it with a numbering value
					IRLOG(("lhs insert\n"));
					v_lhs = cfg_vnt_hash(v_op1, tac->op, v_op2);
					
					// Lookup the v_lhs to check for previous computations of the TAC right hand side
					struct vnt_entry_t *e_lhs_exist = cfg_vnt_hash_lookup_val(v_lhs);
					
					// Previous computation existed, perform optimization
					if(e_lhs_exist != NULL) {
						// Optimize the TAC node
						tac->op1 = e_lhs_exist->tacData;
						tac->op = OP_NO_OP;
						free(tac->op2);
						tac->op2 = NULL;
						
						// Lookup the new op1
						e_op1 = cfg_vnt_hash_lookup_td(tac->op1);
						
						// Insert the optimized left hand side
						e_lhs = cfg_vnt_hash_insert(tac->lhs, v_lhs, block_level);
					} else {
						e_lhs = cfg_vnt_hash_insert(tac->lhs, v_lhs, block_level);
					}
				} else {
					// lhs_id is already in the VNT, copy the numbering value
					IRLOG(("lhs found\n"));
					v_lhs = e_lhs->vnt_node->val;
				}
			}
			
			// Pretty print the value numbered TAC
			if(tac->op == OP_NO_OP) {
				sprintf(ir_vnt_out_buffer, "%s\t%s(%s) := %s(%s)\n", ir_vnt_out_buffer, cfg_tac_data_to_str(tac->lhs), e_lhs->vnt_node->pretty_name, cfg_tac_data_to_str(tac->op1), e_op1->vnt_node->pretty_name);
				IRLOG(("%s", ir_vnt_out_buffer));
			} else if(tac->op == OP_GOTO) {
				sprintf(ir_vnt_out_buffer, "%s\t%s(%s)(%s(%s)) %s %s(%s)\n", ir_vnt_out_buffer, cfg_tac_data_to_str(tac->lhs), e_lhs->vnt_node->pretty_name, cfg_tac_data_to_str(tac->op1), e_op1->vnt_node->pretty_name, op, cfg_tac_data_to_str(tac->op2), e_op2->vnt_node->pretty_name);
				IRLOG(("%s", ir_vnt_out_buffer));
			} else {
				sprintf(ir_vnt_out_buffer, "%s\t%s(%s) := %s(%s) %s %s(%s)\n", ir_vnt_out_buffer, cfg_tac_data_to_str(tac->lhs), e_lhs->vnt_node->pretty_name, cfg_tac_data_to_str(tac->op1), e_op1->vnt_node->pretty_name, op, cfg_tac_data_to_str(tac->op2), e_op2->vnt_node->pretty_name);
				IRLOG(("%s", ir_vnt_out_buffer));
			}
			
			free(op);
			
			tac = tac->next;
		}
		sprintf(ir_vnt_out_buffer, "%s\n", ir_vnt_out_buffer);
	}
	
	// Go through the children of this block and perform numbering
	struct block_list_t *child = block->children;
	while(child != NULL) {
		// Child has multiple parents
		if(cfg_block_list_size(&child->block->parents) > 1) {
			// Base case, Add to the work list for later if it doesn't already exist
			if(!cfg_exists_in_block_list(&workList, child->block))
				cfg_append_block_list(&workList, child->block);
			
			// Roll back the numbering before moving to the next sibling
			cfg_vnt_hash_rollback(block_level);
		} else {
			// Value number the current child block recursively
			ir_evn(child->block, block_level+1);
		}
		
		child = child->next;
	}
}

void ir_print_vnt() {
	printf("%s", ir_vnt_out_buffer);
}

void ir_optimize() {

	ir_resolve_label_aliases();

    printf("\nPrint variables and TAC:\n");
	cfg_print_vars_tac();
	printf("\n");

	printf("\nPrint Blocks:\n");
	cfg_print_blocks();
	printf("\n");

	ir_value_numbering();

	printf("\nPrint value numbering:\n");
	ir_print_vnt();
	printf("\n");
}

// Changes the target for any jump/branch tac nodes to a single block
// If that block had an alias added in label_aliases
void ir_resolve_label_aliases() {
	// Go through each tac node
	struct three_address_list_t *tac_it = tacList;
	while(tac_it != NULL) {
		// If the tac is a jump tac
		if(tac_it->tac->op == OP_GOTO) {
			// See if its label is in the alias list
			struct set_list_t *al_it = label_aliases;
			while(al_it != NULL) {
				// If the label is in the set
				if(set_contains(al_it->set,tac_it->tac->op2->d.id)) {
					// Free the old op2
					free(tac_it->tac->op2);
					// Set to the first element in the alias set
					tac_it->tac->op2->d.id = new_identifier(al_it->set->value);
					break;
				}
				al_it = al_it->next;
			}
		}
		tac_it = tac_it->next;
	}
}


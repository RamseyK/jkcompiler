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
	
	// Clear the output buffers
	memset(ir_vnt_out_buffer, 0, sizeof(ir_vnt_out_buffer));
	memset(ir_opt_const_out_buffer, 0, sizeof(ir_opt_const_out_buffer));
}

void ir_destroy() {
	cfg_vnt_destroy();
}

// Substitues values of known constants in expressions at compile time before value numbering on a TAC
void ir_opt_const_propagation(struct three_address_t *tac) {
	struct vnt_entry_t *entry = NULL, *repEntry = NULL;
	
	char *op = op_str(tac->op);

	// Lookup each tac data element in the VNT to determine if theres a value for it
	if(tac->op1 != NULL && tac->op1->type == TAC_DATA_TYPE_VAR) {
		entry = cfg_vnt_hash_lookup_td(tac->op1);
		
		// If the tac data for op1 (TAC_VAR) is already in the table, replace this tac's op1 with the corresponding constant value (TAC_INT)
		if(entry != NULL) {
			// Output the expression being propagated to ir_opt_const_out_buffer
			char *lhs_str = cfg_tac_data_to_str(tac->lhs), *op1_str = cfg_tac_data_to_str(tac->op1), *op2_str = cfg_tac_data_to_str(tac->op2);
			sprintf(ir_opt_const_out_buffer, "%s\tProp: %s := %s %s %s => ", ir_opt_const_out_buffer, lhs_str, op1_str, op, (tac->op2 != NULL ? op2_str : ""));
			free(op1_str);
			if(op2_str != NULL)
				free(op2_str);
			
			if(entry->vnt_node->val_td->type == TAC_DATA_TYPE_INT) { // Stack node is an INT
				repEntry = cfg_vnt_hash_lookup_td(entry->vnt_node->val_td); // Lookup the entry for the actual value
				tac->op1 = repEntry->vnt_node->val_td; // Replace op1 with the tac data of the looked up entry
			} else if(entry->vnt_node->val_td->type == TAC_DATA_TYPE_BOOL) {
				repEntry = cfg_vnt_hash_lookup_td(entry->vnt_node->val_td);
				tac->op1 = repEntry->vnt_node->val_td;
			} else {
				// Case where the value isn't known at compile time. Nothing to propagate
				//IRLOG(("Cant prop %s with value of %s of type %i\n", op1_str, cfg_tac_data_to_str(entry->vnt_node->val_td), entry->vnt_node->val_td->type));
			}
			
			// Output the expression after propagation to the ir_opt_const_out_buffer
			op1_str = cfg_tac_data_to_str(tac->op1);
			op2_str = cfg_tac_data_to_str(tac->op2);
			sprintf(ir_opt_const_out_buffer, "%s%s := %s %s %s\n", ir_opt_const_out_buffer, lhs_str, op1_str, op, (tac->op2 != NULL ? op2_str : ""));
			free(lhs_str);
			free(op1_str);
			if(op2_str != NULL)
				free(op2_str);
		}
	}
	
	if(tac->op2 != NULL && tac->op2->type == TAC_DATA_TYPE_VAR) {
		entry = cfg_vnt_hash_lookup_td(tac->op2);
	
		// If the tac data for op2 is already in the table, replace this tac's op2 with the corresponding constant value
		if(entry != NULL) {
			// Output the expression being propagated to ir_opt_const_out_buffer
			char *lhs_str = cfg_tac_data_to_str(tac->lhs), *op1_str = cfg_tac_data_to_str(tac->op1), *op2_str = cfg_tac_data_to_str(tac->op2);
			sprintf(ir_opt_const_out_buffer, "%s\tProp: %s := %s %s %s => ", ir_opt_const_out_buffer, lhs_str, op1_str, op, (tac->op2 != NULL ? op2_str : ""));
			if(op1_str != NULL)
				free(op1_str);
			free(op2_str);
			
			if(entry->vnt_node->val_td->type == TAC_DATA_TYPE_INT) {
				repEntry = cfg_vnt_hash_lookup_td(entry->vnt_node->val_td);
				tac->op2 = repEntry->vnt_node->val_td;
			} else if(entry->vnt_node->val_td->type == TAC_DATA_TYPE_BOOL) {
				repEntry = cfg_vnt_hash_lookup_td(entry->vnt_node->val_td);
				tac->op2 = repEntry->vnt_node->val_td;
			} else {
				// Case where the value isn't known at compile time. Nothing to propagate
			}

			// Output the expression after propagation to the ir_opt_const_out_buffer
			op1_str = cfg_tac_data_to_str(tac->op1);
			op2_str = cfg_tac_data_to_str(tac->op2);
			sprintf(ir_opt_const_out_buffer, "%s%s := %s %s %s\n", ir_opt_const_out_buffer, lhs_str, op1_str, op, (tac->op2 != NULL ? op2_str : ""));
			free(lhs_str);
			if(op1_str != NULL)
				free(op1_str);
			free(op2_str);
		}
	}
	
	free(op);
}

// Simplifies constant expressions into a single assignment before value numbering is performed on a TAC
// Should be performed after const_propogation
void ir_opt_const_folding(struct three_address_t *tac) {
	// No optimization to perform
	if(tac->op1 == NULL || tac->op2 == NULL)
		return;
	if(tac->op1->type == TAC_DATA_TYPE_LABEL || tac->op2->type == TAC_DATA_TYPE_LABEL || tac->op1->type == TAC_DATA_TYPE_KEYWORD || tac->op2->type == TAC_DATA_TYPE_KEYWORD)
		return;
	
	// Output the expression being folded to ir_opt_const_out_buffer
	char *pre_op = op_str(tac->op);
	char *lhs_str = cfg_tac_data_to_str(tac->lhs), *op1_str = cfg_tac_data_to_str(tac->op1), *op2_str = cfg_tac_data_to_str(tac->op2);
	sprintf(ir_opt_const_out_buffer, "%s\tFold: %s := %s %s %s => ", ir_opt_const_out_buffer, lhs_str, op1_str, pre_op, op2_str);
	free(pre_op);
	free(op1_str);
	free(op2_str);
	
	struct tac_data_t *new_td = cfg_new_tac_data();

	// Tac datas of the same type
	if(tac->op1->type == TAC_DATA_TYPE_INT && tac->op2->type == TAC_DATA_TYPE_INT) {
		// Simplify the operation between two int constants
		// Test Program: test_tac_consts.p
		
		switch(tac->op) {
			case OP_PLUS:
				new_td->type = TAC_DATA_TYPE_INT;
				new_td->d.val = tac->op1->d.val + tac->op2->d.val;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_MINUS:
				new_td->type = TAC_DATA_TYPE_INT;
				new_td->d.val = tac->op1->d.val - tac->op2->d.val;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_STAR:
				new_td->type = TAC_DATA_TYPE_INT;
				new_td->d.val = tac->op1->d.val * tac->op2->d.val;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_SLASH:
				if(tac->op2->d.val != 0) { // Check for divide by 0
					new_td->type = TAC_DATA_TYPE_INT;
					new_td->d.val = tac->op1->d.val / tac->op2->d.val;
					tac->op1 = new_td;
					tac->op = OP_ASSIGN;
					tac->op2 = NULL;
				}
				break;
			case OP_MOD:
				if(tac->op2->d.val != 0) { // Check for divide by 0
					new_td->type = TAC_DATA_TYPE_INT;
					new_td->d.val = tac->op1->d.val % tac->op2->d.val;
					tac->op1 = new_td;
					tac->op = OP_ASSIGN;
					tac->op2 = NULL;
				}
				break;
			case OP_EQUAL:
				new_td->type = TAC_DATA_TYPE_BOOL;
				new_td->d.b = tac->op1->d.val == tac->op2->d.val;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_NOTEQUAL:
				new_td->type = TAC_DATA_TYPE_BOOL;
				new_td->d.b = tac->op1->d.val != tac->op2->d.val;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_LT:
				new_td->type = TAC_DATA_TYPE_BOOL;
				new_td->d.b = tac->op1->d.val < tac->op2->d.val;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_GT:
				new_td->type = TAC_DATA_TYPE_BOOL;
				new_td->d.b = tac->op1->d.val > tac->op2->d.val;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_LE:
				new_td->type = TAC_DATA_TYPE_BOOL;
				new_td->d.b = tac->op1->d.val <= tac->op2->d.val;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_GE:
				new_td->type = TAC_DATA_TYPE_BOOL;
				new_td->d.b = tac->op1->d.val >= tac->op2->d.val;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			default:
				break;
		}
	} else if(tac->op1->type == TAC_DATA_TYPE_BOOL && tac->op2->type == TAC_DATA_TYPE_BOOL) {
		// Simplify the operation between two boolean constants
		// Test Program: test_tac_consts_bool.p
		
		switch(tac->op) {
			case OP_OR:
				new_td->type = TAC_DATA_TYPE_BOOL;
				new_td->d.b = tac->op1->d.b || tac->op2->d.b;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_AND:
				new_td->type = TAC_DATA_TYPE_BOOL;
				new_td->d.b = tac->op1->d.b && tac->op2->d.b;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_EQUAL:
				new_td->type = TAC_DATA_TYPE_BOOL;
				new_td->d.b = tac->op1->d.b == tac->op2->d.b;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_NOTEQUAL:
				new_td->type = TAC_DATA_TYPE_BOOL;
				new_td->d.b = tac->op1->d.b != tac->op2->d.b;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			default:
				break;
		}
	} else if(tac->op1->type == TAC_DATA_TYPE_VAR && tac->op2->type == TAC_DATA_TYPE_VAR) {
		// Two vars of unknown value will be optimized at a higher arithmetic level
		// Test Program: test_tac_consts_var.p
		
		if(tac->op == OP_PLUS && strcmp(tac->op1->d.id, tac->op2->d.id) == 0) {
			// Add to itself. a+a => 2*a
			new_td->type = TAC_DATA_TYPE_INT;
			new_td->d.val = 2;
			tac->op1 = new_td;
			tac->op = OP_STAR;
		} else if(tac->op == OP_MINUS && strcmp(tac->op1->d.id, tac->op2->d.id) == 0) {
			// Subtract from itself. a-a => 0
			new_td->type = TAC_DATA_TYPE_INT;
			new_td->d.val = 0;
			tac->op1 = new_td;
			tac->op = OP_ASSIGN;
			tac->op2 = NULL;
		} else if(tac->op == OP_SLASH && strcmp(tac->op1->d.id, tac->op2->d.id) == 0) {
			// Divide by itself. a/a => 1
			new_td->type = TAC_DATA_TYPE_INT;
			new_td->d.val = 1;
			tac->op1 = new_td;
			tac->op = OP_ASSIGN;
			tac->op2 = NULL;
		} else {
		}
	} else if(tac->op1->type == TAC_DATA_TYPE_INT || tac->op2->type == TAC_DATA_TYPE_INT) {
		// One var is an integer
		// Test Program: test_tac_consts_var.p
		
		if(tac->op == OP_STAR) {
			if((tac->op1->type == TAC_DATA_TYPE_INT && tac->op1->d.val == 0) || (tac->op2->type == TAC_DATA_TYPE_INT && tac->op2->d.val == 0)) {
				// Multiply by 0
				new_td->type = TAC_DATA_TYPE_INT;
				new_td->d.val = 0;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
			} else if(tac->op1->type == TAC_DATA_TYPE_INT && tac->op2->type == TAC_DATA_TYPE_VAR && tac->op1->d.val == 1) {
				// op1 is 1, op2 is a VAR. 1*a = a
				new_td->type = TAC_DATA_TYPE_VAR;
				new_td->d.id = new_identifier(tac->op2->d.id);
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
			} else if(tac->op1->type == TAC_DATA_TYPE_VAR && tac->op2->type == TAC_DATA_TYPE_INT && tac->op2->d.val == 1) {
				// op2 is 1. a*1 = a
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
			} else {
			}
		}
	} else {
	
	}
	
	// Output the expression after the fold to ir_opt_const_out_buffer
	char *post_op = op_str(tac->op);
	op1_str = cfg_tac_data_to_str(tac->op1);
	op2_str = cfg_tac_data_to_str(tac->op2); // May be null b/c op2 might have been removed
	sprintf(ir_opt_const_out_buffer, "%s%s := %s %s %s\n", ir_opt_const_out_buffer, lhs_str, op1_str, (tac->op != OP_ASSIGN ? post_op : ""), (tac->op2 != NULL ? op2_str : ""));
	free(post_op);
	free(lhs_str);
	free(op1_str);
	if(op2_str != NULL)
		free(op2_str);
}

// Dead code eliminitation should remove any unused temporary tac vars and unreachable code
// Called after value numbering for a block
void ir_opt_dead_code_elim(struct block_t *block) {
	// This will get rid of a lot of the unnecessary temp variables that have been simplified to constants
	struct three_address_t *tac = block->entry;

	while(tac != NULL) {
		// Found the tac was a temporary. If it equals a constant, unlink it it
		if(tac->op == OP_ASSIGN && tac->lhs->temporary) {
			//IRLOG(("Unused temp var, Removing %s\n", cfg_tac_data_to_str(tac->lhs)));
			if(tac->next != NULL)
				tac->next->prev = tac->prev;
			
			if(tac->prev != NULL)
				tac->prev->next = tac->next;
			
			if(block->entry == tac)
				block->entry = tac->next;
		}
		tac = tac->next;
	}
	
	// TODO: Remove impossible IF/WHILE branches
	
	// TODO: Remove temporary variables that aren't used more than once and make a replacement where the temporary was used
	// Example: t = c+1; c = t; -> c = c+1;
}

// Start processing of IR and optimization on the entire cfg
void ir_process_cfg(struct cfg_list_t *cfg) {
	// Work list contains list of blocks that have more than one parent that needs an additional pass by ir_process_block
	workList = cfg_new_block_list(cfg->entryBlock);
	
	// Recursively process all nodes in the workList
	// workList will expand on the first pass through the root node
	struct block_list_t *it = workList;
	while(it != NULL) {
		// Process the current node in the workList
		ir_block_pass(it->block, 0);
		
		// Roll back numbering all the way to the start
		cfg_vnt_hash_rollback(-1);
	
		it = it->next;
	}
	
	// Clean up the workList
	cfg_free_block_list(&workList, false);
}

// Recursive worker function to perform a single processing / optimization pass of a basic block in the CFG
void ir_block_pass(struct block_t *block, int block_level) {
	// Don't look at IF and WHILE blocks because their TAC nodes have already been value numbered and processed in the parent blocks
	if(block->type != BLOCK_IF && block->type != BLOCK_WHILE) {
		// Mark the block in the output
		sprintf(ir_vnt_out_buffer, "%s%s:\n", ir_vnt_out_buffer, block->label);

		// Loop through each TAC in the block
		struct three_address_t *tac = block->entry;
		while(tac != NULL) {
			if(tac->op != OP_BRANCH && tac->op != OP_GOTO) {
				// Perform possible constant optimizations before value numbering the tac
				ir_opt_const_propagation(tac);
				ir_opt_const_folding(tac);
				
				// Value number the tac node
				ir_value_number_tac(tac, block_level);
			}
			tac = tac->next;

		}
		sprintf(ir_vnt_out_buffer, "%s\n", ir_vnt_out_buffer);
		
		// Perform dead code elimintation after a pass through the blocks TAC nodes
		ir_opt_dead_code_elim(block);
	}
	
	// Go through the children of this block and perform processing
	struct block_list_t *child = block->children;
	while(child != NULL) {
		// Child has multiple parents
		if(cfg_block_list_size(&child->block->parents) > 1) {
			// Base case, Add to the work list for later if it doesn't already exist
			if(!cfg_exists_in_block_list(&workList, child->block)) {
				cfg_append_block_list(&workList, child->block);
				IRLOG(("Adding %s to the worklist\n", child->block->label));
			}
			
			// Roll back the numbering before moving to the next sibling
			cfg_vnt_hash_rollback(block_level);
		} else {
			// Process current child block recursively
			ir_block_pass(child->block, block_level+1);
		}
		
		child = child->next;
	}
}

// Perform value numbering on a single tac at a specific block level
void ir_value_number_tac(struct three_address_t *tac, int block_level) {
	char *op = op_str(tac->op);

	struct vnt_entry_t *e_op1 = NULL, *e_op2 = NULL, *e_lhs = NULL;
	char *v_op1 = NULL, *v_op2 = NULL, *v_lhs = NULL; // Numbering values

	// Lookup op1 in the VNT
	e_op1 = cfg_vnt_hash_lookup_td(tac->op1);
	if(e_op1 == NULL) {
		// op1 doesn't exist, insert it with a numbering value
		IRLOG(("op1 insert\n"));
		v_op1 = cfg_vnt_new_name();
		e_op1 = cfg_vnt_hash_insert(tac->op1, v_op1, tac->op1, block_level);
	} else {
		// op1 is already in the VNT, copy the numbering value
		IRLOG(("op1 found\n"));
		if(e_op1->vnt_node == NULL)
			IRLOG(("vnt_node is null\n"));
		v_op1 = new_identifier(e_op1->vnt_node->val);
	}

	// Case: a = b (no op2)
	if(tac->op == OP_ASSIGN) {
		// Copy the value numbering from the right hand side to the left hand side (simple assignment a = b)
		IRLOG(("lhs insert (nop)\n"));
		v_lhs = new_identifier(v_op1);
		e_lhs = cfg_vnt_hash_insert(tac->lhs, v_lhs, tac->op1, block_level);
	} else { // Case: a = b + c (op2 exists)
		// Lookup op2 in the VNT
		IRLOG(("tac op2\n"));
		e_op2 = cfg_vnt_hash_lookup_td(tac->op2);
		if(e_op2 == NULL) {
			// op2 doesn't exist, insert it with a numbering value
			IRLOG(("op2 insert\n"));
			v_op2 = cfg_vnt_new_name();
			e_op2 = cfg_vnt_hash_insert(tac->op2, v_op2, tac->op2, block_level);
		} else {
			// op2 is already in the VNT, copy the numbering value
			IRLOG(("op2 found\n"));
			v_op2 = new_identifier(e_op2->vnt_node->val);
		}

		// Lookup the lhs_id in the VNT
		e_lhs = cfg_vnt_hash_lookup_td(tac->lhs);
		if(e_lhs == NULL) {
			// lhs_id doesn't exist, insert it with a numbering value
			IRLOG(("lhs insert\n"));
			v_lhs = cfg_vnt_hash(v_op1, tac->op, v_op2);

			// Lookup the v_lhs to check for previous computations of the TAC right hand side
			struct vnt_entry_t *e_lhs_exist = cfg_vnt_hash_lookup_val(v_lhs);

			// Previous computation existed, perform local common subexpression elimination (optimization)
			if(e_lhs_exist != NULL) {
				// Optimize the TAC node
				tac->op1 = e_lhs_exist->var_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;

				// Lookup the new op1
				e_op1 = cfg_vnt_hash_lookup_td(tac->op1);

				// Insert the optimized left hand side
				e_lhs = cfg_vnt_hash_insert(tac->lhs, v_lhs, tac->lhs, block_level);
			} else {
				e_lhs = cfg_vnt_hash_insert(tac->lhs, v_lhs, tac->lhs, block_level);
			}
		} else {
			// lhs_id is already in the VNT, copy the numbering value
			IRLOG(("lhs found\n"));
			v_lhs = new_identifier(e_lhs->vnt_node->val);
		}
	}
	
	// Free value numbering strings (they will be copied by insert & hash functions, no longer needed)
	if(v_op1 != NULL)
		free(v_op1);
	if(v_op2 != NULL)
		free(v_op2);
	if(v_lhs != NULL)
		free(v_lhs);

	// Pretty print the value numbered TAC
	char *print_lhs = cfg_tac_data_to_str(tac->lhs), *print_op1 = cfg_tac_data_to_str(tac->op1), *print_op2 = cfg_tac_data_to_str(tac->op2);
	if(tac->op == OP_ASSIGN) {
		// Uses the hash value
		//sprintf(ir_vnt_out_buffer, "%s\t%s(%s) := %s(%s)\n", ir_vnt_out_buffer, print_lhs, e_lhs->vnt_node->val, print_op1, e_op1->vnt_node->val);
		// Uses the pretty name
		sprintf(ir_vnt_out_buffer, "%s\t%s(%s) := %s(%s)\n", ir_vnt_out_buffer, print_lhs, e_lhs->vnt_node->pretty_name, print_op1, e_op1->vnt_node->pretty_name);
	} else if(tac->op == OP_BRANCH) {
		// Uses the hash value
		//sprintf(ir_vnt_out_buffer, "%s\t%s(%s)(%s(%s)) %s %s(%s)\n", ir_vnt_out_buffer, print_lhs, e_lhs->vnt_node->val, print_op1, e_op1->vnt_node->val, op, print_op2, e_op2->vnt_node->val);
		// Uses the pretty name
		sprintf(ir_vnt_out_buffer, "%s\t%s(%s)(%s(%s)) %s %s(%s)\n", ir_vnt_out_buffer, print_lhs, e_lhs->vnt_node->pretty_name, print_op1, e_op1->vnt_node->pretty_name, op, print_op2, e_op2->vnt_node->pretty_name);
	} else if(tac->op == OP_GOTO) {
		// Uses the pretty name
		sprintf(ir_vnt_out_buffer, "%s\t%s %s(%s)\n", ir_vnt_out_buffer, op, print_op2, e_op2->vnt_node->pretty_name);
	} else {
		// Uses the hash value
		//sprintf(ir_vnt_out_buffer, "%s\t%s(%s) := %s(%s) %s %s(%s)\n", ir_vnt_out_buffer, print_lhs, e_lhs->vnt_node->val, print_op1, e_op1->vnt_node->val, op, print_op2, e_op2->vnt_node->val);
		// Uses the pretty name
		sprintf(ir_vnt_out_buffer, "%s\t%s(%s) := %s(%s) %s %s(%s)\n", ir_vnt_out_buffer, print_lhs, e_lhs->vnt_node->pretty_name, print_op1, e_op1->vnt_node->pretty_name, op, print_op2, e_op2->vnt_node->pretty_name);
	}
	
	if(print_lhs != NULL)
		free(print_lhs);
	if(print_op1 != NULL)
		free(print_op1);
	if(print_op2 != NULL)
		free(print_op2);

	free(op);
}

void ir_optimize() {

	ir_resolve_label_aliases();

    printf("\nVariables and TAC:\n");
	cfg_print_vars_tac();
	printf("\n");

	printf("\nPrint Blocks:\n");
	cfg_print_blocks();
	printf("\n");
	
	// Iterate through all of the CFGs and process them
	struct cfg_list_t *cfg_it = cfgList;
	while(cfg_it != NULL) {
		ir_process_cfg(cfg_it);
		cfg_it = cfg_it->next;
	}

	/*printf("\nPrint value numbering:\n");
	printf("%s", ir_vnt_out_buffer);
	printf("\n");
	
	printf("\nPrint constant expression optimizations:\n");
	printf("%s", ir_opt_const_out_buffer);
	printf("\n");
	
	printf("\nPrint Blocks (after dead code elim):\n");
	cfg_print_blocks();
	printf("\n");*/

	printf("\nVariables and TAC after extended value numbering\n");
	cfg_print_vars_tac();
}

// Changes the target for any jump/branch tac nodes to a single block
// If that block had an alias added in label_aliases
void ir_resolve_label_aliases() {
	// Go through each tac node
	struct three_address_list_t *tac_it = tacList;
	while(tac_it != NULL) {
		// If the tac is a branch or goto tac
		if(tac_it->tac->op == OP_BRANCH || tac_it->tac->op == OP_GOTO) {
			// See if its label is in the alias list
			struct set_list_t *al_it = label_aliases;
			while(al_it != NULL) {
				// If the label is in the set
				if(set_contains(al_it->set,tac_it->tac->op2->d.id)) {
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


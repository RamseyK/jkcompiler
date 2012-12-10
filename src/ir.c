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

// Simplifies constant expressions into a single assignment, along with algebraic identities before value numbering is performed on a TAC
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
		// Check for optimizations between two same vars of unknown value (ex. op1 = aa, op2 = aa)
		// Test Program: test_tac_consts_var.p, test_tac_consts_var2.p
		if(strcmp(tac->op1->d.id, tac->op2->d.id) == 0) {
			switch(tac->op) {
			case OP_PLUS:
				// Add to itself. a+a => 2*a
				new_td->type = TAC_DATA_TYPE_INT;
				new_td->d.val = 2;
				tac->op1 = new_td;
				tac->op = OP_STAR;
				break;
			case OP_MINUS:
				// Subtract from itself. a-a => 0
				new_td->type = TAC_DATA_TYPE_INT;
				new_td->d.val = 0;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_SLASH:
				// Divide by itself. a/a => 1
				new_td->type = TAC_DATA_TYPE_INT;
				new_td->d.val = 1;
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_AND:
				// AND itself. aa AND aa = aa
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			case OP_OR:
				// OR itself. aa OR aa = aa
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				break;
			default:
				break;
			}
		}
	} else if(tac->op1->type == TAC_DATA_TYPE_INT || tac->op2->type == TAC_DATA_TYPE_INT) {
		// One var is an integer, perform simplification when possible
		// Test Program: test_tac_consts_var.p
		
		switch(tac->op) {
		case OP_PLUS: // Addition
			if(tac->op2->type == TAC_DATA_TYPE_INT && tac->op2->d.val == 0) {
				// Add zero: a+0 = a
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
			} else if(tac->op1->type == TAC_DATA_TYPE_INT && tac->op1->d.val == 0) {
				// Add to zero. 0+a = a
				new_td->type = TAC_DATA_TYPE_VAR;
				new_td->d.id = new_identifier(tac->op2->d.id);
				tac->op1 = new_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
			}
			break;
		case OP_MINUS: // Subtraction
			if(tac->op2->type == TAC_DATA_TYPE_INT && tac->op2->d.val == 0) {
				// Subtract zero: a-0 = a
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
			}
			break;
		case OP_STAR: // Multiplication
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
			}
			break;
		case OP_SLASH: // Division
			if(tac->op2->type == TAC_DATA_TYPE_INT && tac->op2->d.val == 1) {
				// Divide by 1. aa/1 = aa
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
			}
			break;
		default:
			break;
		}
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

// Dead code eliminitation should remove any useless temporary tac vars and unreachable code
// Called after value numbering for a block
void ir_opt_dead_code_elim(struct block_t *block) {
	// Removes unnecessary temp variables that have been simplified to variables or constants via folding/propagation
	// In the case that a temp equals a variable, references to that temporary are fixed up
	struct three_address_t *tac = block->entry;
	while(tac != NULL) {
		// Found temporary that equals a constant, unlink it
		if(tac->op == OP_ASSIGN && tac->lhs->temporary) {
			if(tac->op1->type == TAC_DATA_TYPE_INT || tac->op1->type == TAC_DATA_TYPE_BOOL) {
				//IRLOG(("Unused temp var, Removing %s\n", cfg_tac_data_to_str(tac->lhs)));
				if(tac->next != NULL)
					tac->next->prev = tac->prev;
				
				if(tac->prev != NULL)
					tac->prev->next = tac->next;
				
				if(block->entry == tac)
					block->entry = tac->next;
			} else if(tac->op1->type == TAC_DATA_TYPE_VAR) {
				// Look ahead and replace references to this temporary with the actual variable it equals
				// Ex: t_7 = gg; ff = t_7; -> ff = gg;
				struct three_address_t *future_tac = tac->next;
				while(future_tac != NULL) {
					if(future_tac->op1 == tac->lhs)
						future_tac->op1 = tac->op1;
					
					if(future_tac->op2 == tac->lhs)
						future_tac->op2 = tac->op1;
					
					future_tac = future_tac->next;
				}
				
				// Unlink the (now) unused temporary
				if(tac->next != NULL)
					tac->next->prev = tac->prev;
				
				if(tac->prev != NULL)
					tac->prev->next = tac->next;
				
				if(block->entry == tac)
					block->entry = tac->next;
			}
		}		
		tac = tac->next;
	}
	
	// TODO: Remove impossible IF/WHILE branches
	
	// TODO: Remove temporary variables that aren't used more than once and make a replacement where the temporary was used
	// Example: t = c+1; c = t; -> c = c+1;
}

// Start processing of IR and optimization on the entire cfg
void ir_process_cfg(struct block_t *entryBlock) {
	// If the entryBlock was null then there weren't any real blocks for this cfg
	// Needed this because it was crashing while trying to process functions
	// That had no cfg's because we had not implemented them for all statements yet
	if(entryBlock == NULL) return;

	// Work list contains list of blocks that have more than one parent that needs an additional pass by ir_process_block
	workList = cfg_new_block_list(entryBlock);
	
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
	IRLOG(("Block pass: %s\n", block->label));
	
	// Initial optimization and value numbering
	// Don't look at IF and WHILE blocks because their TAC nodes have already been value numbered and processed in the parent blocks
	if(block->type != BLOCK_IF && block->type != BLOCK_WHILE) {
		// Mark the block in the output
		sprintf(ir_vnt_out_buffer, "%s%s:\n", ir_vnt_out_buffer, block->label);

		// Perform possible constant optimizations then value number the tac
		struct three_address_t *tac = block->entry;
		while(tac != NULL) {
			if(tac->op != OP_BRANCH && tac->op != OP_GOTO
					&& tac->op != OP_NEW_OBJ && tac->op != OP_FUNC_CALL
					&& tac->op != OP_FUNC_RETURN && tac->op != OP_PRINT) {
				// Constant Optimizations/Eval
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

	// Calculate all data flow sets for the block
	//ir_calc_flow_vars(block);
		
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

	struct vnt_entry_t *entry_op1 = NULL, *entry_op2 = NULL, *entry_lhs = NULL;
	char *vnum_op1 = NULL, *vnum_op2 = NULL, *vnum_lhs = NULL; // Numbering values

	// Lookup op1 in the VNT
	entry_op1 = cfg_vnt_hash_lookup_td(tac->op1);
	if(entry_op1 == NULL) {
		// op1 doesn't exist, insert it with a numbering value
		vnum_op1 = cfg_vnt_new_name();
		entry_op1 = cfg_vnt_hash_insert(tac->op1, vnum_op1, tac->op1, block_level);
	} else {
		// op1 is already in the VNT, copy the numbering value
		IRLOG(("op1 found\n"));
		vnum_op1 = new_identifier(entry_op1->vnt_node->val);
	}

	// Case: a = b (no op2)
	if(tac->op == OP_ASSIGN || tac->op == OP_PARAM_ASSIGN || tac->op == OP_PRINT) {
		// Copy the value numbering from the right hand side to the left hand side (simple assignment a = b)
		IRLOG(("lhs insert (nop)\n"));
		vnum_lhs = new_identifier(vnum_op1);
		entry_lhs = cfg_vnt_hash_insert(tac->lhs, vnum_lhs, tac->op1, block_level);
	} else { // Case: a = b + c (op2 exists)
		// Lookup op2 in the VNT
		entry_op2 = cfg_vnt_hash_lookup_td(tac->op2);
		if(entry_op2 == NULL) {
			// op2 doesn't exist, insert it with a numbering value
			vnum_op2 = cfg_vnt_new_name();
			entry_op2 = cfg_vnt_hash_insert(tac->op2, vnum_op2, tac->op2, block_level);
		} else {
			// op2 is already in the VNT, copy the numbering value
			IRLOG(("op2 found\n"));
			vnum_op2 = new_identifier(entry_op2->vnt_node->val);
		}

		// Lookup the lhs_id in the VNT
		entry_lhs = cfg_vnt_hash_lookup_td(tac->lhs);
		if(entry_lhs == NULL) {
			// lhs_id doesn't exist, insert it with a numbering value
			vnum_lhs = cfg_vnt_hash(vnum_op1, tac->op, vnum_op2);
			IRLOG(("lhs not found in VNT. Created hash: %s\n", vnum_lhs));
			
			// Lookup the vnum_lhs to check for previous computations of the TAC right hand side
			struct vnt_entry_t *entry_lhs_exist = cfg_vnt_hash_lookup_val(vnum_lhs);

			// Previous computation existed, perform local common subexpression elimination (optimization)
			if(entry_lhs_exist != NULL) {
				IRLOG(("rhs previous computation found. node val: %s\n", entry_lhs_exist->vnt_node->val));
				// Optimize the TAC node
				tac->op1 = entry_lhs_exist->vnt_node->val_td;
				tac->op = OP_ASSIGN;
				tac->op2 = NULL;
				
				// Lookup the new op1
				entry_op1 = cfg_vnt_hash_lookup_td(tac->op1);

				// Insert the optimized left hand side
				IRLOG(("lhs insert (lce)\n"));
				entry_lhs = cfg_vnt_hash_insert(tac->lhs, vnum_lhs, tac->lhs, block_level);
			} else {
				IRLOG(("lhs insert (new)\n"));
				entry_lhs = cfg_vnt_hash_insert(tac->lhs, vnum_lhs, tac->lhs, block_level);
			}
		}
	}
	
	// Free value numbering strings (they will be copied by insert & hash functions, no longer needed)
	if(vnum_op1 != NULL)
		free(vnum_op1);
	if(vnum_op2 != NULL)
		free(vnum_op2);
	if(vnum_lhs != NULL)
		free(vnum_lhs);

	// Pretty print the value numbered TAC
	char *print_lhs = cfg_tac_data_to_str(tac->lhs), *print_op1 = cfg_tac_data_to_str(tac->op1), *print_op2 = cfg_tac_data_to_str(tac->op2);
	if(tac->op == OP_ASSIGN) {
		// Uses the hash value
		//sprintf(ir_vnt_out_buffer, "%s\t%s(%s) := %s(%s)\n", ir_vnt_out_buffer, print_lhs, entry_lhs->vnt_node->val, print_op1, entry_op1->vnt_node->val);
		// Uses the pretty name
		sprintf(ir_vnt_out_buffer, "%s\t%s(%s) := %s(%s)\n", ir_vnt_out_buffer, print_lhs, entry_lhs->vnt_node->pretty_name, print_op1, entry_op1->vnt_node->pretty_name);
	} else if(tac->op == OP_BRANCH) {
		// Uses the hash value
		//sprintf(ir_vnt_out_buffer, "%s\t%s(%s)(%s(%s)) %s %s(%s)\n", ir_vnt_out_buffer, print_lhs, entry_lhs->vnt_node->val, print_op1, entry_op1->vnt_node->val, op, print_op2, entry_op2->vnt_node->val);
		// Uses the pretty name
		sprintf(ir_vnt_out_buffer, "%s\t%s(%s)(%s(%s)) %s %s(%s)\n", ir_vnt_out_buffer, print_lhs, entry_lhs->vnt_node->pretty_name, print_op1, entry_op1->vnt_node->pretty_name, op, print_op2, entry_op2->vnt_node->pretty_name);
	} else if(tac->op == OP_GOTO) {
		// Uses the pretty name
		sprintf(ir_vnt_out_buffer, "%s\t%s %s(%s)\n", ir_vnt_out_buffer, op, print_op2, entry_op2->vnt_node->pretty_name);
	} else if(tac->op == OP_PARAM_ASSIGN) {
		sprintf(ir_vnt_out_buffer, "%s\t%s %s(%s)\n", ir_vnt_out_buffer, op, print_op1, entry_op1->vnt_node->pretty_name);
	} else if(tac->op == OP_PRINT) {
		sprintf(ir_vnt_out_buffer, "%s\t%s %s(%s)\n", ir_vnt_out_buffer, op, print_op1, entry_op1->vnt_node->pretty_name);
	} else {
		// Uses the hash value
		//sprintf(ir_vnt_out_buffer, "%s\t%s(%s) := %s(%s) %s %s(%s)\n", ir_vnt_out_buffer, print_lhs, entry_lhs->vnt_node->val, print_op1, entry_op1->vnt_node->val, op, print_op2, entry_op2->vnt_node->val);
		// Uses the pretty name
		sprintf(ir_vnt_out_buffer, "%s\t%s(%s) := %s(%s) %s %s(%s)\n", ir_vnt_out_buffer, print_lhs, entry_lhs->vnt_node->pretty_name, print_op1, entry_op1->vnt_node->pretty_name, op, print_op2, entry_op2->vnt_node->pretty_name);
	}
	
	if(print_lhs != NULL)
		free(print_lhs);
	if(print_op1 != NULL)
		free(print_op1);
	if(print_op2 != NULL)
		free(print_op2);

	free(op);
}

// Add all compiler generated temporaries from the entire CFG to the scope
void ir_add_cfg_temps_to_scope(struct scope_t *scope, struct block_t *block) {
	struct three_address_t *tac = block->entry;
	while(tac != NULL) {
		if(tac->op != OP_BRANCH && tac->op != OP_GOTO
			&& tac->op != OP_NEW_OBJ && tac->op != OP_FUNC_CALL
			&& tac->op != OP_FUNC_RETURN && tac->op != OP_PRINT) {
			
			if(tac->lhs != NULL && tac->lhs->temporary) {
				if(scope->temps == NULL)
					scope->temps = new_set(tac->lhs->d.id);
				else
					set_add(scope->temps, tac->lhs->d.id);
			}
			
		}
		
		tac = tac->next;
	}
	
	struct block_list_t *child = block->children;
	while(child != NULL) {
		ir_add_cfg_temps_to_scope(scope, child->block);
	
		child = child->next;
	}
}

// Calculate all data flow sets
void ir_calc_flow_vars(struct block_t *block) {
	struct set_t* temp_set = EMPTY;

	// LiveIn is comprised of the intersection of all liveOut's from blocks parents
	struct block_list_t *parent = block->parents;
	if(parent != NULL) {
		block->liveIn = set_union(parent->block->liveOut, EMPTY);
	}
	
	while(parent != NULL) {
		// Combine same vars from liveOuts into liveIn
		temp_set = set_intersection(block->liveIn, parent->block->liveOut);
		free(block->liveIn);
		block->liveIn = temp_set;
		
		// Combine assignVar and killVar with parents assignVar and killVar
		temp_set = set_union(parent->block->assignVar, block->assignVar);
		free_set(block->assignVar);
		block->assignVar = temp_set;
		
		temp_set = set_union(parent->block->killVar, block->killVar);
		free_set(block->killVar);
		block->killVar = temp_set;
		temp_set = EMPTY;
		
		parent = parent->next;
	}

	// Calculate upwards exposed and killed variables
	struct three_address_t *tac = block->entry;
	while(tac != NULL) {	
		if(tac->op1 != NULL && !tac->op1->temporary && tac->op1->type == TAC_DATA_TYPE_VAR) {
			if(!set_contains(block->killVar, tac->op1->d.id)) {
				if(block->ueVar == EMPTY)
					block->ueVar = new_set(tac->op1->d.id);
				else
					set_add(block->ueVar, tac->op1->d.id);
			}
		}
		if(tac->op2 != NULL && !tac->op2->temporary && tac->op2->type == TAC_DATA_TYPE_VAR) {
			if(!set_contains(block->killVar, tac->op2->d.id)) {
				if(block->ueVar == EMPTY)
					block->ueVar = new_set(tac->op2->d.id);
				else
					set_add(block->ueVar, tac->op2->d.id);
			}
		}
		if(tac->lhs != NULL && !tac->lhs->temporary) {
			if(!set_contains(block->killVar, tac->lhs->d.id)) {
				if(set_contains(block->assignVar, tac->lhs->d.id)) {
					if(block->killVar == EMPTY)
						block->killVar = new_set(tac->lhs->d.id);
					else
						set_add(block->killVar, tac->lhs->d.id);
					if(set_size(block->assignVar) == 1) {
						free_set(block->assignVar);
						block->assignVar = NULL;
					} else {
						set_remove(block->assignVar, tac->lhs->d.id);
					}
				} else {
					if(block->assignVar == EMPTY)
						block->assignVar = new_set(tac->lhs->d.id);
					else
						set_add(block->assignVar, tac->lhs->d.id);
				}
			}
		}
		
		tac = tac->next;
	}
	
	// Compound vars assigned and not killed / untouched
	temp_set = set_union(block->assignVar, block->liveIn);
	block->liveOut = set_difference(block->killVar, temp_set);
	free(temp_set);
	
	IRLOG(("LIVEIN: "));
	set_print(block->liveIn);
	IRLOG(("ASSIGNVAR: "));
	set_print(block->assignVar);
	IRLOG(("UEVAR: "));
	set_print(block->ueVar);
	IRLOG(("KILLVAR: "));
	set_print(block->killVar);
	IRLOG(("LIVEOUT: "));
	set_print(block->liveOut);
}

// Perform optimization on CFGs
void ir_optimize() {
	// Ensures target of jumps resolves properly
	ir_resolve_label_aliases();
	// Do fixups for parameterless func calls that were taken as identifiers
	ir_resolve_func_calls_no_param();

    printf("\nTACs:\n");
	cfg_print_tacs();
	printf("\n");

	printf("\nPrint Blocks:\n");
	cfg_print_blocks();
	printf("\n");
	
	// Iterate through all of the CFGs and process them
	struct cfg_list_t *cfg_it = cfgList;
	while(cfg_it != NULL) {
		ir_process_cfg(cfg_it->entryBlock);
		ir_add_cfg_temps_to_scope(cfg_it->scope, cfg_it->entryBlock);
		cfg_it = cfg_it->next;
	}

	/*printf("\nPrint value numbering:\n");
	printf("%s", ir_vnt_out_buffer);
	printf("\n");*/
	
	/*printf("\nPrint constant expression optimizations:\n");
	printf("%s", ir_opt_const_out_buffer);
	printf("\n");*/
	
	/*printf("\nPrint Blocks (after dead code elim):\n");
	cfg_print_blocks();
	printf("\n");*/

	printf("\nTAC after extended value numbering\n");
	cfg_print_tacs();
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

void ir_resolve_func_calls_no_param() {
	// Go through each tac node
	struct three_address_list_t *tac_it = tacList;
	while(tac_it != NULL) {
		// If the tac op is assign
		if(tac_it->tac->op == OP_ASSIGN) {
			// If the op1 has the flag tacData type
			if(tac_it->tac->op1->type == TAC_DATA_TYPE_FUNCCALL_NOPARAM) {
				// Insert a new tac before this one that calls the function
					// Problem:  If this tac is the entry to the block, how to update the block entry?

				//Change the op1 of this assign tac to the lhs of the newly made tac

			}

		}

		// If the tac op is an access
		if(tac_it->tac->op == OP_MEM_ACCESS) {
			// Check if op2 has the flag tacData type
			if(tac_it->tac->op2->type == TAC_DATA_TYPE_FUNCCALL_NOPARAM) {
				// Change the op to a functionc all
				//tac_it->tac->op = OP_FUNC_CALL;
			}
		}

		tac_it = tac_it->next;
	}

}


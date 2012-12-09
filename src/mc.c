#include "mc.h"

void mc_init() {
	// Reset used_regs
	for(int i = 0; i < NUM_REGS; i++)
		used_regs[i] = false;

	// Data Section
	dataSection = mc_new_section();
	
	// Default data directive
	struct directive_t *dataDir = mc_new_directive(NULL, "data");
	dataDir->no_val = true;
	
	// Align data datums on word boundaries
	struct directive_t *alignDir = mc_new_directive(NULL, "align");
	alignDir->val = 2;
	
	// Emit data section directives
	mc_emit_directive(dataSection, dataDir);
	mc_emit_directive(dataSection, alignDir);
	
	// Text Section
	textSection = mc_new_section();
	
	// Default text directive
	struct directive_t *textDir = mc_new_directive(NULL, "text");
	textDir->no_val = true;
	
	// Emit text section directives
	mc_emit_directive(textSection, textDir);
}

void mc_destroy() {
	mc_free_section(dataSection);
	mc_free_section(textSection);
}

// Lookup a mips_op structure in the mips_ops_list by its name
struct mips_op_t *mc_lookup_mips_op(const char *mnemonic) {
	int sz = sizeof(mips_ops_list)/sizeof(struct mips_op_t);
	for(int i = 0; i < sz; i++) {
		if(strcmp(mips_ops_list[i].name, mnemonic) == 0)
			return &mips_ops_list[i];
	}
	return NULL;
}

// Instantiates a new section structure
struct section_t *mc_new_section() {
	struct section_t *sec = (struct section_t*)malloc(sizeof(struct section_t));
	
	sec->dirs = NULL;
	sec->instrs = NULL;
	
	return sec;
}

// Frees a new section structure and all components contained within it
void mc_free_section(struct section_t *sec) {
	if(sec == NULL)
		return;
	
}

// Instantiates a new directive structure
struct directive_t *mc_new_directive(const char *label, const char *name) {
	struct directive_t *dir = (struct directive_t*)malloc(sizeof(struct directive_t));
	if(dir == NULL)
		return NULL;
	
	dir->label = new_identifier(label);
	dir->name = new_identifier(name);
	dir->no_val = false;
	dir->val_str = NULL;
	dir->val = 0;
	dir->comment = NULL;
	dir->next = NULL;
	
	return dir;
}

// Free's a directive from memory
void mc_free_directive(struct directive_t *dir) {
	if(dir == NULL)
		return;

	if(dir->label != NULL)
		free(dir->label);
	if(dir->name != NULL)
		free(dir->name);
	if(dir->val_str != NULL)
		free(dir->val_str);
	if(dir->comment != NULL)
		free(dir->comment);
	dir->next = NULL;
	
	free(dir);
}

// Instantiates a new instruction structure
struct instr_t *mc_new_instr(const char *mnemonic) {
	struct instr_t *instr = (struct instr_t*)malloc(sizeof(struct instr_t));
	if(instr == NULL)
		return NULL;
	
	instr->mips_op = mc_lookup_mips_op(mnemonic);
	instr->lhs_reg = -1;
	instr->op1_reg = -1;
	instr->op1_has_offset = false;
	instr->op1_reg_offset = 0;
	instr->op2_reg = -1;
	instr->imm = 0;
	instr->addr_label = NULL;
	instr->comment = (char*)malloc(32);
	memset(instr->comment, 0, 32);
	instr->next = NULL;
	
	return instr;
}

// Free's an instr from memory
void mc_free_instr(struct instr_t *instr) {
	if(instr == NULL)
		return;
		
	if(instr->addr_label != NULL)
		free(instr->addr_label);
		
	if(instr->comment != NULL)
		free(instr->comment);
	
	free(instr);
}

struct instr_list_t *mc_new_instr_list(const char *label) {
	struct instr_list_t *list = (struct instr_list_t*)malloc(sizeof(struct instr_list_t));
	
	list->label = new_identifier(label);
	list->entryInstr = NULL;
	list->next = NULL;
	
	return list;
}

void mc_append_instr_list(struct instr_list_t **list, struct instr_list_t *entry) {
	if(*list == NULL) {
		*list = entry;
		return;
	}
	
	struct instr_list_t *it = *list;
	while(it->next != NULL)
		it = it->next;
	it->next = entry;
}

// Frees an instruction list, all list connected list nodes, and all instruction objects within it
void mc_free_instr_list(struct instr_list_t **list) {
	if((*list)->label != NULL)
		free((*list)->label);
	struct instr_t *it_instr = (*list)->entryInstr, *cur_instr = NULL;
	while(it_instr != NULL) {
		cur_instr = it_instr;
		it_instr = it_instr->next;
		mc_free_instr(cur_instr);
	}
	
	mc_free_instr_list(&((*list)->next));
	free(*list);
	*list = NULL;
}

// Instantiate a memory location structure
struct mem_loc_t *mc_new_mem_loc(const char *id) {
	struct mem_loc_t *mem_loc = (struct mem_loc_t*)malloc(sizeof(struct mem_loc_t));
	if(mem_loc == NULL)
		return NULL;
	
	mem_loc->id = new_identifier(id);
	mem_loc->temp_reg = -1;
	mem_loc->wb = false;
	mem_loc->type = 0;
	
	return mem_loc;
}

// Free memory location structure
void mc_free_mem_loc(struct mem_loc_t *mem_loc) {
	if(mem_loc == NULL)
		return;

	if(mem_loc->id != NULL)
		free(mem_loc->id);

	free(mem_loc);
}

// Process a CFG list (functions) and allocate stack, registers and emit directives, instructions
void mc_consume_cfg_list(struct cfg_list_t *cfgs) {
	struct cfg_list_t *cfg_it = cfgs;
	struct instr_list_t *cfg_instr_list = mc_new_instr_list(cfg_it->entryBlock->label);
	// Loop through function cfgs
	while(cfg_it != NULL) {
		if(cfg_it->scope->attrId != SYM_ATTR_FUNC)
			MCLOG(("Scope is NOT a func as expected..\n"));
	
		// Allocate variables on the stack and backup register values
		mc_alloc_stack(cfg_instr_list, cfg_it->scope);
		
		// Process the function entry block and is children recursively
		mc_process_block(cfg_instr_list, cfg_it->scope, cfg_it->entryBlock);
		
		// Deallocate stack and restore register values
		mc_dealloc_stack(cfg_instr_list, cfg_it->scope);
		
		// Return out of the function (jump to ra)
		mc_leave_func(cfg_instr_list);
		
		cfg_it = cfg_it->next;
	}
	
	// Add the cfg instruction listing to the text section instr lists
	mc_append_instr_list(&(textSection->instrs), cfg_instr_list);
}

// Recursively process a block and its children by converting its TAC to Instructions
void mc_process_block(struct instr_list_t *instr_list, struct scope_t *cfg_scope, struct block_t *block) {
	// Loop through TAC nodes
	struct three_address_t *tac = block->entry;
	struct instr_t *instr = NULL;
	struct mem_loc_t *lhs_loc = NULL, *op1_loc = NULL, *op2_loc = NULL;
	
	while(tac != NULL) {
		// Load variables/compiler temps into registers
		mc_reset_temp_regs();
		lhs_loc = mc_mem_access_var(instr_list, cfg_scope, tac->lhs);
		op1_loc = mc_mem_access_var(instr_list, cfg_scope, tac->op1);
		op2_loc = mc_mem_access_var(instr_list, cfg_scope, tac->op2);
		
		// Load constants into registers for instructions that do not have I_TYPE equivalents
		if(tac->op == OP_STAR || tac->op == OP_SLASH || tac->op == OP_MOD) {
			if(op1_loc == NULL)
				op1_loc = mc_mem_access_const(instr_list, tac->op1);
			if(op2_loc == NULL)
				op2_loc = mc_mem_access_const(instr_list, tac->op2);
		}
		
		// Convert TAC to instruction
		instr = mc_tac_to_instr(tac, lhs_loc, op1_loc, op2_loc);
		if(instr != NULL) {
			// Add converted instruction to the end of the instruction list
			mc_emit_instr(instr_list, instr);
		
			// Write back the lhs_loc if its indicated that it changed (wb flag)
			if(lhs_loc != NULL && lhs_loc->wb)
				mc_mem_writeback(instr_list, lhs_loc);
		}
		
		// Free mem_loc structures, no longer needed
		mc_free_mem_loc(lhs_loc);
		lhs_loc = NULL;
		mc_free_mem_loc(op1_loc);
		op1_loc = NULL;
		mc_free_mem_loc(op2_loc);
		op2_loc = NULL;
	
		tac = tac->next;
	}
	
	// Recursively process this blocks children
	struct block_list_t *child = block->children;
	while(child !=  NULL) {
		// Create an instruction listing for this block
		struct instr_list_t *block_instr_list = mc_new_instr_list(child->block->label);
		
		mc_process_block(block_instr_list, cfg_scope, child->block);
		
		// Add the block instruction listing to the previous instr list
		mc_append_instr_list(&instr_list, block_instr_list);
		
		child = child->next;
	}
}

// Convert three address code to corresponding instructions with registers/memory
struct instr_t *mc_tac_to_instr(struct three_address_t *tac, struct mem_loc_t *lhs_loc, struct mem_loc_t *op1_loc, struct mem_loc_t *op2_loc) {
	// Get string representations of lhs, op1 and op2
	char *lhs_str = cfg_tac_data_to_str(tac->lhs);
	char *op1_str = cfg_tac_data_to_str(tac->op1);
	char *op2_str = cfg_tac_data_to_str(tac->op2);

	struct instr_t *instr = NULL, *instr2 = NULL, *instr3 = NULL;
	switch(tac->op) {
		case OP_PLUS:
			lhs_loc->wb = true;
			if(tac->op2->type == TAC_DATA_TYPE_INT) {
				// Var + Int (Reg + Imm)
				instr = mc_new_instr("addi");
				instr->lhs_reg = lhs_loc->temp_reg;
				instr->op1_reg = op1_loc->temp_reg;
				instr->imm = tac->op2->d.val;
				sprintf(instr->comment, "%s = %s + %i", lhs_str, op1_str, tac->op2->d.val);
			} else {
				// Var + Var (both regs)
				instr = mc_new_instr("add");
				instr->lhs_reg = lhs_loc->temp_reg;
				instr->op1_reg = op1_loc->temp_reg;
				instr->op2_reg = op2_loc->temp_reg;
				sprintf(instr->comment, "%s = %s + %s", lhs_str, op1_str, op2_str);
			}
			break;
		case OP_MINUS:
			lhs_loc->wb = true;
			if(tac->op1->type == TAC_DATA_TYPE_VAR && tac->op2->type == TAC_DATA_TYPE_INT) {
				// Var - Int (Reg - Imm)
				instr = mc_new_instr("addi");
				instr->lhs_reg = lhs_loc->temp_reg;
				instr->op1_reg = op1_loc->temp_reg;
				instr->imm = 0-tac->op2->d.val;
				sprintf(instr->comment, "%s = %s - %i", lhs_str, op1_str, tac->op2->d.val);
			} else if(tac->op1->type == TAC_DATA_TYPE_INT && tac->op2->type == TAC_DATA_TYPE_VAR) {
				// Int - Var (Imm - Reg)
				instr = mc_new_instr("addi");
				instr->lhs_reg = lhs_loc->temp_reg;
				instr->op1_reg = op2_loc->temp_reg;
				instr->imm = 0-tac->op1->d.val;
				sprintf(instr->comment, "%s = %i - %s", lhs_str, tac->op1->d.val, op2_str);
			} else {
				// Var - Var (both regs)
				instr = mc_new_instr("sub");
				instr->lhs_reg = lhs_loc->temp_reg;
				instr->op1_reg = op1_loc->temp_reg;
				instr->op2_reg = op2_loc->temp_reg;
				sprintf(instr->comment, "%s = %s - %s", lhs_str, op1_str, op2_str);
			}
			break;
		case OP_OR:
			lhs_loc->wb = true;
			if(tac->op1->type == TAC_DATA_TYPE_VAR && tac->op2->type == TAC_DATA_TYPE_VAR) {
				// Var OR Var
				instr = mc_new_instr("or");
				instr->lhs_reg = lhs_loc->temp_reg;
				instr->op1_reg = op1_loc->temp_reg;
				instr->op2_reg = op2_loc->temp_reg;
			} else if(tac->op1->type == TAC_DATA_TYPE_VAR) {
				instr = mc_new_instr("ori");
				instr->lhs_reg = lhs_loc->temp_reg;
				instr->op1_reg = op1_loc->temp_reg;
				if(tac->op2->type == TAC_DATA_TYPE_INT)
					instr->imm = tac->op2->d.val;
				else if(tac->op2->type == TAC_DATA_TYPE_BOOL)
					instr->imm = tac->op2->d.b;
				else
					MCLOG(("Invalid OR operand\n"));
			} else if(tac->op2->type == TAC_DATA_TYPE_VAR) {
				instr = mc_new_instr("ori");
				instr->lhs_reg = lhs_loc->temp_reg;
				instr->op1_reg = op2_loc->temp_reg;
				if(tac->op1->type == TAC_DATA_TYPE_INT)
					instr->imm = tac->op1->d.val;
				else if(tac->op1->type == TAC_DATA_TYPE_BOOL)
					instr->imm = tac->op1->d.b;
				else
					MCLOG(("Invalid OR operand\n"));
			}
			break;
		case OP_STAR:
			lhs_loc->wb = true;
			instr = mc_new_instr("mult");
			// Var * Var (both must be regs)
			instr->op1_reg = op1_loc->temp_reg;
			instr->op2_reg = op2_loc->temp_reg;
			sprintf(instr->comment, "Lo = %s * %s", op1_str, op2_str);
			// Move the result from Lo into the LHS reg
			instr2 = mc_new_instr("mflo");
			instr2->lhs_reg = lhs_loc->temp_reg;
			sprintf(instr2->comment, "%s = Lo", lhs_str);
			instr->next = instr2; // Link mult and mflo instructions
			break;
		case OP_SLASH:
			lhs_loc->wb = true;
			instr = mc_new_instr("div");
			// Var * Var (both must be regs)
			instr->op1_reg = op1_loc->temp_reg;
			instr->op2_reg = op2_loc->temp_reg;
			sprintf(instr->comment, "Lo = %s / %s", op1_str, op2_str);
			// Move the result from Lo into the LHS reg
			instr2 = mc_new_instr("mflo");
			instr2->lhs_reg = lhs_loc->temp_reg;
			sprintf(instr2->comment, "%s = Lo", lhs_str);
			instr->next = instr2; // Link div and mflo instructions
			break;
		case OP_MOD:
			lhs_loc->wb = true;
			instr = mc_new_instr("div");
			// Var * Var (both must be regs)
			instr->op1_reg = op1_loc->temp_reg;
			instr->op2_reg = op2_loc->temp_reg;
			sprintf(instr->comment, "Hi = %s / %s", op1_str, op2_str);
			// Move the result from Hi into the LHS reg
			instr2 = mc_new_instr("mfhi");
			instr2->lhs_reg = lhs_loc->temp_reg;
			sprintf(instr2->comment, "%s = Hi (Remainder)", lhs_str);
			break;
		case OP_AND:
			lhs_loc->wb = true;
			if(tac->op1->type == TAC_DATA_TYPE_VAR && tac->op2->type == TAC_DATA_TYPE_VAR) {
				// Var OR Var
				instr = mc_new_instr("and");
				instr->lhs_reg = lhs_loc->temp_reg;
				instr->op1_reg = op1_loc->temp_reg;
				instr->op2_reg = op2_loc->temp_reg;
			} else if(tac->op1->type == TAC_DATA_TYPE_VAR) {
				instr = mc_new_instr("andi");
				instr->lhs_reg = lhs_loc->temp_reg;
				instr->op1_reg = op1_loc->temp_reg;
				if(tac->op2->type == TAC_DATA_TYPE_INT)
					instr->imm = tac->op2->d.val;
				else if(tac->op2->type == TAC_DATA_TYPE_BOOL)
					instr->imm = tac->op2->d.b;
				else
					MCLOG(("Invalid AND operand\n"));
			} else if(tac->op2->type == TAC_DATA_TYPE_VAR) {
				instr = mc_new_instr("andi");
				instr->lhs_reg = lhs_loc->temp_reg;
				instr->op1_reg = op2_loc->temp_reg;
				if(tac->op1->type == TAC_DATA_TYPE_INT)
					instr->imm = tac->op1->d.val;
				else if(tac->op1->type == TAC_DATA_TYPE_BOOL)
					instr->imm = tac->op1->d.b;
				else
					MCLOG(("Invalid AND operand\n"));
			}
			break;
		case OP_EQUAL:
			//instr = mc_new_instr("slt");
			break;
		case OP_NOTEQUAL:
			instr = mc_new_instr("nor");
			instr->lhs_reg = lhs_loc->temp_reg;
			instr->op1_reg = op1_loc->temp_reg;
			instr->op2_reg = $0;
			break;
		case OP_LT:
			instr = mc_new_instr("slt");
			break;
		case OP_GT:
			instr = mc_new_instr("slt");
			break;
		case OP_LE:
			instr = mc_new_instr("slt");
			break;
		case OP_GE:
			instr = mc_new_instr("slt");
			break;
		case OP_ASSIGN: // Move (avoids generating pseudo move or li instructions)
			instr = mc_new_instr("addi");
			instr->lhs_reg = lhs_loc->temp_reg;
			if(tac->op1->type == TAC_DATA_TYPE_VAR) {
				instr->op1_reg = op1_loc->temp_reg;
				instr->op2_reg = $0;			
				sprintf(instr->comment, "%s = %s", lhs_str, op1_str);	
			} else if(tac->op1->type == TAC_DATA_TYPE_INT) {
				instr->op1_reg = $0;
				instr->imm = tac->op1->d.val;
				sprintf(instr->comment, "%s = %s", lhs_str, op1_str);	
			} else if(tac->op1->type == TAC_DATA_TYPE_INT) {
				instr->op1_reg = $0;
				instr->imm = (int)tac->op1->d.b;
				sprintf(instr->comment, "%s = %s", lhs_str, (tac->op1->d.b ? "TRUE" : "FALSE"));	
			} else {
				MCLOG(("Error: Unknown assign operation"));
			}
			break;
		case OP_BRANCH:
			instr = mc_new_instr("j");
			break;
		case OP_GOTO:
			instr = mc_new_instr("j");
			break;
		case OP_CALLER_ASSIGN:
			break;
		case OP_PARAM_ASSIGN:
			break;
		case OP_FUNC_CALL:
			break;
		case OP_MEM_ACCESS:
			break;
		case OP_NEW_OBJ:
			break;
		case OP_PRINT:
			// Set syscall service number for print_int
			instr = mc_new_instr("addi");
			instr->lhs_reg = $v0;
			instr->op1_reg = $0;
			instr->imm = 1;
			
			// Set integer to print (argument)
			instr2 = mc_new_instr("add");
			instr2->lhs_reg = $a0;
			instr2->op1_reg = $0;
			instr2->op2_reg = op1_loc->temp_reg;
			
			// Execute the syscall
			instr3 = mc_new_instr("syscall");
			sprintf(instr3->comment, "print_int");
			
			// Link instructions in order
			instr->next = instr2;
			instr2->next = instr3;
			break;
		default:
			MCLOG(("Could not convert tac to instr: %s\n", cfg_tac_data_to_str(tac->lhs)));
			break;
	}
	
	if(lhs_str != NULL)
		free(lhs_str);
	if(op1_str != NULL)
		free(op1_str);
	if(op2_str != NULL)
		free(op2_str);
	
	return instr;
}

// Control Transfer out of a function (return by jumping to ra)
void mc_leave_func(struct instr_list_t *cfg_instr_list) {
	struct instr_t *instr = mc_new_instr("jr");
	instr->lhs_reg = $ra;
	sprintf(instr->comment, "return");
	mc_emit_instr(cfg_instr_list, instr);
}

// Generate instructions and allocate temporary registers to access identifiers in memory
struct mem_loc_t *mc_mem_access_var(struct instr_list_t *instr_list, struct scope_t *cfg_scope, struct tac_data_t *td) {
	if(td == NULL || td->type != TAC_DATA_TYPE_VAR)
		return NULL;

	struct mem_loc_t *mem_loc = NULL;
	struct scope_t *scope = NULL;
	
	if(td->temporary) {
		if(set_contains(cfg_scope->temps, td->d.id)) { // Temporarys should exist in the function scopes temps set
			scope = cfg_scope;
		} else {
			MCLOG(("Could not find temporary %s in the scopes temps set\n", td->d.id));
			return mem_loc;
		}
	} else { // Get scope for the variable in the TAC
		scope = symtab_lookup_variable_scope(cfg_scope, td->d.id);
		if(scope == NULL) {
			MCLOG(("Could not find scope for %s\n", td->d.id));
			return mem_loc;
		}
	}

	// Determine if the variable is in a register, on stack, or on heap
	// TODO: Register allocation list needs to be processed here. Right now, we're not storing anything in saved regs
	
	// On Stack if the scope of the variable matches the cfg_scope
	if(scope->attrId == SYM_ATTR_FUNC) {
		mem_loc = mc_new_mem_loc(td->d.id);
		mem_loc->type = MEM_STACK;
		
		// Emit instruction to load stack value into a temp regs
		struct offset_list_t *it = scope->offset_list;
		while(it != NULL) {
			// Found the variable in the offset list
			if(strcmp(it->id, td->d.id) == 0) {
				mem_loc->temp_reg = mc_next_temp_reg();
				mem_loc->loc.offset = it->offset;
				
				struct instr_t *instr = mc_new_instr("lw");
				instr->lhs_reg = mem_loc->temp_reg;
				instr->op1_reg = $fp;
				instr->op1_has_offset = true;
				instr->op1_reg_offset = mem_loc->loc.offset;
				sprintf(instr->comment, "Load %s", it->id);
				mc_emit_instr(instr_list, instr);
				break;
			}
			
			it = it->next;
		}
	} /*else if(scope->attrId == SYM_ATTR_CLASS) {
		mem_loc = mc_new_mem_loc(td->d.id);
		mem_loc->type = MEM_HEAP;
		
		// Emit instructions to load heap values into temp regs
	}*/ else {
		MCLOG(("Scope of variable %s is unhandled: %i\n", td->d.id, scope->attrId));
		return mem_loc;
	}
	
	return mem_loc;
}

// Generate instructions to allocate a temporary register to hold a constant
// This is needed for instructions that do not have I_TYPE counterparts to their R_TYPE
struct mem_loc_t *mc_mem_access_const(struct instr_list_t *instr_list, struct tac_data_t *td) {
	if(td == NULL || td->type == TAC_DATA_TYPE_VAR)
		return NULL;
	
	struct mem_loc_t *mem_loc = mc_new_mem_loc(NULL);
	mem_loc->type = MEM_REG;
	mem_loc->temp_reg = mc_next_temp_reg();
	mem_loc->loc.reg = mem_loc->temp_reg;
	
	struct instr_t *instr = mc_new_instr("addi");
	instr->lhs_reg = mem_loc->temp_reg;
	instr->op1_reg = $0;
	if(td->type == TAC_DATA_TYPE_INT) {
		instr->imm = td->d.val;
	} else if(td->type == TAC_DATA_TYPE_BOOL) {
		instr->imm = (int)(td->d.b);
	} else {
		MCLOG(("Unable to load constant from tac\n"));
		mc_free_instr(instr);
		return NULL;
	}
	sprintf(instr->comment, "Temp constant");
	mc_emit_instr(instr_list, instr);
	
	return mem_loc;
}

// Write back a register value to the permanent location
void mc_mem_writeback(struct instr_list_t *instr_list, struct mem_loc_t *mem_loc) {
	if(mem_loc == NULL || mem_loc->temp_reg == -1)
		return;

	if(mem_loc->type == MEM_STACK) {
		struct instr_t *instr = mc_new_instr("sw");
		instr->lhs_reg = mem_loc->temp_reg;
		instr->op1_reg = $fp;
		instr->op1_has_offset = true;
		instr->op1_reg_offset = mem_loc->loc.offset;
		sprintf(instr->comment, "Store %s", mem_loc->id);
		mc_emit_instr(instr_list, instr);
	} else {
		MCLOG(("Unsupported write back for mem_loc of type %i\n", mem_loc->type));
	}
}

// Returns the next available temporary register and marks it as used. -1 if none available
int mc_next_temp_reg() {
	for(int i = $t0; i <= $t7; i++) {
		if(!used_regs[i]) {
			used_regs[i] = true;
			return i;
		}
	}
	for(int i = $t8; i <= $t9; i++) {
		if(!used_regs[i]) {
			used_regs[i] = true;
			return i;
		}
	}
	return -1;
}

// Resets all temporary registers to unused
void mc_reset_temp_regs() {
	for(int i = $t0; i <= $t7; i++) {
		used_regs[i] = false;
	}
	for(int i = $t8; i <= $t9; i++) {
		used_regs[i] = false;
	}
}

// Gets the next available saved register and marks it as used. -1 if none available
int mc_next_saved_reg() {
	for(int i = $s0; i <= $s7; i++) {
		if(!used_regs[i]) {
			used_regs[i] = true;
			return i;
		}
	}
	return -1;
}

// Returns the number of saved registers in use
int mc_num_saved_regs_used() {
	int count = 0;
	for(int i = $s0; i <= $s7; i++) {
		if(used_regs[i])
			count++;
	}
	return count;
}

// Backup preserved registers and allocate stack memory for each variable and emits the appropriate instructions
// Called just before jumping into a new function
void mc_alloc_stack(struct instr_list_t *cfg_instr_list, struct scope_t *scope) {
	// Saved registers: s0-s7. Always preserved: fp, ra
	int backup_size = SIZE_WORD*mc_num_saved_regs_used() + SIZE_WORD*2;
	int frame_size = symtab_calc_scope_size(scope);
	int sp_size = frame_size+backup_size;
	int sp_pointer = 0;
	struct instr_t *instr = NULL;
	
	// Allocate space on the stack addi $sp, $sp, -sp_size
	instr = mc_new_instr("addi");
	instr->lhs_reg = $sp;
	instr->op1_reg = $sp;
	instr->imm = 0-sp_size;
	sprintf(instr->comment, "Allocate stack");
	mc_emit_instr(cfg_instr_list, instr);
	
	// Save arguments on stack
	
	// Restore registers and saved registers from previous frames
	// Backup registers $s0 - $s7 if in use
	for(int i = $s0; i <= $s7; i++) {
		if(!used_regs[i])
			continue;
	
		instr = mc_new_instr("sw");
		instr->lhs_reg = i;
		instr->op1_reg = $sp;
		instr->op1_has_offset = true;
		instr->op1_reg_offset = sp_pointer;
		sp_pointer += SIZE_WORD;
		mc_emit_instr(cfg_instr_list, instr);
	}
	
	// Backup $fp
	instr = mc_new_instr("sw");
	instr->lhs_reg = $fp;
	instr->op1_reg = $sp;
	instr->op1_has_offset = true;
	instr->op1_reg_offset = sp_pointer;
	sp_pointer += SIZE_WORD;
	mc_emit_instr(cfg_instr_list, instr);
	
	// Backup $ra
	instr = mc_new_instr("sw");
	instr->lhs_reg = $ra;
	instr->op1_reg = $sp;
	instr->op1_has_offset = true;
	instr->op1_reg_offset = sp_pointer;
	sp_pointer += SIZE_WORD;
	mc_emit_instr(cfg_instr_list, instr);
	
	// Set new Frame Pointer to start of local variables
	instr = mc_new_instr("addi");
	instr->lhs_reg = $fp;
	instr->op1_reg = $sp;
	instr->imm = backup_size;
	sprintf(instr->comment, "Start of frame");
	mc_emit_instr(cfg_instr_list, instr);
}

// Restore preserved registers and deallocate stack memory
void mc_dealloc_stack(struct instr_list_t *cfg_instr_list, struct scope_t *scope) {
	// Saved registers: s0-s7. Always preserved: fp, ra
	int backup_size = SIZE_WORD*mc_num_saved_regs_used() + SIZE_WORD*2;
	int frame_size = symtab_calc_scope_size(scope);
	int sp_size = frame_size+backup_size;
	int sp_pointer = backup_size-SIZE_WORD;
	struct instr_t *instr = NULL;
	
	// Restore registers and saved registers from previous frames
	// Restore $ra
	instr = mc_new_instr("lw");
	instr->lhs_reg = $ra;
	instr->op1_reg = $sp;
	instr->op1_has_offset = true;
	instr->op1_reg_offset = sp_pointer;
	sp_pointer -= SIZE_WORD;
	mc_emit_instr(cfg_instr_list, instr);
	
	// Restore $fp
	instr = mc_new_instr("lw");
	instr->lhs_reg = $fp;
	instr->op1_reg = $sp;
	instr->op1_has_offset = true;
	instr->op1_reg_offset = sp_pointer;
	sp_pointer -= SIZE_WORD;
	mc_emit_instr(cfg_instr_list, instr);
	
	// Restore registers $s7 - $s0 if used
	for(int i = $s7; i >= $s0; i--) {
		if(!used_regs[i])
			continue;
		
		instr = mc_new_instr("lw");
		instr->lhs_reg = i;
		instr->op1_reg = $sp;
		instr->op1_has_offset = true;
		instr->op1_reg_offset = sp_pointer;
		sp_pointer -= SIZE_WORD;
		mc_emit_instr(cfg_instr_list, instr);
	}
	
	// Deallocate space on the stack addi $sp, $sp, sp_size
	instr = mc_new_instr("addi");
	instr->lhs_reg = $sp;
	instr->op1_reg = $sp;
	instr->imm = sp_size;
	sprintf(instr->comment, "Deallocate stack");
	mc_emit_instr(cfg_instr_list, instr);
}

// Generates .globl main_func_name, and extra bootstrap code to jump into the main func of the main class, and termination syscall when last method returns
void mc_add_bootstrap(const char *entry_block_label) {
	// Label the bootstrap function
	struct directive_t *gldir = mc_new_directive(NULL, "globl");
	gldir->val_str = new_identifier("main");
	mc_emit_directive(textSection, gldir);
	
	// Create instruction listing for the bootstrap
	struct instr_list_t *instr_list = mc_new_instr_list("main");

	// Jump into starting function
	struct instr_t *instr = mc_new_instr("jal");
	instr->addr_label = new_identifier(entry_block_label);
	mc_emit_instr(instr_list, instr);
	
	// Syscall to terminate the program properly
	instr = mc_new_instr("addi");
	instr->lhs_reg = $v0;
	instr->op1_reg = $0;
	instr->imm = 10;
	mc_emit_instr(instr_list, instr);
	instr = mc_new_instr("syscall");
	mc_emit_instr(instr_list, instr);
	
	// Add the instruction listing to HEAD of the text section instr lists
	struct instr_list_t *old_head = textSection->instrs;
	textSection->instrs = instr_list;
	instr_list->next = old_head;
}

// Add directive to a section
void mc_emit_directive(struct section_t *sec, struct directive_t *dir) {
	if(sec == NULL || dir == NULL)
		return;

	if(sec->dirs == NULL) {
		sec->dirs = dir;
	} else {
		struct directive_t *it = sec->dirs;
		while(it->next != NULL)
			it = it->next;
		it->next = dir;
	}
}

// Add instruction to instruction list
void mc_emit_instr(struct instr_list_t *list, struct instr_t *instr) {
	if(list == NULL || instr == NULL)
		return;

	if(list->entryInstr == NULL) {
		list->entryInstr = instr;
	} else {
		struct instr_t *it = list->entryInstr;
		while(it->next != NULL)
			it = it->next;
		it->next = instr;
	}
}

// Prints all supported MIPS instructions
void mc_print_mips_ops() {
	int sz = sizeof(mips_ops_list)/sizeof(struct mips_op_t);
	MCLOG(("%i MIPS instructions supported\n", sz));
	for(int i = 0; i < sz; i++) {
		MCLOG(("%s\n", mips_ops_list[i].name));
	}
	MCLOG(("\n"));
}

// Generate current asm listing as a string
char *mc_gen_listing() {
	char buf[8192];
	memset(buf, 0, sizeof(buf));
	
	// .data section and all of it's sub directives
	struct directive_t *dir = dataSection->dirs;
	while(dir != NULL) {
		// label:
		if(dir->label != NULL)
			sprintf(buf, "%s%s:\n", buf, dir->label);
		
		// .name
		sprintf(buf, "%s.%s ", buf, dir->name);
		
		if(!dir->no_val) {
			if(dir->val_str != NULL) { // String or number value
				sprintf(buf, "%s%s", buf, dir->val_str);
			} else { // Integer value
				sprintf(buf, "%s%i", buf, dir->val);
			}
		}
		
		// Optional Comment
		if(dir->comment != NULL)
			sprintf(buf, "%s # %s", buf, dir->comment);
		
		sprintf(buf, "%s\n", buf);
		
		dir = dir->next;
	}
	
	sprintf(buf, "%s\n", buf);
	
	// .text section with directives and instructions
	dir = textSection->dirs;
	while(dir != NULL) {
		// label:
		if(dir->label != NULL)
			sprintf(buf, "%s%s:\n", buf, dir->label);
		
		// .name
		sprintf(buf, "%s.%s ", buf, dir->name);
		
		if(!dir->no_val) {
			if(dir->val_str != NULL) { // String or number value
				sprintf(buf, "%s%s", buf, dir->val_str);
			} else { // Integer value
				sprintf(buf, "%s%i", buf, dir->val);
			}
		}
		
		// Optional Comment
		if(dir->comment != NULL)
			sprintf(buf, "%s # %s", buf, dir->comment);
		
		sprintf(buf, "%s\n", buf);
		
		dir = dir->next;
	}
	
	struct instr_list_t *instr_list = textSection->instrs;
	struct instr_t *instr = NULL;
	while(instr_list != NULL) {
		instr = instr_list->entryInstr;
		
		// Instruction List label
		sprintf(buf, "%s%s:\n", buf, instr_list->label);
		
		while(instr != NULL) {
			
			// Mnemonic
			sprintf(buf, "%s\t%s ", buf, instr->mips_op->name);
			switch(instr->mips_op->format) {
			case R_TYPE:
				if(instr->lhs_reg != -1)
					sprintf(buf, "%s%s", buf, reg_names[instr->lhs_reg]); // R[lhs]
				
				if(instr->lhs_reg != -1 && instr->op1_reg != -1) // Comma between R[lhs] and R[op1] if both are present
					sprintf(buf, "%s, ", buf);
				
				if(instr->op2_reg != -1) {
					if(instr->imm == 0)
						sprintf(buf, "%s%s, %s", buf, reg_names[instr->op1_reg], reg_names[instr->op2_reg]); // = R[op1] op R[op2]
					else
						sprintf(buf, "%s%s, %i(%s)", buf, reg_names[instr->op1_reg], instr->imm, reg_names[instr->op2_reg]); // = R[op1] op (off)R[op2]
				}
				break;
			case I_TYPE:
				if(instr->op1_has_offset) {
					sprintf(buf, "%s%s, %i(%s)", buf, reg_names[instr->lhs_reg], instr->op1_reg_offset, reg_names[instr->op1_reg]); // R[lhs] op (off)R[op1]
				} else if(instr->addr_label != NULL) {
					sprintf(buf, "%s%s, %i, %s", buf, reg_names[instr->op1_reg], instr->imm, instr->addr_label); // R[op1] op imm addr
				} else {
					sprintf(buf, "%s%s, %s, %i", buf, reg_names[instr->lhs_reg], reg_names[instr->op1_reg], instr->imm); // R[lhs] = R[op1] op imm
				}
				break;
			case J_TYPE:
				if(instr->addr_label != NULL)
					sprintf(buf, "%s%s", buf, instr->addr_label);
				break;
			case P_TYPE: // Pseudo instructions dont follow a pattern, so each must have a finer tuned output
				if(strcmp(instr->mips_op->name, "li") == 0 || strcmp(instr->mips_op->name, "la") == 0 ) {
					sprintf(buf, "%s%s, %i", buf, reg_names[instr->lhs_reg], instr->imm); // R[lhs] = imm
				} else if(strcmp(instr->mips_op->name, "move") == 0) {
					sprintf(buf, "%s%s, %s", buf, reg_names[instr->lhs_reg], reg_names[instr->op1_reg]); // R[lhs] = R[rs]
				} else if(instr->addr_label != NULL) {
					sprintf(buf, "%s%s, %s, %s", buf, reg_names[instr->op1_reg], reg_names[instr->op2_reg], instr->addr_label); // R[op1] op R[op2] addr
				} else {
				
				}
				break;
			default:
				break;
			}
			
			// Optional Comment
			if(instr->comment != NULL && strlen(instr->comment) > 0)
				sprintf(buf, "%s # %s", buf, instr->comment);
			
			sprintf(buf, "%s\n", buf);
			
			instr = instr->next;
		}
		
		sprintf(buf, "%s\n", buf);
	
		instr_list = instr_list->next;
	}
	
	// Allocate return buffer
	char *ret = (char*)malloc(strlen(buf) + 1);
	strcpy(ret, buf);
	
	return ret;
}

// Generate and Print current listing
void mc_print_listing() {
	char *listing = mc_gen_listing();
	if(listing != NULL) {
		MCLOG(("%s\n", listing));
		free(listing);
	} else {
		MCLOG(("No listing to print\n"));
	}
}

// Generate and Write the current asm listing to a file
void mc_write_listing(const char *filename) {
	char *listing = mc_gen_listing();
	if(listing == NULL) {
		MCLOG(("No listing to write\n"));
		return;
	}

	FILE *fp = fopen(filename, "w");
	if(fp == NULL) {
		printf("Could not open %s to write assembly listing\n", filename);
		return;
	}
	
	fprintf(fp, "# jkcompiler assembly output\n# File: %s\n\n", filename);
	fprintf(fp, "%s", listing);
	
	fclose(fp);
	free(listing);
}

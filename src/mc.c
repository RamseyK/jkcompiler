#include "mc.h"

void mc_init() {
	// Reset mc_used_regs
	for(int i = 0; i < NUM_REGS; i++)
		mc_used_regs[i] = false;

	// Data Section
	mc_data_section = mc_new_section();
	
	// Default data directive
	struct directive_t *dataDir = mc_new_directive(NULL, "data");
	dataDir->no_val = true;
	
	// Align data datums on word boundaries
	struct directive_t *alignDir = mc_new_directive(NULL, "align");
	alignDir->val = 2;
	
	// Allocate space for the heap
	struct directive_t *heapDir = mc_new_directive("heap", "space");
	heapDir->val = HEAP_SIZE;
	
	// Setup newline ascii string
	struct directive_t *nlDir = mc_new_directive("newline", "asciiz");
	nlDir->val_str = new_identifier("\"\\n\"");
	
	// Emit data section directives
	mc_emit_directive(mc_data_section, dataDir);
	mc_emit_directive(mc_data_section, alignDir);
	mc_emit_directive(mc_data_section, heapDir);
	mc_emit_directive(mc_data_section, nlDir);
	
	// Text Section
	mc_text_section = mc_new_section();
	
	// Default text directive
	struct directive_t *textDir = mc_new_directive(NULL, "text");
	textDir->no_val = true;
	
	// Emit text section directives
	mc_emit_directive(mc_text_section, textDir);
}

void mc_destroy() {
	mc_free_section(mc_data_section);
	mc_free_section(mc_text_section);
	free_set(mc_processed_labels);
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
	instr->comment = (char*)malloc(64);
	memset(instr->comment, 0, 64);
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

// Add an instr_list_t node to the end of an instr_list_t list
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

// Get an instr_list_t node from a list given its label
struct instr_list_t *mc_get_instr_list(struct instr_list_t **list, const char *label) {
	if(*list == NULL || label == NULL)
		return NULL;
	
	struct instr_list_t *it = *list;
	while(it != NULL) {
		if(strcmp(it->label, label) == 0)
			return it;
		it = it->next;
	}
	
	return NULL;
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
	mem_loc->objSymbol = NULL;
	mem_loc->temp_reg = -1;
	mem_loc->wb = false;
	mem_loc->type = 0;
	mem_loc->loc.reg = 0;
	
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

// Process the master CFG list (functions) and generate machine code
void mc_consume_cfg_list() {
	struct cfg_list_t *cfg_it = cfgList;
	struct instr_list_t *cfg_instr_list = NULL;
	// Loop through function cfgs
	while(cfg_it != NULL) {
		MCLOG(("Code Generation for Block: %s\n", cfg_it->entryBlock->label));
	
		// Process the function entry block and is children recursively
		cfg_instr_list = mc_process_block(cfg_it->scope, cfg_it->entryBlock);
		
		// Add the cfg instruction listing to the text section instr lists
		mc_append_instr_list(&(mc_text_section->instrs), cfg_instr_list);
		
		cfg_it = cfg_it->next;
	}
}

// Recursively process a block and its children by converting its TAC to Instructions
struct instr_list_t *mc_process_block(struct scope_t *cfg_scope, struct block_t *block) {
	// Add the block label to the mc_processed_labels set so this will be the only time this block is processed
	if(mc_processed_labels == NULL)
		mc_processed_labels = new_set(block->label);
	else
		set_add(mc_processed_labels, block->label);
	
	// Create an instruction listing for this block
	struct instr_list_t *instr_list = mc_new_instr_list(block->label);

	// Loop through TAC nodes
	struct three_address_t *tac = block->entry, *param_tac = NULL;
	struct instr_t *instr = NULL;
	struct mem_loc_t *lhs_loc = NULL, *op1_loc = NULL, *op2_loc = NULL;
	
	while(tac != NULL) {
		// Load variables/compiler temps into registers
		mc_reset_temp_regs();
		
		// Fast-forward through parameters, only tracking the first (if any)
		param_tac = NULL;
		if(tac->op == OP_PARAM_ASSIGN) {
			param_tac = tac;
			while(tac != NULL && tac->op == OP_PARAM_ASSIGN)
				tac = tac->next;
			if(tac == NULL)
				break;
		}
		
		if(tac->op == OP_NEW_OBJ) {

			// If the lhs tac is local to the scope, then load its address.
			struct variable_declaration_t *local = symtab_lookup_variable(cfg_scope, tac->lhs->d.id);
			if(local != NULL) {
				lhs_loc = mc_mem_access_addr(instr_list, cfg_scope, tac->lhs);
			} else {
				// Otherwise load the value
				lhs_loc = mc_mem_access_var(instr_list, cfg_scope, tac->lhs);
			}

			op1_loc = mc_mem_alloc_heap(instr_list, cfg_scope, tac->op1);
		} else if(tac->op == OP_FUNC_CALL) {
			struct scope_t *class_scope = NULL, *func_scope = NULL;
			char *class_name = NULL, *func_name = tac->op2->d.id;
			struct symbol_list_t *callerSymbol = NULL;
			if(tac->op1 != NULL) { // Method in another object
				callerSymbol = symtab_get_variable_symbol(cfg_scope, tac->op1->d.id);
				class_scope = callerSymbol->objScope;
				func_scope = symtab_lookup_function(class_scope, func_name);

				MCLOG(("Function call %s in class %s\n", func_name, class_name));
			} else { // Method within current class
				func_scope = symtab_lookup_function(cfg_scope->parent, func_name);
				MCLOG(("Function call %s in same class\n", func_name));
			}
			
			// Setup function call memory
			op1_loc = mc_call_func(instr_list, func_scope, param_tac, tac->op1);
			lhs_loc = mc_mem_access_addr(instr_list, cfg_scope, tac->lhs);

			if(tac->op1 != NULL) {
				//struct mem_loc_t *callerLoc = mc_mem_access_var(instr_list, callerSymbol->objScope, tac->op1);

				struct instr_t *callerVal = mc_new_instr("lw");
				callerVal->lhs_reg = mc_next_temp_reg();
				if(callerSymbol->objScope->attrId == SYM_ATTR_CLASS) {
					callerVal->op1_reg = $s6;
					callerVal->op1_reg_offset = callerSymbol->offset;
				} else if(callerSymbol->objScope->attrId == SYM_ATTR_FUNC) {
					callerVal->op1_reg = $fp;
					callerVal->op1_reg_offset = -1 * callerSymbol->offset;
				} else {
					MCLOG(("Wrong scope?!\n"));
				}
				callerVal->op1_has_offset = true;
				sprintf(callerVal->comment, "Get the obj value to save");
				mc_emit_instr(instr_list, callerVal);

				struct instr_t *caller_instr = mc_new_instr("addi");
				caller_instr->lhs_reg = $s6;
				caller_instr->op1_reg = callerVal->lhs_reg;
				caller_instr->imm = 0;
				sprintf(caller_instr->comment, "Set caller to %s", callerSymbol->id);
				mc_emit_instr(instr_list, caller_instr);
			}
		} else if(tac->op == OP_MEM_ACCESS) { // Depending on the operator for the tac, determine how to get the location for op2
			//MCLOG(("MEM ACCESS\n"));
			lhs_loc = mc_mem_access_addr(instr_list, cfg_scope, tac->lhs);

			op1_loc = mc_mem_access_var(instr_list, cfg_scope, tac->op1);

			// Look up the offset of op1
			struct symbol_list_t *op1Offset = symtab_get_variable_symbol(cfg_scope, tac->op1->d.id);
			// Use the objScope from op1's Offset to calculate the offset for the field in op2
			op2_loc = mc_new_mem_loc(tac->op2->d.id);
			op2_loc->type = MEM_HEAP;
			struct symbol_list_t *symbol = symtab_get_variable_symbol(op1Offset->objScope, tac->op2->d.id);
			if(symbol != NULL) {
				// Found the variable in the offset list
				op2_loc->loc.offset = symbol->offset;
				op2_loc->objSymbol = symbol;
			}

			// If the lhs tacData is a temp, update its objScope to that of op2_loc
			if(tac->lhs->temporary == true) {
				MCLOG(("Updating lhs objScope for %s\n", lhs_loc->id));
				lhs_loc->objSymbol->objScope = op2_loc->objSymbol->objScope;
			}

			lhs_loc->objSymbol->memLoc = MEM_HEAP;
		} else if(tac->op == OP_ASSIGN && tac->lhs != NULL && tac->lhs->type == TAC_DATA_TYPE_FUNC_RET) {
			lhs_loc = mc_new_mem_loc(cfg_scope->fd->fh->id);
			lhs_loc->type = MEM_STACK;
			lhs_loc->loc.offset = SIZE_WORD;

			op1_loc = mc_mem_access_var(instr_list, cfg_scope, tac->op1);
			if(op1_loc == NULL)
				op1_loc = mc_mem_access_const(instr_list, tac->op1);
		} else {
			//MCLOG(("Not mem access or new\n"));
			lhs_loc = mc_mem_access_addr(instr_list, cfg_scope, tac->lhs);
			op1_loc = mc_mem_access_var(instr_list, cfg_scope, tac->op1);
			op2_loc = mc_mem_access_var(instr_list, cfg_scope, tac->op2);
		}

		// Check if the lhs should be flagged as a heap value (for ASSIGN and NEW)
		if(lhs_loc != NULL && lhs_loc->objSymbol != NULL && lhs_loc->objSymbol->memLoc == MEM_HEAP) {
			MCLOG(("Heap addr on lhs!\n"));
			lhs_loc->type = MEM_HEAP;
		}


		// Load constants into registers for instructions that do not have I_TYPE equivalents
		if(tac->op == OP_STAR || tac->op == OP_SLASH || tac->op == OP_MOD
			|| tac->op == OP_LT || tac->op == OP_GT || tac->op == OP_LE || tac->op == OP_GE
			|| tac->op == OP_EQUAL || tac->op == OP_NOTEQUAL) {
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
			//if(lhs_loc != NULL && lhs_loc->wb)
				//mc_mem_writeback(instr_list, lhs_loc);
		} else {
			MCLOG(("mc_tac_to_instr returned null!\n"));
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
		// Ensure a child block isnt being processed twice
		if(set_contains(mc_processed_labels, child->block->label)) {
			child = child->next;
			continue;
		}
	
		// Create an instruction listing for this block
		struct instr_list_t *block_instr_list = NULL;
		
		// Process the child block	
		block_instr_list = mc_process_block(cfg_scope, child->block);
		
		// Add the completed child instruction listing
		mc_append_instr_list(&instr_list, block_instr_list);
		
		child = child->next;
	}
	
	return instr_list;
}

// Convert three address code to corresponding instructions with registers/memory
struct instr_t *mc_tac_to_instr(struct three_address_t *tac, struct mem_loc_t *lhs_loc, struct mem_loc_t *op1_loc, struct mem_loc_t *op2_loc) {
	MCLOG(("Converting tac of type %i to an instr\n", tac->op));
	// Get string representations of lhs, op1 and op2
	char *lhs_str = cfg_tac_data_to_str(tac->lhs);
	char *op1_str = cfg_tac_data_to_str(tac->op1);
	char *op2_str = cfg_tac_data_to_str(tac->op2);

	// Instructions for converting rvalues from addr to val if needed
	struct instr_t *conv1Instr = NULL, *conv2Instr = NULL;
	bool conv1, conv2;
	// Assume we will convert for most instructions
	if(op1_loc != NULL && tac->op1 != NULL && tac->op1->type == TAC_DATA_TYPE_VAR && op1_loc->objSymbol != NULL && op1_loc->objSymbol->memLoc == MEM_HEAP)
		conv1 = true;
	else
		conv1 = false;

	if(op2_loc != NULL && tac->op2 != NULL && tac->op2->type == TAC_DATA_TYPE_VAR && op2_loc->objSymbol != NULL && op2_loc->objSymbol->memLoc == MEM_HEAP)
		conv2 = true;
	else
		conv2 = false;


	// Instructions for computing
	struct instr_t *instr = NULL, *instr2 = NULL, *instr3 = NULL;
	// Instruction for writing back the result
	struct instr_t *wbInstr = NULL;

	switch(tac->op) {
		case OP_PLUS:
			//lhs_loc->wb = true;
			if(tac->op2->type == TAC_DATA_TYPE_INT) {
				// Var + Int (Reg + Imm)
				instr = mc_new_instr("addi");
				//instr->lhs_reg = lhs_loc->temp_reg;
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = op1_loc->temp_reg;
				instr->imm = tac->op2->d.val;
				sprintf(instr->comment, "%s = %s + %i", lhs_str, op1_str, tac->op2->d.val);
			} else {
				// Var + Var (both regs)
				instr = mc_new_instr("add");
				//instr->lhs_reg = lhs_loc->temp_reg;
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = op1_loc->temp_reg;
				instr->op2_reg = op2_loc->temp_reg;
				sprintf(instr->comment, "%s = %s + %s", lhs_str, op1_str, op2_str);
			}

			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr->lhs_reg);
			instr->next = wbInstr;
			break;
		case OP_MINUS:
			//lhs_loc->wb = true;
			if(tac->op1->type == TAC_DATA_TYPE_VAR && tac->op2->type == TAC_DATA_TYPE_INT) {
				// Var - Int (Reg - Imm)
				instr = mc_new_instr("addi");
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = op1_loc->temp_reg;
				instr->imm = 0-tac->op2->d.val;
				sprintf(instr->comment, "%s = %s - %i", lhs_str, op1_str, tac->op2->d.val);
			} else if(tac->op1->type == TAC_DATA_TYPE_INT && tac->op2->type == TAC_DATA_TYPE_VAR) {
				// Int - Var (Imm - Reg)
				instr = mc_new_instr("addi");
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = op2_loc->temp_reg;
				instr->imm = 0-tac->op1->d.val;
				sprintf(instr->comment, "%s = %i - %s", lhs_str, tac->op1->d.val, op2_str);
			} else {
				// Var - Var (both regs)
				instr = mc_new_instr("sub");
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = op1_loc->temp_reg;
				instr->op2_reg = op2_loc->temp_reg;
				sprintf(instr->comment, "%s = %s - %s", lhs_str, op1_str, op2_str);
			}
			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr->lhs_reg);
			instr->next = wbInstr;
			break;
		case OP_OR:
			//lhs_loc->wb = true;
			if(tac->op1->type == TAC_DATA_TYPE_VAR && tac->op2->type == TAC_DATA_TYPE_VAR) {
				// Var OR Var
				instr = mc_new_instr("or");
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = op1_loc->temp_reg;
				instr->op2_reg = op2_loc->temp_reg;
			} else if(tac->op1->type == TAC_DATA_TYPE_VAR) {
				// Var OR Const
				instr = mc_new_instr("ori");
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = op1_loc->temp_reg;
				if(tac->op2->type == TAC_DATA_TYPE_INT)
					instr->imm = tac->op2->d.val;
				else if(tac->op2->type == TAC_DATA_TYPE_BOOL)
					instr->imm = tac->op2->d.b;
				else
					MCLOG(("Invalid OR operand\n"));
			} else if(tac->op2->type == TAC_DATA_TYPE_VAR) {
				// Const OR Var
				instr = mc_new_instr("ori");
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = op2_loc->temp_reg;
				if(tac->op1->type == TAC_DATA_TYPE_INT)
					instr->imm = tac->op1->d.val;
				else if(tac->op1->type == TAC_DATA_TYPE_BOOL)
					instr->imm = tac->op1->d.b;
				else
					MCLOG(("Invalid OR operand\n"));
			} else {
				MCLOG(("Invalid OR operand\n"));
			}
			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr->lhs_reg);
			instr->next = wbInstr;
			break;
		case OP_STAR:
			//lhs_loc->wb = true;
			instr = mc_new_instr("mult");
			// Var * Var (both must be regs)
			instr->op1_reg = op1_loc->temp_reg;
			instr->op2_reg = op2_loc->temp_reg;
			sprintf(instr->comment, "Lo = %s * %s", op1_str, op2_str);
			// Move the result from Lo into the LHS reg
			instr2 = mc_new_instr("mflo");
			instr2->lhs_reg = mc_next_temp_reg();
			sprintf(instr2->comment, "%s = Lo", lhs_str);
			instr->next = instr2; // Link mult and mflo instructions

			// Store
			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr2->lhs_reg);
			instr2->next = wbInstr;
			break;
		case OP_SLASH:
			//lhs_loc->wb = true;
			instr = mc_new_instr("div");
			// Var * Var (both must be regs)
			instr->op1_reg = op1_loc->temp_reg;
			instr->op2_reg = op2_loc->temp_reg;
			sprintf(instr->comment, "Lo = %s / %s", op1_str, op2_str);
			// Move the result from Lo into the LHS reg
			instr2 = mc_new_instr("mflo");
			instr2->lhs_reg = mc_next_temp_reg();
			sprintf(instr2->comment, "%s = Lo", lhs_str);
			instr->next = instr2; // Link div and mflo instructions

			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr2->lhs_reg);
			instr2->next = wbInstr;
			break;
		case OP_MOD:
			//lhs_loc->wb = true;
			instr = mc_new_instr("div");
			// Var * Var (both must be regs)
			instr->op1_reg = op1_loc->temp_reg;
			instr->op2_reg = op2_loc->temp_reg;
			sprintf(instr->comment, "Hi = %s / %s", op1_str, op2_str);
			// Move the result from Hi into the LHS reg
			instr2 = mc_new_instr("mfhi");
			instr2->lhs_reg = mc_next_temp_reg();
			sprintf(instr2->comment, "%s = Hi (Remainder)", lhs_str);

			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr2->lhs_reg);
			instr2->next = wbInstr;
			break;
		case OP_AND:
			//lhs_loc->wb = true;
			if(tac->op1->type == TAC_DATA_TYPE_VAR && tac->op2->type == TAC_DATA_TYPE_VAR) {
				// Var AND Var
				instr = mc_new_instr("and");
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = op1_loc->temp_reg;
				instr->op2_reg = op2_loc->temp_reg;
			} else if(tac->op1->type == TAC_DATA_TYPE_VAR) {
				// Var AND Const
				instr = mc_new_instr("andi");
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = op1_loc->temp_reg;
				if(tac->op2->type == TAC_DATA_TYPE_INT)
					instr->imm = tac->op2->d.val;
				else if(tac->op2->type == TAC_DATA_TYPE_BOOL)
					instr->imm = tac->op2->d.b;
				else
					MCLOG(("Invalid AND operand\n"));
			} else if(tac->op2->type == TAC_DATA_TYPE_VAR) {
				// Const AND Var
				instr = mc_new_instr("andi");
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = op2_loc->temp_reg;
				if(tac->op1->type == TAC_DATA_TYPE_INT)
					instr->imm = tac->op1->d.val;
				else if(tac->op1->type == TAC_DATA_TYPE_BOOL)
					instr->imm = tac->op1->d.b;
				else
					MCLOG(("Invalid AND operand\n"));
			} else {
				MCLOG(("Invalid AND operand\n"));
			}

			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr->lhs_reg);
			instr->next = wbInstr;
			break;
		case OP_EQUAL:
			//lhs_loc->wb = true;
			instr = mc_new_instr("seq");
			instr->lhs_reg = mc_next_temp_reg();
			instr->op1_reg = op1_loc->temp_reg;
			instr->op2_reg = op2_loc->temp_reg;
			sprintf(instr->comment, "%s = %s == %s", lhs_str, op1_str, op2_str);

			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr->lhs_reg);
			instr->next = wbInstr;
			break;
		case OP_NOTEQUAL:
			//lhs_loc->wb = true;
			instr = mc_new_instr("sne");
			instr->lhs_reg = mc_next_temp_reg();
			instr->op1_reg = op1_loc->temp_reg;
			instr->op2_reg = op2_loc->temp_reg;
			sprintf(instr->comment, "%s = %s != %s", lhs_str, op1_str, op2_str);

			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr->lhs_reg);
			instr->next = wbInstr;
			break;
		case OP_LT:
			//lhs_loc->wb = true;
			instr = mc_new_instr("slt");
			instr->lhs_reg = mc_next_temp_reg();
			instr->op1_reg = op1_loc->temp_reg;
			instr->op2_reg = op2_loc->temp_reg;
			sprintf(instr->comment, "%s = %s < %s", lhs_str, op1_str, op2_str);

			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr->lhs_reg);
			instr->next = wbInstr;
			break;
		case OP_GT:
			//lhs_loc->wb = true;
			instr = mc_new_instr("sgt");
			instr->lhs_reg = mc_next_temp_reg();
			instr->op1_reg = op1_loc->temp_reg;
			instr->op2_reg = op2_loc->temp_reg;
			sprintf(instr->comment, "%s = %s > %s", lhs_str, op1_str, op2_str);

			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr->lhs_reg);
			instr->next = wbInstr;
			break;
		case OP_LE:
			//lhs_loc->wb = true;
			instr = mc_new_instr("sle");
			instr->lhs_reg = mc_next_temp_reg();
			instr->op1_reg = op1_loc->temp_reg;
			instr->op2_reg = op2_loc->temp_reg;
			sprintf(instr->comment, "%s = %s <= %s", lhs_str, op1_str, op2_str);

			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr->lhs_reg);
			instr->next = wbInstr;
			break;
		case OP_GE:
			//lhs_loc->wb = true;
			instr = mc_new_instr("sge");
			instr->lhs_reg = mc_next_temp_reg();
			instr->op1_reg = op1_loc->temp_reg;
			instr->op2_reg = op2_loc->temp_reg;
			sprintf(instr->comment, "%s = %s >= %s", lhs_str, op1_str, op2_str);

			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr->lhs_reg);
			instr->next = wbInstr;
			break;
		case OP_ASSIGN: // Move (avoids generating pseudo move or li instructions)
			// If the lhs represents an object on the heap then store the value directly to that addr
			// and don't perform a writeback (would overwrite the addr with the value - bad)


			// Two parts:  Put the value to be stored in a register, writeback
			// Put the value in a temp reg
			instr = mc_new_instr("addi");
			if(tac->op1->type == TAC_DATA_TYPE_VAR) {
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = op1_loc->temp_reg;
				instr->op2_reg = $0;
				sprintf(instr->comment, "%s = %s", lhs_str, op1_str);
			} else if(tac->op1->type == TAC_DATA_TYPE_INT) {
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = $0;
				instr->imm = tac->op1->d.val;
				sprintf(instr->comment, "%s = %s", lhs_str, op1_str);
			} else if(tac->op1->type == TAC_DATA_TYPE_BOOL) {
				instr->lhs_reg = mc_next_temp_reg();
				instr->op1_reg = $0;
				instr->imm = (int)tac->op1->d.b;
				sprintf(instr->comment, "%s = %s", lhs_str, (tac->op1->d.b ? "TRUE" : "FALSE"));
			} else {
				MCLOG(("Error: Unknown assign operation"));
			}

			// Store in the appropriate place
			if(tac->lhs->type == TAC_DATA_TYPE_VAR) {
				wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr->lhs_reg);
				instr->next = wbInstr;
			} else if(tac->lhs->type == TAC_DATA_TYPE_FUNC_RET) {
				// Write back to the stack
				instr = mc_new_instr("sw");
				instr->lhs_reg = op1_loc->temp_reg;
				instr->op1_reg = $fp;
				instr->op1_has_offset = true;
				instr->op1_reg_offset = lhs_loc->loc.offset;
				sprintf(instr->comment, "Setting return value to %s (on stack)", lhs_loc->id);
			} else {
				MCLOG(("Invalid LHS tac data type for ASSIGN\n"));
				mc_free_instr(instr);
				return NULL;
			}

			// If the lhs is MEM_HEAP, load its value into itself so writeback is done correctly
			// Don't do this if the lhs is a return tac data
			if(tac->lhs->type != TAC_DATA_TYPE_FUNC_RET && lhs_loc->objSymbol != NULL && lhs_loc->objSymbol->memLoc == MEM_HEAP) {
				instr2 = mc_new_instr("lw");
				instr2->lhs_reg = lhs_loc->temp_reg;
				instr2->op1_reg = lhs_loc->temp_reg;
				instr2->op1_has_offset = true;
				instr2->op1_reg_offset = 0;
				sprintf(instr2->comment, "Load heap addr");

				// Insert this instruction between isntr1 and the wb
				instr2->next = wbInstr;
				instr->next = instr2;
			}

			break;
		case OP_BRANCH:
			if(tac->op2->type == TAC_DATA_TYPE_LABEL) {
				instr = mc_new_instr("beq");
				instr->op1_reg = op1_loc->temp_reg;
				instr->imm = true;
				instr->addr_label = tac->op2->d.id;
				sprintf(instr->comment, "if %s goto %s", op1_str, instr->addr_label);
			} else {
				MCLOG(("Error: Invalid BRANCH TAC"));
			}
			break;
		case OP_GOTO:
			if(tac->op2->type == TAC_DATA_TYPE_LABEL) {
				instr = mc_new_instr("j");
				instr->addr_label = tac->op2->d.id;
			} else {
				MCLOG(("Error: Invalid GOTO TAC"));
			}
			break;
		case OP_CALLER_ASSIGN:
			break;
		case OP_PARAM_ASSIGN:
			// Constant: lw
			
			// Variable: addi address
			break;
		case OP_FUNC_CALL:			
			// Load from the stack
			instr = mc_new_instr("lw");
			instr->lhs_reg = mc_next_temp_reg();
			instr->op1_reg = $sp;
			instr->op1_has_offset = true;
			instr->op1_reg_offset = op1_loc->loc.offset;
			sprintf(instr->comment, "Load Return value %s from stack", op1_loc->id);
			
			conv1 = false;
			conv2 = false;

			// Write back the return value
			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr->lhs_reg);
			instr->next = wbInstr;
			break;
		case OP_FUNC_RETURN:
			// Jump to the return address
			instr = mc_new_instr("jr");
			instr->lhs_reg = $ra;
			sprintf(instr->comment, "return");
			break;
		case OP_MEM_ACCESS:
			// At this point we have:
			// lhs_loc is a reg that contains the address where our result goes
			// op1_loc is a reg that contains the address of the object on the heap
			// op2_loc contains the offset for the field being accessed

			instr = mc_new_instr("addi");
			instr->lhs_reg = mc_next_temp_reg();
			instr->op1_reg = op1_loc->temp_reg;
			instr->imm = op2_loc->loc.offset;
			sprintf(instr->comment, "Calc addr");

			wbInstr = mc_mem_writeback(lhs_loc->temp_reg, instr->lhs_reg);
			instr->next = wbInstr;

			// Don't convert ops
			conv1 = false;
			conv2 = false;

			break;
		case OP_NEW_OBJ:
			// lhs_loc is a reg that has the addr where the result goes
			// op1_loc contains the address from the heap that we want to store

			// If the lhs represents something on the heap, then we have to store on the heap before writing back
			//if(lhs_loc->objSymbol->memLoc == MEM_HEAP) {
			if(lhs_loc->type == MEM_HEAP) {
				instr = mc_new_instr("sw");
				instr->op1_reg = lhs_loc->temp_reg;
				instr->lhs_reg = op1_loc->temp_reg;
				instr->op1_has_offset = true;
				instr->op1_reg_offset = 0;
				sprintf(instr->comment, "%s = new %s (to heap)", lhs_loc->id, op1_loc->id);

				// Move the addr so it can be written back to the stack
				/*instr2 = mc_new_instr("addi");
				instr2->lhs_reg = lhs_loc->temp_reg;
				instr2->op1_reg = op1_loc->temp_reg;
				instr2->imm = 0;
				sprintf(instr2->comment, "Move to %s", lhs_loc->id);*/

				// Write back to the stack
				wbInstr = mc_new_instr("sw");
				wbInstr->lhs_reg = op1_loc->temp_reg;
				wbInstr->op1_reg = $fp;
				wbInstr->op1_has_offset = true;
				wbInstr->op1_reg_offset = -1 * lhs_loc->loc.offset;
				sprintf(wbInstr->comment, "Store %s (on stack)", lhs_loc->id);

				conv1 = false;
				conv2 = false;

				/*instr->next = instr2;
				instr2->next = wbInstr;*/
				instr->next = wbInstr;
			} else {
				/*// Move the addr so it can be written back to the stack
				instr = mc_new_instr("addi");
				instr->lhs_reg = lhs_loc->temp_reg;
				instr->op1_reg = op1_loc->temp_reg;
				instr->imm = 0;
				sprintf(instr->comment, "%s = new %s", lhs_loc->id, op1_loc->id);

				// Write back to the stack
				wbInstr = mc_new_instr("sw");
				wbInstr->lhs_reg = lhs_loc->temp_reg;
				wbInstr->op1_reg = $fp;
				wbInstr->op1_has_offset = true;
				wbInstr->op1_reg_offset = -1 * lhs_loc->loc.offset;
				sprintf(wbInstr->comment, "Store %s (on stack)", lhs_loc->id);

				instr->next = wbInstr;
				*/

				instr = mc_new_instr("sw");
				instr->lhs_reg = op1_loc->temp_reg;
				instr->op1_reg = lhs_loc->temp_reg;
				instr->op1_has_offset = true;
				instr->op1_reg_offset = 0;
				sprintf(instr->comment, "Store %s", lhs_loc->id);

				conv1 = false;
				conv2 = false;
			}

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

			// Set syscall service number for print_string
			struct instr_t *instr4 = mc_new_instr("addi");
			instr4->lhs_reg = $v0;
			instr4->op1_reg = $0;
			instr4->imm = 4;

			// Set string address to print
			struct instr_t *instr5 = mc_new_instr("la");
			instr5->lhs_reg = $a0;
			instr5->addr_label = new_identifier("newline");

			// Execute the syscall
			struct instr_t *instr6 = mc_new_instr("syscall");
			sprintf(instr6->comment, "print_string (newline)");

			// Link instructions in order
			instr->next = instr2;
			instr2->next = instr3;
			instr3->next = instr4;
			instr4->next = instr5;
			instr5->next = instr6;
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

	// Perform any op conversions then insert these before instr
	// The conversion basically loads the value into itself
	if(conv1) {
		conv1Instr = mc_new_instr("lw");
		conv1Instr->lhs_reg = op1_loc->temp_reg;
		conv1Instr->op1_reg = op1_loc->temp_reg;
		conv1Instr->op1_has_offset = true;
		conv1Instr->op1_reg_offset = 0;
		sprintf(conv1Instr->comment, "Convert %s from addr to val", op1_loc->id);
	}
	if(conv2) {
		conv2Instr = mc_new_instr("lw");
		conv2Instr->lhs_reg = op2_loc->temp_reg;
		conv2Instr->op1_reg = op2_loc->temp_reg;
		conv2Instr->op1_has_offset = true;
		conv2Instr->op1_reg_offset = 0;
		sprintf(conv2Instr->comment, "Convert %s from addr to val", op2_loc->id);
	}
	// Link the instructions
	if(conv1 && conv2) {
		conv1Instr->next = conv2Instr;
		conv2Instr->next = instr;
		return conv1Instr;
	} else if(conv1) {
		conv1Instr->next = instr;
		return conv1Instr;
	} else if(conv2) {
		conv2Instr->next = instr;
		return conv2Instr;
	} else {
		return instr;
	}
}

// Generate instructions to control Transfer into and out of a function
// Setup the stack and jump and link into the entry block, then destroy the stack
struct mem_loc_t *mc_call_func(struct instr_list_t *instr_list, struct scope_t *funcScope, struct three_address_t *param_tac, struct tac_data_t *callerTac) {
	// Look through the master cfg list and find the cfg_list node
	struct cfg_list_t *cfg_it = cfgList;
	while(cfg_it != NULL) {
		if(cfg_it->scope == funcScope)
			break;
		cfg_it = cfg_it->next;
	}
	
	// Set the entry block label for the function
	if(cfg_it->entryBlock->label == NULL) {
		MCLOG(("Unable to find entry block label for function call\n"));
		return NULL;
	}
	
	// Setup a memory location for the return value
	// Saved registers: s0-s7 + Always preserved: fp, ra + Space for return value
	int backup_size = SIZE_WORD*mc_num_saved_regs_used() + SIZE_WORD*2 + SIZE_WORD;
	struct mem_loc_t *retLoc = mc_new_mem_loc(funcScope->fd->fh->id);
	retLoc->type = MEM_STACK;
	retLoc->loc.offset = (backup_size-SIZE_WORD)*-1;

	// Allocate variables on the stack and backup register values
	mc_alloc_stack(instr_list, funcScope, param_tac);

	// Jump into entry block
	struct instr_t *instr = mc_new_instr("jal");
	instr->addr_label = new_identifier(cfg_it->entryBlock->label);
	sprintf(instr->comment, "%s", funcScope->fd->fh->id);
	mc_emit_instr(instr_list, instr);
	
	// Deallocate stack and restore register values
	mc_dealloc_stack(instr_list, funcScope);
	
	return retLoc;
}

// Generate instructions and allocate temporary registers to access identifiers in memory
struct mem_loc_t *mc_mem_access_var(struct instr_list_t *instr_list, struct scope_t *cfg_scope, struct tac_data_t *td) {
	if(td == NULL || td->type != TAC_DATA_TYPE_VAR)
		return NULL;

	struct mem_loc_t *mem_loc = NULL;
	struct scope_t *scope = NULL;
	
	// Get scope for the variable in the TAC
	MCLOG(("Looking for scope for %s\n",td->d.id));
	scope = symtab_lookup_variable_scope(cfg_scope, td->d.id);
	if(scope == NULL) {
		// Or check if its a parameter in the current CFG
		if(symtab_lookup_function_param(cfg_scope, td->d.id) != NULL)
			scope = cfg_scope;
		
		// Still NULL? Last resort is to check the temporaries set
		if(scope == NULL) {
			// Could not find scope, so it might be a temporary
			if(set_contains(cfg_scope->temps, td->d.id)) { // Temporaries should exist in the function scopes temps set
				scope = cfg_scope;
			} else {
				MCLOG(("Could not find temporary %s in the scopes temps set\n", td->d.id));
				return mem_loc;
			}
		}
	}


	// Determine if the variable is in a register, on stack, or on heap
	// TODO: Register allocation list needs to be processed here. Right now, we're not storing anything in saved regs
	
	// On Stack if the scope of the variable matches the cfg_scope
	if(scope->attrId == SYM_ATTR_FUNC) {
		mem_loc = mc_new_mem_loc(td->d.id);
		mem_loc->type = MEM_STACK;
		
		// In this case the value is on the call stack for a function
		struct symbol_list_t *symbol = symtab_get_variable_symbol(scope, td->d.id);
		if(symbol != NULL) {
			// Found the variable in the offset list
			mem_loc->temp_reg = mc_next_temp_reg();
			mem_loc->loc.offset = symbol->offset;
			mem_loc->objSymbol = symbol;
			
			struct instr_t *instr = mc_new_instr("lw");
			instr->lhs_reg = mem_loc->temp_reg;
			instr->op1_reg = $fp;
			instr->op1_has_offset = true;
			instr->op1_reg_offset = -1 * mem_loc->loc.offset;
			sprintf(instr->comment, "Load %s (from stack)", td->d.id);
			mc_emit_instr(instr_list, instr);
		} else {
			MCLOG(("Could not find the offset for variable %s\n",td->d.id));
		}
	} else if(scope->attrId == SYM_ATTR_CLASS) {
		mem_loc = mc_new_mem_loc(td->d.id);
		mem_loc->type = MEM_HEAP;
		
		// In this case the value is on the heap for the function caller (in $s6)
		struct symbol_list_t *symbol = symtab_get_variable_symbol(scope, td->d.id);
		if(symbol != NULL) {
			// Found the variable in the offset list
			mem_loc->temp_reg = mc_next_temp_reg();
			mem_loc->loc.offset = symbol->offset;
			mem_loc->objSymbol = symbol;
			//symbol->memLoc = MEM_HEAP;
			if(symbol->objScope == NULL) {
				symbol->memLoc = MEM_STACK;
			} else {
				symbol->memLoc = MEM_HEAP;
			}

			struct instr_t *instr = mc_new_instr("lw");
			instr->lhs_reg = mem_loc->temp_reg;
			instr->op1_reg = $s6;
			instr->op1_has_offset = true;
			instr->op1_reg_offset = mem_loc->loc.offset;
			sprintf(instr->comment, "Load %s (from heap)", td->d.id);
			mc_emit_instr(instr_list, instr);

		} else {
			MCLOG(("Could not find the offset for variable %s\n",td->d.id));
		}

	} else {
		MCLOG(("Scope of variable %s is unhandled: %i\n", td->d.id, scope->attrId));
		return mem_loc;
	}
	
	return mem_loc;
}

// Generate instructions to allocate a temporary register to hold a constant
// This is needed for instructions that do not have I_TYPE counterparts to their R_TYPE
struct mem_loc_t *mc_mem_access_const(struct instr_list_t *instr_list, struct tac_data_t *td) {
	if(td == NULL || td->type == TAC_DATA_TYPE_VAR || td->type == TAC_DATA_TYPE_KEYWORD || td->type == TAC_DATA_TYPE_LABEL)
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

struct mem_loc_t *mc_mem_access_addr(struct instr_list_t *instr_list, struct scope_t *cfg_scope, struct tac_data_t *td) {
	if(td == NULL || td->type != TAC_DATA_TYPE_VAR)
		return NULL;

	struct mem_loc_t *mem_loc = NULL;
	struct scope_t *scope = NULL;

	// Get scope for the variable in the TAC
	MCLOG(("Looking for scope for %s\n",td->d.id));
	scope = symtab_lookup_variable_scope(cfg_scope, td->d.id);
	if(scope == NULL) {
		// Could not find scope, so it might be a temporary
		if(set_contains(cfg_scope->temps, td->d.id)) { // Temporaries should exist in the function scopes temps set
			scope = cfg_scope;
		} else {
			MCLOG(("Could not find temporary %s in the scopes temps set\n", td->d.id));
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
		struct symbol_list_t *symbol = symtab_get_variable_symbol(scope, td->d.id);
		if(symbol != NULL) {
			// Found the variable in the offset list
			mem_loc->temp_reg = mc_next_temp_reg();
			mem_loc->loc.offset = symbol->offset;
			mem_loc->objSymbol = symbol;

			struct instr_t *instr = mc_new_instr("addi");
			instr->lhs_reg = mem_loc->temp_reg;
			instr->op1_reg = $fp;
			instr->imm = -1 * mem_loc->loc.offset;
			sprintf(instr->comment, "Load addr %s (from stack)", td->d.id);
			mc_emit_instr(instr_list, instr);
			//printf("In mem_acces_var emitted instr for %s\n", mem_loc->id);
		} else {
			MCLOG(("Could not find the offset for variable %s\n",td->d.id));
		}
	} else if(scope->attrId == SYM_ATTR_CLASS) {
		mem_loc = mc_new_mem_loc(td->d.id);
		mem_loc->type = MEM_HEAP;

		// Emit instruction to load the heap offset value into a temp reg
		struct symbol_list_t *symbol = symtab_get_variable_symbol(scope, td->d.id);
		if(symbol != NULL) {

			// Check if the symbols objScope is null
			// If it isn't null then its not a primitive type and we should load the address-value stored
			// At this location instead of the address of the actual location
			if(symbol->objScope == NULL) {
				MCLOG(("%s's symbol's objScope is null, so changing its memLoc to stack\n", mem_loc->id));
				symbol->memLoc = MEM_STACK;
			}

			// Found the variable in the symbol list
			mem_loc->temp_reg = mc_next_temp_reg();
			mem_loc->loc.offset = symbol->offset;
			mem_loc->objSymbol = symbol;

			struct instr_t *instr = mc_new_instr("addi");
			instr->lhs_reg = mem_loc->temp_reg;
			instr->op1_reg = $s6;
			instr->imm = mem_loc->loc.offset;
			sprintf(instr->comment, "Load addr %s (from heap)", td->d.id);
			mc_emit_instr(instr_list, instr);

		} else {
			MCLOG(("Could not find the offset for variable %s\n",td->d.id));
		}

	} else {
		MCLOG(("Scope of variable %s is unhandled: %i\n", td->d.id, scope->attrId));
		return mem_loc;
	}

	return mem_loc;
}

// Write back a register value to the permanent location
// OLD VERSION
/*void mc_mem_writeback(struct instr_list_t *instr_list, struct mem_loc_t *mem_loc) {
	if(mem_loc == NULL || mem_loc->temp_reg == -1)
		return;

	if(mem_loc->type == MEM_STACK) {
		struct instr_t *instr = mc_new_instr("sw");
		instr->lhs_reg = mem_loc->temp_reg;
		instr->op1_reg = $fp;
		instr->op1_has_offset = true;
		instr->op1_reg_offset = -1 * mem_loc->loc.offset;
		sprintf(instr->comment, "Store %s", mem_loc->id);
		mc_emit_instr(instr_list, instr);
	} else {
		MCLOG(("Unsupported write back for mem_loc of type %i\n", mem_loc->type));
	}
}*/

// NEW VERSION
// Basically creates a sw instr to store the value in valueReg at 0(addrReg)
struct instr_t *mc_mem_writeback(int addrReg, int valueReg) {

	struct instr_t *instr = mc_new_instr("sw");
	instr->lhs_reg = valueReg;
	instr->op1_reg = addrReg;
	instr->op1_has_offset = true;
	instr->op1_reg_offset = 0;
	sprintf(instr->comment, "Writeback");
	return instr;

}

// Returns the next available temporary register and marks it as used. -1 if none available
int mc_next_temp_reg() {
	for(int i = $t0; i <= $t7; i++) {
		if(!mc_used_regs[i]) {
			mc_used_regs[i] = true;
			return i;
		}
	}
	for(int i = $t8; i <= $t9; i++) {
		if(!mc_used_regs[i]) {
			mc_used_regs[i] = true;
			return i;
		}
	}
	return -1;
}

// Resets all temporary registers to unused
void mc_reset_temp_regs() {
	for(int i = $t0; i <= $t7; i++) {
		mc_used_regs[i] = false;
	}
	for(int i = $t8; i <= $t9; i++) {
		mc_used_regs[i] = false;
	}
}

// Gets the next available saved register and marks it as used. -1 if none available
int mc_next_saved_reg() {
	for(int i = $s0; i <= $s7; i++) {
		if(!mc_used_regs[i]) {
			mc_used_regs[i] = true;
			return i;
		}
	}
	return -1;
}

// Returns the number of saved registers in use
int mc_num_saved_regs_used() {
	int count = 0;
	for(int i = $s0; i <= $s7; i++) {
		if(mc_used_regs[i])
			count++;
	}
	return count;
}

// Backup preserved registers and allocate stack memory for each variable and emits the appropriate instructions
// Called just before jumping into a new function
void mc_alloc_stack(struct instr_list_t *cfg_instr_list, struct scope_t *scope, struct three_address_t *param_tac) {
	// Saved registers: s0-s7 + Always preserved: fp, ra + Space for return value
	int backup_size = SIZE_WORD*mc_num_saved_regs_used() + SIZE_WORD*2 + SIZE_WORD;
	int frame_size = symtab_calc_scope_size(scope);
	int sp_size = frame_size+backup_size;
	int sp_pointer = 0;
	struct instr_t *instr = NULL;
		
	// Backup $fp
	instr = mc_new_instr("sw");
	instr->lhs_reg = $fp;
	instr->op1_reg = $sp;
	instr->op1_has_offset = true;
	instr->op1_reg_offset = sp_pointer;
	sp_pointer -= SIZE_WORD;
	sprintf(instr->comment, "Save Frame Pointer");
	mc_emit_instr(cfg_instr_list, instr);
	
	// Backup $ra
	instr = mc_new_instr("sw");
	instr->lhs_reg = $ra;
	instr->op1_reg = $sp;
	instr->op1_has_offset = true;
	instr->op1_reg_offset = sp_pointer;
	sp_pointer -= SIZE_WORD;
	sprintf(instr->comment, "Save Return Address");
	mc_emit_instr(cfg_instr_list, instr);
	
	// Backup registers $s0 - $s7 if in use
	for(int i = $s0; i <= $s7; i++) {
		if(!mc_used_regs[i])
			continue;
	
		instr = mc_new_instr("sw");
		instr->lhs_reg = i;
		instr->op1_reg = $sp;
		instr->op1_has_offset = true;
		instr->op1_reg_offset = sp_pointer;
		sp_pointer -= SIZE_WORD;
		mc_emit_instr(cfg_instr_list, instr);
	}
	
	// Save parameters on the frame
	int param_off = 0;
	struct mem_loc_t *param_loc = NULL;
	struct instr_t *param_instr = NULL;
	while(param_tac != NULL && param_tac->op == OP_PARAM_ASSIGN) {
		switch(param_tac->op1->type) {
		case TAC_DATA_TYPE_VAR:
			MCLOG(("Saving parameter %s on the frame\n", param_tac->op1->d.id));
			
			param_loc = mc_mem_access_var(cfg_instr_list, scope, param_tac->op1);
			break;
		case TAC_DATA_TYPE_INT:
			MCLOG(("Saving parameter %i on the frame\n", param_tac->op1->d.val));
			
			param_loc = mc_mem_access_const(cfg_instr_list, param_tac->op1);
			break;
		case TAC_DATA_TYPE_BOOL:
			MCLOG(("Saving parameter %s on the frame\n", (param_tac->op1->d.b ? "TRUE" : "FALSE")));
			
			param_loc = mc_mem_access_const(cfg_instr_list, param_tac->op1);
			break;
		default:
			MCLOG(("mc_alloc_stack: Unknown parameter type\n"));
			break;
		}
		
		// Generate store the parameters value to the frame
		if(param_loc != NULL) {
			instr = mc_new_instr("sw");
			instr->lhs_reg = param_loc->temp_reg;
			instr->op1_reg = $fp;
			instr->op1_has_offset = true;
			instr->op1_reg_offset = param_off;
			param_off -= SIZE_WORD;
			sp_pointer -= SIZE_WORD;
			sprintf(instr->comment, "Argument %i", (param_off/-4));
			if(param_instr == NULL) {
				param_instr = instr;
			} else {
				GOTO_END_OF_LIST(param_instr);
				param_instr->next = instr;
			}
		}
		
		param_tac = param_tac->next;
	}
	
	// Set new Frame Pointer to start of local variables
	instr = mc_new_instr("addi");
	instr->lhs_reg = $fp;
	instr->op1_reg = $sp;
	instr->imm = 0-backup_size;
	sprintf(instr->comment, "Move Frame Pointer");
	mc_emit_instr(cfg_instr_list, instr);
	
	// Move the stack pointer addi $sp, $sp, -sp_size
	instr = mc_new_instr("addi");
	instr->lhs_reg = $sp;
	instr->op1_reg = $sp;
	instr->imm = 0-sp_size;
	sprintf(instr->comment, "Move Stack Pointer");
	mc_emit_instr(cfg_instr_list, instr);
	
	// Emit param store instructions
	mc_emit_instr(cfg_instr_list, param_instr);
}

// Restore preserved registers and deallocate stack memory
void mc_dealloc_stack(struct instr_list_t *cfg_instr_list, struct scope_t *scope) {
	// Saved registers: s0-s7 + Always preserved: fp, ra + Space for return value
	int backup_size = SIZE_WORD*mc_num_saved_regs_used() + SIZE_WORD*2 + SIZE_WORD;
	int frame_size = symtab_calc_scope_size(scope);
	int sp_size = frame_size+backup_size;
	int sp_pointer = 0;
	struct instr_t *instr = NULL;
	
	// Deallocate space on the stack addi $sp, $sp, sp_size
	instr = mc_new_instr("addi");
	instr->lhs_reg = $sp;
	instr->op1_reg = $sp;
	instr->imm = sp_size;
	sprintf(instr->comment, "Deallocate stack");
	mc_emit_instr(cfg_instr_list, instr);
	
	// Restore $fp
	instr = mc_new_instr("lw");
	instr->lhs_reg = $fp;
	instr->op1_reg = $sp;
	instr->op1_has_offset = true;
	instr->op1_reg_offset = sp_pointer;
	sp_pointer -= SIZE_WORD;
	sprintf(instr->comment, "Restore Frame Pointer");
	mc_emit_instr(cfg_instr_list, instr);
	
	// Restore $ra
	instr = mc_new_instr("lw");
	instr->lhs_reg = $ra;
	instr->op1_reg = $sp;
	instr->op1_has_offset = true;
	instr->op1_reg_offset = sp_pointer;
	sp_pointer -= SIZE_WORD;
	sprintf(instr->comment, "Restore Return Address");
	mc_emit_instr(cfg_instr_list, instr);
	
	// Restore registers $s7 - $s0 if used
	for(int i = $s7; i >= $s0; i--) {
		if(!mc_used_regs[i])
			continue;
		
		instr = mc_new_instr("lw");
		instr->lhs_reg = i;
		instr->op1_reg = $sp;
		instr->op1_has_offset = true;
		instr->op1_reg_offset = sp_pointer;
		sp_pointer -= SIZE_WORD;
		mc_emit_instr(cfg_instr_list, instr);
	}
}

//TODO: FINISH
struct mem_loc_t *mc_mem_alloc_heap(struct instr_list_t *instr_list, struct scope_t *cfg_scope, struct tac_data_t *td) {
	// Create the mem_loc_t
	struct mem_loc_t *mem_loc = mc_new_mem_loc(td->d.id);
	// Type is heap
	mem_loc->type = MEM_HEAP;

	// Lookup the class to get the scope
	struct scope_t *scope = symtab_lookup_class(td->d.id);

	mem_loc->temp_reg = mc_next_temp_reg();

	// Calculate the size needed from the heap for this object
	int heapSize = symtab_calc_scope_size(scope);

	// Save the heap pointer to the memory location for the lhs
	// Copy the heap pointer to the temp register for op1
	struct instr_t *set_lvalue_instr = mc_new_instr("addi");
	set_lvalue_instr->lhs_reg = mem_loc->temp_reg;
	set_lvalue_instr->op1_reg = $s7;
	set_lvalue_instr->imm = 0;
	sprintf(set_lvalue_instr->comment, "Copying HP");
	mc_emit_instr(instr_list, set_lvalue_instr);

	// Update the heap pointer to create room for the object
	struct instr_t *alloc_heap_instr = mc_new_instr("addi");
	alloc_heap_instr->lhs_reg = $s7;
	alloc_heap_instr->op1_reg = $s7;
	alloc_heap_instr->imm = heapSize;
	sprintf(alloc_heap_instr->comment, "Move HP");
	mc_emit_instr(instr_list, alloc_heap_instr);

	return mem_loc;
}

// Generates .globl main_func_name, and extra bootstrap code to jump into the main func of the main class, and termination syscall when last method returns
void mc_add_bootstrap(char *program_name) {
	// Label the bootstrap function
	struct directive_t *gldir = mc_new_directive(NULL, "globl");
	gldir->val_str = new_identifier("main");
	mc_emit_directive(mc_text_section, gldir);
	
	// Create instruction listing for the bootstrap
	struct instr_list_t *instr_list = mc_new_instr_list("main");
	
	// Load the address of the heap into $s7
	struct instr_t *instr = mc_new_instr("la");
	instr->lhs_reg = $s7;
	instr->addr_label = new_identifier("heap");
	mc_emit_instr(instr_list, instr);

	// Instantiate the program object on the heap and load it as the calling object in $s6
	// First move the heap pointer to s6
	instr = mc_new_instr("addi");
	instr->lhs_reg = $s6;
	instr->op1_reg = $s7;
	instr->imm = 0;
	sprintf(instr->comment, "Heap alloc for main obj");
	mc_emit_instr(instr_list, instr);

	// Mark s6 as used (should be permanent)
	mc_used_regs[$s6] = true;

	// Increment the heap pointer by the appropriate amount
	// Look up the class matching program_name
	struct scope_t *mainScope = symtab_lookup_class(program_name);
	int size = symtab_calc_scope_size(mainScope);
	instr = mc_new_instr("addi");
	instr->lhs_reg = $s7;
	instr->op1_reg = $s7;
	instr->imm = size;
	sprintf(instr->comment, "Move HP");
	mc_emit_instr(instr_list, instr);

	// Call the actual entry function
	struct scope_t *classScope = symtab_lookup_class(program_name);
	struct scope_t *funcScope = symtab_lookup_function(classScope, program_name);
	mc_call_func(instr_list, funcScope, NULL, NULL); // Need to actually determine entry block
	
	// Syscall to terminate the program properly
	instr = mc_new_instr("addi");
	instr->lhs_reg = $v0;
	instr->op1_reg = $0;
	instr->imm = 10;
	mc_emit_instr(instr_list, instr);
	instr = mc_new_instr("syscall");
	mc_emit_instr(instr_list, instr);
	
	// Add the instruction listing to HEAD of the text section instr lists
	struct instr_list_t *old_head = mc_text_section->instrs;
	mc_text_section->instrs = instr_list;
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
	struct directive_t *dir = mc_data_section->dirs;
	while(dir != NULL) {
		// label:
		if(dir->label != NULL)
			sprintf(buf, "%s%s: ", buf, dir->label);
		
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
	dir = mc_text_section->dirs;
	while(dir != NULL) {
		// label:
		if(dir->label != NULL)
			sprintf(buf, "%s%s: ", buf, dir->label);
		
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
	
	struct instr_list_t *instr_list = mc_text_section->instrs;
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
				if(strcmp(instr->mips_op->name, "la") == 0) {
					sprintf(buf, "%s%s, %s", buf, reg_names[instr->lhs_reg], instr->addr_label); // R[lhs] = addr_label
				} else if(strcmp(instr->mips_op->name, "li") == 0) {
					sprintf(buf, "%s%s, %i", buf, reg_names[instr->lhs_reg], instr->imm); // R[lhs] = imm
				} else if(strcmp(instr->mips_op->name, "move") == 0) {
					sprintf(buf, "%s%s, %s", buf, reg_names[instr->lhs_reg], reg_names[instr->op1_reg]); // R[lhs], R[rs]
				} else if(strcmp(instr->mips_op->name, "syscall") == 0) {
				} else if(instr->addr_label != NULL) { // blt, bgt, ble, bge
					sprintf(buf, "%s%s, %s, %s", buf, reg_names[instr->op1_reg], reg_names[instr->op2_reg], instr->addr_label); // R[op1] op R[op2] addr
				} else {
					sprintf(buf, "%s%s, %s, %s", buf, reg_names[instr->lhs_reg], reg_names[instr->op1_reg], reg_names[instr->op2_reg]); // R[lhs], R[rs], R[rt]
				}
				break;
			default:
				break;
			}
			
			// Optional Comment
			if(instr->comment != NULL && strlen(instr->comment) > 0)
				sprintf(buf, "%s  \t# %s", buf, instr->comment);
			
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
	printf("\n\n");
	if(listing != NULL) {
		printf("# jkcompiler assembly output\n\n%s\n", listing);
		free(listing);
	} else {
		printf("No listing to print\n");
	}
}

// Generate and Write the current asm listing to a file
void mc_write_listing(const char *filename) {
	char *listing = mc_gen_listing();
	if(listing == NULL) {
		printf("No listing to write\n");
		return;
	}

	FILE *fp = fopen(filename, "w");
	if(fp == NULL) {
		printf("Could not open %s to write assembly listing\n", filename);
		return;
	}
	
	fprintf(fp, "# jkcompiler assembly output\n# File: %s\n\n", filename);
	fprintf(fp, "%s", listing);
	
	printf("Wrote assembly listing to %s\n", filename);
	
	fclose(fp);
	free(listing);
}


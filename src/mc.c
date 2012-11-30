#include "mc.h"

void mc_init() {
	dataSection = mc_new_section();
	mc_emit_directive(dataSection, mc_new_directive(NULL, ".data"));
	
	textSection = mc_new_section();
	mc_emit_directive(textSection, mc_new_directive(NULL, ".text"));
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
	return NULL;
}

// Generate and Print current listing to screen
void mc_print_listing() {
	char *listing = mc_gen_listing();
	if(listing != NULL)
		printf("%s\n", listing);
	else
		printf("No listing to print\n");
}

// Generate and Write the current asm listing to a file
void mc_write_listing(const char *filename) {

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
	instr->lhs_reg = 0;
	instr->op1_reg = 0;
	instr->op1_reg_offset = 0;
	instr->op2_reg = 0;
	instr->op2_imm = 0;
	instr->jump_label = NULL;
	instr->next = NULL;
	
	return instr;
}

// Free's an instr from memory
void mc_free_instr(struct instr_t *instr) {
	if(instr == NULL)
		return;
	
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

// Process a CFG list (functions) and allocate stack, registers and emit directives, instructions
void mc_consume_cfg_list(struct cfg_list_t *cfgList) {
	struct cfg_list_t *cfg_it = cfgList;
	struct instr_list_t *cfg_instr_list = mc_new_instr_list(cfg_it->entryBlock->label);
	while(cfg_it != NULL) {
		if(cfg_it->scope->attrId != SYM_ATTR_FUNC)
			MCLOG(("Scope is NOT a func as expected..\n"));
	
		// Allocate variables on the stack and backup register values
		mc_alloc_stack(cfg_instr_list, cfg_it->scope);
		
		// Process the block and is children recursively
		mc_process_block(cfg_instr_list, cfg_it->entryBlock);
		
		// Deallocate stack and restore register values
		mc_dealloc_stack(cfg_instr_list, cfg_it->scope);
		
		// Add the cfg instruction listing to the data section instr lists
		mc_append_instr_list(&dataSection->instrs, cfg_instr_list);
	}
}

// Recursively process a block and its children by converting its TAC to Instructions
void mc_process_block(struct instr_list_t *instr_list, struct block_t *block) {
	// Loop through TAC nodes
	struct three_address_t *tac = block->entry;
	struct instr_t *instr = NULL;
	while(tac != NULL) {
		// Convert TAC to instruction
		instr = mc_tac_to_instr(tac);

		// Add instruction to the end of the instruction list
		mc_emit_instr(instr_list, instr);
	
		tac = tac->next;
	}
	
	// Recursively process this blocks children
	struct block_list_t *child = block->children;
	while(child !=  NULL) {
		// Create an instruction listing for this block
		struct instr_list_t *block_instr_list = mc_new_instr_list(child->block->label);
		
		mc_process_block(block_instr_list, child->block);
		
		// Add the block instruction listing to the previous instr list
		mc_append_instr_list(&instr_list, block_instr_list);
		
		child = child->next;
	}
}

// Convert three address code to corresponding instructions with registers/memory allocated in mc_alloc_mem
struct instr_t *mc_tac_to_instr(struct three_address_t *tac) {
	struct instr_t *instr = NULL;
	switch(tac->op) {
		case OP_PLUS:
			if(tac->op2->type == TAC_DATA_TYPE_INT) {
				instr = mc_new_instr("addi");
			} else {
				instr = mc_new_instr("add");
			}
			break;
		case OP_MINUS:
			instr = mc_new_instr("sub");
			break;
		case OP_OR:
			instr = mc_new_instr("or");
			break;
		case OP_STAR:
			instr = mc_new_instr("mult");
			break;
		case OP_SLASH:
			instr = mc_new_instr("div");
			break;
		case OP_MOD:
			//instr = mc_new_instr("");
			break;
		case OP_AND:
			instr = mc_new_instr("and");
			break;
		case OP_EQUAL:
			//instr = mc_new_instr("slt");
			break;
		case OP_NOTEQUAL:
			//instr = mc_new_instr("slt");
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
		case OP_ASSIGN:
			instr = mc_new_instr("move");
			break;
		case OP_BRANCH:
			instr = mc_new_instr("beq");
			break;
		case OP_GOTO:
			instr = mc_new_instr("j");
			break;
		case OP_PARAM_ASSIGN:
			//instr = mc_new_instr("");
			break;
		case OP_FUNC_CALL:
			//instr = mc_new_instr("");
			break;
		case OP_MEM_ACCESS:
			//instr = mc_new_instr("");
			break;
		default:
			MCLOG(("Could not convert tac to instr: %s\n", cfg_tac_data_to_str(tac->lhs)));
			break;
	}
	
	if(instr == NULL)
		return NULL;
	
	// Setup memory access for the instruction
	mc_mem_load(tac, instr);
	
	return instr;
}

// Locates and emits load instrs for all operands in the instruction
// Sets correct registers for the instruction to access
void mc_mem_load(struct three_address_t *tac, struct instr_t *instr) {

}

// Backup preserved registers and allocate stack memory for each variable and emits the appropriate instructions
void mc_alloc_stack(struct instr_list_t *cfg_instr_list, struct scope_t *scope) {
	int sp_size = symtab_calc_scope_size(scope)+8; // s0-s7, fp, ra
	int sp_pointer = sp_size;
	struct instr_t *instr = NULL;
	
	// Allocate space on the stack addi $sp, $sp, -sp_size
	instr = mc_new_instr("addi");
	instr->lhs_reg = $sp;
	instr->op1_reg = $sp;
	instr->op2_imm = 0-sp_size;
	mc_emit_instr(cfg_instr_list, instr);
	
	// Save arguments on stack
	
	// Set Frame Pointer
	instr = mc_new_instr("addi");
	instr->lhs_reg = $fp;
	instr->op1_reg = $sp;
	instr->op2_imm = sp_size;
	mc_emit_instr(cfg_instr_list, instr);
	
	// Backup registers $s0 - $s7
	for(int i = $s0; i <= $s7; i++) {
		instr = mc_new_instr("sw");
		instr->lhs_reg = i;
		instr->op1_reg = $sp;
		instr->op1_reg_offset = sp_pointer;
		sp_pointer -= 4;
		mc_emit_instr(cfg_instr_list, instr);
	}
	
	// Backup $fp
	instr = mc_new_instr("sw");
	instr->lhs_reg = $fp;
	instr->op1_reg = $sp;
	instr->op1_reg_offset = sp_pointer;
	sp_pointer -= 4;
	mc_emit_instr(cfg_instr_list, instr);
	
	// Backup $ra
	instr = mc_new_instr("sw");
	instr->lhs_reg = $ra;
	instr->op1_reg = $sp;
	instr->op1_reg_offset = sp_pointer;
	sp_pointer -= 4;
	mc_emit_instr(cfg_instr_list, instr);
	
	// Store all local variables on the stack
	struct offset_list_t *it = scope->offset_list;
	while(it != NULL) {
		instr = mc_new_instr("sw");
		instr->lhs_reg = $ra;
		instr->op1_reg = $sp;
		instr->op1_reg_offset = sp_size - it->offset;
		mc_emit_instr(cfg_instr_list, instr);
	}
}

// Restore preserved registers and deallocate stack memory
void mc_dealloc_stack(struct instr_list_t *cfg_instr_list, struct scope_t *scope) {

}

// Generates .globl main_func_name, and extra bootstrap code to jump into the main func of the main class
void mc_add_bootstrap(const char *main_func_name) {

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

/* symtab.c
 *
 * Implements the symbol table
 *
 * A flat symbol table is the root of all evil.
 */

#include "symtab.h"



/* ------------------------------------------------------------
 * Initializes the symbol table
 * ------------------------------------------------------------
 */
void symtab_init(struct program_t* program) {
    // Initialize root scope
    rootScope = (struct scope_t *) malloc(sizeof(struct scope_t));
    
    rootScope->attrId = SYM_ATTR_PROGRAM;
    rootScope->ptr = program;
    rootScope->next = NULL;
    rootScope->inner = NULL;
    rootScope->outer = NULL;
    currScope = rootScope;
	enterNext = true;
}



/* ------------------------------------------------------------
 * Prints the contents of the symbol table
 * ------------------------------------------------------------
 */
void symtab_print(int numOfTabs) {
	symtab_print_recursive(rootScope);
}

/**
 * Print's adjacent nodes to the passed in node value
 * Decends into inner nodes recursively
 */
void symtab_print_recursive(struct scope_t* start) {
	if(start == NULL)
		return;

	printf("<Enter Scope>\n");

	struct scope_t* node = start;

	while(node != NULL) {
		// Find node information based on the attribute type
		char *name = NULL;
		char *attrName = NULL;
		if(node->attrId == SYM_ATTR_VAR) {
			struct variable_declaration_list_t* vdl = (struct variable_declaration_list_t*)node->ptr;
			name = vdl->vd->il->id;
			if(name == NULL)
				printf("id is null!\n");
			attrName = (char*)malloc(4);
			strcpy(attrName, "VAR");
		} else if(node->attrId == SYM_ATTR_FUNC) {
			struct func_declaration_list_t* fdl = (struct func_declaration_list_t*)node->ptr;
			name = fdl->fd->fh->id;
			attrName = (char*)malloc(5);
			strcpy(attrName, "FUNC");
		} else if(node->attrId == SYM_ATTR_CLASS) {
			struct class_list_t* cl = (struct class_list_t*)node->ptr;
			name = cl->ci->id;
			attrName = (char*)malloc(6);
			strcpy(attrName, "CLASS");
		} else if(node->attrId == SYM_ATTR_PROGRAM) {
			struct program_t* p = (struct program_t*)node->ptr;
			name = p->ph->id;
			attrName = (char*)malloc(8);
			strcpy(attrName, "PROGRAM");
		} else {
			printf("??\n");
		}

		printf("%p: %s(%s), ", node->ptr, name, attrName);
		free(attrName);

		// Move to the adjacent node
		node = node->next;
	}
	printf("\n\n");

	// Decend into inner nodes if they exist in the order that they appear in the horizontal list
	node = start;
	while(node != NULL) {
		symtab_print_recursive(node->inner);

		// Move to the adjacent node
		node = node->next;
	}
	printf("<End Scope>\n\n");
}

/**
 * Set's the currentScope variable to a new scope. All insert operations will be done relative to the new scope
 */
void symtab_set_current_scope(struct scope_t *newCurrentScope) {
	currScope = newCurrentScope;
}

/**
 * Indicate the next symtab_insert call should insert a child node into the symbol table instead of an adjacent node
 */
void symtab_enter_scope() {
	enterNext = true;
}

/**
 * Move the currentScope pointer one level higher and set a pointer to the expected data structure in the node
 * 
 * @param pointer Pointer to the expected data structure to set in the currentScope node
 */
void symtab_exit_scope(void *pointer) {
	currScope->ptr = pointer;
	currScope = currScope->outer;
}

/**
 * Insert an adjacent or child node into the symbol table
 * A child node will be inserted if symtab_enter_scope preceded a call to this function
 *
 * @param attr Attribute ID correponding to values in the sym_attr_t enum
 * @param pointer Pointer to the expected data structure associated with the node.
 * 	If unknown at insert time (as is the case in classes and functions), set pointer to NULL
 */
void symtab_insert(int attr, void *pointer) {
	struct scope_t *newScope = (struct scope_t *) malloc(sizeof(struct scope_t));
	CHECK_MEM_ERROR(newScope);
		
	newScope->attrId = attr;
	newScope->ptr = pointer;
	newScope->inner = NULL;
	
	// If the scope is supposed to be inside the current scope, set current scope to the last inner node, and update the newScope parent
	if(enterNext) {
		newScope->outer = currScope;
		currScope->inner = newScope;
	
		newScope->next = NULL;
		// The current scope shifts inside previous current scope
		currScope = newScope;
		
		enterNext = false;
	} else { 
		// Scope is adjacent
		newScope->outer = currScope->outer;
		newScope->inner = NULL;
		
		// Go to the last adjacent node in the current scope
		struct scope_t *lastAdj = currScope;
		GOTO_END_OF_LIST(lastAdj);
	
		// Add the new scope to the end of the list
		lastAdj->next = newScope;
	}
	
}

/*
 * Insert an existing scope node at the currentNode
 *
 * @param nScope The scope to insert at the currentNode
 */
void symtab_insert_scope(struct scope_t *nScope) {	
	// If the scope is supposed to be inside the current scope, set the nScope parent, and set current scope to the last inner node
	if(enterNext) {
		nScope->outer = currScope;
		struct scope_t* inStart = currScope->inner;
		if(inStart == NULL) {
			inStart = nScope;
			currScope->inner = inStart;
		} else {
			GOTO_END_OF_LIST(inStart);
			inStart->next = nScope;
		}
	
		// The current scope shifts inside previous current scope
		currScope = nScope;
		
		enterNext = false;
	} else {
		nScope->outer = currScope->outer;
		// Go to the last adjacent node in the current scope
		struct scope_t *lastAdj = currScope;
		GOTO_END_OF_LIST(lastAdj);
	
		// Add the new scope to the end of the list
		lastAdj->next = nScope;
	}
}

// looks from the root
struct scope_t *symtab_lookup_class(char *name) {
	// Loop through all CLASS scope nodes in the inner scope of the root
	struct scope_t *it = rootScope->inner;
	while(it != NULL) {
		if(it->attrId == SYM_ATTR_CLASS) {
			struct class_list_t *cl = (struct class_list_t*)it->ptr;
		
			// Found class, names match
			if(strcmp(cl->ci->id, name) == 0) {
				return it;
			}
		}
		
		// Move horizontally along root scope
		it = it->next;
	}
	
	// Class name not found
	return NULL;
}

//looks in a class
struct scope_t *symtab_lookup_function(struct scope_t *classScope, char *name) {
	// Loop through all FUNC scope nodes adjacent to the specified classScope
	struct scope_t *it = classScope->inner;
	while(it != NULL) {
		if(it->attrId == SYM_ATTR_FUNC) {
			struct func_declaration_list_t *fdl = (struct func_declaration_list_t*)it->ptr;
		
			// Found function, names match
			if(strcmp(fdl->fd->fh->id, name) == 0) {
				return it;
			}
		}
		
		// Move horizontally along classScope
		it = it->next;
	}
	
	// Check for the function in a parent class until the root is reached
	it = classScope->outer;
	while (it != rootScope) {
		return symtab_lookup_function(it, name);
	}
	
	// Function name not found
	return NULL;
}

// looks in a class or function, allowed to move to outer scopes to search
struct scope_t *symtab_lookup_variable(struct scope_t *scope, char *name) {
	// Reached root's outer scope, still not found
	if(scope == NULL)
		return NULL;

	// Loop through all variables nodes adjacent to the specified scope
	struct scope_t *it = scope->inner;
	while(it != NULL) {
		if(it->attrId == SYM_ATTR_VAR) {
			struct variable_declaration_list_t *vdl = (struct variable_declaration_list_t*)it->ptr;
			
			// Found variable, names match
			if(strcmp(vdl->vd->il->id, name) == 0) {
				return it;
			}
		}

		// Move horizontally along scope
		it = it->next;
	}
	
	// Variable not found, move to outer scope
	return symtab_lookup_variable(scope->outer, name);
}



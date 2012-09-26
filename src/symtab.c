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
void symtab_init() {
    // Initialize root scope
    root = (struct scope_t *) malloc(sizeof(struct scope_t));
    
    root->attrId = symAttr.PROGRAM;
    root->ptr = program;
    root->next = NULL;
    root->inner = NULL;
    root->outer = NULL;
    currScope = root;
}



/* ------------------------------------------------------------
 * Prints the contents of the symbol table
 * ------------------------------------------------------------
 */
void symtab_print(int numOfTabs) {


}

void symtab_enter_scope() {
	enterNext = true;
}

void symtab_exit_scope() {
	currScope = currScope->outer;
}

void symtab_insert(int attr, void *pointer) {
	struct scope_t *newScope = (struct scope_t *) malloc(sizeof(struct scope_t));
	CHECK_MEM_ERROR(newScope);
	
	// Go to the last adjacent node in the current scope
	struct scope_t *lastAdj = currScope;
	END_OF_LIST(lastAdj);
	
	// Add the new scope to the end of the list
	lastAdj->next = newScope;
		
	newScope->attrId = attr;
	newScope->ptr = pointer;
	newScope->outer = currScope->outer;
	newScope->inner = NULL;
	
	// If the previous insert opened a new scope, set current scope's inner, and update current scope
	if(enterNext) {
		newScope->outer = currScope;
		currScope->inner = newScope;
	
		// The current scope shifts inside previous current scope
		currScope = newScope;
		
		enterNext = false;
	}
	
}

// looks from the root
struct scope_t *symtab_lookup_class(char *name) {
	// Loop through all CLASS scope nodes adjacent to the root
	struct scope_t *it = rootScope->next;
	while(it != NULL) {
		if(it->attrId == symAttr.CLASS) {
			class_list_t *cl = (struct class_list_t*)it->ptr;
		
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
	struct scope_t *it = classScope->next;
	while(it != NULL) {
		if(it->attrId == symAttr.FUNC) {
			function_declaraction_list_t *fdl = (struct function_declaraction_list_t*)it->ptr;
		
			// Found function, names match
			if(strcmp(fdl->fd->fh->id, name) == 0) {
				return it;
			}
		}
		
		// Move horizontally along classScope
		it = it->next;
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
	struct scope_t *it = scope;
	while(it != NULL) {
		if(it->attrId == symAttr.VAR) {
			variable_declaration_list_t *vdl = (struct variable_declaration_list_t*)it->ptr;
			
			// Found variable, names match
			if(strcmp(vdl->vd->il->id, name) == 0) {
				return it;
			}
			
			// Move horizontally along scope
			it = it->next;
		}
	}
	
	// Variable not found, move to outer scope
	return symtab_lookup_variable(scope->outer, name);
}



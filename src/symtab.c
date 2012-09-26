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


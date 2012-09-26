/* symtab.h
 *
 * Holds function definitions for the symbol table. The symbol table
 * is implemented as a global hash table that contains local symbol
 * tables for each function
 */

#ifndef SYMTAB_H
#define SYMTAB_H

#include "shared.h"
#include "usrdef.h"
#include <stdlib.h>
#include <string.h>

/* ----------------------------------------------------------------
 * Table structures
 * ----------------------------------------------------------------
 */

// Attributes that designate the object scope_t->ptr points to
enum symAttr_t {
	VAR, //variable_declaration_list_t (need to get to type_denoter for type checking)
	CLASS, // class_list_t
	FUNC, //func_declaration_list_t
	IF, //if_statement_t
	WHILE, //while_statement_t
	PROGRAM
} symAttr;

struct scope_t {
	int attrId;
	void *ptr; // class_list or type_den depending on attrId
	struct scope_t *inner; // Points to the head of the inner scope
	struct scope_t *outer; // Points to the head of the outer scope
	struct scope_t *next; // Points to the head of the adjacent scope
};

/* ----------------------------------------------------------------
 * Table state variables
 * ----------------------------------------------------------------
 */

bool enterNext;
struct scope_t *rootScope;
struct scope_t *currScope;


/* ----------------------------------------------------------------
 * Table function declarations
 * ----------------------------------------------------------------
 */

void symtab_init();
void symtab_print(int numOfTabs);
void symtab_enter_scope();
void symtab_exit_scope();
void symtab_insert(int attrId, void *ptr);
struct scope_t *symtab_lookup_class(char *name); // looks from the root
struct scope_t *symtab_lookup_function(struct scope_t *classScope, char *name); //looks in a class
struct scope_t *symtab_lookup_variable(struct scope_t *scope, char *name); // looks in a class or function, allowed to move to outer scopes to search




#endif

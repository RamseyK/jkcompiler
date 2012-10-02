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

#define SYM_ATTR_VAR 1 //variable_declaration_list_t (need to get to type_denoter for type checking)
#define SYM_ATTR_CLASS 2 // class_list_t
#define SYM_ATTR_FUNC 3 //func_declaration_list_t
#define SYM_ATTR_PROGRAM 4

/*struct scope_t {
	int attrId;
	void *ptr; // class_list or type_den depending on attrId
	struct scope_t *inner; // Points to the head of the inner scope
	struct scope_t *outer; // Points to the head of the outer scope
	struct scope_t *next; // Points to the head of the adjacent scope
};*/

struct scope_t {
	int attrId;
	struct scope_t *parent;
	struct program_t *program;
	struct scope_t *class_scopes;
	struct class_list_t *cl;
	struct scope_t *func_scopes;
	struct function_declaration_t *fd;
	struct scope_t *next;
	struct scope_t *nextSibling;
};

/* ----------------------------------------------------------------
 * Table state variables
 * ----------------------------------------------------------------
 */

bool enterNext;
struct scope_t *rootScope;
struct scope_t *currScope;
struct scope_t *tailScope;


/* ----------------------------------------------------------------
 * Table function declarations
 * ----------------------------------------------------------------
 */

void symtab_print_list_addr();
void symtab_init(struct program_t* program);
void symtab_print(int numOfTabs);
void symtab_print_recursive(struct scope_t* start, int numTabs);
struct scope_t *symtab_lookup_class(char *name); // looks from the root
struct scope_t *symtab_lookup_function(struct scope_t *classScope, char *name); //looks in a class
struct scope_t *symtab_lookup_variable(struct scope_t *scope, char *name); // looks in a class or function, allowed to move to outer scopes to search

struct scope_t *new_scope();
void symtab_create_function_scope(struct function_declaration_t *fd);
void symtab_create_class_scope(struct class_list_t *cl);
struct scope_t *symtab_lookup_function_scope(struct function_declaration_t *fd);

#endif

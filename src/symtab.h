/*
 * symtab.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
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

// Type of the scope node which determines which struct members are set
#define SYM_ATTR_VAR 1 // is this used..?
#define SYM_ATTR_CLASS 2
#define SYM_ATTR_FUNC 3
#define SYM_ATTR_PROGRAM 4

struct scope_t {
	int attrId; // Type of the scope node which determines which struct members are set
	struct scope_t *parent; // Parent scope
	struct program_t *program; // Program in this scope
	struct scope_t *class_scopes; // Head node of class scopes in this scope
	struct class_list_t *cl; // Class in this scope
	struct scope_t *func_scopes; // Head node of function scopes in this scope
	struct function_declaration_t *fd; // Function declaration in this scope
	struct offset_list_t *offset_list; // Offset list of variables for the current scope
	struct scope_t *next; // Next scope in the horizontal master list
	struct scope_t *nextSibling; // Next node within the same scope (class_scopes, func_scopes)
};

// List of variable identifiers matched with their offsets
struct offset_list_t {
	char *id;
	int offset;
	struct offset_list_t *next;
};

/* ----------------------------------------------------------------
 * Table state variables
 * ----------------------------------------------------------------
 */

bool enterNext;
struct scope_t *rootScope;
struct scope_t *allScopes;


/* ----------------------------------------------------------------
 * Table function declarations
 * ----------------------------------------------------------------
 */

void symtab_print_list_addr();
void symtab_init(struct program_t* program);
void symtab_destroy();
void symtab_print(int numOfTabs);
void symtab_print_recursive(struct scope_t* start, int numTabs);
void symtab_print_var_offsets(struct offset_list_t *offsets, int numTabs);
struct scope_t *symtab_lookup_class(char *name); // looks from the root
struct scope_t *symtab_lookup_function(struct scope_t *classScope, char *name); //looks in a class
struct variable_declaration_t *symtab_lookup_variable(struct scope_t *scope, char *name); // looks in a class or function, allowed to move to outer scopes to search
struct scope_t *symtab_lookup_variable_scope(struct scope_t *scope, char *name);
struct formal_parameter_section_t *symtab_lookup_function_param(struct scope_t *funcScope, char *name);

struct scope_t *new_scope();
struct scope_t *symtab_create_function_scope(struct function_declaration_t *fd);
struct scope_t *symtab_create_class_scope(struct class_list_t *cl);
struct scope_t *symtab_lookup_function_scope(struct function_declaration_t *fd);


//Functions to help with code gen
void symtab_calc_sizes(); // Calculates the size of type denoters corresponding to classes in the symtab
int symtab_calc_scope_size(struct scope_t *scope); // Calculates the size of a single scope entry recursively to include inheritance
int symtab_calc_td_size(struct type_denoter_t *td); // Returns the size of a type denoter or calculates it if it is unknown
void symtab_calc_offsets(); // Calculates offsets for vars in alls copes
int symtab_calc_scope_offsets(struct scope_t *scope); // Calculates the offsets of each variable declared in a scope
struct offset_list_t *add_to_offset_list(struct offset_list_t **offsetList, char *id, int offset); // Adds to an offset list
struct offset_list_t *new_offset_list();
#endif

/*
 * semantic.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Functions that participate in semantic analysis (error checking after parsing)
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "shared.h"
#include "semantic.h"
#include "rulefuncs.h"
#include "usrdef.h"
#include "set.h"
#include "symtab.h"

struct semantic_state_t {
	struct scope_t *scope; // Scope being analyzed
	int line_number; // Line number of statement
	bool allowThis; // Toggle to allow "this." as an identifier
	bool left; // Indicates evaluation of left hand side
};

void semantic_analysis(struct program_t *p);

bool compatible_types(struct type_denoter_t *t1, struct type_denoter_t *t2);
bool compatible_arrays(struct array_type_t *a1, struct array_type_t *a2);
bool compatible_classes(struct class_list_t *c1, struct class_list_t *c2);
bool compatible_class_assignment(struct class_list_t *lhs, struct class_list_t *rhs);

void verify_statements_in_sequence(struct semantic_state_t *sem_state, struct statement_t *s);
char *verify_variable_access(struct semantic_state_t *sem_state, struct variable_access_t *va);
char *verify_simple_expression(struct semantic_state_t *sem_state, struct simple_expression_t *se);
char *verify_expression(struct semantic_state_t *sem_state, struct expression_t *e);
char *verify_term(struct semantic_state_t *sem_state, struct term_t *t);
char *verify_primary(struct semantic_state_t *sem_state, struct primary_t *p);

void process_variable_declaration_list(struct scope_t *scope, struct variable_declaration_list_t *vdl);
void check_variable_declared_in_parent(struct scope_t *classScope);
bool check_variable_valid_name(char *id);

#endif /* SEMANTIC_H */

/*
 * semantic.h
 *
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "symtab.h"

void semantic_analysis(struct program_t *p);

bool compatible_types(struct type_denoter_t *t1, struct type_denoter_t *t2);
bool compatible_arrays(struct array_type_t *a1, struct array_type_t *a2);
bool compatible_classes(struct class_list_t *c1, struct class_list_t *c2);
bool compatible_class_assignment(struct class_list_t *lhs, struct class_list_t *rhs);

void verify_statements_in_sequence(struct scope_t *scope, struct statement_t *s);
void verify_variable_access(struct scope_t *scope, struct variable_access_t *va, int line_number);
void verify_simple_expression(struct scope_t *scope, struct simple_expression_t *se, int line_number);
void verify_expression(struct scope_t *scope, struct expression_t *e, int line_number);

void process_variable_declaration_list(struct scope_t *scope, struct variable_declaration_list_t *vdl);
void check_variable_declared_in_parent(struct scope_t *classScope);

int identifier_list_size(struct identifier_list_t *il);
bool check_variable_valid_name(char *id);

#endif /* SEMANTIC_H */

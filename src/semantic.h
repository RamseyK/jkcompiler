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

void verify_statements_in_sequence(struct statement_sequence_t *ss);
void verify_identifiers_in_variable_access(struct variable_access_t *va);

void check_variable_list_types_defined(struct variable_declaration_list_t *vdl);
void fix_type_denoters();

int identifier_list_size(struct identifier_list_t *il);


#endif /* SEMANTIC_H */

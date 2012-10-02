/*
 * semantic.h
 *
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H


void semantic_analysis(struct program_t *p);

bool compatible_types(struct type_denoter_t *t1, struct type_denoter_t *t2);
bool compatible_arrays(struct array_type_t *a1, struct array_type_t *a2);
bool compatible_classes(struct class_list_t *c1, struct class_list_t *c2);
bool compatible_class_assignment(struct class_list_t *lhs, struct class_list_t *rhs);

void check_variable_list_types_defined(struct variable_declaration_list_t *vdl);
//void check_types_defined();

int identifier_list_size(struct identifier_list_t *il);


#endif /* SEMANTIC_H */

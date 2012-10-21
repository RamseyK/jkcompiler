/*
 * rulefuncs.h
 * jkcompiler
 * Keilan Jackson, Ramsey Kant
 *
 * Functions that assist in developing the yacc rules
 */

#ifndef RULEFUNCS_H
#define RULEFUNCS_H

#include "shared.h"
#include "usrdef.h"
#include "symtab.h"
#include "y.tab.h"
#include "error.h"


/* -----------------------------------------------------------------------
 * Program
 * -----------------------------------------------------------------------
 */
struct program_t *new_program();
void free_program(struct program_t* p);

struct program_heading_t *new_program_heading();
void free_program_heading(struct program_heading_t *ph);

/* -----------------------------------------------------------------------
 * Identifiers
 * -----------------------------------------------------------------------
 */
struct identifier_list_t *new_identifier_list();
void add_to_identifier_list(struct identifier_list_t **il, char *id);
long identifier_list_count(struct identifier_list_t *il);
struct identifier_list_t *find_identifier_list(struct identifier_list_t *il, char *id);
void free_identifier_list(struct identifier_list_t *il);

char *new_identifier(char *text);
void free_identifier(char *id);

/* -----------------------------------------------------------------------
 * Classes
 * -----------------------------------------------------------------------
 */
struct class_list_t *new_class_list();
struct class_list_t *add_to_class_list(struct class_list_t **cl, struct class_identification_t *ci, struct class_block_t *b);
struct class_list_t *find_class_list(struct class_list_t *cl, char *id);
void free_class_list(struct class_list_t *cl);

struct class_identification_t *new_class_identification();
void free_class_identification(struct class_identification_t *ci);

struct class_block_t *new_class_block();
void free_class_block(struct class_block_t *cb);

/* -----------------------------------------------------------------------
 * Parameters
 * -----------------------------------------------------------------------
 */
struct actual_parameter_list_t *new_actual_parameter_list();
void add_to_actual_parameter_list(struct actual_parameter_list_t **apl, struct actual_parameter_t *ap);
void free_actual_parameter_list(struct actual_parameter_list_t *apl);

struct actual_parameter_t *new_actual_parameter();
void free_actual_parameter(struct actual_parameter_t *ap);

struct formal_parameter_section_list_t *new_formal_parameter_section_list();
void add_to_formal_parameter_section_list(struct formal_parameter_section_list_t **fpsl, struct formal_parameter_section_t *fps);
void free_formal_parameter_section_list(struct formal_parameter_section_list_t *fpsl);

struct formal_parameter_section_t *new_formal_parameter_section();
void free_formal_parameter_section(struct formal_parameter_section_t *fps);

/* -----------------------------------------------------------------------
 * Types
 * -----------------------------------------------------------------------
 */
struct type_denoter_t *new_type_denoter();
void free_type_denoter(struct type_denoter_t *td);

struct array_type_t *new_array_type();
void free_array_type(struct array_type_t *at);

/* -----------------------------------------------------------------------
 * Variables
 * -----------------------------------------------------------------------
 */
struct variable_declaration_list_t *new_variable_declaration_list();
void add_to_variable_declaration_list(struct variable_declaration_list_t **vdl, struct variable_declaration_t *vd);
void free_variable_declaration_list(struct variable_declaration_list_t *vdl);

struct variable_declaration_t *new_variable_declaration();
void free_variable_declaration(struct variable_declaration_t *vd);

/* -----------------------------------------------------------------------
 * Funcs
 * -----------------------------------------------------------------------
 */
struct func_declaration_list_t *new_func_declaration_list();
void add_to_func_declaration_list(struct func_declaration_list_t **fdl, struct function_declaration_t *fd);
struct func_declaration_list_t *find_func_list(struct func_declaration_list_t *fdl, char *headingId);
void free_func_declaration_list(struct func_declaration_list_t *fdl);

struct function_declaration_t *new_function_declaration();
void free_function_declaration(struct function_declaration_t *fd);

struct function_heading_t *new_function_heading();
void free_function_heading(struct function_heading_t *fh);

struct function_block_t *new_function_block();
void free_function_block(struct function_block_t *fb);

/* -----------------------------------------------------------------------
 * Statements
 * -----------------------------------------------------------------------
 */
struct statement_t *new_statement();
void free_statement(struct statement_t *s);

struct statement_sequence_t *new_statement_sequence();
void add_to_statement_sequence(struct statement_sequence_t **ss, struct statement_t *s);
void free_statement_sequence(struct statement_sequence_t *ss);

struct if_statement_t *new_if_statement();
void free_if_statement(struct if_statement_t *is);

struct while_statement_t *new_while_statement();
void free_while_statement(struct while_statement_t *ws);

struct assignment_statement_t *new_assignment_statement();
void free_assignement_statement(struct assignment_statement_t *as);

struct print_statement_t *new_print_statement();
void free_print_statement(struct print_statement_t *ps);


/* -----------------------------------------------------------------------
 * Expressions
 * -----------------------------------------------------------------------
 */
struct expression_t *new_expression();
void free_expression(struct expression_t* e);

struct expression_data_t *new_expression_data();
void free_expression_data(struct expression_data_t *ed);

struct index_expression_list_t *new_index_expression_list();
void add_to_index_expression_list(struct index_expression_list_t **iel, struct expression_t *ie);
long index_expression_list_count(struct index_expression_list_t *iel);
void free_index_expression_list(struct index_expression_list_t *iel);

struct variable_access_t *new_variable_access();
void free_variable_access(struct variable_access_t *va);

struct object_instantiation_t *new_object_instantiation();
void free_object_instantiation(struct object_instantiation_t *oe);

struct indexed_variable_t *new_indexed_variable();
void free_indexed_variable(struct indexed_variable_t *iv);

struct attribute_designator_t *new_attribute_designator();
void free_attribute_designator(struct attribute_designator_t *ad);

struct method_designator_t *new_method_designator();
void free_method_designator(struct method_designator_t *md);

struct function_designator_t *new_function_designator();
void free_function_designator(struct function_designator_t *fd);

struct simple_expression_t *new_simple_expression();
void add_to_simple_expression(struct simple_expression_t **se, int addop, struct term_t *t);
void free_simple_expression(struct simple_expression_t *se);

struct range_t *new_range();
void free_range(struct range_t *r);

struct term_t *new_term();
void add_to_term(struct term_t **t, int mulop, struct factor_t *f);
void free_term(struct term_t *t);

struct factor_t *new_factor();
void free_factor(struct factor_t *f);

int *new_sign();
void free_sign(int *s);

struct primary_t *new_primary();
void free_primary(struct primary_t *p);

struct unsigned_number_t *new_unsigned_number();
void free_unsigned_number(struct unsigned_number_t *un);



#endif


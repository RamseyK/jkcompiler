%{
/*
 * grammar.y
 *
 * Pascal grammar in Yacc format, based originally on BNF given
 * in "Standard Pascal -- User Reference Manual", by Doug Cooper.
 * This in turn is the BNF given by the ANSI and ISO Pascal standards,
 * and so, is PUBLIC DOMAIN. The grammar is for ISO Level 0 Pascal.
 * The grammar has been massaged somewhat to make it LALR.
 */

#include "shared.h"
#include "rulefuncs.h"
#include "symtab.h"

  int yylex(void);
  void yyerror(const char *error);

  extern char *yytext;          /* yacc text variable */
  extern int line_number;       /* Holds the current line number; specified
				   in the lexer */
  struct program_t *program;    /* points to our program */
%}

%token AND ARRAY ASSIGNMENT CLASS COLON COMMA DIGSEQ
%token DO DOT DOTDOT ELSE END EQUAL EXTENDS FUNCTION
%token GE GT IDENTIFIER IF LBRAC LE LPAREN LT MINUS MOD NEW NOT
%token NOTEQUAL OF OR PBEGIN PLUS PRINT PROGRAM RBRAC
%token RPAREN SEMICOLON SLASH STAR THEN
%token VAR WHILE

%type <tden> type_denoter
%type <id> result_type
%type <id> identifier
%type <idl> identifier_list
%type <fdes> function_designator
%type <apl> actual_parameter_list
%type <apl> params
%type <ap> actual_parameter
%type <vd> variable_declaration
%type <vdl> variable_declaration_list
%type <r> range
%type <un> unsigned_integer
%type <fpsl> formal_parameter_section_list
%type <fps> formal_parameter_section
%type <fps> value_parameter_specification
%type <fps> variable_parameter_specification
%type <va> variable_access
%type <as> assignment_statement
%type <os> object_instantiation
%type <ps> print_statement
%type <e> expression
%type <s> statement
%type <ss> compound_statement
%type <ss> statement_sequence
%type <ss> statement_part
%type <is> if_statement
%type <ws> while_statement
%type <e> boolean_expression
%type <iv> indexed_variable
%type <ad> attribute_designator
%type <md> method_designator
%type <iel> index_expression_list
%type <e> index_expression
%type <se> simple_expression
%type <t> term
%type <f> factor
%type <i> sign
%type <p> primary
%type <un> unsigned_constant
%type <un> unsigned_number
%type <at> array_type
%type <cb> class_block
%type <vdl> variable_declaration_part
%type <fdl> func_declaration_list
%type <funcd> function_declaration
%type <fb> function_block
%type <fh> function_heading
%type <id> function_identification
%type <fpsl> formal_parameter_list
%type <cl> class_list
%type <ci> class_identification
%type <program> program
%type <ph> program_heading
%type <op> relop
%type <op> addop
%type <op> mulop

%union {
  struct type_denoter_t *tden;
  char *id;
  struct identifier_list_t *idl;
  struct function_designator_t *fdes;
  struct actual_parameter_list_t *apl;
  struct actual_parameter_t *ap;
  struct variable_declaration_list_t *vdl;
  struct variable_declaration_t *vd;
  struct range_t *r;
  struct unsigned_number_t *un;
  struct formal_parameter_section_list_t *fpsl;
  struct formal_parameter_section_t *fps;
  struct variable_access_t *va;
  struct assignment_statement_t *as;
  struct object_instantiation_t *os;
  struct print_statement_t *ps;
  struct expression_t *e;
  struct statement_t *s;
  struct statement_sequence_t *ss;
  struct if_statement_t *is;
  struct while_statement_t *ws;
  struct indexed_variable_t *iv;
  struct attribute_designator_t *ad;
  struct method_designator_t *md;
  struct index_expression_list_t *iel;
  struct simple_expression_t *se;
  struct term_t *t;
  struct factor_t *f;
  int *i;
  struct primary_t *p;
  struct array_type_t *at;
  struct class_block_t *cb;
  struct func_declaration_list_t *fdl;
  struct function_declaration_t *funcd;
  struct function_block_t *fb;
  struct function_heading_t *fh;
  struct class_identification_t *ci;
  struct class_list_t *cl;
  struct program_t *program;
  struct program_heading_t *ph;
  int op;
}

%%

program : program_heading SEMICOLON class_list DOT
	{
		program->ph = $1;
		program->cl = $3;
	}
 ;

program_heading : PROGRAM identifier
	{
		$$ = new_program_heading();
		$$->id = $2;
		$$->il = NULL;
	}
 | PROGRAM identifier LPAREN identifier_list RPAREN
	{
		$$ = new_program_heading();
		$$->id = $2;
		$$->il = $4;
	}
 ;

identifier_list : identifier_list comma identifier
        {
        	// Check the identifier against the current identifier_list for duplicates
        	struct identifier_list_t *matched_il = get_identifier($1, $3);
        	if(matched_il != NULL) {
        		error_variable_already_declared(line_number, matched_il->id, line_number-1);
        	}
        	
			// Add the identifier node to the identifier_list
			add_to_identifier_list(&$1, $3);
        }
 | identifier
        {
        	// Check the identifier against the current identifier_list for duplicates
        
        	// Create and add first identifier node
        	$$ = new_identifier_list();

			// Add identifier to $$ (current identifier_list node):
			add_to_identifier_list(&$$, $1);
        }
 ;

class_list : class_list class_identification PBEGIN class_block END
	{
		// add_to_class_list(class_list($1), class_identification($2), class_block($4))
		// symtab_insert(symAttr(SA_CLASS), $1->next);
		// symtab_exit_scope();
	}
 | class_identification PBEGIN class_block END
	{
		// Create the tail of the class list, $$ = new_class_list()
		// add_to_class_list($$, class_identification($1), class_block($3))
		// symtab_insert(symAttr(SA_CLASS), $$);
		// symtab_exit_scope();
	}
 ;

class_identification : CLASS identifier
	{
		// Create class_identification
		// symtab_enter_scope();
		
	}
| CLASS identifier EXTENDS identifier
	{
		// Create class_identification (base case)
		// symtab_enter_scope();
	}
;

class_block:
 variable_declaration_part
 func_declaration_list
	{

	}
 ;

type_denoter : array_type
	{

	}
 | identifier
	{

	}
 ;

array_type : ARRAY LBRAC range RBRAC OF type_denoter
	{

	}
 ;

range : unsigned_integer DOTDOT unsigned_integer
	{

	}
 ;

variable_declaration_part : VAR variable_declaration_list semicolon
	{

	}
 |
	{

	}
 ;

variable_declaration_list : variable_declaration_list semicolon variable_declaration
	{
		//add_to_variable_list($1, $3);
		//symtab_insert(symAttr(SA_VAR), $1->next);
	}
 | variable_declaration
	{
		// $$ = new_variable_list()
		//symtab_insert(symAttr(SA_VAR), $$)
	}

 ;

variable_declaration : identifier_list COLON type_denoter
	{

	}
 ;

func_declaration_list : func_declaration_list semicolon function_declaration
	{
		
	}
 | function_declaration
	{

	}
 |
	{

	}
 ;

formal_parameter_list : LPAREN formal_parameter_section_list RPAREN 
	{

	}
;
formal_parameter_section_list : formal_parameter_section_list semicolon formal_parameter_section
	{

	}
 | formal_parameter_section
	{

	}
 ;

formal_parameter_section : value_parameter_specification
 | variable_parameter_specification
 ;

value_parameter_specification : identifier_list COLON identifier
	{

	}
 ;

variable_parameter_specification : VAR identifier_list COLON identifier
	{

	}
 ;

function_declaration : function_identification semicolon function_block
	{

	}
 | function_heading semicolon function_block
	{

	}
 ;

function_heading : FUNCTION identifier COLON result_type
	{

	}
 | FUNCTION identifier formal_parameter_list COLON result_type
	{

	}
 ;

result_type : identifier ;

function_identification : FUNCTION identifier
	{

	}
;

function_block : 
  variable_declaration_part
  statement_part
	{

	}
;

statement_part : compound_statement
 ;

compound_statement : PBEGIN statement_sequence END
	{

	}
 ;

statement_sequence : statement
	{

	}
 | statement_sequence semicolon statement
	{

	}
 ;

statement : assignment_statement
	{

	}
 | compound_statement
	{

	}
 | if_statement
	{

	}
 | while_statement
	{

	}
 | print_statement
        {

        }
 ;

while_statement : WHILE boolean_expression DO statement
	{

	}
 ;

if_statement : IF boolean_expression THEN statement ELSE statement
	{

	}
 ;

assignment_statement : variable_access ASSIGNMENT expression
	{

	}
 | variable_access ASSIGNMENT object_instantiation
	{

	}
 ;

object_instantiation: NEW identifier
	{

	}
 | NEW identifier params
	{

	}
;

print_statement : PRINT variable_access
        {

        }
;

variable_access : identifier
	{
		$$ = new_variable_access();
		$$->type = VARIABLE_ACCESS_T_IDENTIFIER;
		$$->data->id = $1;
	}
 | indexed_variable
	{
		$$ = new_variable_access();
		$$->type = VARIABLE_ACCESS_T_INDEXED_VARIABLE;
		$$->data->iv = $1;
	}
 | attribute_designator
	{
		$$ = new_variable_access();
		$$->type = VARIABLE_ACCESS_T_ATTRIBUTE_DESIGNATOR;
		$$->data->ad = $1;
	}
 | method_designator
	{
		$$ = new_variable_access();
		$$->type = VARIABLE_ACCESS_T_METHOD_DESIGNATOR;
		$$->data->md = $1;
	}
 ;

indexed_variable : variable_access LBRAC index_expression_list RBRAC
	{
		$$ = new_indexed_variable();
		$$->va = $1;
		$$->iel = $3;
	}
 ;

index_expression_list : index_expression_list comma index_expression
	{
		add_to_index_expression_list(&$1, $3);
	}
 | index_expression
	{
		$$ = new_index_espression_list();
		$$->e = $1;
	}
 ;

index_expression : expression ;

attribute_designator : variable_access DOT identifier
	{
		$$ = new_attribute_designator();
		$$->va = $1;
		$$->id = $3;
	}
;

method_designator: variable_access DOT function_designator
	{
		$$ = new_method_designator();
		$$->va = $1;
		$$->fd = $3;
	}
 ;


params : LPAREN actual_parameter_list RPAREN 
	{
		$$ = $2;
	}
 ;

actual_parameter_list : actual_parameter_list comma actual_parameter
	{
		add_to_actual_parameter_list(&$1, $3);
	}
 | actual_parameter 
	{
		$$ = new_actual_parameter_list();
		$$->ap = $1;
	}
 ;

actual_parameter : expression
	{
		$$ = new_actual_parameter();
		$$->e1 = $1;
	}
 | expression COLON expression
	{
		$$ = new_actual_parameter();
		$$->e1 = $1;
		$$->e2 = $3;
	}
 | expression COLON expression COLON expression
	{
		$$ = new_actual_parameter();
		$$->e1 = $1;
		$$->e2 = $3;
		$$->e3 = $5;
	}
 ;

boolean_expression : expression ;

expression : simple_expression
	{
		$$ = new_expression();
		$$->se1 = $1;
		$$->expr = $1->expr;
	}
 | simple_expression relop simple_expression
	{
		$$ = new_expression();
		$$->se1 = $1;
		$$->relop = $2;
		$$->se2 = $3;
		//Do some kind of type checking ??
		if($1->expr->type.strcmp(PRIMITIVE_TYPE_INTEGER) && $3->expr->type.strcmp(PRIMITIVE_TYPE_INTEGER)) {
			$$->expr = new_expression_data();
			$$->expr->type = PRIMITIVE_TYPE_BOOL;
			if(relop == EQUAL)
				$$->expr->val = ($1->expr->val == $3->expr->val)
			if(relop == NOTEQUAL)
				$$->expr->val = ($1->expr->val != $3->expr->val)
			if(relop == GT)
				$$->expr->val = ($1->expr->val > $3->expr->val)
			if(relop == LT)
				$$->expr->val = ($1->expr->val < $3->expr->val)
			if(relop == GE)
				$$->expr->val = ($1->expr->val >= $3->expr->val)
			if(relop == LE)
				$$->expr->val = ($1->expr->val <= $3->expr->val)
		} else if($1->expr->type.strcmp(PRIMITIVE_TYPE_BOOLEAN) && $3->expr->type.strcmp(PRIMITIVE_TYPE_BOOLEAN)) {
			if(relop == EQUAL)
				$$->expr->val = ($1->expr->val == $3->expr->val)
			if(relop == NOTEQUAL)
				$$->expr->val = ($1->expr->val != $3->expr->val)
		} else {
			//Type mismatch
			$$->expr = new_expression_data();
			$$->expr->type = PRIMITIVE_TYPE_NAME_UNKNOWN;
		}
	}
 ;

simple_expression : term
	{
		$$ = new_simple_expression();
		$$->t = $1;
		$$->expr = $1->expr;
	}
 | simple_expression addop term
	{
		add_to_simple_expression(&$1, $2, $3);
		//Do some kind of type checking ??
		if($1->expr->type.strcmp(PRIMITIVE_TYPE_INTEGER) && $3->expr->type.strcmp(PRIMITIVE_TYPE_INTEGER)) {
			$$->expr = new_expression_data();
			$$->expr->type = PRIMITIVE_TYPE_INTEGER;
			if(addop == PLUS) 
				$$->expr->val = $1->expr->val + $3->expr->val;
			else if(addop == MINUS)
				$$->expr->val = $1->expr->val - $3->expr->val;
			else
				// Invalid operation with integers
		} else if($1->expr->type.strcmp(PRIMITIVE_TYPE_BOOLEAN) && $3->expr->type.strcmp(PRIMITIVE_TYPE_BOOLEAN)) {
			$$->expr = new_expression_data();
			$$->expr->type = PRIMITIVE_TYPE_BOOLEAN
			if(addop == OR)
				$$->expr->val = $1->expr->val | $3->expr->val;
			else
				// Invalid operation with boolean
		} else {
			//Type mismatch
			$$->expr = new_expression_data();
			$$->expr->type = PRIMITIVE_TYPE_NAME_UNKNOWN;
		}
	}
 ;

term : factor
	{
		$$ = new_term();
		$$->f = $1;		
	}
 | term mulop factor
	{		
		add_to_term(&$1, $2, $3);	
	}
 ;

sign : PLUS
	{
		$$ = new_sign();
		*$$ = $1;
	}
 | MINUS
	{
		$$ = new_sign();
		*$$ = $1;
	}
 ;

factor : sign factor
	{
		$$ = new_factor();
		$$->type = FACTOR_T_SINGFACTOR;
		$$->data->f = (struct factor_data_t *) malloc(sizeof(struct factor_data_t));
		CHECK_MEM_ERROR($$->data->f);
		$$->data->f->sign = $1;
		$$->data->f->next = $2;
		$$->expr = $2->expr;
	}
 | primary 
	{
		$$ = new_factor();
		$$->type = FACTOR_T_PRIMARY;
		$$->data->p = $1;
		$$->expr = $1->expr;
	}
 ;

primary : variable_access
	{
		$$ = new_primary();
		$$->type = PRIMARY_T_VARIABLE_ACCESS;
		$$->data->va = $1;
		$$->expr = $1->expr;
	}
 | unsigned_constant
	{
		$$ = new_primary();
		$$->type = PRIMARY_T_UNSIGNED_CONSTANT;
		$$->data->un = $1;
		$$->expr = $1->expr;
	}
 | function_designator
	{
		$$ = new_primary();
		$$->type = PRIMARY_T_FUNCTION_DESIGNATOR;
		$$->data->fd = $1;
	}
 | LPAREN expression RPAREN
	{
		$$ = new_primary();
		$$->type = PRIMARY_T_EXPRESSION;
		$$->data->e = $2;
		$$->expr = $2->expr;
	}
 | NOT primary
	{
		$$ = new_primary();
		$$->type = PRIMARY_T_PRIMARY;
		$$->data->p = (struct primary_data_t *) malloc(sizeof(struct primary_data_t)));
		CHECK_MEM_ERROR($$->data->p);
		$$->data->p->not = TRUE;
		$$->data->p->next = $2;
		$$->expr = $1->expr;
	}
 ;

unsigned_constant : unsigned_number
 ;

unsigned_number : unsigned_integer ;

unsigned_integer : DIGSEQ
	{
		$$ = new_unsigned_integer;
		$$->ui = atoi(yytext);
		$$->expr = new_expression_data(); 
        $$->expr->type = new_primitive_type(PRIMITIVE_TYPE_NAME_INTEGER); 
        $$->expr->val = $$->ui;
	}
 ;

/* functions with no params will be handled by plain identifier */
function_designator : identifier params
	{
		$$ = new_function_designator();
		$$->id = $1;
		$$->apl = $2;
	}
 ;

addop: PLUS
	{
		$$ = $1;
	}
 | MINUS
	{
		$$ = $1;
	}
 | OR
	{
		$$ = $1;
	}
 ;

mulop : STAR
	{
		$$ = $1;
	}
 | SLASH
	{
		$$ = $1;
	}
 | MOD
	{
		$$ = $1;
	}
 | AND
	{
		$$ = $1;
	}
 ;

relop : EQUAL
	{
		$$ = $1;
	}
 | NOTEQUAL
	{
		$$ = $1;
	}
 | LT
	{
		$$ = $1;
	}
 | GT
	{
		$$ = $1;
	}
 | LE
	{
		$$ = $1;
	}
 | GE
	{
		$$ = $1;
	}
 ;

identifier : IDENTIFIER
	{
		$$ = new_identifier(yytext);
	}
 ;

semicolon : SEMICOLON
 ;

comma : COMMA
 ;

%%

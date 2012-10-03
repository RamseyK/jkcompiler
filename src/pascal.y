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
#include "usrdef.h"

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
			// Add the identifier node to the identifier_list
			add_to_identifier_list(&$1, $3);
        }
 | identifier
        {
        	// Create and add first identifier node
        	$$ = new_identifier_list();
        	$$->id = $1;
        	$$->next = NULL;
        }
 ;

class_list : class_list class_identification PBEGIN class_block END
	{
        // Check the class_identification against the current class_list for duplicates
        struct class_list_t *matched = find_class_list($1, $2->id);
        if(matched != NULL) {
        	error_class_already_declared($2->line_number, $2->id, matched->ci->line_number);
        }

		struct class_list_t *addedClass = add_to_class_list(&$1, $2, $4);
		
		// Create the class scope
		symtab_create_class_scope(addedClass);
		
		// Add to usrdef list
		struct type_denoter_t *new_type = new_type_denoter();
		new_type->type = TYPE_DENOTER_T_CLASS_TYPE;
		new_type->name = $2->id;
		new_type->data.cl = addedClass;
		struct type_denoter_t *found_type = usrdef_lookup_td(new_type);
		if(found_type == NULL) {
			// See if the type can be found by name (e.g. a variable declared it
			// as an identifier and now we know its a class
			found_type = usrdef_lookup_name(new_type->name);
			if(found_type == NULL) {
				usrdef_insert(new_type);
			} else { // Update it
				found_type->type = TYPE_DENOTER_T_CLASS_TYPE;
				found_type->data.cl = addedClass;
			}
		} else {
			error_type_already_defined($2->line_number, $2->id, found_type->data.cl->ci->line_number);
		}
	}
 | class_identification PBEGIN class_block END
	{
		// Create the tail of the class list, $$ = new_class_list()
		$$ = new_class_list();
		$$->ci = $1;
		$$->cb = $3;
		$$->next = NULL;
		
		// Create the class scope
		symtab_create_class_scope($$);
		
		// Add to usrdef list
		struct type_denoter_t *new_type = new_type_denoter();
		new_type->type = TYPE_DENOTER_T_CLASS_TYPE;
		new_type->name = $1->id;
		new_type->data.cl = $$;
		struct type_denoter_t *found_type = usrdef_lookup_td(new_type);
		if(found_type == NULL) {
			// See if the type can be found by name (e.g. a variable declared it
			// as an identifier and now we know its a class
			found_type = usrdef_lookup_name(new_type->name);
			if(found_type == NULL) {
				usrdef_insert(new_type);
			} else { // Update it
				found_type->type = TYPE_DENOTER_T_CLASS_TYPE;
				found_type->data.cl = $$;
			}
		} else {
			error_type_already_defined($1->line_number, $1->id, found_type->data.cl->ci->line_number);
		}
		//usrdef_print();
		
	}
 ;

class_identification : CLASS identifier
	{
		// Create class_identification without an extend id
		$$ = new_class_identification();
		$$->id = $2;
		$$->extend = NULL;
		$$->line_number = line_number;
	}
| CLASS identifier EXTENDS identifier
	{
		// Create class_identification with an extend id
		$$ = new_class_identification();
		$$->id = $2;
		$$->extend = $4;
		$$->line_number = line_number;
	}
;

class_block: variable_declaration_part func_declaration_list
	{
		$$ = new_class_block();
		$$->vdl = $1;
		$$->fdl = $2;
	}
 ;

type_denoter : array_type
	{
		$$ = new_type_denoter();
		$$->type = TYPE_DENOTER_T_ARRAY_TYPE;
		$$->name = $1->inner_type_name;
		$$->data.at = $1;
	}
 | identifier
	{
		$$ = new_type_denoter();
		$$->type = TYPE_DENOTER_T_IDENTIFIER;
		$$->name = $1;
		$$->data.id = $1;
		
		struct type_denoter_t *new_type = new_type_denoter();
		new_type->type = TYPE_DENOTER_T_IDENTIFIER;
		new_type->name = $1;
		new_type->data.id = $1;
		struct type_denoter_t *found_type = usrdef_lookup_name(new_type->name);
		if(found_type == NULL) {
			$$ = usrdef_insert(new_type);
		} else {
			$$ = found_type;
		}
	}
 ;

array_type : ARRAY LBRAC range RBRAC OF type_denoter
	{
		$$ = new_array_type();
		$$->r = $3;
		$$->td = $6;
		$$->inner_type_name = $6->name;
	}
 ;

range : unsigned_integer DOTDOT unsigned_integer
	{
		$$ = new_range();
		$$->min = $1;
		$$->max = $3;
		
		// If the min is greater than the max, error
		if($$->min->ui > $$->max->ui)
			error_array_range_invalid(line_number, $$->min->ui, $$->max->ui);
	}
 ;

variable_declaration_part : VAR variable_declaration_list semicolon
	{
		$$ = $2;
	}
 |
	{
		$$ = NULL;
	}
 ;

variable_declaration_list : variable_declaration_list semicolon variable_declaration
	{
		add_to_variable_declaration_list(&$1, $3);
	}
 | variable_declaration
	{
		// Create the head of the variable list
		$$ = new_variable_declaration_list();
		$$->vd = $1;
		$$->next = NULL;

	}

 ;

variable_declaration : identifier_list COLON type_denoter
	{
		$$ = new_variable_declaration();
		$$->il = $1;
		$$->tden = $3;
		$$->line_number = line_number;
	}
 ;

func_declaration_list : func_declaration_list semicolon function_declaration
	{
        // Check the func_declaration header against the current func_declaration_list for duplicates
        struct func_declaration_list_t *matched = find_func_list($1, $3->fh->id);
        if(matched != NULL) {
        	error_function_already_declared($3->line_number, $3->fh->id, matched->fd->line_number);
        }

		add_to_func_declaration_list(&$1, $3);
		
		symtab_create_function_scope($3);
	}
 | function_declaration
	{
		$$ = new_func_declaration_list();
		$$->fd = $1;
		$$->next = NULL;
		
		symtab_create_function_scope($1);
	}
 |
	{
		$$ = NULL;
	}
 ;

formal_parameter_list : LPAREN formal_parameter_section_list RPAREN 
	{
		$$ = $2;
	}
;
formal_parameter_section_list : formal_parameter_section_list semicolon formal_parameter_section
	{
		add_to_formal_parameter_section_list(&$1, $3);
	}
 | formal_parameter_section
	{
		// Create the head of the formal_parameter_section_list
		$$ = new_formal_parameter_section_list();
		$$->fps = $1;
		$$->next = NULL;
	}
 ;

formal_parameter_section : value_parameter_specification
	{
		$$ = new_formal_parameter_section();
		$$->il = $1->il;
		$$->id = $1->id;
		$$->is_var = true;
	}
 | variable_parameter_specification
 	{
 		$$ = new_formal_parameter_section();
 		$$->il = $1->il;
		$$->id = $1->id;
		$$->is_var = false;
 	}
 ;

value_parameter_specification : identifier_list COLON identifier
	{
		$$ = new_formal_parameter_section();
		$$->il = $1;
		$$->id = $3;
	}
 ;

variable_parameter_specification : VAR identifier_list COLON identifier
	{
		$$ = new_formal_parameter_section();
		$$->il = $2;
		$$->id = $4;
	}
 ;

function_declaration : function_identification semicolon function_block
	{
		$$ = new_function_declaration();
		struct function_heading_t *fh = new_function_heading();
		fh->id = $1;
		$$->fh = fh;
		$$->fb = $3;
		$$->line_number = line_number;
	}
 | function_heading semicolon function_block
	{
		$$ = new_function_declaration();
		$$->fh = $1;
		$$->fb = $3;
		$$->line_number = line_number;
	}
 ;

function_heading : FUNCTION identifier COLON result_type
	{
		$$ = new_function_heading();
		$$->id = $2;
		$$->res = $4;
		$$->fpsl = NULL;
	}
 | FUNCTION identifier formal_parameter_list COLON result_type
	{
		$$ = new_function_heading();
		$$->id = $2;
		$$->res = $5;
		$$->fpsl = $3;
	}
 ;

result_type : identifier ;

function_identification : FUNCTION identifier
	{
		// Create an identifier for the function
		$$ = new_identifier($2);
	}
;

function_block : variable_declaration_part statement_part
	{
		$$ = new_function_block();
		$$->vdl = $1;
		$$->ss = $2;
	}
;

statement_part : compound_statement
	{
		$$ = $1;
	}
 ;

compound_statement : PBEGIN statement_sequence END
	{
		GLOG(("compound_statement : PBEGIN statement_sequence END\n"));
		$$ = $2;
	}
 ;

statement_sequence : statement
	{
		GLOG(("statement_sequence: statement\n"));
		// Create head of the statement_sequence list
		$$ = new_statement_sequence();
		$$->s = $1;
		$$->next = NULL;
	}
 | statement_sequence semicolon statement
	{
		GLOG(("statement_sequence : statement_sequence semicolon statement\n"));
		// Add to statement_sequence list
		add_to_statement_sequence(&$1, $3);
	}
 ;

statement : assignment_statement
	{
		GLOG(("statement : assignment_statement\n"));
		$$ = new_statement();
		$$->type = STATEMENT_T_ASSIGNMENT;
		$$->data.as = $1;
		$$->line_number = line_number;
	}
 | compound_statement
	{
		GLOG(("statement : compound_statement\n"));
		$$ = new_statement();
		$$->type = STATEMENT_T_SEQUENCE;
		$$->data.ss = $1;
		$$->line_number = line_number;
	}
 | if_statement
	{
		GLOG(("statement : if_statement\n"));
		$$ = new_statement();
		$$->type = STATEMENT_T_IF;
		$$->data.is = $1;
		$$->line_number = line_number;	
	}
 | while_statement
	{
		GLOG(("statement : while_statement\n"));
		$$ = new_statement();
		$$->type = STATEMENT_T_WHILE;
		$$->data.ws = $1;
		$$->line_number = line_number;
	}
 | print_statement
    {
    	GLOG(("statement : print_statement\n"));
		$$ = new_statement();
		$$->type = STATEMENT_T_PRINT;
		$$->data.ps = $1;
		$$->line_number = line_number;
    }
 ;

while_statement : WHILE boolean_expression DO statement
	{
		$$ = new_while_statement();
		$$->e = $2;
		$$->s = $4;
	}
 ;

if_statement : IF boolean_expression THEN statement ELSE statement
	{
		GLOG(("if_statement : IF boolean_expression THEN statement ELSE statement\n"));
		$$ = new_if_statement();
		$$->e = $2;
		$$->s1 = $4;
		$$->s2 = $6;
	}
 ;

assignment_statement : variable_access ASSIGNMENT expression
	{
		GLOG(("assignment_statement : variable_access ASSIGNMENT expression\n"));
		$$ = new_assignment_statement();
		$$->va = $1;
		$$->e = $3;
		$$->oe = NULL;
	}
 | variable_access ASSIGNMENT object_instantiation
	{
		GLOG(("assignment_statement : variable_access ASSIGNMENT object_instantiation\n"));
		$$ = new_assignment_statement();
		$$->va = $1;
		$$->e = NULL;
		$$->oe = $3;
	}
 ;

object_instantiation: NEW identifier
	{
		$$ = new_object_instantiation();
		$$->id = $2;
	}
 | NEW identifier params
	{
		$$ = new_object_instantiation();
		$$->id = $2;
		$$->apl = $3;
	}
;

print_statement : PRINT variable_access
    {
		$$ = new_print_statement();
		$$->va = $2;
    }
;

variable_access : identifier
	{
		GLOG(("variable_access : identifier = %s\n", $1));
		$$ = new_variable_access();
		$$->type = VARIABLE_ACCESS_T_IDENTIFIER;
		$$->data.id = $1;
		$$->expr = new_expression_data();
		$$->expr->type = PRIMITIVE_TYPE_NAME_UNKNOWN;
	}
 | indexed_variable
	{
		$$ = new_variable_access();
		$$->type = VARIABLE_ACCESS_T_INDEXED_VARIABLE;
		$$->data.iv = $1;
		$$->expr = new_expression_data();
		$$->expr->type = PRIMITIVE_TYPE_NAME_UNKNOWN;
	}
 | attribute_designator
	{
		$$ = new_variable_access();
		$$->type = VARIABLE_ACCESS_T_ATTRIBUTE_DESIGNATOR;
		$$->data.ad = $1;
		$$->expr = new_expression_data();
		$$->expr->type = PRIMITIVE_TYPE_NAME_UNKNOWN;
	}
 | method_designator
	{
		$$ = new_variable_access();
		$$->type = VARIABLE_ACCESS_T_METHOD_DESIGNATOR;
		$$->data.md = $1;
		$$->expr = new_expression_data();
		$$->expr->type = PRIMITIVE_TYPE_NAME_UNKNOWN;
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
		$$ = new_index_expression_list();
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

boolean_expression : expression 
	{
		GLOG(("boolean_expression : expression\n"));
		$$ = $1;
	}
;

expression : simple_expression
	{
		GLOG(("expression : simple_expression\n"));
		$$ = new_expression();
		$$->se1 = $1;
		$$->expr = $1->expr;
	}
 | simple_expression relop simple_expression
	{
		GLOG(("expression : simple_expression relop=%i simple_expression\n",$2));
		$$ = new_expression();
		$$->se1 = $1;
		$$->relop = $2;
		$$->se2 = $3;
		//Do some kind of type checking ??	
		if(strcmp(PRIMITIVE_TYPE_NAME_INTEGER, $1->expr->type) && strcmp(PRIMITIVE_TYPE_NAME_INTEGER, $3->expr->type)) {
			$$->expr = new_expression_data();
			$$->expr->type = PRIMITIVE_TYPE_NAME_BOOLEAN;
			if($2 == OP_EQUAL)
				$$->expr->val = ($1->expr->val == $3->expr->val);
			if($2 == OP_NOTEQUAL)
				$$->expr->val = ($1->expr->val != $3->expr->val);
			if($2 == OP_GT)
				$$->expr->val = ($1->expr->val > $3->expr->val);
			if($2 == OP_LT)
				$$->expr->val = ($1->expr->val < $3->expr->val);
			if($2 == OP_GE)
				$$->expr->val = ($1->expr->val >= $3->expr->val);
			if($2 == OP_LE)
				$$->expr->val = ($1->expr->val <= $3->expr->val);
		} else if(strcmp(PRIMITIVE_TYPE_NAME_BOOLEAN, $1->expr->type) && strcmp(PRIMITIVE_TYPE_NAME_BOOLEAN, $3->expr->type)) {
				$$->expr = new_expression_data();
				$$->expr->type = PRIMITIVE_TYPE_NAME_BOOLEAN;
			if($2 == OP_EQUAL)
				$$->expr->val = ($1->expr->val == $3->expr->val);
			if($2 == OP_NOTEQUAL)
				$$->expr->val = ($1->expr->val != $3->expr->val);
		} else {
			//Type mismatch
			$$->expr = new_expression_data();
			$$->expr->type = PRIMITIVE_TYPE_NAME_UNKNOWN;
		}
	}
 ;

simple_expression : term
	{
		GLOG(("simple_expression : term\n"));
		$$ = new_simple_expression();
		$$->t = $1;
		$$->expr = $1->expr;
	}
 | simple_expression addop term
	{
		GLOG(("simple_expression : simple_expressoin addop term\n"));
		add_to_simple_expression(&$1, $2, $3);
		//Do some kind of type checking ??
		if(strcmp(PRIMITIVE_TYPE_NAME_INTEGER, $1->expr->type) && strcmp(PRIMITIVE_TYPE_NAME_INTEGER, $3->expr->type)) {
			$$->expr = new_expression_data();
			$$->expr->type = PRIMITIVE_TYPE_NAME_INTEGER;
			if($2 == OP_PLUS) 
				$$->expr->val = $1->expr->val + $3->expr->val;
			else if($2 == OP_MINUS)
				$$->expr->val = $1->expr->val - $3->expr->val;
			else {
				// Invalid operation with integers
				}
		} else if(strcmp(PRIMITIVE_TYPE_NAME_BOOLEAN, $1->expr->type) && strcmp(PRIMITIVE_TYPE_NAME_BOOLEAN, $3->expr->type)) {
			$$->expr = new_expression_data();
			$$->expr->type = PRIMITIVE_TYPE_NAME_BOOLEAN;
			if($2 == OP_OR)
				$$->expr->val = (int)$1->expr->val || (int)$3->expr->val;
			else {
				// Invalid operation with boolean
				}
		} else {
			//Type mismatch
			$$->expr = new_expression_data();
			$$->expr->type = PRIMITIVE_TYPE_NAME_UNKNOWN;
		}
	}
 ;

term : factor
	{
		GLOG(("term : factor\n"));
		$$ = new_term();
		$$->f = $1;
		$$->expr = $1->expr;	
	}
 | term mulop factor
	{
		GLOG(("term : term mulop factor\n"));
		add_to_term(&$1, $2, $3);	
		//Do some kind of type checking ??
		if(strcmp(PRIMITIVE_TYPE_NAME_INTEGER, $1->expr->type) && strcmp(PRIMITIVE_TYPE_NAME_INTEGER, $3->expr->type)) {
			$$->expr = new_expression_data();
			$$->expr->type = PRIMITIVE_TYPE_NAME_INTEGER;
			if($2 == OP_STAR) 
				$$->expr->val = $1->expr->val * $3->expr->val;
			else if($2 == OP_SLASH)
				$$->expr->val = $1->expr->val / $3->expr->val;
			else if($2 == OP_MOD)
				$$->expr->val = (int)$1->expr->val % (int)$3->expr->val;
			else {
				// Invalid operation with integers
				}
		} else if(strcmp(PRIMITIVE_TYPE_NAME_BOOLEAN, $1->expr->type) && strcmp(PRIMITIVE_TYPE_NAME_BOOLEAN, $3->expr->type)) {
			$$->expr = new_expression_data();
			$$->expr->type = PRIMITIVE_TYPE_NAME_BOOLEAN;
			if($2 == OP_AND)
				$$->expr->val = (int)$1->expr->val && (int)$3->expr->val;
			else {
				// Invalid operation with boolean
				}
		} else {
			//Type mismatch
			$$->expr = new_expression_data();
			$$->expr->type = PRIMITIVE_TYPE_NAME_UNKNOWN;
		}
	}
 ;

sign : PLUS
	{
		$$ = new_sign();
		*$$ = OP_PLUS;
	}
 | MINUS
	{
		$$ = new_sign();
		*$$ = OP_MINUS;
	}
 ;

factor : sign factor
	{
		GLOG(("factor : sign factor\n"));
		// Check if the factor $2 already has a sign
		if($2->type == FACTOR_T_SIGNFACTOR) {
			error_too_many_signs(line_number);
		}
		$$ = new_factor();
		$$->type = FACTOR_T_SIGNFACTOR;
		$$->data.f.sign = $1;
		$$->data.f.next = $2;
		$$->expr = $2->expr;
	}
 | primary 
	{
		GLOG(("factor : primary\n"));
		$$ = new_factor();
		$$->type = FACTOR_T_PRIMARY;
		$$->data.p = $1;
		$$->expr = $1->expr;
	}
 ;

primary : variable_access
	{
		GLOG(("primary : variable_access\n"));
		$$ = new_primary();
		$$->type = PRIMARY_T_VARIABLE_ACCESS;
		$$->data.va = $1;
		$$->expr = $1->expr;
	}
 | unsigned_constant
	{
		GLOG(("primary : unsigned_constant=%lf\n",$1->expr->val));
		$$ = new_primary();
		$$->type = PRIMARY_T_UNSIGNED_CONSTANT;
		$$->data.un = $1;
		$$->expr = $1->expr;
	}
 | function_designator
	{
		GLOG(("primary : function_designator\n"));
		$$ = new_primary();
		$$->type = PRIMARY_T_FUNCTION_DESIGNATOR;
		$$->data.fd = $1;
		$$->expr = new_expression_data();
		$$->expr->type = PRIMITIVE_TYPE_NAME_UNKNOWN;
	}
 | LPAREN expression RPAREN
	{
		GLOG(("primary : LPAREN expression RPAREN\n"));
		$$ = new_primary();
		$$->type = PRIMARY_T_EXPRESSION;
		$$->data.e = $2;
		$$->expr = $2->expr;
	}
 | NOT primary
	{
		GLOG(("primary : NOT primary\n"));
		$$ = new_primary();
		$$->type = PRIMARY_T_PRIMARY;
		$$->data.p.not = true;
		$$->data.p.next = $2;
		$$->expr = $2->expr;
	}
 ;

unsigned_constant : unsigned_number
 ;

unsigned_number : unsigned_integer ;

unsigned_integer : DIGSEQ
	{
		GLOG(("unsigned_integer : DIGSEQ\n"));
		$$ = new_unsigned_number();
		$$->ui = atoi(yytext);
		$$->expr = new_expression_data(); 
        $$->expr->type = PRIMITIVE_TYPE_NAME_INTEGER; 
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
		$$ = OP_PLUS;
	}
 | MINUS
	{
		$$ = OP_MINUS;
	}
 | OR
	{
		$$ = OP_OR;
	}
 ;

mulop : STAR
	{
		$$ = OP_STAR;
	}
 | SLASH
	{
		$$ = OP_SLASH;
	}
 | MOD
	{
		$$ = OP_MOD;
	}
 | AND
	{
		$$ = OP_AND;
	}
 ;

relop : EQUAL
	{
		$$ = OP_EQUAL;
	}
 | NOTEQUAL
	{
		$$ = OP_NOTEQUAL;
	}
 | LT
	{
		$$ = OP_LT;
	}
 | GT
	{
		$$ = OP_GT;
	}
 | LE
	{
		$$ = OP_LE;
	}
 | GE
	{
		$$ = OP_GE;
	}
 ;

identifier : IDENTIFIER
	{
		$$ = new_identifier(yytext);
		GLOG(("identifier : IDENTIFIER = %s\n",$$));
	}
 ;

semicolon : SEMICOLON
 ;

comma : COMMA
 ;

%%

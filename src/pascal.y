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
		add_to_class_list($1, $2, $4)
		
		// Return to the higher scope and set the class_list_t ptr
		symtab_exit_scope($1->next);
	}
 | class_identification PBEGIN class_block END
	{
		// Create the tail of the class list, $$ = new_class_list()
		$$ = new_class_list();
		$$->ci = $1;
		$$->cb = $3;
		$$->next = NULL;
		
		// Return to the higher scope and set the class_list_t ptr
		symtab_exit_scope($$);
	}
 ;

class_identification : CLASS identifier
	{
		// Create class_identification without an extend id
		$$ = new_class_identification();
		$$->id = $1;
		$$->extend = NULL;
		
		// After encountering a class ident, start a class scope
		symtab_insert(symAttr(SA_CLASS), NULL);
		symtab_enter_scope();
	}
| CLASS identifier EXTENDS identifier
	{
		// Create class_identification with an extend id
		$$ = new_class_identification();
		$$->id = $1;
		$$->extend = $3;
		
		// After encountering a class ident, start a class scope
		symtab_insert(symAttr(SA_CLASS), NULL);
		symtab_enter_scope();
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
		$$->name = 
		$$->data.at = $1;
	}
 | identifier
	{
		$$ = new_type_denoter();
		$$->type = TYPE_DENOTER_T_IDENTIFIER;
		$$->name = $1;
		$$->data.id = $1;
	}
 ;

array_type : ARRAY LBRAC range RBRAC OF type_denoter
	{
		$$ = new_array_type();
		$$->r = $3;
		$$->td = $6;
	}
 ;

range : unsigned_integer DOTDOT unsigned_integer
	{
		$$ = new_range();
		$$->min = $1;
		$$->max = $3;
	}
 ;

variable_declaration_part : VAR variable_declaration_list semicolon
	{
		add_to_variable_declaration_list($2);
	}
 |
	{
		$$ = new_variable_declaration_list();
	}
 ;

variable_declaration_list : variable_declaration_list semicolon variable_declaration
	{
		add_to_variable_declaration_list($1, $3);
		symtab_insert(symAttr(SA_VAR), $1->next);
	}
 | variable_declaration
	{
		// Create the head of the variable list
		$$ = new_variable_declaration_list();
		
		// Insert the variable declaration list into the currentScope's next node
		symtab_insert(symAttr(SA_VAR), $$)
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
		add_to_func_declaration_list(&$1, $3);
		
		// Exit the function's scope
		symtab_exit_scope($1->next);
	}
 | function_declaration
	{
		$$ = new_func_declaration_list();
		$$->fd = $1;
		$$->next = NULL;
		
		// Exit the function's scope
		symtab_exit_scope($$);
	}
 |
	{
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
		$$->il = $1->next;
		$$->id = $1->id;
		$$->is_var = true;
	}
 | variable_parameter_specification
 	{
 		$$ = new_formal_parameter_section();
 		$$->il = $1->next;
		$$->id = $1->id;
		$$->is_var = false;
 	}
 ;

value_parameter_specification : identifier_list COLON identifier
	{
		$$ = new_identifier_list();
		$$->next = $2;
		$$-id = $4;
	}
 ;

variable_parameter_specification : VAR identifier_list COLON identifier
	{
		$$ = new_identifier_list();
		$$->next = $2;
		$$-id = $4;
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
		
		// Insert the declaration list entry into the symbol table
		symtab_insert(sym_attr_t(SA_FUNC), NULL);
		symtab_enter_scope();
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
 ;

compound_statement : PBEGIN statement_sequence END
	{
		$$ = $2;
	}
 ;

statement_sequence : statement
	{
		// Create head of the statement_sequence list
		$$ = new_statement_sequence();
		$$->s = $1;
		$$->next = NULL;
	}
 | statement_sequence semicolon statement
	{
		// Add to statement_sequence list
		$$ = add_to_statement_sequence(&$1, $3);
	}
 ;

statement : assignment_statement
	{
		$$ = new_statement();
		$$->type = STATEMENT_T_ASSIGNMENT;
		$$->data.as = $1;
		$$->line_number = line_number;
	}
 | compound_statement
	{
		$$ = new_statement();
		$$->type = STATEMENT_T_SEQUENCE;
		$$->data.ss = $1;
		$$->line_number = line_number;
	}
 | if_statement
	{
		$$ = new_statement();
		$$->type = STATEMENT_T_IF;
		$$->data.is = $1;
		$$->line_number = line_number;
	}
 | while_statement
	{
		$$ = new_statement();
		$$->type = STATEMENT_T_WHILE;
		$$->data.ws = $1;
		$$->line_number = line_number;
	}
 | print_statement
    {
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
		$$ = new_if_statement();
		$$->e = $2;
		$$->s1 = $4;
		$$->s2 = $6;
	}
 ;

assignment_statement : variable_access ASSIGNMENT expression
	{
		$$ = new_assignment_statement();
		$$->va = $1;
		$$->e = $3;
		$$->oe = NULL;
	}
 | variable_access ASSIGNMENT object_instantiation
	{
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
		$$->apl = params;
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

	}
 | indexed_variable
	{

	}
 | attribute_designator
	{

	}
 | method_designator
	{

	}
 ;

indexed_variable : variable_access LBRAC index_expression_list RBRAC
	{

	}
 ;

index_expression_list : index_expression_list comma index_expression
	{

	}
 | index_expression
	{

	}
 ;

index_expression : expression ;

attribute_designator : variable_access DOT identifier
	{

	}
;

method_designator: variable_access DOT function_designator
	{

	}
 ;


params : LPAREN actual_parameter_list RPAREN 
	{

	}
 ;

actual_parameter_list : actual_parameter_list comma actual_parameter
	{

	}
 | actual_parameter 
	{

	}
 ;

actual_parameter : expression
	{

	}
 | expression COLON expression
	{

	}
 | expression COLON expression COLON expression
	{

	}
 ;

boolean_expression : expression ;

expression : simple_expression
	{

	}
 | simple_expression relop simple_expression
	{

	}
 ;

simple_expression : term
	{

	}
 | simple_expression addop term
	{

	}
 ;

term : factor
	{

	}
 | term mulop factor
	{

	}
 ;

sign : PLUS
	{

	}
 | MINUS
	{

	}
 ;

factor : sign factor
	{

	}
 | primary 
	{

	}
 ;

primary : variable_access
	{

	}
 | unsigned_constant
	{

	}
 | function_designator
	{

	}
 | LPAREN expression RPAREN
	{

	}
 | NOT primary
	{

	}
 ;

unsigned_constant : unsigned_number
 ;

unsigned_number : unsigned_integer ;

unsigned_integer : DIGSEQ
	{

	}
 ;

/* functions with no params will be handled by plain identifier */
function_designator : identifier params
	{

	}
 ;

addop: PLUS
	{

	}
 | MINUS
	{

	}
 | OR
	{

	}
 ;

mulop : STAR
	{

	}
 | SLASH
	{

	}
 | MOD
	{

	}
 | AND
	{

	}
 ;

relop : EQUAL
	{

	}
 | NOTEQUAL
	{

	}
 | LT
	{

	}
 | GT
	{

	}
 | LE
	{

	}
 | GE
	{

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

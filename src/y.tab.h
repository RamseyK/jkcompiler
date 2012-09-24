#include "shared.h"
/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     AND = 258,
     ARRAY = 259,
     ASSIGNMENT = 260,
     CLASS = 261,
     COLON = 262,
     COMMA = 263,
     DIGSEQ = 264,
     DO = 265,
     DOT = 266,
     DOTDOT = 267,
     ELSE = 268,
     END = 269,
     EQUAL = 270,
     EXTENDS = 271,
     FUNCTION = 272,
     GE = 273,
     GT = 274,
     IDENTIFIER = 275,
     IF = 276,
     LBRAC = 277,
     LE = 278,
     LPAREN = 279,
     LT = 280,
     MINUS = 281,
     MOD = 282,
     NEW = 283,
     NOT = 284,
     NOTEQUAL = 285,
     OF = 286,
     OR = 287,
     PBEGIN = 288,
     PLUS = 289,
     PRINT = 290,
     PROGRAM = 291,
     RBRAC = 292,
     RPAREN = 293,
     SEMICOLON = 294,
     SLASH = 295,
     STAR = 296,
     THEN = 297,
     VAR = 298,
     WHILE = 299
   };
#endif
/* Tokens.  */
#define AND 258
#define ARRAY 259
#define ASSIGNMENT 260
#define CLASS 261
#define COLON 262
#define COMMA 263
#define DIGSEQ 264
#define DO 265
#define DOT 266
#define DOTDOT 267
#define ELSE 268
#define END 269
#define EQUAL 270
#define EXTENDS 271
#define FUNCTION 272
#define GE 273
#define GT 274
#define IDENTIFIER 275
#define IF 276
#define LBRAC 277
#define LE 278
#define LPAREN 279
#define LT 280
#define MINUS 281
#define MOD 282
#define NEW 283
#define NOT 284
#define NOTEQUAL 285
#define OF 286
#define OR 287
#define PBEGIN 288
#define PLUS 289
#define PRINT 290
#define PROGRAM 291
#define RBRAC 292
#define RPAREN 293
#define SEMICOLON 294
#define SLASH 295
#define STAR 296
#define THEN 297
#define VAR 298
#define WHILE 299




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 88 "pascal.y"
{
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
/* Line 1529 of yacc.c.  */
#line 181 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;


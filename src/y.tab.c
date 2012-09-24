/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "pascal.y"

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

  int yylex(void);
  void yyerror(const char *error);

  extern char *yytext;          /* yacc text variable */
  extern int line_number;       /* Holds the current line number; specified
				   in the lexer */
  struct program_t *program;    /* points to our program */


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 193 of yacc.c.  */
#line 251 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 264 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   171

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
#define YYNRULES  105
/* YYNRULES -- Number of states.  */
#define YYNSTATES  182

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   299

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     8,    11,    17,    21,    23,    29,    34,
      37,    42,    45,    47,    49,    56,    60,    64,    65,    69,
      71,    75,    79,    81,    82,    86,    90,    92,    94,    96,
     100,   105,   109,   113,   118,   124,   126,   129,   132,   134,
     138,   140,   144,   146,   148,   150,   152,   154,   159,   166,
     170,   174,   177,   181,   184,   186,   188,   190,   192,   197,
     201,   203,   205,   209,   213,   217,   221,   223,   225,   229,
     235,   237,   239,   243,   245,   249,   251,   255,   257,   259,
     262,   264,   266,   268,   270,   274,   277,   279,   281,   283,
     286,   288,   290,   292,   294,   296,   298,   300,   302,   304,
     306,   308,   310,   312,   314,   316
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      46,     0,    -1,    47,    39,    49,    11,    -1,    36,   101,
      -1,    36,   101,    24,    48,    38,    -1,    48,   103,   101,
      -1,   101,    -1,    49,    50,    33,    51,    14,    -1,    50,
      33,    51,    14,    -1,     6,   101,    -1,     6,   101,    16,
     101,    -1,    55,    58,    -1,    53,    -1,   101,    -1,     4,
      22,    54,    37,    31,    52,    -1,    96,    12,    96,    -1,
      43,    56,   102,    -1,    -1,    56,   102,    57,    -1,    57,
      -1,    48,     7,    52,    -1,    58,   102,    64,    -1,    64,
      -1,    -1,    24,    60,    38,    -1,    60,   102,    61,    -1,
      61,    -1,    62,    -1,    63,    -1,    48,     7,   101,    -1,
      43,    48,     7,   101,    -1,    67,   102,    68,    -1,    65,
     102,    68,    -1,    17,   101,     7,    66,    -1,    17,   101,
      59,     7,    66,    -1,   101,    -1,    17,   101,    -1,    55,
      69,    -1,    70,    -1,    33,    71,    14,    -1,    72,    -1,
      71,   102,    72,    -1,    75,    -1,    70,    -1,    74,    -1,
      73,    -1,    77,    -1,    44,    87,    10,    72,    -1,    21,
      87,    42,    72,    13,    72,    -1,    78,     5,    88,    -1,
      78,     5,    76,    -1,    28,   101,    -1,    28,   101,    84,
      -1,    35,    78,    -1,   101,    -1,    79,    -1,    82,    -1,
      83,    -1,    78,    22,    80,    37,    -1,    80,   103,    81,
      -1,    81,    -1,    88,    -1,    78,    11,   101,    -1,    78,
      11,    97,    -1,    24,    85,    38,    -1,    85,   103,    86,
      -1,    86,    -1,    88,    -1,    88,     7,    88,    -1,    88,
       7,    88,     7,    88,    -1,    88,    -1,    89,    -1,    89,
     100,    89,    -1,    90,    -1,    89,    98,    90,    -1,    92,
      -1,    90,    99,    92,    -1,    34,    -1,    26,    -1,    91,
      92,    -1,    93,    -1,    78,    -1,    94,    -1,    97,    -1,
      24,    88,    38,    -1,    29,    93,    -1,    95,    -1,    96,
      -1,     9,    -1,   101,    84,    -1,    34,    -1,    26,    -1,
      32,    -1,    41,    -1,    40,    -1,    27,    -1,     3,    -1,
      15,    -1,    30,    -1,    25,    -1,    19,    -1,    23,    -1,
      18,    -1,    20,    -1,    39,    -1,     8,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   134,   134,   140,   143,   148,   159,   171,   175,   182,
     191,   202,   209,   213,   219,   225,   231,   236,   241,   245,
     252,   258,   262,   267,   272,   277,   281,   287,   288,   291,
     297,   303,   307,   313,   317,   323,   325,   332,   339,   342,
     348,   352,   358,   362,   366,   370,   374,   380,   386,   392,
     396,   402,   406,   412,   418,   422,   426,   430,   436,   442,
     446,   452,   454,   460,   467,   473,   477,   483,   487,   491,
     497,   499,   503,   509,   513,   519,   523,   529,   533,   539,
     543,   549,   553,   557,   561,   565,   571,   574,   576,   583,
     589,   593,   597,   603,   607,   611,   615,   621,   625,   629,
     633,   637,   641,   647,   653,   656
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "AND", "ARRAY", "ASSIGNMENT", "CLASS",
  "COLON", "COMMA", "DIGSEQ", "DO", "DOT", "DOTDOT", "ELSE", "END",
  "EQUAL", "EXTENDS", "FUNCTION", "GE", "GT", "IDENTIFIER", "IF", "LBRAC",
  "LE", "LPAREN", "LT", "MINUS", "MOD", "NEW", "NOT", "NOTEQUAL", "OF",
  "OR", "PBEGIN", "PLUS", "PRINT", "PROGRAM", "RBRAC", "RPAREN",
  "SEMICOLON", "SLASH", "STAR", "THEN", "VAR", "WHILE", "$accept",
  "program", "program_heading", "identifier_list", "class_list",
  "class_identification", "class_block", "type_denoter", "array_type",
  "range", "variable_declaration_part", "variable_declaration_list",
  "variable_declaration", "func_declaration_list", "formal_parameter_list",
  "formal_parameter_section_list", "formal_parameter_section",
  "value_parameter_specification", "variable_parameter_specification",
  "function_declaration", "function_heading", "result_type",
  "function_identification", "function_block", "statement_part",
  "compound_statement", "statement_sequence", "statement",
  "while_statement", "if_statement", "assignment_statement",
  "object_instantiation", "print_statement", "variable_access",
  "indexed_variable", "index_expression_list", "index_expression",
  "attribute_designator", "method_designator", "params",
  "actual_parameter_list", "actual_parameter", "boolean_expression",
  "expression", "simple_expression", "term", "sign", "factor", "primary",
  "unsigned_constant", "unsigned_number", "unsigned_integer",
  "function_designator", "addop", "mulop", "relop", "identifier",
  "semicolon", "comma", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    45,    46,    47,    47,    48,    48,    49,    49,    50,
      50,    51,    52,    52,    53,    54,    55,    55,    56,    56,
      57,    58,    58,    58,    59,    60,    60,    61,    61,    62,
      63,    64,    64,    65,    65,    66,    67,    68,    69,    70,
      71,    71,    72,    72,    72,    72,    72,    73,    74,    75,
      75,    76,    76,    77,    78,    78,    78,    78,    79,    80,
      80,    81,    82,    83,    84,    85,    85,    86,    86,    86,
      87,    88,    88,    89,    89,    90,    90,    91,    91,    92,
      92,    93,    93,    93,    93,    93,    94,    95,    96,    97,
      98,    98,    98,    99,    99,    99,    99,   100,   100,   100,
     100,   100,   100,   101,   102,   103
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     2,     5,     3,     1,     5,     4,     2,
       4,     2,     1,     1,     6,     3,     3,     0,     3,     1,
       3,     3,     1,     0,     3,     3,     1,     1,     1,     3,
       4,     3,     3,     4,     5,     1,     2,     2,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     4,     6,     3,
       3,     2,     3,     2,     1,     1,     1,     1,     4,     3,
       1,     1,     3,     3,     3,     3,     1,     1,     3,     5,
       1,     1,     3,     1,     3,     1,     3,     1,     1,     2,
       1,     1,     1,     1,     3,     2,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,   103,     3,     1,     0,     0,     0,
       0,     0,     0,     6,     9,     2,     0,    17,   105,     4,
       0,     0,    17,     0,     0,    23,     5,    10,     0,     0,
       0,    19,     8,     0,    11,    22,     0,     0,     7,     0,
     104,    16,    36,     0,    17,    17,     0,    20,    12,    13,
      18,     0,     0,     0,    21,     0,    32,    31,     0,    33,
      35,     0,     0,     0,    26,    27,    28,     0,     0,    37,
      38,    88,     0,     0,     0,     0,    24,     0,    34,     0,
       0,     0,    43,     0,    40,    45,    44,    42,    46,     0,
      55,    56,    57,    54,     0,     0,     0,    29,    25,     0,
      78,     0,    77,    81,     0,    70,    71,    73,     0,    75,
      80,    82,    86,    87,    83,    54,    53,     0,    39,     0,
       0,     0,     0,     0,    15,    30,     0,    85,     0,    97,
     102,   100,   101,    99,    91,    98,    92,    90,     0,     0,
      96,    95,    94,    93,     0,    79,     0,    89,     0,    41,
       0,    50,    49,    63,    62,     0,    60,    61,    14,    84,
       0,    74,    72,    76,     0,    66,    67,    47,    51,    58,
       0,     0,    64,     0,     0,    52,    59,    48,    65,    68,
       0,    69
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,    29,    10,    11,    24,    47,    48,    72,
      25,    30,    31,    34,    53,    63,    64,    65,    66,    35,
      36,    59,    37,    56,    69,    82,    83,    84,    85,    86,
      87,   151,    88,   103,    90,   155,   156,    91,    92,   147,
     164,   165,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   138,   144,   139,   115,    41,    20
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -115
static const yytype_int16 yypact[] =
{
     -26,     7,    31,    10,  -115,    39,  -115,    59,     7,     7,
      78,    42,    15,  -115,    56,  -115,    52,    55,  -115,  -115,
       7,     7,    55,     7,    89,    65,  -115,  -115,    92,    22,
      72,  -115,  -115,     7,    72,  -115,    72,    72,  -115,    17,
    -115,     7,     2,    65,    55,    55,   104,  -115,  -115,  -115,
    -115,     7,     5,   121,  -115,   102,  -115,  -115,   128,  -115,
    -115,     7,   101,    95,  -115,  -115,  -115,     7,    79,  -115,
    -115,  -115,   107,   127,   109,     7,  -115,     5,  -115,    81,
       7,    81,  -115,    -3,  -115,  -115,  -115,  -115,  -115,   108,
    -115,  -115,  -115,  -115,   112,   128,     7,  -115,  -115,    81,
    -115,    49,  -115,    69,   103,  -115,   106,     1,    81,  -115,
    -115,  -115,  -115,  -115,  -115,   122,    69,   138,  -115,    79,
      68,     7,    81,    17,  -115,  -115,   113,  -115,    79,  -115,
    -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,    81,    81,
    -115,  -115,  -115,  -115,    81,  -115,    81,  -115,    79,  -115,
       7,  -115,  -115,  -115,   122,     8,  -115,  -115,  -115,  -115,
     137,     1,    36,  -115,    16,  -115,   145,  -115,   122,  -115,
      81,    79,  -115,    81,    81,  -115,  -115,  -115,  -115,   146,
      81,  -115
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -115,  -115,  -115,    -6,  -115,   144,   133,    33,  -115,  -115,
      97,  -115,   116,  -115,  -115,  -115,    82,  -115,  -115,   115,
    -115,    93,  -115,   117,  -115,   110,  -115,  -114,  -115,  -115,
    -115,  -115,  -115,   -67,  -115,  -115,    -9,  -115,  -115,    -5,
    -115,    -7,    83,   -87,    28,    30,  -115,  -105,    70,  -115,
    -115,   -52,    48,  -115,  -115,  -115,    -1,   -19,  -108
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
       5,    89,    12,   145,   140,   149,    73,    13,    14,    51,
       1,   118,   126,   116,   160,    43,    18,    44,    45,    26,
      27,    46,    13,    18,    18,     4,    52,     4,   141,    39,
      18,     6,    42,   152,   167,   157,    40,     4,    49,   163,
      13,   142,   143,   124,    77,   169,    62,   170,    61,     7,
      60,    13,    89,    19,   172,    74,   173,   177,    71,   166,
      13,    89,   134,     8,   119,     9,    60,    93,   136,     4,
     137,    62,    21,    99,    97,    17,    13,    71,   101,    93,
     121,    89,    33,   157,     9,    22,   166,   179,     4,    15,
      71,   122,    99,   181,   100,   125,   150,   101,    23,     4,
      79,     4,   102,    32,    89,    99,    38,   100,    75,    18,
     101,    40,    68,   120,    80,   102,    96,    18,    93,   121,
     154,   129,    49,    81,   130,   131,    58,    93,    67,   132,
     122,   133,   134,    76,    40,    68,   135,    71,   136,    95,
     137,    55,    55,   123,    94,   128,   146,    93,   148,   168,
     171,   159,   174,   180,    16,    28,   158,    50,    54,    98,
      78,   176,    57,   175,   117,    70,   178,   162,   161,   153,
      93,   127
};

static const yytype_uint8 yycheck[] =
{
       1,    68,     8,   108,     3,   119,    58,     8,     9,     7,
      36,    14,    99,    80,   128,    34,     8,    36,    37,    20,
      21,     4,    23,     8,     8,    20,    24,    20,    27,     7,
       8,     0,    33,   120,   148,   122,    39,    20,    39,   144,
      41,    40,    41,    95,    63,    37,    52,   155,    43,    39,
      51,    52,   119,    38,    38,    61,   164,   171,     9,   146,
      61,   128,    26,    24,    83,     6,    67,    68,    32,    20,
      34,    77,    16,    24,    75,    33,    77,     9,    29,    80,
      11,   148,    17,   170,     6,    33,   173,   174,    20,    11,
       9,    22,    24,   180,    26,    96,    28,    29,    43,    20,
      21,    20,    34,    14,   171,    24,    14,    26,     7,     8,
      29,    39,    33,     5,    35,    34,     7,     8,   119,    11,
     121,    15,   123,    44,    18,    19,    22,   128,     7,    23,
      22,    25,    26,    38,    39,    33,    30,     9,    32,    12,
      34,    44,    45,    31,    37,    42,    24,   148,    10,   150,
      13,    38,     7,     7,    10,    22,   123,    41,    43,    77,
      67,   170,    45,   168,    81,    55,   173,   139,   138,   121,
     171,   101
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    36,    46,    47,    20,   101,     0,    39,    24,     6,
      49,    50,    48,   101,   101,    11,    50,    33,     8,    38,
     103,    16,    33,    43,    51,    55,   101,   101,    51,    48,
      56,    57,    14,    17,    58,    64,    65,    67,    14,     7,
      39,   102,   101,   102,   102,   102,     4,    52,    53,   101,
      57,     7,    24,    59,    64,    55,    68,    68,    22,    66,
     101,    43,    48,    60,    61,    62,    63,     7,    33,    69,
      70,     9,    54,    96,    48,     7,    38,   102,    66,    21,
      35,    44,    70,    71,    72,    73,    74,    75,    77,    78,
      79,    82,    83,   101,    37,    12,     7,   101,    61,    24,
      26,    29,    34,    78,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,   101,    78,    87,    14,   102,
       5,    11,    22,    31,    96,   101,    88,    93,    42,    15,
      18,    19,    23,    25,    26,    30,    32,    34,    98,   100,
       3,    27,    40,    41,    99,    92,    24,    84,    10,    72,
      28,    76,    88,    97,   101,    80,    81,    88,    52,    38,
      72,    90,    89,    92,    85,    86,    88,    72,   101,    37,
     103,    13,    38,   103,     7,    84,    81,    72,    86,    88,
       7,    88
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 135 "pascal.y"
    {

	}
    break;

  case 3:
#line 141 "pascal.y"
    {
	}
    break;

  case 4:
#line 144 "pascal.y"
    {
	}
    break;

  case 5:
#line 149 "pascal.y"
    {
        	// Check the identifier against the current identifier_list for duplicates
        	struct identifier_list_t *matched_il = get_identifier((yyvsp[(1) - (3)].idl), (yyvsp[(3) - (3)].id));
        	if(matched_il != NULL) {
        		error_variable_already_declared(line_number, matched_il->id, line_number-1);
        	}
        	
			// Add the identifier node to the identifier_list
			add_to_identifier_list(&(yyvsp[(1) - (3)].idl), (yyvsp[(3) - (3)].id));
        }
    break;

  case 6:
#line 160 "pascal.y"
    {
        	// Check the identifier against the current identifier_list for duplicates
        
        	// Create and add first identifier node
        	(yyval.idl) = new_identifier_list();

			// Add identifier to $$ (current identifier_list node):
			add_to_identifier_list(&(yyval.idl), (yyvsp[(1) - (1)].id));
        }
    break;

  case 7:
#line 172 "pascal.y"
    {
		// add_to_class_list(class_list($1), class_identification($2), class_block($4))
	}
    break;

  case 8:
#line 176 "pascal.y"
    {
		// Create the tail of the class list, $$ = new_class_list
		// add_to_class_list($$, class_identification($1), class_block($3))
	}
    break;

  case 9:
#line 183 "pascal.y"
    {
		// Check the class_identification against the class_list
		// Create class_identification node and set to $$
			// Set identifier to $2
			// Set extend to NULL
			// Set line number
		
	}
    break;

  case 10:
#line 192 "pascal.y"
    {
		// Check the class_identification against the class_list
		// Create class_identification node and set to $$
			// Set identifier to $2
			// Set extend to $4
			// Set line number
	}
    break;

  case 11:
#line 204 "pascal.y"
    {

	}
    break;

  case 12:
#line 210 "pascal.y"
    {

	}
    break;

  case 13:
#line 214 "pascal.y"
    {

	}
    break;

  case 14:
#line 220 "pascal.y"
    {

	}
    break;

  case 15:
#line 226 "pascal.y"
    {

	}
    break;

  case 16:
#line 232 "pascal.y"
    {

	}
    break;

  case 17:
#line 236 "pascal.y"
    {

	}
    break;

  case 18:
#line 242 "pascal.y"
    {

	}
    break;

  case 19:
#line 246 "pascal.y"
    {

	}
    break;

  case 20:
#line 253 "pascal.y"
    {

	}
    break;

  case 21:
#line 259 "pascal.y"
    {

	}
    break;

  case 22:
#line 263 "pascal.y"
    {

	}
    break;

  case 23:
#line 267 "pascal.y"
    {

	}
    break;

  case 24:
#line 273 "pascal.y"
    {

	}
    break;

  case 25:
#line 278 "pascal.y"
    {

	}
    break;

  case 26:
#line 282 "pascal.y"
    {

	}
    break;

  case 29:
#line 292 "pascal.y"
    {

	}
    break;

  case 30:
#line 298 "pascal.y"
    {

	}
    break;

  case 31:
#line 304 "pascal.y"
    {

	}
    break;

  case 32:
#line 308 "pascal.y"
    {

	}
    break;

  case 33:
#line 314 "pascal.y"
    {

	}
    break;

  case 34:
#line 318 "pascal.y"
    {

	}
    break;

  case 36:
#line 326 "pascal.y"
    {

	}
    break;

  case 37:
#line 334 "pascal.y"
    {

	}
    break;

  case 39:
#line 343 "pascal.y"
    {

	}
    break;

  case 40:
#line 349 "pascal.y"
    {

	}
    break;

  case 41:
#line 353 "pascal.y"
    {

	}
    break;

  case 42:
#line 359 "pascal.y"
    {

	}
    break;

  case 43:
#line 363 "pascal.y"
    {

	}
    break;

  case 44:
#line 367 "pascal.y"
    {

	}
    break;

  case 45:
#line 371 "pascal.y"
    {

	}
    break;

  case 46:
#line 375 "pascal.y"
    {

        }
    break;

  case 47:
#line 381 "pascal.y"
    {

	}
    break;

  case 48:
#line 387 "pascal.y"
    {

	}
    break;

  case 49:
#line 393 "pascal.y"
    {

	}
    break;

  case 50:
#line 397 "pascal.y"
    {

	}
    break;

  case 51:
#line 403 "pascal.y"
    {

	}
    break;

  case 52:
#line 407 "pascal.y"
    {

	}
    break;

  case 53:
#line 413 "pascal.y"
    {

        }
    break;

  case 54:
#line 419 "pascal.y"
    {

	}
    break;

  case 55:
#line 423 "pascal.y"
    {

	}
    break;

  case 56:
#line 427 "pascal.y"
    {

	}
    break;

  case 57:
#line 431 "pascal.y"
    {

	}
    break;

  case 58:
#line 437 "pascal.y"
    {

	}
    break;

  case 59:
#line 443 "pascal.y"
    {

	}
    break;

  case 60:
#line 447 "pascal.y"
    {

	}
    break;

  case 62:
#line 455 "pascal.y"
    {

	}
    break;

  case 63:
#line 461 "pascal.y"
    {

	}
    break;

  case 64:
#line 468 "pascal.y"
    {

	}
    break;

  case 65:
#line 474 "pascal.y"
    {

	}
    break;

  case 66:
#line 478 "pascal.y"
    {

	}
    break;

  case 67:
#line 484 "pascal.y"
    {

	}
    break;

  case 68:
#line 488 "pascal.y"
    {

	}
    break;

  case 69:
#line 492 "pascal.y"
    {

	}
    break;

  case 71:
#line 500 "pascal.y"
    {

	}
    break;

  case 72:
#line 504 "pascal.y"
    {

	}
    break;

  case 73:
#line 510 "pascal.y"
    {

	}
    break;

  case 74:
#line 514 "pascal.y"
    {

	}
    break;

  case 75:
#line 520 "pascal.y"
    {

	}
    break;

  case 76:
#line 524 "pascal.y"
    {

	}
    break;

  case 77:
#line 530 "pascal.y"
    {

	}
    break;

  case 78:
#line 534 "pascal.y"
    {

	}
    break;

  case 79:
#line 540 "pascal.y"
    {

	}
    break;

  case 80:
#line 544 "pascal.y"
    {

	}
    break;

  case 81:
#line 550 "pascal.y"
    {

	}
    break;

  case 82:
#line 554 "pascal.y"
    {

	}
    break;

  case 83:
#line 558 "pascal.y"
    {

	}
    break;

  case 84:
#line 562 "pascal.y"
    {

	}
    break;

  case 85:
#line 566 "pascal.y"
    {

	}
    break;

  case 88:
#line 577 "pascal.y"
    {

	}
    break;

  case 89:
#line 584 "pascal.y"
    {

	}
    break;

  case 90:
#line 590 "pascal.y"
    {

	}
    break;

  case 91:
#line 594 "pascal.y"
    {

	}
    break;

  case 92:
#line 598 "pascal.y"
    {

	}
    break;

  case 93:
#line 604 "pascal.y"
    {

	}
    break;

  case 94:
#line 608 "pascal.y"
    {

	}
    break;

  case 95:
#line 612 "pascal.y"
    {

	}
    break;

  case 96:
#line 616 "pascal.y"
    {

	}
    break;

  case 97:
#line 622 "pascal.y"
    {

	}
    break;

  case 98:
#line 626 "pascal.y"
    {

	}
    break;

  case 99:
#line 630 "pascal.y"
    {

	}
    break;

  case 100:
#line 634 "pascal.y"
    {

	}
    break;

  case 101:
#line 638 "pascal.y"
    {

	}
    break;

  case 102:
#line 642 "pascal.y"
    {

	}
    break;

  case 103:
#line 648 "pascal.y"
    {
		(yyval.id) = new_identifier(yytext);
	}
    break;


/* Line 1267 of yacc.c.  */
#line 2321 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 659 "pascal.y"



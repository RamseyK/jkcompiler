/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
#define YYBISON_VERSION "2.4.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
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
#include "symtab.h"
#include "usrdef.h"

  int yylex(void);
  void yyerror(const char *error);

  extern char *yytext;          /* yacc text variable */
  extern int line_number;       /* Holds the current line number; specified
				   in the lexer */
  struct program_t *program;    /* points to our program */


/* Line 189 of yacc.c  */
#line 98 "y.tab.c"

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
{

/* Line 214 of yacc.c  */
#line 90 "pascal.y"

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



/* Line 214 of yacc.c  */
#line 268 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 280 "y.tab.c"

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
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
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
       0,   136,   136,   143,   149,   157,   162,   171,   185,   199,
     212,   227,   235,   242,   257,   265,   277,   283,   288,   293,
     306,   315,   329,   340,   344,   349,   353,   362,   369,   378,
     386,   394,   403,   412,   419,   428,   430,   442,   450,   453,
     459,   466,   473,   480,   487,   494,   501,   510,   518,   527,
     534,   543,   548,   556,   563,   569,   575,   581,   589,   597,
     601,   608,   610,   618,   627,   633,   637,   644,   649,   655,
     664,   666,   672,   707,   713,   743,   748,   754,   759,   766,
     774,   783,   790,   797,   803,   810,   820,   823,   825,   836,
     844,   848,   852,   858,   862,   866,   870,   876,   880,   884,
     888,   892,   896,   902,   908,   911
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

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
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
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
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
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


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

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
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
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

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
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

/* Line 1464 of yacc.c  */
#line 137 "pascal.y"
    {
		program->ph = (yyvsp[(1) - (4)].ph);
		program->cl = (yyvsp[(3) - (4)].cl);
	}
    break;

  case 3:

/* Line 1464 of yacc.c  */
#line 144 "pascal.y"
    {
		(yyval.ph) = new_program_heading();
		(yyval.ph)->id = (yyvsp[(2) - (2)].id);
		(yyval.ph)->il = NULL;
	}
    break;

  case 4:

/* Line 1464 of yacc.c  */
#line 150 "pascal.y"
    {
		(yyval.ph) = new_program_heading();
		(yyval.ph)->id = (yyvsp[(2) - (5)].id);
		(yyval.ph)->il = (yyvsp[(4) - (5)].idl);
	}
    break;

  case 5:

/* Line 1464 of yacc.c  */
#line 158 "pascal.y"
    {        	
			// Add the identifier node to the identifier_list
			add_to_identifier_list(&(yyvsp[(1) - (3)].idl), (yyvsp[(3) - (3)].id));
        }
    break;

  case 6:

/* Line 1464 of yacc.c  */
#line 163 "pascal.y"
    {
        	// Create and add first identifier node
        	(yyval.idl) = new_identifier_list();
        	(yyval.idl)->id = (yyvsp[(1) - (1)].id);
        	(yyval.idl)->next = NULL;
        }
    break;

  case 7:

/* Line 1464 of yacc.c  */
#line 172 "pascal.y"
    {
        // Check the class_identification against the current class_list for duplicates
        struct class_list_t *matched = find_class_list((yyvsp[(1) - (5)].cl), (yyvsp[(2) - (5)].ci)->id);
        if(matched != NULL) {
        	error_class_already_declared(line_number, (yyvsp[(2) - (5)].ci)->id, line_number-1);
        }

		add_to_class_list(&(yyvsp[(1) - (5)].cl), (yyvsp[(2) - (5)].ci), (yyvsp[(4) - (5)].cb));
		
		// Return to the higher scope and set the class_list_t ptr
		//printf("exiting class-id scope\n");
		symtab_exit_scope((yyvsp[(1) - (5)].cl)->next);
	}
    break;

  case 8:

/* Line 1464 of yacc.c  */
#line 186 "pascal.y"
    {
		// Create the tail of the class list, $$ = new_class_list()
		(yyval.cl) = new_class_list();
		(yyval.cl)->ci = (yyvsp[(1) - (4)].ci);
		(yyval.cl)->cb = (yyvsp[(3) - (4)].cb);
		(yyval.cl)->next = NULL;
		
		// Return to the higher scope and set the class_list_t ptr
		//printf("exiting class-id scope (base) %p\n", $$);
		symtab_exit_scope((yyval.cl));
	}
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 200 "pascal.y"
    {
		// Create class_identification without an extend id
		(yyval.ci) = new_class_identification();
		(yyval.ci)->id = (yyvsp[(2) - (2)].id);
		(yyval.ci)->extend = NULL;
		
		// After encountering a class ident, start a class scope
		//printf("entering class scope\n");
		symtab_insert(SYM_ATTR_CLASS, NULL);
		symtab_enter_scope();

	}
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 213 "pascal.y"
    {
		// Create class_identification with an extend id
		(yyval.ci) = new_class_identification();
		(yyval.ci)->id = (yyvsp[(2) - (4)].id);
		(yyval.ci)->extend = (yyvsp[(4) - (4)].id);

		// After encountering a class ident, start a class scope
		//printf("entering class scope (base)\n");
		symtab_insert(SYM_ATTR_CLASS, NULL);
		symtab_enter_scope();
		
	}
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 228 "pascal.y"
    {
		(yyval.cb) = new_class_block();
		(yyval.cb)->vdl = (yyvsp[(1) - (2)].vdl);
		(yyval.cb)->fdl = (yyvsp[(2) - (2)].fdl);
	}
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 236 "pascal.y"
    {
		(yyval.tden) = new_type_denoter();
		(yyval.tden)->type = TYPE_DENOTER_T_ARRAY_TYPE;
		(yyval.tden)->name = NULL;
		(yyval.tden)->data.at = (yyvsp[(1) - (1)].at);
	}
    break;

  case 13:

/* Line 1464 of yacc.c  */
#line 243 "pascal.y"
    {
		struct type_denoter_t *new_type = new_type_denoter();
		new_type->type = TYPE_DENOTER_T_IDENTIFIER;
		new_type->name = (yyvsp[(1) - (1)].id);
		new_type->data.id = (yyvsp[(1) - (1)].id);
		struct type_denoter_t *found_type = usrdef_lookup_td(new_type);
		if(found_type == NULL) {
			(yyval.tden) = usrdef_insert(new_type);
		} else {
			(yyval.tden) = found_type;
		}
	}
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 258 "pascal.y"
    {
		(yyval.at) = new_array_type();
		(yyval.at)->r = (yyvsp[(3) - (6)].r);
		(yyval.at)->td = (yyvsp[(6) - (6)].tden);
	}
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 266 "pascal.y"
    {
		(yyval.r) = new_range();
		(yyval.r)->min = (yyvsp[(1) - (3)].un);
		(yyval.r)->max = (yyvsp[(3) - (3)].un);
		
		// If the min is greater than the max, error
		if((yyval.r)->min->ui > (yyval.r)->max->ui)
			error_array_range_invalid(line_number, (yyval.r)->min->ui, (yyval.r)->max->ui);
	}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 278 "pascal.y"
    {
		(yyval.vdl) = new_variable_declaration_list();
		(yyval.vdl)->next = (yyvsp[(2) - (3)].vdl);
	}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 283 "pascal.y"
    {
		(yyval.vdl) = new_variable_declaration_list();
	}
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 289 "pascal.y"
    {
		add_to_variable_declaration_list(&(yyvsp[(1) - (3)].vdl), (yyvsp[(3) - (3)].vd));
		symtab_insert(SYM_ATTR_VAR, (yyvsp[(1) - (3)].vdl)->next);
	}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 294 "pascal.y"
    {
		// Create the head of the variable list
		(yyval.vdl) = new_variable_declaration_list();
		(yyval.vdl)->vd = (yyvsp[(1) - (1)].vd);
		(yyval.vdl)->next = NULL;
		
		// Insert the variable declaration list into the currentScope's next node
		symtab_insert(SYM_ATTR_VAR, (yyval.vdl));
	}
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 307 "pascal.y"
    {
		(yyval.vd) = new_variable_declaration();
		(yyval.vd)->il = (yyvsp[(1) - (3)].idl);
		(yyval.vd)->tden = (yyvsp[(3) - (3)].tden);
		(yyval.vd)->line_number = line_number;
	}
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 316 "pascal.y"
    {
        // Check the func_declaration header against the current func_declaration_list for duplicates
        struct func_declaration_list_t *matched = find_func_list((yyvsp[(1) - (3)].fdl), (yyvsp[(3) - (3)].funcd)->fh->id);
        if(matched != NULL) {
        	error_function_already_declared(line_number, (yyvsp[(3) - (3)].funcd)->fh->id, line_number-1);
        }

		add_to_func_declaration_list(&(yyvsp[(1) - (3)].fdl), (yyvsp[(3) - (3)].funcd));
		
		// Exit the function's scope
		//printf("Exiting scope of function\n");
		symtab_exit_scope((yyvsp[(1) - (3)].fdl)->next);
	}
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 330 "pascal.y"
    {
		(yyval.fdl) = new_func_declaration_list();
		(yyval.fdl)->fd = (yyvsp[(1) - (1)].funcd);
		(yyval.fdl)->next = NULL;
		
		// Exit the function's scope
		//printf("Exiting scope of function\n");
		symtab_exit_scope((yyval.fdl));
	}
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 340 "pascal.y"
    {
	}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 345 "pascal.y"
    {
		(yyval.fpsl) = (yyvsp[(2) - (3)].fpsl);
	}
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 350 "pascal.y"
    {
		add_to_formal_parameter_section_list(&(yyvsp[(1) - (3)].fpsl), (yyvsp[(3) - (3)].fps));
	}
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 354 "pascal.y"
    {
		// Create the head of the formal_parameter_section_list
		(yyval.fpsl) = new_formal_parameter_section_list();
		(yyval.fpsl)->fps = (yyvsp[(1) - (1)].fps);
		(yyval.fpsl)->next = NULL;
	}
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 363 "pascal.y"
    {
		(yyval.fps) = new_formal_parameter_section();
		(yyval.fps)->il = (yyvsp[(1) - (1)].fps)->il;
		(yyval.fps)->id = (yyvsp[(1) - (1)].fps)->id;
		(yyval.fps)->is_var = true;
	}
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 370 "pascal.y"
    {
 		(yyval.fps) = new_formal_parameter_section();
 		(yyval.fps)->il = (yyvsp[(1) - (1)].fps)->il;
		(yyval.fps)->id = (yyvsp[(1) - (1)].fps)->id;
		(yyval.fps)->is_var = false;
 	}
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 379 "pascal.y"
    {
		(yyval.fps) = new_formal_parameter_section();
		(yyval.fps)->il = (yyvsp[(1) - (3)].idl);
		(yyval.fps)->id = (yyvsp[(3) - (3)].id);
	}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 387 "pascal.y"
    {
		(yyval.fps) = new_formal_parameter_section();
		(yyval.fps)->il = (yyvsp[(2) - (4)].idl);
		(yyval.fps)->id = (yyvsp[(4) - (4)].id);
	}
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 395 "pascal.y"
    {
		(yyval.funcd) = new_function_declaration();
		struct function_heading_t *fh = new_function_heading();
		fh->id = (yyvsp[(1) - (3)].id);
		(yyval.funcd)->fh = fh;
		(yyval.funcd)->fb = (yyvsp[(3) - (3)].fb);
		(yyval.funcd)->line_number = line_number;
	}
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 404 "pascal.y"
    {
		(yyval.funcd) = new_function_declaration();
		(yyval.funcd)->fh = (yyvsp[(1) - (3)].fh);
		(yyval.funcd)->fb = (yyvsp[(3) - (3)].fb);
		(yyval.funcd)->line_number = line_number;
	}
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 413 "pascal.y"
    {
		(yyval.fh) = new_function_heading();
		(yyval.fh)->id = (yyvsp[(2) - (4)].id);
		(yyval.fh)->res = (yyvsp[(4) - (4)].id);
		(yyval.fh)->fpsl = NULL;
	}
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 420 "pascal.y"
    {
		(yyval.fh) = new_function_heading();
		(yyval.fh)->id = (yyvsp[(2) - (5)].id);
		(yyval.fh)->res = (yyvsp[(5) - (5)].id);
		(yyval.fh)->fpsl = (yyvsp[(3) - (5)].fpsl);
	}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 431 "pascal.y"
    {
		// Create an identifier for the function
		(yyval.id) = new_identifier((yyvsp[(2) - (2)].id));
		
		// Insert the declaration list entry into the symbol table
		//printf("Entering scope of function\n");
		symtab_insert(SYM_ATTR_FUNC, NULL);
		symtab_enter_scope();
	}
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 443 "pascal.y"
    {
		(yyval.fb) = new_function_block();
		(yyval.fb)->vdl = (yyvsp[(1) - (2)].vdl);
		(yyval.fb)->ss = (yyvsp[(2) - (2)].ss);
	}
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 454 "pascal.y"
    {
		(yyval.ss) = (yyvsp[(2) - (3)].ss);
	}
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 460 "pascal.y"
    {
		// Create head of the statement_sequence list
		(yyval.ss) = new_statement_sequence();
		(yyval.ss)->s = (yyvsp[(1) - (1)].s);
		(yyval.ss)->next = NULL;
	}
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 467 "pascal.y"
    {
		// Add to statement_sequence list
		add_to_statement_sequence(&(yyvsp[(1) - (3)].ss), (yyvsp[(3) - (3)].s));
	}
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 474 "pascal.y"
    {
		(yyval.s) = new_statement();
		(yyval.s)->type = STATEMENT_T_ASSIGNMENT;
		(yyval.s)->data.as = (yyvsp[(1) - (1)].as);
		(yyval.s)->line_number = line_number;
	}
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 481 "pascal.y"
    {
		(yyval.s) = new_statement();
		(yyval.s)->type = STATEMENT_T_SEQUENCE;
		(yyval.s)->data.ss = (yyvsp[(1) - (1)].ss);
		(yyval.s)->line_number = line_number;
	}
    break;

  case 44:

/* Line 1464 of yacc.c  */
#line 488 "pascal.y"
    {
		(yyval.s) = new_statement();
		(yyval.s)->type = STATEMENT_T_IF;
		(yyval.s)->data.is = (yyvsp[(1) - (1)].is);
		(yyval.s)->line_number = line_number;
	}
    break;

  case 45:

/* Line 1464 of yacc.c  */
#line 495 "pascal.y"
    {
		(yyval.s) = new_statement();
		(yyval.s)->type = STATEMENT_T_WHILE;
		(yyval.s)->data.ws = (yyvsp[(1) - (1)].ws);
		(yyval.s)->line_number = line_number;
	}
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 502 "pascal.y"
    {
		(yyval.s) = new_statement();
		(yyval.s)->type = STATEMENT_T_PRINT;
		(yyval.s)->data.ps = (yyvsp[(1) - (1)].ps);
		(yyval.s)->line_number = line_number;
    }
    break;

  case 47:

/* Line 1464 of yacc.c  */
#line 511 "pascal.y"
    {
		(yyval.ws) = new_while_statement();
		(yyval.ws)->e = (yyvsp[(2) - (4)].e);
		(yyval.ws)->s = (yyvsp[(4) - (4)].s);
	}
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 519 "pascal.y"
    {
		(yyval.is) = new_if_statement();
		(yyval.is)->e = (yyvsp[(2) - (6)].e);
		(yyval.is)->s1 = (yyvsp[(4) - (6)].s);
		(yyval.is)->s2 = (yyvsp[(6) - (6)].s);
	}
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 528 "pascal.y"
    {
		(yyval.as) = new_assignment_statement();
		(yyval.as)->va = (yyvsp[(1) - (3)].va);
		(yyval.as)->e = (yyvsp[(3) - (3)].e);
		(yyval.as)->oe = NULL;
	}
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 535 "pascal.y"
    {
		(yyval.as) = new_assignment_statement();
		(yyval.as)->va = (yyvsp[(1) - (3)].va);
		(yyval.as)->e = NULL;
		(yyval.as)->oe = (yyvsp[(3) - (3)].os);
	}
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 544 "pascal.y"
    {
		(yyval.os) = new_object_instantiation();
		(yyval.os)->id = (yyvsp[(2) - (2)].id);
	}
    break;

  case 52:

/* Line 1464 of yacc.c  */
#line 549 "pascal.y"
    {
		(yyval.os) = new_object_instantiation();
		(yyval.os)->id = (yyvsp[(2) - (3)].id);
		(yyval.os)->apl = (yyvsp[(3) - (3)].apl);
	}
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 557 "pascal.y"
    {
		(yyval.ps) = new_print_statement();
		(yyval.ps)->va = (yyvsp[(2) - (2)].va);
    }
    break;

  case 54:

/* Line 1464 of yacc.c  */
#line 564 "pascal.y"
    {
		(yyval.va) = new_variable_access();
		(yyval.va)->type = VARIABLE_ACCESS_T_IDENTIFIER;
		(yyval.va)->data.id = (yyvsp[(1) - (1)].id);
	}
    break;

  case 55:

/* Line 1464 of yacc.c  */
#line 570 "pascal.y"
    {
		(yyval.va) = new_variable_access();
		(yyval.va)->type = VARIABLE_ACCESS_T_INDEXED_VARIABLE;
		(yyval.va)->data.iv = (yyvsp[(1) - (1)].iv);
	}
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 576 "pascal.y"
    {
		(yyval.va) = new_variable_access();
		(yyval.va)->type = VARIABLE_ACCESS_T_ATTRIBUTE_DESIGNATOR;
		(yyval.va)->data.ad = (yyvsp[(1) - (1)].ad);
	}
    break;

  case 57:

/* Line 1464 of yacc.c  */
#line 582 "pascal.y"
    {
		(yyval.va) = new_variable_access();
		(yyval.va)->type = VARIABLE_ACCESS_T_METHOD_DESIGNATOR;
		(yyval.va)->data.md = (yyvsp[(1) - (1)].md);
	}
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 590 "pascal.y"
    {
		(yyval.iv) = new_indexed_variable();
		(yyval.iv)->va = (yyvsp[(1) - (4)].va);
		(yyval.iv)->iel = (yyvsp[(3) - (4)].iel);
	}
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 598 "pascal.y"
    {
		add_to_index_expression_list(&(yyvsp[(1) - (3)].iel), (yyvsp[(3) - (3)].e));
	}
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 602 "pascal.y"
    {
		(yyval.iel) = new_index_expression_list();
		(yyval.iel)->e = (yyvsp[(1) - (1)].e);
	}
    break;

  case 62:

/* Line 1464 of yacc.c  */
#line 611 "pascal.y"
    {
		(yyval.ad) = new_attribute_designator();
		(yyval.ad)->va = (yyvsp[(1) - (3)].va);
		(yyval.ad)->id = (yyvsp[(3) - (3)].id);
	}
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 619 "pascal.y"
    {
		(yyval.md) = new_method_designator();
		(yyval.md)->va = (yyvsp[(1) - (3)].va);
		(yyval.md)->fd = (yyvsp[(3) - (3)].fdes);
	}
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 628 "pascal.y"
    {
		(yyval.apl) = (yyvsp[(2) - (3)].apl);
	}
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 634 "pascal.y"
    {
		add_to_actual_parameter_list(&(yyvsp[(1) - (3)].apl), (yyvsp[(3) - (3)].ap));
	}
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 638 "pascal.y"
    {
		(yyval.apl) = new_actual_parameter_list();
		(yyval.apl)->ap = (yyvsp[(1) - (1)].ap);
	}
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 645 "pascal.y"
    {
		(yyval.ap) = new_actual_parameter();
		(yyval.ap)->e1 = (yyvsp[(1) - (1)].e);
	}
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 650 "pascal.y"
    {
		(yyval.ap) = new_actual_parameter();
		(yyval.ap)->e1 = (yyvsp[(1) - (3)].e);
		(yyval.ap)->e2 = (yyvsp[(3) - (3)].e);
	}
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 656 "pascal.y"
    {
		(yyval.ap) = new_actual_parameter();
		(yyval.ap)->e1 = (yyvsp[(1) - (5)].e);
		(yyval.ap)->e2 = (yyvsp[(3) - (5)].e);
		(yyval.ap)->e3 = (yyvsp[(5) - (5)].e);
	}
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 667 "pascal.y"
    {
		(yyval.e) = new_expression();
		(yyval.e)->se1 = (yyvsp[(1) - (1)].se);
		(yyval.e)->expr = (yyvsp[(1) - (1)].se)->expr;
	}
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 673 "pascal.y"
    {
		(yyval.e) = new_expression();
		(yyval.e)->se1 = (yyvsp[(1) - (3)].se);
		(yyval.e)->relop = (yyvsp[(2) - (3)].op);
		(yyval.e)->se2 = (yyvsp[(3) - (3)].se);
		//Do some kind of type checking ??
		if(strcmp(PRIMITIVE_TYPE_NAME_INTEGER, (yyvsp[(1) - (3)].se)->expr->type) && strcmp(PRIMITIVE_TYPE_NAME_INTEGER, (yyvsp[(3) - (3)].se)->expr->type)) {
			(yyval.e)->expr = new_expression_data();
			(yyval.e)->expr->type = PRIMITIVE_TYPE_NAME_BOOLEAN;
			if((yyvsp[(2) - (3)].op) == OP_EQUAL)
				(yyval.e)->expr->val = ((yyvsp[(1) - (3)].se)->expr->val == (yyvsp[(3) - (3)].se)->expr->val);
			if((yyvsp[(2) - (3)].op) == OP_NOTEQUAL)
				(yyval.e)->expr->val = ((yyvsp[(1) - (3)].se)->expr->val != (yyvsp[(3) - (3)].se)->expr->val);
			if((yyvsp[(2) - (3)].op) == OP_GT)
				(yyval.e)->expr->val = ((yyvsp[(1) - (3)].se)->expr->val > (yyvsp[(3) - (3)].se)->expr->val);
			if((yyvsp[(2) - (3)].op) == OP_LT)
				(yyval.e)->expr->val = ((yyvsp[(1) - (3)].se)->expr->val < (yyvsp[(3) - (3)].se)->expr->val);
			if((yyvsp[(2) - (3)].op) == OP_GE)
				(yyval.e)->expr->val = ((yyvsp[(1) - (3)].se)->expr->val >= (yyvsp[(3) - (3)].se)->expr->val);
			if((yyvsp[(2) - (3)].op) == OP_LE)
				(yyval.e)->expr->val = ((yyvsp[(1) - (3)].se)->expr->val <= (yyvsp[(3) - (3)].se)->expr->val);
		} else if(strcmp(PRIMITIVE_TYPE_NAME_BOOLEAN, (yyvsp[(1) - (3)].se)->expr->type) && strcmp(PRIMITIVE_TYPE_NAME_BOOLEAN, (yyvsp[(3) - (3)].se)->expr->type)) {
			if((yyvsp[(2) - (3)].op) == OP_EQUAL)
				(yyval.e)->expr->val = ((yyvsp[(1) - (3)].se)->expr->val == (yyvsp[(3) - (3)].se)->expr->val);
			if((yyvsp[(2) - (3)].op) == OP_NOTEQUAL)
				(yyval.e)->expr->val = ((yyvsp[(1) - (3)].se)->expr->val != (yyvsp[(3) - (3)].se)->expr->val);
		} else {
			//Type mismatch
			(yyval.e)->expr = new_expression_data();
			(yyval.e)->expr->type = PRIMITIVE_TYPE_NAME_UNKNOWN;
		}
	}
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 708 "pascal.y"
    {
		(yyval.se) = new_simple_expression();
		(yyval.se)->t = (yyvsp[(1) - (1)].t);
		(yyval.se)->expr = (yyvsp[(1) - (1)].t)->expr;
	}
    break;

  case 74:

/* Line 1464 of yacc.c  */
#line 714 "pascal.y"
    {
		add_to_simple_expression(&(yyvsp[(1) - (3)].se), (yyvsp[(2) - (3)].op), (yyvsp[(3) - (3)].t));
		//Do some kind of type checking ??
		if(strcmp(PRIMITIVE_TYPE_NAME_INTEGER, (yyvsp[(1) - (3)].se)->expr->type) && strcmp(PRIMITIVE_TYPE_NAME_INTEGER, (yyvsp[(3) - (3)].t)->expr->type)) {
			(yyval.se)->expr = new_expression_data();
			(yyval.se)->expr->type = PRIMITIVE_TYPE_NAME_INTEGER;
			if((yyvsp[(2) - (3)].op) == OP_PLUS) 
				(yyval.se)->expr->val = (yyvsp[(1) - (3)].se)->expr->val + (yyvsp[(3) - (3)].t)->expr->val;
			else if((yyvsp[(2) - (3)].op) == OP_MINUS)
				(yyval.se)->expr->val = (yyvsp[(1) - (3)].se)->expr->val - (yyvsp[(3) - (3)].t)->expr->val;
			else {
				// Invalid operation with integers
				}
		} else if(strcmp(PRIMITIVE_TYPE_NAME_BOOLEAN, (yyvsp[(1) - (3)].se)->expr->type) && strcmp(PRIMITIVE_TYPE_NAME_BOOLEAN, (yyvsp[(3) - (3)].t)->expr->type)) {
			(yyval.se)->expr = new_expression_data();
			(yyval.se)->expr->type = PRIMITIVE_TYPE_NAME_BOOLEAN;
			if((yyvsp[(2) - (3)].op) == OP_OR)
				(yyval.se)->expr->val = (int)(yyvsp[(1) - (3)].se)->expr->val | (int)(yyvsp[(3) - (3)].t)->expr->val;
			else {
				// Invalid operation with boolean
				}
		} else {
			//Type mismatch
			(yyval.se)->expr = new_expression_data();
			(yyval.se)->expr->type = PRIMITIVE_TYPE_NAME_UNKNOWN;
		}
	}
    break;

  case 75:

/* Line 1464 of yacc.c  */
#line 744 "pascal.y"
    {
		(yyval.t) = new_term();
		(yyval.t)->f = (yyvsp[(1) - (1)].f);		
	}
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 749 "pascal.y"
    {		
		add_to_term(&(yyvsp[(1) - (3)].t), (yyvsp[(2) - (3)].op), (yyvsp[(3) - (3)].f));	
	}
    break;

  case 77:

/* Line 1464 of yacc.c  */
#line 755 "pascal.y"
    {
		(yyval.i) = new_sign();
		*(yyval.i) = OP_PLUS;
	}
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 760 "pascal.y"
    {
		(yyval.i) = new_sign();
		*(yyval.i) = OP_MINUS;
	}
    break;

  case 79:

/* Line 1464 of yacc.c  */
#line 767 "pascal.y"
    {
		(yyval.f) = new_factor();
		(yyval.f)->type = FACTOR_T_SIGNFACTOR;
		(yyval.f)->data.f.sign = (yyvsp[(1) - (2)].i);
		(yyval.f)->data.f.next = (yyvsp[(2) - (2)].f);
		(yyval.f)->expr = (yyvsp[(2) - (2)].f)->expr;
	}
    break;

  case 80:

/* Line 1464 of yacc.c  */
#line 775 "pascal.y"
    {
		(yyval.f) = new_factor();
		(yyval.f)->type = FACTOR_T_PRIMARY;
		(yyval.f)->data.p = (yyvsp[(1) - (1)].p);
		(yyval.f)->expr = (yyvsp[(1) - (1)].p)->expr;
	}
    break;

  case 81:

/* Line 1464 of yacc.c  */
#line 784 "pascal.y"
    {
		(yyval.p) = new_primary();
		(yyval.p)->type = PRIMARY_T_VARIABLE_ACCESS;
		(yyval.p)->data.va = (yyvsp[(1) - (1)].va);
		(yyval.p)->expr = (yyvsp[(1) - (1)].va)->expr;
	}
    break;

  case 82:

/* Line 1464 of yacc.c  */
#line 791 "pascal.y"
    {
		(yyval.p) = new_primary();
		(yyval.p)->type = PRIMARY_T_UNSIGNED_CONSTANT;
		(yyval.p)->data.un = (yyvsp[(1) - (1)].un);
		(yyval.p)->expr = (yyvsp[(1) - (1)].un)->expr;
	}
    break;

  case 83:

/* Line 1464 of yacc.c  */
#line 798 "pascal.y"
    {
		(yyval.p) = new_primary();
		(yyval.p)->type = PRIMARY_T_FUNCTION_DESIGNATOR;
		(yyval.p)->data.fd = (yyvsp[(1) - (1)].fdes);
	}
    break;

  case 84:

/* Line 1464 of yacc.c  */
#line 804 "pascal.y"
    {
		(yyval.p) = new_primary();
		(yyval.p)->type = PRIMARY_T_EXPRESSION;
		(yyval.p)->data.e = (yyvsp[(2) - (3)].e);
		(yyval.p)->expr = (yyvsp[(2) - (3)].e)->expr;
	}
    break;

  case 85:

/* Line 1464 of yacc.c  */
#line 811 "pascal.y"
    {
		(yyval.p) = new_primary();
		(yyval.p)->type = PRIMARY_T_PRIMARY;
		(yyval.p)->data.p.not = true;
		(yyval.p)->data.p.next = (yyvsp[(2) - (2)].p);
		(yyval.p)->expr = (yyvsp[(2) - (2)].p)->expr;
	}
    break;

  case 88:

/* Line 1464 of yacc.c  */
#line 826 "pascal.y"
    {
		(yyval.un) = new_unsigned_number();
		(yyval.un)->ui = atoi(yytext);
		(yyval.un)->expr = new_expression_data(); 
        (yyval.un)->expr->type = new_primitive_type(PRIMITIVE_TYPE_NAME_INTEGER); 
        (yyval.un)->expr->val = (yyval.un)->ui;
	}
    break;

  case 89:

/* Line 1464 of yacc.c  */
#line 837 "pascal.y"
    {
		(yyval.fdes) = new_function_designator();
		(yyval.fdes)->id = (yyvsp[(1) - (2)].id);
		(yyval.fdes)->apl = (yyvsp[(2) - (2)].apl);
	}
    break;

  case 90:

/* Line 1464 of yacc.c  */
#line 845 "pascal.y"
    {
		(yyval.op) = OP_PLUS;
	}
    break;

  case 91:

/* Line 1464 of yacc.c  */
#line 849 "pascal.y"
    {
		(yyval.op) = OP_MINUS;
	}
    break;

  case 92:

/* Line 1464 of yacc.c  */
#line 853 "pascal.y"
    {
		(yyval.op) = OP_OR;
	}
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 859 "pascal.y"
    {
		(yyval.op) = OP_STAR;
	}
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 863 "pascal.y"
    {
		(yyval.op) = OP_SLASH;
	}
    break;

  case 95:

/* Line 1464 of yacc.c  */
#line 867 "pascal.y"
    {
		(yyval.op) = OP_MOD;
	}
    break;

  case 96:

/* Line 1464 of yacc.c  */
#line 871 "pascal.y"
    {
		(yyval.op) = OP_AND;
	}
    break;

  case 97:

/* Line 1464 of yacc.c  */
#line 877 "pascal.y"
    {
		(yyval.op) = OP_EQUAL;
	}
    break;

  case 98:

/* Line 1464 of yacc.c  */
#line 881 "pascal.y"
    {
		(yyval.op) = OP_NOTEQUAL;
	}
    break;

  case 99:

/* Line 1464 of yacc.c  */
#line 885 "pascal.y"
    {
		(yyval.op) = OP_LT;
	}
    break;

  case 100:

/* Line 1464 of yacc.c  */
#line 889 "pascal.y"
    {
		(yyval.op) = OP_GT;
	}
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 893 "pascal.y"
    {
		(yyval.op) = OP_LE;
	}
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 897 "pascal.y"
    {
		(yyval.op) = OP_GE;
	}
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 903 "pascal.y"
    {
		(yyval.id) = new_identifier(yytext);
	}
    break;



/* Line 1464 of yacc.c  */
#line 2801 "y.tab.c"
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
      /* If just tried and failed to reuse lookahead token after an
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

  /* Else will try to reuse lookahead token after shifting the error
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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
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



/* Line 1684 of yacc.c  */
#line 914 "pascal.y"



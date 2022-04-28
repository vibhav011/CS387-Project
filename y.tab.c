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
#define YYPURE 2

<<<<<<< HEAD
/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 5 "parser.y"

#include "./receiver/query.h"
#include "ast.h"
#include "utils.h"

extern vector<Temp_Table*> results;
int const_type;


#line 80 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif
=======
/* Using locations.  */
#define YYLSP_NEEDED 0
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce


<<<<<<< HEAD
/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 15 "parser.y"

  typedef void* yyscan_t;

#line 127 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     DOT_NAME = 258,
     NAME = 259,
     TEXT_CONSTANT = 260,
     INT_CONSTANT = 261,
     FLOAT_CONSTANT = 262,
     INTEGER = 263,
     FLOAT = 264,
     TEXT = 265,
     AND = 266,
     OR = 267,
     NOT = 268,
     MULT = 269,
     PLUS = 270,
     MINUS = 271,
     DIV = 272,
     GE = 273,
     LT = 274,
     GT = 275,
     LE = 276,
     NE = 277,
     EQ = 278,
     SEMICOLON = 279,
     COMMIT = 280,
     ROLLBACK = 281,
     WITH = 282,
     COMMA = 283,
     AS = 284,
     ROUND_BRACKET_OPEN = 285,
     ROUND_BRACKET_CLOSE = 286,
     SELECT = 287,
     FROM = 288,
     WHERE = 289,
     CREATE = 290,
     TABLE = 291,
     RANGE = 292,
     PRIMARY = 293,
     KEY = 294,
     INSERT = 295,
     INTO = 296,
     VALUES = 297,
     ASSIGN = 298,
     UPDATE = 299,
     SET = 300,
     DELETE = 301,
     INFINITY = 302,
     BETWEEN = 303,
     REDC = 304
   };
#endif
/* Tokens.  */
#define DOT_NAME 258
#define NAME 259
#define TEXT_CONSTANT 260
#define INT_CONSTANT 261
#define FLOAT_CONSTANT 262
#define INTEGER 263
#define FLOAT 264
#define TEXT 265
#define AND 266
#define OR 267
#define NOT 268
#define MULT 269
#define PLUS 270
#define MINUS 271
#define DIV 272
#define GE 273
#define LT 274
#define GT 275
#define LE 276
#define NE 277
#define EQ 278
#define SEMICOLON 279
#define COMMIT 280
#define ROLLBACK 281
#define WITH 282
#define COMMA 283
#define AS 284
#define ROUND_BRACKET_OPEN 285
#define ROUND_BRACKET_CLOSE 286
#define SELECT 287
#define FROM 288
#define WHERE 289
#define CREATE 290
#define TABLE 291
#define RANGE 292
#define PRIMARY 293
#define KEY 294
#define INSERT 295
#define INTO 296
#define VALUES 297
#define ASSIGN 298
#define UPDATE 299
#define SET 300
#define DELETE 301
#define INFINITY 302
#define BETWEEN 303
#define REDC 304




/* Copy the first part of user declarations.  */
#line 1 "parser.y"

#include "./receiver/query.h"
#include "ast.h"
#include "utils.h"

int yyerror(int id, const char *);
void checkerr(int err_code);

extern vector<Temp_Table*> results;
int const_type;

extern int yylex();
extern int readInputForLexer(char* buffer,unsigned long *numBytesRead,int maxBytesToRead);



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
<<<<<<< HEAD
union YYSTYPE
{
#line 26 "parser.y"

=======
typedef union YYSTYPE
#line 19 "parser.y"
{
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    int int_val;
    string* str;
    Range* range;
    Constant* constant;
    ColumnDesc* col_desc;
    Update_Pair* update_pair;
    Temp_Table* table;
    pair<string, vector<string> >* name_cols;
    vector<Temp_Table*>* vec_table;
    vector<string>* vec_string;
    vector<ColumnDesc*>* vec_col_desc;
    vector<Update_Pair*>* vec_update_pair;
    ExprAST* exprast;
    CondAST* logast;
    RelAST* relast;
}
/* Line 193 of yacc.c.  */
#line 228 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

<<<<<<< HEAD


int yyparse (yyscan_t scanner);
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce

/* Copy the second part of user declarations.  */


<<<<<<< HEAD
/* Unqualified %code blocks.  */
#line 19 "parser.y"

    int yylex(YYSTYPE* yylvalp, YYLTYPE* yyllocp, yyscan_t scanner);
    void yyerror(YYLTYPE* yyllocp, yyscan_t unused, const char* msg);
    void* yyget_extra ( yyscan_t yyscanner );
    void checkerr(int err_code);

#line 291 "y.tab.c"
=======
/* Line 216 of yacc.c.  */
#line 241 "y.tab.c"
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce

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
<<<<<<< HEAD
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))
=======
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
<<<<<<< HEAD
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};
=======
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
<<<<<<< HEAD
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)
=======
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce

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
#define YYFINAL  30
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   135

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  65
/* YYNRULES -- Number of states.  */
#define YYNSTATES  138

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   304

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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     8,    11,    14,    17,    20,    23,    26,
      29,    32,    36,    38,    42,    47,    52,    59,    63,    65,
      67,    69,    73,    75,    77,    81,    85,    88,    90,    94,
      98,   102,   106,   110,   114,   118,   122,   126,   130,   132,
     134,   136,   138,   140,   149,   156,   160,   162,   166,   169,
     171,   173,   175,   183,   189,   197,   201,   203,   205,   212,
     217,   221,   223,   227,   231,   236
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      51,     0,    -1,    52,    24,    51,    24,    -1,    56,    24,
      -1,    65,    24,    -1,    71,    24,    -1,    74,    24,    -1,
      77,    24,    -1,    25,    24,    -1,    26,    24,    -1,    27,
      53,    -1,    54,    28,    54,    -1,    54,    -1,    55,    29,
      56,    -1,     4,    30,    57,    31,    -1,    32,    57,    33,
      59,    -1,    32,    57,    33,    59,    34,    61,    -1,    57,
      28,    58,    -1,    58,    -1,     3,    -1,     4,    -1,    60,
      28,    60,    -1,    60,    -1,     4,    -1,    61,    11,    61,
      -1,    61,    12,    61,    -1,    13,    61,    -1,    62,    -1,
      63,    18,    63,    -1,    63,    19,    63,    -1,    63,    20,
      63,    -1,    63,    21,    63,    -1,    63,    22,    63,    -1,
      63,    23,    63,    -1,    63,    14,    63,    -1,    63,    15,
      63,    -1,    63,    16,    63,    -1,    63,    17,    63,    -1,
      64,    -1,     3,    -1,     5,    -1,     6,    -1,     7,    -1,
      35,    36,     4,    30,    66,    28,    70,    31,    -1,    35,
      36,     4,    30,    66,    31,    -1,    66,    28,    67,    -1,
      67,    -1,     4,    68,    69,    -1,     4,    68,    -1,     8,
      -1,     9,    -1,    10,    -1,    37,    30,    48,    64,    11,
      64,    31,    -1,    38,    39,    30,    57,    31,    -1,    40,
      41,     4,    42,    30,    72,    31,    -1,    72,    28,    73,
      -1,    73,    -1,     5,    -1,    44,     4,    45,    75,    34,
      61,    -1,    44,     4,    45,    75,    -1,    75,    28,    76,
      -1,    76,    -1,     4,    43,     4,    -1,     4,    43,    64,
      -1,    46,    33,     4,    61,    -1,    46,    33,     4,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    77,    77,    78,    84,    85,    86,    87,    88,    89,
      93,   100,   105,   112,   121,   128,   134,   143,   148,   155,
     156,   160,   165,   172,   176,   180,   184,   188,   195,   199,
     203,   207,   211,   215,   222,   226,   230,   234,   238,   242,
     249,   250,   251,   255,   259,   266,   270,   277,   283,   290,
     291,   292,   296,   303,   307,   314,   318,   325,   329,   333,
     340,   344,   351,   355,   362,   366
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "DOT_NAME", "NAME", "TEXT_CONSTANT",
  "INT_CONSTANT", "FLOAT_CONSTANT", "INTEGER", "FLOAT", "TEXT", "AND",
  "OR", "NOT", "MULT", "PLUS", "MINUS", "DIV", "GE", "LT", "GT", "LE",
  "NE", "EQ", "SEMICOLON", "COMMIT", "ROLLBACK", "WITH", "COMMA", "AS",
  "ROUND_BRACKET_OPEN", "ROUND_BRACKET_CLOSE", "SELECT", "FROM", "WHERE",
  "CREATE", "TABLE", "RANGE", "PRIMARY", "KEY", "INSERT", "INTO", "VALUES",
  "ASSIGN", "UPDATE", "SET", "DELETE", "INFINITY", "BETWEEN", "REDC",
  "$accept", "query", "with_query", "table_as_list", "table_as",
  "table_desc", "select_query", "column_list", "column", "table_list",
  "table", "condition", "relex", "expression", "constant", "create_query",
  "column_desc_list", "column_desc", "type", "range", "constraint",
  "insert_query", "column_val_list", "column_val", "update_query",
  "update_list", "update", "delete_query", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    50,    51,    51,    51,    51,    51,    51,    51,    51,
      52,    53,    53,    54,    55,    56,    56,    57,    57,    58,
      58,    59,    59,    60,    61,    61,    61,    61,    62,    62,
      62,    62,    62,    62,    63,    63,    63,    63,    63,    63,
      64,    64,    64,    65,    65,    66,    66,    67,    67,    68,
      68,    68,    69,    70,    71,    72,    72,    73,    74,    74,
      75,    75,    76,    76,    77,    77
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     2,     2,     2,     2,     2,     2,     2,
       2,     3,     1,     3,     4,     4,     6,     3,     1,     1,
       1,     3,     1,     1,     3,     3,     2,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     8,     6,     3,     1,     3,     2,     1,
       1,     1,     7,     5,     7,     3,     1,     1,     6,     4,
       3,     1,     3,     3,     4,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     8,     9,     0,    10,
      12,     0,    19,    20,     0,    18,     0,     0,     0,     0,
       1,     0,     3,     4,     5,     6,     7,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,     0,    11,    13,
      17,    23,    15,    22,     0,     0,     0,    59,    61,    39,
      40,    41,    42,     0,    64,    27,     0,    38,     2,    14,
       0,     0,     0,     0,    46,     0,     0,     0,     0,    26,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    16,    21,    49,    50,    51,    48,     0,    44,
      57,     0,    56,    62,    63,    60,    58,    24,    25,    34,
      35,    36,    37,    28,    29,    30,    31,    32,    33,     0,
      47,     0,    45,     0,     0,    54,     0,     0,    43,    55,
       0,     0,     0,     0,     0,    53,     0,    52
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     9,    10,    19,    20,    21,    11,    24,    25,    52,
      53,    64,    65,    66,    67,    12,    73,    74,    97,   120,
     123,    13,   101,   102,    14,    57,    58,    15
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -76
static const yytype_int8 yypact[] =
{
     -22,   -13,   -12,    12,     3,    -6,    15,    31,    25,    87,
      64,    65,    66,    68,    69,    71,   -76,   -76,    61,   -76,
      70,    72,   -76,   -76,   -19,   -76,    92,    93,    54,    96,
     -76,   -22,   -76,   -76,   -76,   -76,   -76,     3,    12,    74,
       3,    98,    75,    62,    99,    14,    83,    29,   -76,   -76,
     -76,   -76,    76,    80,   105,    81,    77,     0,   -76,   -76,
     -76,   -76,   -76,    14,    67,   -76,    23,   -76,   -76,   -76,
      14,    98,    73,    44,   -76,   107,    43,    99,    14,   -76,
      14,    14,    26,    26,    26,    26,    26,    26,    26,    26,
      26,    26,    67,   -76,   -76,   -76,   -76,    78,    -2,   -76,
     -76,    45,   -76,   -76,   -76,   -76,    67,   -76,   102,   -76,
      57,    57,   -76,    37,    37,    37,    37,    37,    37,    84,
     -76,    82,   -76,    85,   107,   -76,    86,    88,   -76,   -76,
      79,     3,   106,    49,    79,   -76,    91,   -76
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -76,    94,   -76,   -76,    89,   -76,    90,   -37,    95,   -76,
      48,   -55,   -76,   -21,   -75,   -76,   -76,    28,   -76,   -76,
     -76,   -76,   -76,    -1,   -76,   -76,    47,   -76
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      47,   104,    72,     1,     2,     3,    22,    23,    79,    40,
       4,    16,    17,     5,    41,    92,    18,    59,     6,    60,
      61,    62,     7,   106,     8,   107,   108,    63,    77,    59,
      26,    60,    61,    62,    78,    28,   121,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,   103,    60,    61,
      62,    82,    83,    84,    85,   132,    27,    40,    29,   136,
      69,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    82,    98,   124,    85,    99,   125,    40,    80,    81,
     135,    94,    95,    96,    60,    61,    62,    30,    31,    32,
      33,    37,    34,    35,   133,    36,    42,    43,    38,    44,
      45,    39,    51,    56,    55,    54,     4,    68,    71,    72,
      70,    75,   100,    80,   126,   119,   128,   134,   131,    93,
      76,   127,   137,   129,   105,    46,   122,    48,     0,    49,
       0,     0,     0,     0,   130,    50
};

static const yytype_int16 yycheck[] =
{
      37,    76,     4,    25,    26,    27,     3,     4,    63,    28,
      32,    24,    24,    35,    33,    70,     4,     3,    40,     5,
       6,     7,    44,    78,    46,    80,    81,    13,    28,     3,
      36,     5,     6,     7,    34,     4,    38,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,     4,     5,     6,
       7,    14,    15,    16,    17,   130,    41,    28,    33,   134,
      31,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    14,    28,    28,    17,    31,    31,    28,    11,    12,
      31,     8,     9,    10,     5,     6,     7,     0,    24,    24,
      24,    30,    24,    24,   131,    24,     4,     4,    28,    45,
       4,    29,     4,     4,    42,    30,    32,    24,    28,     4,
      34,    30,     5,    11,    30,    37,    31,    11,    30,    71,
      43,    39,    31,   124,    77,    31,    98,    38,    -1,    39,
      -1,    -1,    -1,    -1,    48,    40
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    25,    26,    27,    32,    35,    40,    44,    46,    51,
      52,    56,    65,    71,    74,    77,    24,    24,     4,    53,
      54,    55,     3,     4,    57,    58,    36,    41,     4,    33,
       0,    24,    24,    24,    24,    24,    24,    30,    28,    29,
      28,    33,     4,     4,    45,     4,    51,    57,    54,    56,
      58,     4,    59,    60,    30,    42,     4,    75,    76,     3,
       5,     6,     7,    13,    61,    62,    63,    64,    24,    31,
      34,    28,     4,    66,    67,    30,    43,    28,    34,    61,
      11,    12,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    61,    60,     8,     9,    10,    68,    28,    31,
       5,    72,    73,     4,    64,    76,    61,    61,    61,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    37,
      69,    38,    67,    70,    28,    31,    30,    39,    31,    73,
      48,    30,    64,    57,    11,    31,    64,    31
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

<<<<<<< HEAD
#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256
=======
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
      yyerror (worker_id, YY_("syntax error: cannot back up")); \
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

>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce

/* YYLEX -- calling `yylex' with the right arguments.  */

<<<<<<< HEAD
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])

=======
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

<<<<<<< HEAD
# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (scanner);
=======
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
		  Type, Value, worker_id); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, int worker_id)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, worker_id)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    int worker_id;
#endif
{
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
  if (!yyvaluep)
    return;
  YYUSE (worker_id);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, int worker_id)
#else
static void
<<<<<<< HEAD
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner)
=======
yy_symbol_print (yyoutput, yytype, yyvaluep, worker_id)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    int worker_id;
#endif
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

<<<<<<< HEAD
  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp, scanner);
  YYFPRINTF (yyo, ")");
=======
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, worker_id);
  YYFPRINTF (yyoutput, ")");
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
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
<<<<<<< HEAD
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, yyscan_t scanner)
=======
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, int worker_id)
#else
static void
yy_reduce_print (yyvsp, yyrule, worker_id)
    YYSTYPE *yyvsp;
    int yyrule;
    int worker_id;
#endif
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
<<<<<<< HEAD
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner); \
} while (0)
=======
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , worker_id);
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, worker_id); \
} while (YYID (0))
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce

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
<<<<<<< HEAD
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t scanner)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
=======
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, int worker_id)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, worker_id)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    int worker_id;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (worker_id);

>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
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
int yyparse (int worker_id);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



<<<<<<< HEAD
=======
/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
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
<<<<<<< HEAD
yyparse (yyscan_t scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYPTRDIFF_T yystacksize;

=======
yyparse (int worker_id)
#else
int
yyparse (worker_id)
    int worker_id;
#endif
#endif
{
  
  int yystate;
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
<<<<<<< HEAD
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

<<<<<<< HEAD
#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))
=======
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
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

<<<<<<< HEAD
  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
<<<<<<< HEAD
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;
=======
  yychar = YYEMPTY;		/* Cause a token to be read.  */
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce

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
<<<<<<< HEAD
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
=======
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
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
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
<<<<<<< HEAD
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
=======
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
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;


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
<<<<<<< HEAD
      yychar = yylex (&yylval, &yylloc, scanner);
=======
      yychar = YYLEX;
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
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
<<<<<<< HEAD
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
<<<<<<< HEAD
  case 3:
#line 79 "parser.y"
    {   
        cout << "here" << endl;
        cout << *(int *)yyget_extra(scanner) << endl;
        results[*(int *)yyget_extra(scanner)] = (yyvsp[-1].table);
    }
#line 1655 "y.tab.c"
=======
        case 3:
#line 72 "parser.y"
    {
        results[worker_id] = (yyvsp[(1) - (2)].table);
    }
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 10:
#line 94 "parser.y"
    {
        checkerr(execute_create_temp(*((yyvsp[(2) - (2)].vec_table))));
    }
<<<<<<< HEAD
#line 1663 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 11:
#line 101 "parser.y"
    {
        (yyval.vec_table) = new table_list(1, (yyvsp[(1) - (3)].table)); 
        (yyval.vec_table)->push_back((yyvsp[(3) - (3)].table));
    }
<<<<<<< HEAD
#line 1672 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 12:
#line 106 "parser.y"
    {
        (yyval.vec_table) = new table_list(1, (yyvsp[(1) - (1)].table));
    }
<<<<<<< HEAD
#line 1680 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 13:
#line 113 "parser.y"
    {
        (yyval.table) = (yyvsp[(3) - (3)].table); 
        (yyval.table)->set_schema_columns((yyvsp[(1) - (3)].name_cols)->second);
        (yyval.table)->set_name((yyvsp[(1) - (3)].name_cols)->first);
    }
<<<<<<< HEAD
#line 1690 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 14:
#line 122 "parser.y"
    {
        (yyval.name_cols) = new pair<string, vector<string> > (*(yyvsp[(1) - (4)].str), *(yyvsp[(3) - (4)].vec_string));
    }
<<<<<<< HEAD
#line 1698 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 15:
#line 129 "parser.y"
    {
        Temp_Table *temp = new Temp_Table();
        checkerr(execute_select(temp, *(yyvsp[(4) - (4)].vec_string), *(yyvsp[(2) - (4)].vec_string)));
        (yyval.table) = temp;
    }
<<<<<<< HEAD
#line 1708 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 16:
#line 135 "parser.y"
    {
        Temp_Table *temp = new Temp_Table();
        checkerr(execute_select(temp, *(yyvsp[(4) - (6)].vec_string), *(yyvsp[(2) - (6)].vec_string), (yyvsp[(6) - (6)].logast)));
        (yyval.table) = temp;
    }
<<<<<<< HEAD
#line 1718 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 17:
#line 144 "parser.y"
    {
        (yyval.vec_string) = (yyvsp[(1) - (3)].vec_string);
        (yyval.vec_string)->push_back(*(yyvsp[(3) - (3)].str));
    }
<<<<<<< HEAD
#line 1727 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 18:
#line 149 "parser.y"
    {
        (yyval.vec_string) = new vector<string> (1, *(yyvsp[(1) - (1)].str));
    }
<<<<<<< HEAD
#line 1735 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 21:
#line 161 "parser.y"
    {
        (yyval.vec_string) = new vector<string> (1, *(yyvsp[(1) - (3)].str));
        (yyval.vec_string)->push_back(*(yyvsp[(3) - (3)].str));
    }
<<<<<<< HEAD
#line 1744 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 22:
#line 166 "parser.y"
    {
        (yyval.vec_string) = new vector<string> (1, *(yyvsp[(1) - (1)].str));
    }
<<<<<<< HEAD
#line 1752 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 24:
#line 177 "parser.y"
    {
        (yyval.logast) = new BinLogAST((yyvsp[(1) - (3)].logast), (yyvsp[(3) - (3)].logast), _AND);
    }
<<<<<<< HEAD
#line 1760 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 25:
#line 181 "parser.y"
    {
        (yyval.logast) = new BinLogAST((yyvsp[(1) - (3)].logast), (yyvsp[(3) - (3)].logast), _OR);
    }
<<<<<<< HEAD
#line 1768 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 26:
#line 185 "parser.y"
    {
        (yyval.logast) = new UnaryLogAST((yyvsp[(2) - (2)].logast));
    }
<<<<<<< HEAD
#line 1776 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 27:
#line 189 "parser.y"
    {
        (yyval.logast) = (CondAST *)(yyvsp[(1) - (1)].relast);
    }
<<<<<<< HEAD
#line 1784 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 28:
#line 196 "parser.y"
    {
        (yyval.relast) = new RelAST((yyvsp[(1) - (3)].exprast), (yyvsp[(3) - (3)].exprast), _GE);
    }
<<<<<<< HEAD
#line 1792 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 29:
#line 200 "parser.y"
    {
        (yyval.relast) = new RelAST((yyvsp[(1) - (3)].exprast), (yyvsp[(3) - (3)].exprast), _LT);
    }
<<<<<<< HEAD
#line 1800 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 30:
#line 204 "parser.y"
    {
        (yyval.relast) = new RelAST((yyvsp[(1) - (3)].exprast), (yyvsp[(3) - (3)].exprast), _GT);
    }
<<<<<<< HEAD
#line 1808 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 31:
#line 208 "parser.y"
    {
        (yyval.relast) = new RelAST((yyvsp[(1) - (3)].exprast), (yyvsp[(3) - (3)].exprast), _LE);
    }
<<<<<<< HEAD
#line 1816 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 32:
#line 212 "parser.y"
    {
        (yyval.relast) = new RelAST((yyvsp[(1) - (3)].exprast), (yyvsp[(3) - (3)].exprast), _NE);
    }
<<<<<<< HEAD
#line 1824 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 33:
#line 216 "parser.y"
    {
        (yyval.relast) = new RelAST((yyvsp[(1) - (3)].exprast), (yyvsp[(3) - (3)].exprast), _EQ);
    }
<<<<<<< HEAD
#line 1832 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 34:
#line 223 "parser.y"
    {
        (yyval.exprast) = new BinArithAST((yyvsp[(1) - (3)].exprast), (yyvsp[(3) - (3)].exprast), _MULT);
    }
<<<<<<< HEAD
#line 1840 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 35:
#line 227 "parser.y"
    {
        (yyval.exprast) = new BinArithAST((yyvsp[(1) - (3)].exprast), (yyvsp[(3) - (3)].exprast), _PLUS);
    }
<<<<<<< HEAD
#line 1848 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 36:
#line 231 "parser.y"
    {
        (yyval.exprast) = new BinArithAST((yyvsp[(1) - (3)].exprast), (yyvsp[(3) - (3)].exprast), _MINUS);
    }
<<<<<<< HEAD
#line 1856 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 37:
#line 235 "parser.y"
    {
        (yyval.exprast) = new BinArithAST((yyvsp[(1) - (3)].exprast), (yyvsp[(3) - (3)].exprast), _DIV);
    }
<<<<<<< HEAD
#line 1864 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 38:
#line 239 "parser.y"
    {
        (yyval.exprast) = new ConstAST((yyvsp[(1) - (1)].constant));
    }
<<<<<<< HEAD
#line 1872 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 39:
#line 243 "parser.y"
    {
        (yyval.exprast) = new ColAST(*(yyvsp[(1) - (1)].str));
    }
<<<<<<< HEAD
#line 1880 "y.tab.c"
    break;

  case 40:
#line 249 "parser.y"
                    {(yyval.constant) = new Constant(*(yyvsp[0].str), _TEXT);}
#line 1886 "y.tab.c"
    break;

  case 41:
#line 250 "parser.y"
                   {(yyval.constant) = new Constant(*(yyvsp[0].str), _INT);}
#line 1892 "y.tab.c"
    break;

  case 42:
#line 251 "parser.y"
                     {(yyval.constant) = new Constant(*(yyvsp[0].str), _FLOAT);}
#line 1898 "y.tab.c"
=======
    break;

  case 40:
#line 240 "parser.y"
    {(yyval.constant) = new Constant(*(yyvsp[(1) - (1)].str), _TEXT);}
    break;

  case 41:
#line 241 "parser.y"
    {(yyval.constant) = new Constant(*(yyvsp[(1) - (1)].str), _INT);}
    break;

  case 42:
#line 242 "parser.y"
    {(yyval.constant) = new Constant(*(yyvsp[(1) - (1)].str), _FLOAT);}
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 43:
#line 256 "parser.y"
    {
        checkerr(execute_create(*(yyvsp[(3) - (8)].str), *(yyvsp[(5) - (8)].vec_col_desc), *(yyvsp[(7) - (8)].vec_string)));
    }
<<<<<<< HEAD
#line 1906 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 44:
#line 260 "parser.y"
    {
        checkerr(execute_create(*(yyvsp[(3) - (6)].str), *(yyvsp[(5) - (6)].vec_col_desc)));
    }
<<<<<<< HEAD
#line 1914 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 45:
#line 267 "parser.y"
    {
        (yyval.vec_col_desc)->push_back((yyvsp[(3) - (3)].col_desc));
    }
<<<<<<< HEAD
#line 1922 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 46:
#line 271 "parser.y"
    {
        (yyval.vec_col_desc) = new vector<ColumnDesc*> (1, (yyvsp[(1) - (1)].col_desc));
    }
<<<<<<< HEAD
#line 1930 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 47:
#line 278 "parser.y"
    {
        if((yyvsp[(2) - (3)].int_val) == _TEXT)
            return -1;
        (yyval.col_desc) = new ColumnDesc(&(*(yyvsp[(1) - (3)].str))[0], (yyvsp[(2) - (3)].int_val), (yyvsp[(3) - (3)].range)->lower_bound, (yyvsp[(3) - (3)].range)->upper_bound);
    }
<<<<<<< HEAD
#line 1940 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 48:
#line 284 "parser.y"
    {
        (yyval.col_desc) = new ColumnDesc(&(*(yyvsp[(1) - (2)].str))[0], (yyvsp[(2) - (2)].int_val));
    }
<<<<<<< HEAD
#line 1948 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 52:
#line 297 "parser.y"
    {
        (yyval.range) = new Range(*(yyvsp[(4) - (7)].constant), *(yyvsp[(6) - (7)].constant));
    }
<<<<<<< HEAD
#line 1956 "y.tab.c"
    break;

  case 53:
#line 303 "parser.y"
                                                                     {(yyval.vec_string) = (yyvsp[-1].vec_string);}
#line 1962 "y.tab.c"
=======
    break;

  case 53:
#line 294 "parser.y"
    {(yyval.vec_string) = (yyvsp[(4) - (5)].vec_string);}
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 54:
#line 308 "parser.y"
    {
        checkerr(execute_insert(*(yyvsp[(3) - (7)].str), *(yyvsp[(6) - (7)].vec_string)));
    }
<<<<<<< HEAD
#line 1970 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 55:
#line 315 "parser.y"
    {
        (yyval.vec_string)->push_back(*(yyvsp[(3) - (3)].str));
    }
<<<<<<< HEAD
#line 1978 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 56:
#line 319 "parser.y"
    {
        (yyval.vec_string) = new vector<string> (1, *(yyvsp[(1) - (1)].str));
    }
<<<<<<< HEAD
#line 1986 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 58:
#line 330 "parser.y"
    {
        checkerr(execute_update(*(yyvsp[(2) - (6)].str), *(yyvsp[(4) - (6)].vec_update_pair), (yyvsp[(6) - (6)].logast)));
    }
<<<<<<< HEAD
#line 1994 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 59:
#line 334 "parser.y"
    {
        checkerr(execute_update(*(yyvsp[(2) - (4)].str), *(yyvsp[(4) - (4)].vec_update_pair)));
    }
<<<<<<< HEAD
#line 2002 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 60:
#line 341 "parser.y"
    {
        (yyval.vec_update_pair)->push_back((yyvsp[(3) - (3)].update_pair));
    }
<<<<<<< HEAD
#line 2010 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 61:
#line 345 "parser.y"
    {
        (yyval.vec_update_pair) = new vector<Update_Pair*> (1, (yyvsp[(1) - (1)].update_pair));
    }
<<<<<<< HEAD
#line 2018 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 62:
#line 352 "parser.y"
    {
        (yyval.update_pair) = new Update_Pair(*(yyvsp[(1) - (3)].str), *(yyvsp[(3) - (3)].str));
    }
<<<<<<< HEAD
#line 2026 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 63:
#line 356 "parser.y"
    {
        (yyval.update_pair) = new Update_Pair(*(yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].constant)->val);
    }
<<<<<<< HEAD
#line 2034 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 64:
#line 363 "parser.y"
    {
        checkerr(execute_delete(*(yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].logast)));
    }
<<<<<<< HEAD
#line 2042 "y.tab.c"
=======
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
    break;

  case 65:
#line 367 "parser.y"
    {
        checkerr(execute_delete(*(yyvsp[(3) - (3)].str)));
    }
<<<<<<< HEAD
#line 2050 "y.tab.c"
    break;


#line 2054 "y.tab.c"

=======
    break;


/* Line 1267 of yacc.c.  */
#line 1915 "y.tab.c"
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;


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
      yyerror (&yylloc, scanner, YY_("syntax error"));
#else
      {
<<<<<<< HEAD
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, scanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
=======
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
	    yyerror (worker_id, yymsg);
	  }
	else
	  {
	    yyerror (worker_id, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
      }
#endif
    }

  yyerror_range[1] = yylloc;

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
<<<<<<< HEAD
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, scanner);
          yychar = YYEMPTY;
        }
=======
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, worker_id);
	  yychar = YYEMPTY;
	}
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
<<<<<<< HEAD
                  yystos[yystate], yyvsp, yylsp, scanner);
=======
		  yystos[yystate], yyvsp, worker_id);
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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
  yyerror (&yylloc, scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
<<<<<<< HEAD
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
=======
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, worker_id);
  /* Do not reclaim the symbols of the rule which action triggered
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
<<<<<<< HEAD
                  yystos[+*yyssp], yyvsp, yylsp, scanner);
=======
		  yystos[*yyssp], yyvsp, worker_id);
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce
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
<<<<<<< HEAD
#line 372 "parser.y"
=======


#line 363 "parser.y"
>>>>>>> 0ccd54bbe6a8a034f483279a3a68e01b8a6153ce


void yyerror(YYLTYPE* yyllocp, yyscan_t unused, const char* msg)
{
    // cerr<<err<<endl;
    return;
}

void checkerr(int err_code) {
    switch(err_code) {
        case C_OK:
            cout<<"successfully terminated"<<endl;
            break;
        case C_TRUE:
            cout<<"true output"<<endl;
            break;
        case C_FALSE:
            cout<<"false output"<<endl;
            break;
        case C_ERROR:
            cout<<"error"<<endl;
            break;
        case C_TABLE_NOT_FOUND:
            cout<<"table not found"<<endl;
            break;
        case C_FIELD_NOT_FOUND:
            cout<<"field not found"<<endl;
            break;
    }
}

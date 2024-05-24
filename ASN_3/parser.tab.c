/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

#include <cstdio>
#include <iostream>
#include <unistd.h>
#include "treeUtils.h"
#include "treeNodes.h"
#include "scanType.h"
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

int numErrors;
int numWarnings;
extern int line;


TreeNode *addSibling(TreeNode *t, TreeNode *s) {
   TreeNode *newNode;
   if (s == NULL) {
      printf("Stop! S can't be null");
      exit(1);
   }
   else if (t == NULL) {
      return s;
   }
   else {
      newNode = t;
      while (newNode->sibling != NULL) {
         newNode = newNode->sibling;
      }
      newNode->sibling = s;
      return t;
   }
}

void setType(TreeNode *t, ExpType type, bool isStatic) {
      while (t != NULL) {
         t->type = type;
         t->isStatic = isStatic;
         t = t->sibling;
      }
}


TreeNode *syntaxTree;

void yyerror(const char *msg);

void printDebug(const char *msg) {
   if (1==0) {
      printf("%s\n", msg);
   }
}


#line 124 "parser.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NEQ = 258,
    EQ = 259,
    LEQ = 260,
    GEQ = 261,
    AND = 262,
    OR = 263,
    NOT = 264,
    PRECOMPILER = 265,
    NUMCONST = 266,
    CHARCONST = 267,
    STRINGCONST = 268,
    BOOLCONST = 269,
    ERROR = 270,
    ID = 271,
    IF = 272,
    THEN = 273,
    ELSE = 274,
    INT = 275,
    CHAR = 276,
    BOOL = 277,
    DEC = 278,
    INC = 279,
    WHILE = 280,
    FOR = 281,
    TO = 282,
    BY = 283,
    DO = 284,
    RETURN = 285,
    BREAK = 286,
    STATIC = 287,
    MIN = 288,
    MAX = 289,
    MULASS = 290,
    DIVASS = 291,
    ADDASS = 292,
    SUBASS = 293
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 59 "parser.y" /* yacc.c:355  */

   TokenData *tinfo;
   TreeNode *tree;
   ExpType type;

#line 209 "parser.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 226 "parser.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   231

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  114
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  175

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    10,     2,     2,
      12,    13,     3,     4,    14,     5,     2,     6,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    20,    15,
       7,     8,     9,    11,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    16,     2,    19,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    17,     2,    18,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    87,    87,    90,    91,    92,    95,    96,    99,   100,
     103,   107,   108,   112,   113,   116,   117,   120,   121,   124,
     125,   126,   129,   130,   133,   134,   137,   138,   141,   144,
     145,   148,   149,   152,   153,   157,   158,   159,   160,   161,
     162,   163,   166,   167,   170,   171,   172,   173,   176,   177,
     180,   183,   184,   187,   188,   191,   192,   195,   198,   199,
     200,   201,   204,   205,   206,   207,   208,   211,   212,   215,
     216,   219,   220,   223,   224,   227,   228,   229,   230,   231,
     232,   235,   236,   239,   240,   243,   244,   247,   248,   251,
     252,   255,   256,   257,   260,   261,   264,   265,   266,   270,
     271,   274,   275,   278,   279,   280,   283,   286,   287,   290,
     291,   294,   295,   296,   297
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'*'", "'+'", "'-'", "'/'", "'<'", "'='",
  "'>'", "'%'", "'?'", "'('", "')'", "','", "';'", "'['", "'{'", "'}'",
  "']'", "':'", "NEQ", "EQ", "LEQ", "GEQ", "AND", "OR", "NOT",
  "PRECOMPILER", "NUMCONST", "CHARCONST", "STRINGCONST", "BOOLCONST",
  "ERROR", "ID", "IF", "THEN", "ELSE", "INT", "CHAR", "BOOL", "DEC", "INC",
  "WHILE", "FOR", "TO", "BY", "DO", "RETURN", "BREAK", "STATIC", "MIN",
  "MAX", "MULASS", "DIVASS", "ADDASS", "SUBASS", "$accept", "program",
  "precomList", "declList", "decl", "varDecl", "scopedVarDecl",
  "varDeclList", "varDeclInit", "varDeclId", "typeSpec", "funDecl",
  "parms", "parmList", "parmTypeList", "parmIdList", "parmId", "stmt",
  "matched", "iterRange", "unmatched", "expStmt", "compoundStmt",
  "localDecls", "stmtList", "returnStmt", "breakStmt", "exp", "assignop",
  "simpleExp", "andExp", "unaryRelExp", "relExp", "relop", "minmaxExp",
  "minmaxop", "sumExp", "sumop", "mulExp", "mulop", "unaryExp", "unaryop",
  "factor", "mutable", "immutable", "call", "args", "argList", "constant", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,    42,    43,    45,    47,    60,    61,    62,
      37,    63,    40,    41,    44,    59,    91,   123,   125,    93,
      58,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293
};
# endif

#define YYPACT_NINF -130

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-130)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -12,  -130,    43,    42,  -130,  -130,    25,  -130,  -130,  -130,
      52,  -130,  -130,    11,  -130,    55,  -130,     6,    36,  -130,
      45,    35,    66,    85,  -130,    55,    76,    73,  -130,   197,
      93,    97,  -130,   163,    55,   102,   101,   106,  -130,  -130,
    -130,  -130,   197,   197,  -130,  -130,  -130,  -130,    32,    98,
     100,  -130,  -130,    10,    20,    46,  -130,   118,  -130,  -130,
    -130,  -130,  -130,   107,    35,  -130,  -130,   197,   197,    94,
      72,   112,  -130,  -130,  -130,  -130,  -130,  -130,  -130,   117,
      98,    18,  -130,   163,  -130,   123,  -130,   197,   197,   197,
     197,  -130,  -130,  -130,  -130,  -130,  -130,  -130,  -130,   118,
     118,  -130,  -130,   118,  -130,  -130,  -130,   118,  -130,  -130,
    -130,    28,   -16,   -20,   133,  -130,   127,  -130,  -130,  -130,
    -130,  -130,  -130,  -130,  -130,  -130,   197,  -130,  -130,  -130,
     138,   139,   135,   100,  -130,    37,    20,    46,  -130,    55,
    -130,    73,   128,   163,   163,   197,  -130,  -130,  -130,   197,
    -130,    73,    83,  -130,  -130,  -130,   119,  -130,  -130,   114,
     -15,  -130,   103,  -130,   163,   163,   197,  -130,  -130,  -130,
    -130,  -130,   -18,   197,    98
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     4,     0,     0,     1,     3,     0,    19,    21,    20,
       2,     7,     8,     0,     9,    25,     6,    17,     0,    14,
      15,     0,     0,    24,    27,    25,     0,     0,    10,     0,
      31,    28,    30,     0,     0,     0,     0,    17,    13,    97,
      96,    98,     0,     0,   111,   112,   113,   114,   101,    16,
      68,    70,    72,    74,    82,    86,    90,     0,    95,   100,
      99,   104,   105,     0,     0,    49,    52,     0,     0,     0,
       0,     0,    23,    33,    34,    38,    39,    40,    41,     0,
      61,   100,    26,     0,    18,     0,    71,   108,     0,     0,
       0,    76,    77,    80,    79,    75,    78,    84,    83,     0,
       0,    87,    88,     0,    91,    92,    93,     0,    94,    32,
      29,    54,     0,     0,     0,    55,     0,    57,    48,    62,
      60,    59,    65,    66,    63,    64,     0,    22,   103,   110,
       0,   107,     0,    67,    69,    73,    81,    85,    89,     0,
      51,     0,     0,     0,     0,     0,    56,    58,   106,     0,
     102,     0,     0,    50,    53,    44,    33,    36,    46,     0,
       0,   109,     0,    12,     0,     0,     0,    11,    35,    45,
      37,    47,    42,     0,    43
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -130,  -130,  -130,  -130,   154,  -130,  -130,  -128,   140,  -130,
      -1,  -130,   144,  -130,   131,  -130,   109,   -79,  -129,  -130,
    -123,  -130,  -130,  -130,  -130,  -130,  -130,   -41,  -130,   -29,
      81,   -36,  -130,  -130,    80,  -130,    82,  -130,    78,  -130,
     -52,  -130,  -130,   -30,  -130,  -130,  -130,  -130,  -130
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,    10,    11,    12,   140,    18,    19,    20,
      21,    14,    22,    23,    24,    31,    32,    72,    73,   159,
      74,    75,    76,   111,   142,    77,    78,    79,   126,    80,
      50,    51,    52,    99,    53,   100,    54,   103,    55,   107,
      56,    57,    58,    59,    60,    61,   130,   131,    62
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      49,    85,    13,    81,   127,   108,    89,    86,    89,    13,
      89,    89,    81,   152,   156,   157,     1,    91,    25,    92,
     143,   158,    26,   162,   101,   102,   119,   144,   173,   116,
     166,    93,    94,    95,    96,   168,   170,    15,   112,   113,
      81,   169,   171,     4,    87,    17,   129,   132,    88,   104,
      27,    28,   105,    81,   134,   138,   106,    81,    81,   120,
     121,    97,    98,   154,   155,    29,     7,     8,     9,    30,
       5,   122,   123,   124,   125,    39,     6,    40,   139,    33,
       7,     8,     9,    41,    42,   147,     6,   115,    97,    98,
       7,     8,     9,     7,     8,     9,    81,    27,   163,    43,
      34,    44,    45,    46,    47,    36,    48,    37,   161,    63,
     141,    64,    81,    81,    81,    83,   160,    27,   167,    81,
      84,    39,    26,    40,    89,    90,   109,   117,   114,    41,
      42,    39,   118,    40,    81,    81,   128,   172,   151,    41,
      42,   145,   146,    65,   174,    66,   153,    44,    45,    46,
      47,   148,    48,   149,   150,    43,   164,    44,    45,    46,
      47,   165,    48,    67,    16,    82,    39,    38,    40,    35,
     133,    68,    69,   110,    41,    42,    70,    71,    65,   135,
      66,   137,   136,     0,     0,     0,     0,     0,     0,     0,
      43,     0,    44,    45,    46,    47,     0,    48,    67,     0,
      39,     0,    40,     0,     0,     0,    68,    69,    41,    42,
       0,    70,    71,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,     0,    44,    45,    46,    47,
       0,    48
};

static const yytype_int16 yycheck[] =
{
      29,    42,     3,    33,    83,    57,    26,    43,    26,    10,
      26,    26,    42,   141,   143,   144,    28,     7,    12,     9,
      36,   144,    16,   151,     4,     5,     8,    47,    46,    70,
      45,    21,    22,    23,    24,   164,   165,    12,    67,    68,
      70,   164,   165,     0,    12,    34,    87,    88,    16,     3,
      14,    15,     6,    83,    90,   107,    10,    87,    88,    41,
      42,    51,    52,   142,   143,    20,    38,    39,    40,    34,
      28,    53,    54,    55,    56,     3,    34,     5,    50,    13,
      38,    39,    40,    11,    12,   126,    34,    15,    51,    52,
      38,    39,    40,    38,    39,    40,   126,    14,    15,    27,
      15,    29,    30,    31,    32,    29,    34,    34,   149,    16,
     111,    14,   142,   143,   144,    13,   145,    14,    15,   149,
      19,     3,    16,     5,    26,    25,    19,    15,    34,    11,
      12,     3,    15,     5,   164,   165,    13,   166,   139,    11,
      12,     8,    15,    15,   173,    17,    18,    29,    30,    31,
      32,    13,    34,    14,    19,    27,    37,    29,    30,    31,
      32,    47,    34,    35,    10,    34,     3,    27,     5,    25,
      89,    43,    44,    64,    11,    12,    48,    49,    15,    99,
      17,   103,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    29,    30,    31,    32,    -1,    34,    35,    -1,
       3,    -1,     5,    -1,    -1,    -1,    43,    44,    11,    12,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    29,    30,    31,    32,
      -1,    34
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    28,    58,    59,     0,    28,    34,    38,    39,    40,
      60,    61,    62,    67,    68,    12,    61,    34,    64,    65,
      66,    67,    69,    70,    71,    12,    16,    14,    15,    20,
      34,    72,    73,    13,    15,    69,    29,    34,    65,     3,
       5,    11,    12,    27,    29,    30,    31,    32,    34,    86,
      87,    88,    89,    91,    93,    95,    97,    98,    99,   100,
     101,   102,   105,    16,    14,    15,    17,    35,    43,    44,
      48,    49,    74,    75,    77,    78,    79,    82,    83,    84,
      86,   100,    71,    13,    19,    84,    88,    12,    16,    26,
      25,     7,     9,    21,    22,    23,    24,    51,    52,    90,
      92,     4,     5,    94,     3,     6,    10,    96,    97,    19,
      73,    80,    86,    86,    34,    15,    84,    15,    15,     8,
      41,    42,    53,    54,    55,    56,    85,    74,    13,    84,
     103,   104,    84,    87,    88,    91,    93,    95,    97,    50,
      63,    67,    81,    36,    47,     8,    15,    84,    13,    14,
      19,    67,    64,    18,    74,    74,    75,    75,    77,    76,
      86,    84,    64,    15,    37,    47,    45,    15,    75,    77,
      75,    77,    86,    46,    86
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    57,    58,    59,    59,    59,    60,    60,    61,    61,
      62,    63,    63,    64,    64,    65,    65,    66,    66,    67,
      67,    67,    68,    68,    69,    69,    70,    70,    71,    72,
      72,    73,    73,    74,    74,    75,    75,    75,    75,    75,
      75,    75,    76,    76,    77,    77,    77,    77,    78,    78,
      79,    80,    80,    81,    81,    82,    82,    83,    84,    84,
      84,    84,    85,    85,    85,    85,    85,    86,    86,    87,
      87,    88,    88,    89,    89,    90,    90,    90,    90,    90,
      90,    91,    91,    92,    92,    93,    93,    94,    94,    95,
      95,    96,    96,    96,    97,    97,    98,    98,    98,    99,
      99,   100,   100,   101,   101,   101,   102,   103,   103,   104,
     104,   105,   105,   105,   105
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     1,     0,     2,     1,     1,     1,
       3,     4,     3,     3,     1,     1,     3,     1,     4,     1,
       1,     1,     6,     5,     1,     0,     3,     1,     2,     3,
       1,     1,     3,     1,     1,     6,     4,     6,     1,     1,
       1,     1,     3,     5,     4,     6,     4,     6,     2,     1,
       4,     2,     0,     2,     0,     2,     3,     2,     3,     2,
       2,     1,     1,     1,     1,     1,     1,     3,     1,     3,
       1,     2,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     1,     1,     4,     1,     0,     3,
       1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

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
#line 87 "parser.y" /* yacc.c:1646  */
    {syntaxTree = (yyvsp[0].tree); }
#line 1451 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 90 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = NULL; printf("%s\n", yylval.tinfo->tokenstr); }
#line 1457 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 91 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = NULL; printf("%s\n", yylval.tinfo->tokenstr); }
#line 1463 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 92 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = NULL; }
#line 1469 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 95 "parser.y" /* yacc.c:1646  */
    {/* addSibling*/ (yyval.tree) = addSibling((yyvsp[-1].tree), (yyvsp[0].tree));}
#line 1475 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 96 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = (yyvsp[0].tree); }
#line 1481 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 99 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = (yyvsp[0].tree); }
#line 1487 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 100 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = (yyvsp[0].tree); }
#line 1493 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 103 "parser.y" /* yacc.c:1646  */
    {/* DRBC Note: Be careful! And setType.*/  (yyval.tree) = (yyvsp[-1].tree); setType((yyvsp[-1].tree), (yyvsp[-2].type), 0); yyerrok;}
#line 1499 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 107 "parser.y" /* yacc.c:1646  */
    {/* DRBC Note: Be careful! And setType.*/}
#line 1505 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 108 "parser.y" /* yacc.c:1646  */
    {/* DRBC Note: Be careful! And setType.*/ (yyval.tree) = (yyvsp[-1].tree); setType((yyvsp[-1].tree), (yyvsp[-2].type), 0); /* IF SEGFAULTS, DELETE THIS */}
#line 1511 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 112 "parser.y" /* yacc.c:1646  */
    {/* addSibling */  (yyval.tree) = addSibling((yyval.tree), (yyvsp[0].tree));}
#line 1517 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 113 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = (yyvsp[0].tree); }
#line 1523 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 116 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = (yyvsp[0].tree); }
#line 1529 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 117 "parser.y" /* yacc.c:1646  */
    {/* DRBC Note: $$ = $1; if ($$ != NULL) $$->child[0] = $3; */ (yyval.tree) = (yyvsp[-2].tree); if ((yyval.tree) != NULL) (yyval.tree)->child[0] = (yyvsp[0].tree); }
#line 1535 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 120 "parser.y" /* yacc.c:1646  */
    {/* newDeclNode */ (yyval.tree) = newDeclNode(VarK, Void, (yyvsp[0].tinfo)); /* IF SEGFAULT, DELETE THIS */}
#line 1541 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 121 "parser.y" /* yacc.c:1646  */
    {/* newDeclNode  $$ = newDeclNode(VarK, Void, $1); $$->isArray = true; */ }
#line 1547 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 124 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = Integer;}
#line 1553 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 125 "parser.y" /* yacc.c:1646  */
    { (yyval.type) = Boolean;}
#line 1559 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 126 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = Char;}
#line 1565 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 129 "parser.y" /* yacc.c:1646  */
    {/* newDeclNode */ (yyval.tree) = newDeclNode(FuncK, (yyvsp[-5].type), (yyvsp[-4].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1571 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 130 "parser.y" /* yacc.c:1646  */
    {/* newDeclNode */ (yyval.tree) = newDeclNode(FuncK, Void, (yyvsp[-4].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1577 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 133 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = (yyvsp[0].tree); }
#line 1583 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 134 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = NULL;}
#line 1589 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 137 "parser.y" /* yacc.c:1646  */
    {/* addSibling */}
#line 1595 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 141 "parser.y" /* yacc.c:1646  */
    {/* DRBC Note: Be careful! And setType.*/}
#line 1601 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 144 "parser.y" /* yacc.c:1646  */
    {/* addSibling*/}
#line 1607 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 148 "parser.y" /* yacc.c:1646  */
    {/* newDeclNode*/}
#line 1613 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 149 "parser.y" /* yacc.c:1646  */
    {/* newDeclNode*/}
#line 1619 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 152 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1625 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 153 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1631 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 157 "parser.y" /* yacc.c:1646  */
    {/* newStmtNode*/}
#line 1637 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 158 "parser.y" /* yacc.c:1646  */
    {/* newStmtNode*/}
#line 1643 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 159 "parser.y" /* yacc.c:1646  */
    {/* newStmtNode(newDeclNode)*/}
#line 1649 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 160 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1655 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 161 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1661 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 162 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1667 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 163 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1673 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 166 "parser.y" /* yacc.c:1646  */
    {/* newStmtNode*/}
#line 1679 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 167 "parser.y" /* yacc.c:1646  */
    {/* newStmtNode*/}
#line 1685 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 170 "parser.y" /* yacc.c:1646  */
    {/* newStmtNode*/}
#line 1691 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 171 "parser.y" /* yacc.c:1646  */
    {/* newStmtNode*/}
#line 1697 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 172 "parser.y" /* yacc.c:1646  */
    {/* newStmtNode*/}
#line 1703 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 173 "parser.y" /* yacc.c:1646  */
    {/* newStmtNode(newDeclNode)*/}
#line 1709 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 176 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[-1].tree);}
#line 1715 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 177 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = NULL; }
#line 1721 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 180 "parser.y" /* yacc.c:1646  */
    {/* newStmtNode*/ (yyval.tree) = newStmtNode(CompoundK, (yyvsp[-3].tinfo), (yyvsp[-2].tree), (yyvsp[-1].tree));}
#line 1727 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 183 "parser.y" /* yacc.c:1646  */
    {/* addSibling*/ (yyval.tree) = addSibling((yyvsp[-1].tree), (yyvsp[0].tree));}
#line 1733 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 184 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = NULL; printDebug("localDecls empty");}
#line 1739 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 187 "parser.y" /* yacc.c:1646  */
    {/* addSibling*/ (yyval.tree) = addSibling((yyvsp[-1].tree), (yyvsp[0].tree));}
#line 1745 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 188 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = NULL; printDebug("stmtList  empty");}
#line 1751 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 191 "parser.y" /* yacc.c:1646  */
    {/* newStmtNode*/}
#line 1757 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 192 "parser.y" /* yacc.c:1646  */
    {/* newStmtNode*/}
#line 1763 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 195 "parser.y" /* yacc.c:1646  */
    {/* newStmtNode*/}
#line 1769 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 198 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/  (yyval.tree) = newExpNode(AssignK, (yyvsp[-1].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree)); printDebug("In the exp rule");}
#line 1775 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 199 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/}
#line 1781 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 200 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/}
#line 1787 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 201 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = (yyvsp[0].tree); }
#line 1793 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 204 "parser.y" /* yacc.c:1646  */
    { (yyval.tinfo) = (yyvsp[0].tinfo); printDebug("Top assignop");}
#line 1799 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 205 "parser.y" /* yacc.c:1646  */
    { (yyval.tinfo) = (yyvsp[0].tinfo); }
#line 1805 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 206 "parser.y" /* yacc.c:1646  */
    { (yyval.tinfo) = (yyvsp[0].tinfo); }
#line 1811 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 207 "parser.y" /* yacc.c:1646  */
    { (yyval.tinfo) = (yyvsp[0].tinfo); }
#line 1817 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 208 "parser.y" /* yacc.c:1646  */
    { (yyval.tinfo) = (yyvsp[0].tinfo); }
#line 1823 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 211 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = newExpNode(OpK, (yyvsp[-1].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1829 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 212 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = (yyvsp[0].tree); }
#line 1835 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 215 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/ (yyval.tree) = newExpNode(OpK, (yyvsp[-1].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));}
#line 1841 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 216 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = (yyvsp[0].tree);}
#line 1847 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 219 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = newExpNode(OpK, (yyvsp[-1].tinfo), (yyvsp[0].tree));}
#line 1853 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 220 "parser.y" /* yacc.c:1646  */
    {(yyval.tree) = (yyvsp[0].tree);}
#line 1859 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 223 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/ (yyval.tree) = newExpNode(AssignK, (yyvsp[-1].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));  /* IF SEGFAULT, $$ = NULL; */}
#line 1865 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 224 "parser.y" /* yacc.c:1646  */
    { /* $$ = $1; */ (yyval.tree) = (yyvsp[0].tree);  /* IF SEGFAULT, $$ = NULL; */}
#line 1871 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 235 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/  (yyval.tree) = newExpNode(AssignK, (yyvsp[-1].tinfo), (yyvsp[-2].tree), (yyvsp[0].tree));  /* IF SEGFAULT, CLEAR THIS */}
#line 1877 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 236 "parser.y" /* yacc.c:1646  */
    { (yyval.tree) = (yyvsp[0].tree); /* IF SEGFAULT, CLEAR THIS */}
#line 1883 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 243 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/}
#line 1889 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 251 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/}
#line 1895 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 260 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/}
#line 1901 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 264 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/}
#line 1907 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 265 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/}
#line 1913 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 266 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/}
#line 1919 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 274 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/ (yyval.tree) = newExpNode(IdK, (yyvsp[0].tinfo));}
#line 1925 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 275 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/}
#line 1931 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 278 "parser.y" /* yacc.c:1646  */
    {/* DRBC Note: Be careful!*/}
#line 1937 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 283 "parser.y" /* yacc.c:1646  */
    {/*newExpNode*/}
#line 1943 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 287 "parser.y" /* yacc.c:1646  */
    { /* NULL;*/}
#line 1949 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 290 "parser.y" /* yacc.c:1646  */
    {/* addSibling*/}
#line 1955 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 294 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/ (yyval.tree) = newExpNode(ConstantK, (yyvsp[0].tinfo));}
#line 1961 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 295 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/}
#line 1967 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 296 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/}
#line 1973 "parser.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 297 "parser.y" /* yacc.c:1646  */
    {/* newExpNode*/}
#line 1979 "parser.tab.c" /* yacc.c:1646  */
    break;


#line 1983 "parser.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 300 "parser.y" /* yacc.c:1906  */


void yyerror (const char *msg)
{ 
   cout << "Error: " <<  msg << endl;
}

int main(int argc, char **argv) {
   int option, index;
   numErrors = 0;
   char *file = NULL;
   extern FILE *yyin;
   while ((option = getopt (argc, argv, "")) != -1)
      switch (option)
      {
      default:
         ;
      }
   if ( optind == argc ) yyparse();
   for (index = optind; index < argc; index++) 
   {
      yyin = fopen (argv[index], "r");
      yyparse();
      fclose (yyin);
   }
   if (numErrors == 0) {
      printTree(stdout, syntaxTree);
   }
   printf("Number of warnings: %d\n", numWarnings);
   printf("Number of errors: %d\n", numErrors);
   return 0;
}


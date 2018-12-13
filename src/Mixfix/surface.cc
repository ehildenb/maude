/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 27 "surface.yy" /* yacc.c:339  */

#include <string>
#include <stack>

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//	core class definitions
#include "lineNumber.hh"

//	front end class definitions
#include "token.hh"
#include "renaming.hh"
#include "syntacticView.hh"
#include "moduleExpression.hh"
#include "fileTable.hh"
#include "directoryManager.hh"
#include "syntacticPreModule.hh"
#include "visibleModule.hh"  // HACK
#include "userLevelRewritingContext.hh"
#include "interpreter.hh"

#include "global.hh"
#define clear()			tokenSequence.clear();
#define store(token)		tokenSequence.append(token)
#define fragClear()		fragments.contractTo(0);
#define fragStore(token)	fragments.append(token)
//#define YYPARSE_PARAM	parseResult
//#define PARSE_RESULT	(*((UserLevelRewritingContext::ParseResult*) parseResult))
#define PARSE_RESULT	(*parseResult)

#define CM		interpreter.getCurrentModule()
#define CV		interpreter.getCurrentView()

#include "lexerAux.hh"
/*
void lexerInitialMode();
void lexerIdMode();
void lexerCmdMode();
void lexerFileNameMode();
void lexerStringMode();
void lexerLatexMode();
bool handleEof();
bool includeFile(const string& directory, const string& fileName, bool silent, int lineNr);
//void eatComment(bool firstNonWhite);
*/
Vector<Token> singleton(1);
Vector<Token> tokenSequence;
Vector<Token> lexerBubble;
Vector<Token> fragments;
Vector<Token> moduleExpr;
Vector<Token> opDescription;
stack<ModuleExpression*> moduleExpressions;
Renaming* currentRenaming = 0;
SyntaxContainer* currentSyntaxContainer = 0;
SyntaxContainer* oldSyntaxContainer = 0;

Int64 number;
Int64 number2;

static void yyerror(UserLevelRewritingContext::ParseResult* parseResult, char *s);

void cleanUpModuleExpression();
void cleanUpParser();
void missingSpace(const Token& token);

#line 141 "surface.c" /* yacc.c:339  */

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
   by #include "surface.h".  */
#ifndef YY_YY_SURFACE_H_INCLUDED
# define YY_YY_SURFACE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FILE_NAME_STRING = 258,
    UNINTERPRETED_STRING = 259,
    LATEX_STRING = 260,
    KW_MOD = 261,
    KW_OMOD = 262,
    KW_VIEW = 263,
    KW_PARSE = 264,
    KW_NORMALIZE = 265,
    KW_REDUCE = 266,
    KW_REWRITE = 267,
    KW_LOOP = 268,
    KW_NARROW = 269,
    KW_XG_NARROW = 270,
    KW_MATCH = 271,
    KW_XMATCH = 272,
    KW_UNIFY = 273,
    KW_CHECK = 274,
    KW_GET = 275,
    KW_VARIANTS = 276,
    KW_VARIANT = 277,
    KW_EREWRITE = 278,
    KW_FREWRITE = 279,
    KW_SREWRITE = 280,
    KW_CONTINUE = 281,
    KW_SEARCH = 282,
    KW_SET = 283,
    KW_SHOW = 284,
    KW_ON = 285,
    KW_OFF = 286,
    KW_TRACE = 287,
    KW_BODY = 288,
    KW_BUILTIN = 289,
    KW_WHOLE = 290,
    KW_SELECT = 291,
    KW_DESELECT = 292,
    KW_CONDITION = 293,
    KW_SUBSTITUTION = 294,
    KW_PRINT = 295,
    KW_GRAPH = 296,
    KW_MIXFIX = 297,
    KW_FLAT = 298,
    KW_ATTRIBUTE = 299,
    KW_NEWLINE = 300,
    KW_WITH = 301,
    KW_PARENS = 302,
    KW_ALIASES = 303,
    KW_GC = 304,
    KW_TIME = 305,
    KW_STATS = 306,
    KW_TIMING = 307,
    KW_CMD = 308,
    KW_BREAKDOWN = 309,
    KW_BREAK = 310,
    KW_PATH = 311,
    KW_MODULE = 312,
    KW_MODULES = 313,
    KW_VIEWS = 314,
    KW_ALL = 315,
    KW_SORTS = 316,
    KW_OPS2 = 317,
    KW_VARS = 318,
    KW_MBS = 319,
    KW_EQS = 320,
    KW_RLS = 321,
    KW_SUMMARY = 322,
    KW_KINDS = 323,
    KW_ADVISE = 324,
    KW_VERBOSE = 325,
    KW_DO = 326,
    KW_CLEAR = 327,
    KW_PROTECT = 328,
    KW_EXTEND = 329,
    KW_INCLUDE = 330,
    KW_EXCLUDE = 331,
    KW_CONCEAL = 332,
    KW_REVEAL = 333,
    KW_COMPILE = 334,
    KW_COUNT = 335,
    KW_DEBUG = 336,
    KW_IRREDUNDANT = 337,
    KW_RESUME = 338,
    KW_ABORT = 339,
    KW_STEP = 340,
    KW_WHERE = 341,
    KW_CREDUCE = 342,
    KW_SREDUCE = 343,
    KW_DUMP = 344,
    KW_PROFILE = 345,
    KW_NUMBER = 346,
    KW_RAT = 347,
    KW_COLOR = 348,
    SIMPLE_NUMBER = 349,
    KW_PWD = 350,
    KW_CD = 351,
    KW_PUSHD = 352,
    KW_POPD = 353,
    KW_LS = 354,
    KW_LOAD = 355,
    KW_SLOAD = 356,
    KW_QUIT = 357,
    KW_EOF = 358,
    KW_TEST = 359,
    KW_SMT_SEARCH = 360,
    KW_VU_NARROW = 361,
    KW_FVU_NARROW = 362,
    KW_ENDM = 363,
    KW_IMPORT = 364,
    KW_ENDV = 365,
    KW_SORT = 366,
    KW_SUBSORT = 367,
    KW_OP = 368,
    KW_OPS = 369,
    KW_MSGS = 370,
    KW_VAR = 371,
    KW_CLASS = 372,
    KW_SUBCLASS = 373,
    KW_MB = 374,
    KW_CMB = 375,
    KW_EQ = 376,
    KW_CEQ = 377,
    KW_RL = 378,
    KW_CRL = 379,
    KW_IS = 380,
    KW_FROM = 381,
    KW_ARROW = 382,
    KW_ARROW2 = 383,
    KW_PARTIAL = 384,
    KW_IF = 385,
    KW_LABEL = 386,
    KW_TO = 387,
    KW_COLON2 = 388,
    KW_ASSOC = 389,
    KW_COMM = 390,
    KW_ID = 391,
    KW_IDEM = 392,
    KW_ITER = 393,
    KW_LEFT = 394,
    KW_RIGHT = 395,
    KW_PREC = 396,
    KW_GATHER = 397,
    KW_METADATA = 398,
    KW_STRAT = 399,
    KW_POLY = 400,
    KW_MEMO = 401,
    KW_FROZEN = 402,
    KW_CTOR = 403,
    KW_LATEX = 404,
    KW_SPECIAL = 405,
    KW_CONFIG = 406,
    KW_OBJ = 407,
    KW_MSG = 408,
    KW_DITTO = 409,
    KW_FORMAT = 410,
    KW_ID_HOOK = 411,
    KW_OP_HOOK = 412,
    KW_TERM_HOOK = 413,
    KW_IN = 414,
    IDENTIFIER = 415,
    NUMERIC_ID = 416,
    ENDS_IN_DOT = 417,
    FORCE_LOOKAHEAD = 418
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 105 "surface.yy" /* yacc.c:355  */

  bool yyBool;
  Int64 yyInt64;
  const char* yyString;
  Token yyToken;
  ImportModule::ImportMode yyImportMode;
  Interpreter::Flags yyFlags;
  Interpreter::PrintFlags yyPrintFlags;
  Interpreter::SearchKind yySearchKind;

#line 356 "surface.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (UserLevelRewritingContext::ParseResult* parseResult);

#endif /* !YY_YY_SURFACE_H_INCLUDED  */

/* Copy the second part of user declarations.  */
#line 116 "surface.yy" /* yacc.c:358  */

int yylex(YYSTYPE* lvalp);

#line 373 "surface.c" /* yacc.c:358  */

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
#define YYFINAL  113
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2410

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  178
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  248
/* YYNRULES -- Number of rules.  */
#define YYNRULES  625
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  935

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   418

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     133,   134,   145,   144,   139,     2,   135,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   131,     2,
     136,   132,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   137,     2,   138,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   146,   140,   147,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   141,   142,   143,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   235,   235,   237,   242,   243,   244,   245,   251,   251,
     261,   261,   271,   271,   281,   285,   285,   296,   296,   307,
     318,   318,   323,   328,   361,   365,   369,   370,   371,   372,
     380,   389,   390,   400,   401,   404,   405,   406,   413,   422,
     422,   431,   434,   435,   442,   447,   441,   453,   454,   457,
     458,   461,   465,   469,   470,   469,   476,   476,   478,   484,
     484,   486,   489,   490,   493,   494,   494,   496,   496,   498,
     498,   505,   507,   513,   518,   505,   531,   532,   535,   540,
     543,   547,   548,   548,   550,   553,   554,   564,   572,   563,
     581,   580,   613,   614,   623,   623,   630,   632,   630,   645,
     646,   655,   656,   659,   660,   663,   671,   672,   680,   689,
     690,   693,   693,   696,   697,   700,   707,   707,   710,   710,
     713,   714,   713,   717,   719,   721,   722,   721,   725,   726,
     727,   725,   730,   731,   730,   734,   735,   736,   734,   739,
     740,   739,   743,   744,   745,   743,   748,   749,   748,   752,
     758,   757,   764,   764,   767,   778,   779,   782,   783,   786,
     791,   792,   795,   796,   799,   800,   803,   804,   805,   812,
     818,   821,   822,   827,   834,   841,   842,   845,   846,   849,
     850,   853,   858,   858,   865,   866,   869,   870,   873,   874,
     877,   881,   885,   891,   895,   899,   899,   901,   905,   909,
     910,   910,   912,   912,   914,   914,   916,   916,   918,   922,
     926,   931,   931,   933,   937,   941,   945,   949,   949,   951,
     952,   962,   963,   966,   967,   970,   971,   974,   975,   976,
     977,   983,   985,   991,   993,  1001,  1002,  1005,  1006,  1007,
    1007,  1014,  1026,  1027,  1027,  1031,  1031,  1033,  1039,  1039,
    1039,  1039,  1039,  1042,  1042,  1042,  1043,  1043,  1043,  1046,
    1046,  1046,  1046,  1047,  1047,  1047,  1047,  1047,  1047,  1050,
    1050,  1050,  1051,  1051,  1051,  1051,  1052,  1052,  1052,  1052,
    1052,  1055,  1055,  1061,  1061,  1061,  1061,  1061,  1061,  1061,
    1061,  1062,  1062,  1065,  1065,  1065,  1065,  1068,  1068,  1071,
    1071,  1071,  1071,  1071,  1071,  1072,  1072,  1072,  1073,  1073,
    1073,  1073,  1073,  1073,  1073,  1073,  1076,  1076,  1076,  1076,
    1076,  1076,  1076,  1079,  1079,  1079,  1082,  1082,  1082,  1082,
    1082,  1082,  1082,  1083,  1083,  1083,  1083,  1083,  1083,  1084,
    1084,  1084,  1084,  1085,  1085,  1085,  1085,  1086,  1086,  1086,
    1113,  1113,  1118,  1118,  1125,  1124,  1137,  1136,  1149,  1148,
    1161,  1160,  1173,  1172,  1185,  1184,  1198,  1197,  1211,  1210,
    1224,  1223,  1236,  1235,  1249,  1248,  1261,  1260,  1273,  1272,
    1286,  1285,  1297,  1302,  1301,  1318,  1317,  1328,  1334,  1334,
    1340,  1340,  1346,  1346,  1352,  1352,  1358,  1366,  1366,  1372,
    1372,  1378,  1378,  1384,  1384,  1390,  1394,  1398,  1398,  1404,
    1404,  1410,  1410,  1416,  1416,  1422,  1422,  1428,  1428,  1434,
    1434,  1440,  1440,  1446,  1450,  1454,  1458,  1458,  1467,  1471,
    1475,  1479,  1483,  1487,  1491,  1495,  1499,  1503,  1507,  1511,
    1515,  1519,  1520,  1519,  1525,  1526,  1525,  1531,  1535,  1539,
    1543,  1547,  1551,  1558,  1562,  1566,  1570,  1577,  1581,  1588,
    1588,  1595,  1596,  1597,  1598,  1599,  1600,  1601,  1602,  1603,
    1604,  1607,  1608,  1609,  1610,  1611,  1612,  1613,  1614,  1615,
    1616,  1617,  1620,  1621,  1624,  1625,  1628,  1629,  1632,  1633,
    1638,  1639,  1640,  1641,  1642,  1643,  1646,  1647,  1650,  1651,
    1654,  1655,  1658,  1659,  1662,  1663,  1664,  1673,  1674,  1674,
    1678,  1678,  1680,  1690,  1690,  1692,  1693,  1693,  1698,  1698,
    1700,  1700,  1702,  1706,  1706,  1708,  1708,  1710,  1723,  1723,
    1725,  1726,  1726,  1735,  1735,  1737,  1737,  1739,  1739,  1741,
    1749,  1749,  1751,  1751,  1753,  1753,  1755,  1763,  1763,  1765,
    1765,  1767,  1776,  1775,  1782,  1782,  1784,  1792,  1792,  1794,
    1794,  1796,  1804,  1804,  1806,  1806,  1808,  1811,  1811,  1813,
    1817,  1817,  1819,  1819,  1821,  1832,  1832,  1832,  1832,  1832,
    1832,  1832,  1835,  1835,  1835,  1835,  1835,  1835,  1839,  1839,
    1839,  1839,  1839,  1839,  1843,  1843,  1843,  1843,  1843,  1843,
    1847,  1847,  1847,  1847,  1847,  1851,  1851,  1851,  1851,  1851,
    1857,  1858,  1861,  1862,  1865,  1872,  1873,  1876,  1881,  1888,
    1888,  1888,  1888,  1888,  1888,  1888
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FILE_NAME_STRING",
  "UNINTERPRETED_STRING", "LATEX_STRING", "KW_MOD", "KW_OMOD", "KW_VIEW",
  "KW_PARSE", "KW_NORMALIZE", "KW_REDUCE", "KW_REWRITE", "KW_LOOP",
  "KW_NARROW", "KW_XG_NARROW", "KW_MATCH", "KW_XMATCH", "KW_UNIFY",
  "KW_CHECK", "KW_GET", "KW_VARIANTS", "KW_VARIANT", "KW_EREWRITE",
  "KW_FREWRITE", "KW_SREWRITE", "KW_CONTINUE", "KW_SEARCH", "KW_SET",
  "KW_SHOW", "KW_ON", "KW_OFF", "KW_TRACE", "KW_BODY", "KW_BUILTIN",
  "KW_WHOLE", "KW_SELECT", "KW_DESELECT", "KW_CONDITION",
  "KW_SUBSTITUTION", "KW_PRINT", "KW_GRAPH", "KW_MIXFIX", "KW_FLAT",
  "KW_ATTRIBUTE", "KW_NEWLINE", "KW_WITH", "KW_PARENS", "KW_ALIASES",
  "KW_GC", "KW_TIME", "KW_STATS", "KW_TIMING", "KW_CMD", "KW_BREAKDOWN",
  "KW_BREAK", "KW_PATH", "KW_MODULE", "KW_MODULES", "KW_VIEWS", "KW_ALL",
  "KW_SORTS", "KW_OPS2", "KW_VARS", "KW_MBS", "KW_EQS", "KW_RLS",
  "KW_SUMMARY", "KW_KINDS", "KW_ADVISE", "KW_VERBOSE", "KW_DO", "KW_CLEAR",
  "KW_PROTECT", "KW_EXTEND", "KW_INCLUDE", "KW_EXCLUDE", "KW_CONCEAL",
  "KW_REVEAL", "KW_COMPILE", "KW_COUNT", "KW_DEBUG", "KW_IRREDUNDANT",
  "KW_RESUME", "KW_ABORT", "KW_STEP", "KW_WHERE", "KW_CREDUCE",
  "KW_SREDUCE", "KW_DUMP", "KW_PROFILE", "KW_NUMBER", "KW_RAT", "KW_COLOR",
  "SIMPLE_NUMBER", "KW_PWD", "KW_CD", "KW_PUSHD", "KW_POPD", "KW_LS",
  "KW_LOAD", "KW_SLOAD", "KW_QUIT", "KW_EOF", "KW_TEST", "KW_SMT_SEARCH",
  "KW_VU_NARROW", "KW_FVU_NARROW", "KW_ENDM", "KW_IMPORT", "KW_ENDV",
  "KW_SORT", "KW_SUBSORT", "KW_OP", "KW_OPS", "KW_MSGS", "KW_VAR",
  "KW_CLASS", "KW_SUBCLASS", "KW_MB", "KW_CMB", "KW_EQ", "KW_CEQ", "KW_RL",
  "KW_CRL", "KW_IS", "KW_FROM", "KW_ARROW", "KW_ARROW2", "KW_PARTIAL",
  "KW_IF", "':'", "'='", "'('", "')'", "'.'", "'<'", "'['", "']'", "','",
  "'|'", "KW_LABEL", "KW_TO", "KW_COLON2", "'+'", "'*'", "'{'", "'}'",
  "KW_ASSOC", "KW_COMM", "KW_ID", "KW_IDEM", "KW_ITER", "KW_LEFT",
  "KW_RIGHT", "KW_PREC", "KW_GATHER", "KW_METADATA", "KW_STRAT", "KW_POLY",
  "KW_MEMO", "KW_FROZEN", "KW_CTOR", "KW_LATEX", "KW_SPECIAL", "KW_CONFIG",
  "KW_OBJ", "KW_MSG", "KW_DITTO", "KW_FORMAT", "KW_ID_HOOK", "KW_OP_HOOK",
  "KW_TERM_HOOK", "KW_IN", "IDENTIFIER", "NUMERIC_ID", "ENDS_IN_DOT",
  "FORCE_LOOKAHEAD", "$accept", "top", "item", "directive", "$@1", "$@2",
  "$@3", "$@4", "$@5", "$@6", "moduleExprDot", "moduleExpr", "moduleExpr2",
  "moduleExpr3", "renameExpr", "instantExpr", "$@7", "parenExpr",
  "argList", "renaming", "$@8", "$@9", "renaming2", "mappingList",
  "mapping", "$@10", "$@11", "fromSpec", "$@12", "toAttributes", "$@13",
  "toAttributeList", "toAttribute", "$@14", "$@15", "$@16", "view", "$@17",
  "$@18", "$@19", "$@20", "viewDecList", "skipStrayArrow",
  "viewDeclaration", "$@21", "sortDot", "viewEndOpMap", "$@22", "$@23",
  "$@24", "endBubble", "parenBubble", "$@25", "module", "$@26", "$@27",
  "dot", "parameters", "parameterList", "parameter", "colon2", "badType",
  "typeDot", "startModule", "decList", "declaration", "$@28", "$@29",
  "$@30", "$@31", "$@32", "$@33", "$@34", "$@35", "$@36", "$@37", "$@38",
  "$@39", "$@40", "$@41", "$@42", "$@43", "$@44", "$@45", "$@46", "$@47",
  "$@48", "$@49", "$@50", "classDef", "cPairList", "cPair", "varNameList",
  "opNameList", "simpleOpName", "domainRangeAttr", "skipStrayColon",
  "dra2", "rangeAttr", "typeAttr", "arrow", "typeList", "typeName", "$@51",
  "sortNames", "attributes", "attributeList", "idKeyword", "attribute",
  "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58", "identity",
  "idList", "hookList", "hook", "expectedIs", "expectedDot",
  "sortNameList", "subsortList", "$@59", "sortName", "sortNameFrag",
  "$@60", "sortNameFrags", "$@61", "token", "tokenBarDot", "tokenBarColon",
  "sortToken", "endsInDot", "inert", "identifier", "startKeyword",
  "startKeyword2", "midKeyword", "attrKeyword", "attrKeyword2", "command",
  "$@62", "$@63", "$@64", "$@65", "$@66", "$@67", "$@68", "$@69", "$@70",
  "$@71", "$@72", "$@73", "$@74", "$@75", "$@76", "$@77", "$@78", "$@79",
  "$@80", "$@81", "$@82", "$@83", "$@84", "$@85", "$@86", "$@87", "$@88",
  "$@89", "$@90", "$@91", "$@92", "$@93", "$@94", "$@95", "$@96", "$@97",
  "$@98", "$@99", "$@100", "$@101", "printOption", "traceOption",
  "polarity", "select", "exclude", "conceal", "search", "match",
  "optDebug", "optIrredundant", "optNumber", "importMode", "moduleAndTerm",
  "$@102", "inEnd", "$@103", "numberModuleTerm", "$@104", "$@105",
  "numberModuleTerm1", "$@106", "$@107", "numberModuleTerm2", "$@108",
  "$@109", "numbersModuleTerm", "$@110", "$@111", "numbersModuleTerm1",
  "$@112", "$@113", "$@114", "numbersModuleTerm2", "$@115", "$@116",
  "$@117", "numbersModuleTerm3", "$@118", "$@119", "numbersModuleTerm4",
  "$@120", "$@121", "numbersModuleTerm5", "$@122", "$@123",
  "numbersModuleTerm6", "$@124", "$@125", "miscEndBubble", "$@126",
  "initialEndBubble", "$@127", "$@128", "cTokenBarIn", "cTokenBarLeftIn",
  "cTokenBarDotNumber", "cTokenBarDotRight", "cTokenBarDotCommaNumber",
  "cTokenBarDotCommaRight", "opSelect", "endSelect", "badSelect",
  "cOpNameList", "cSimpleOpName", "cSimpleTokenBarDot", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,    58,    61,    40,    41,    46,    60,    91,    93,    44,
     124,   386,   387,   388,    43,    42,   123,   125,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   417,   418
};
# endif

#define YYPACT_NINF -796

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-796)))

#define YYTABLE_NINF -500

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     837,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,   303,   538,   104,  -796,    93,   270,   -14,  -796,   -64,
     -61,   -30,   -15,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,    90,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,   136,    -8,  1152,
     284,   284,   365,   284,    67,    57,   456,     5,   116,   281,
     281,   281,   -27,  -796,  -796,  -796,    77,   281,  -796,  -796,
    -796,   124,   -42,  -796,    42,    89,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,   -85,  -796,  -796,  -796,  -796,    69,  -796,  -796,
    -796,  -796,   284,   284,   -85,   199,   244,   252,   258,   286,
     284,   180,   296,  -796,  1152,   365,  -796,  -796,  -796,  -796,
     236,   302,  -796,  -796,  -796,   232,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,   279,   281,   281,   281,   281,   281,   281,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,   281,  -796,
    -796,  -796,   108,   312,  -796,  -796,  -796,  -796,  -796,   281,
     281,  -796,  -796,   202,   215,   221,   281,   281,   281,   281,
     248,   165,   -85,   -85,   250,   251,   294,   -85,  -796,  -796,
     -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,
    2234,  2234,  -796,  -796,  -796,  2234,  2234,   259,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,   284,   365,  -796,   343,  -796,   498,   498,   365,  -796,
     263,   498,   262,   -85,   -10,   -85,   508,   -85,   165,   281,
     281,   275,   276,   277,   278,   283,   285,   289,   281,   291,
    -796,  -796,   292,   295,  -796,  -796,  -796,   298,   299,   304,
     327,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,   328,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
     520,  -796,  -796,  -796,  -796,  -796,  -796,   262,  -796,  -796,
    -796,   365,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    1152,   288,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,   329,   330,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
     334,  -796,  -796,  -796,  -796,  -796,  -796,  -796,   281,  -796,
    -796,  -796,  -796,  -796,   313,   365,  -796,   529,   -85,     7,
    -796,   -75,   876,   -85,   -85,   737,   -85,   281,  -796,  -796,
    -796,   335,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  1152,  -796,  -796,  -796,
     876,   876,   293,   326,   331,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,   338,  -796,   332,  2152,  2161,   -85,  -796,   293,
     -67,   876,   287,   341,  -796,   284,   -85,  -796,   348,  -796,
     945,  -796,  -796,  -796,  1221,  1221,  1428,  1152,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,   876,  -796,  -796,  1152,  -796,  -796,
    -796,   349,   135,   201,   100,  -796,   342,   120,   114,   178,
     350,   352,   226,   235,   246,   256,  -796,  2110,   345,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  1014,  -796,  -796,
    -796,  -796,  -796,  1083,  1290,  -796,  -796,  2110,   347,   357,
     369,   373,   378,   379,   366,  2170,   -85,   284,  2179,   -85,
     293,    11,    29,  -796,   945,  -796,  -796,  -796,  -796,  -796,
    -796,  1635,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  1566,  -796,   362,  -796,  -796,  -796,  -796,   642,
    -796,   642,   383,  -796,   374,  1566,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,   313,  -796,  2110,  -796,  1152,  -796,
     377,  -796,  1152,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,   642,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  1704,  -796,  -796,  -796,  -796,  1704,  1842,
     382,  -796,   -85,   389,   -85,   396,   -85,   397,   642,   284,
     -85,  2225,   -85,  -796,   386,   399,   390,   401,   -52,  -796,
    1976,  2110,  -796,  2110,  -796,  -796,   394,  1497,  -796,  -796,
     350,  -796,  -796,   398,  -796,   402,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,    18,  2110,  -796,   403,  1152,
    -796,  -796,  -796,   362,    81,   229,  -796,   797,   350,  -796,
    -796,  -796,   166,  -796,  -796,  -796,  1842,  1152,   -85,   -85,
     -85,   284,   -85,   408,  -796,  2110,  -796,  1428,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  2110,  -796,  -796,  -796,
    -796,  -796,   400,   404,   381,   414,   384,   418,   419,  -796,
     420,  -796,   424,   427,  -796,  -796,  -796,  -796,   428,  2228,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,   421,   -39,  1359,  1909,   425,  2110,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,   234,  -796,
    -796,  -796,   359,  1773,  -796,  -796,  -796,   383,  2043,  -796,
    -796,   362,   392,   392,   392,   392,   559,  1152,  1152,  1152,
      80,  -796,   392,  -796,  -796,  -796,  -796,   350,  -796,   -85,
    1704,  -796,   129,  -796,   -90,   -74,    10,    35,   433,   435,
     435,   435,  -796,  -796,    38,  -796,  1909,  -796,  -796,   405,
     437,   438,   440,   119,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  2043,  -796,  -796,  -796,  -796,
    -796,  -796,   432,   392,   570,   392,  -796,    39,   442,    41,
     -85,  -796,  -796,  -796,  -796
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,   459,   111,   112,    71,   354,   385,   497,   496,   376,
     370,     0,     0,     0,   350,     0,     0,     0,   498,     0,
       0,     0,     0,   356,   358,   352,    14,    15,    17,    19,
      20,    10,    12,    22,    23,   383,    94,     8,     0,     2,
       6,     5,   387,     4,    96,     7,   374,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   471,     0,     0,     0,
       0,     0,     0,   504,   505,   506,     0,     0,   441,   397,
     403,     0,     0,   399,     0,     0,   401,   407,   409,   411,
     413,   415,   417,   419,   421,   426,   484,   485,   487,   486,
     388,   390,     0,   488,   489,   394,   392,     0,   453,   454,
     455,   456,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     0,     0,   360,   362,   490,   491,
     501,     0,   364,   366,   368,   503,   492,   493,   494,   495,
     372,   460,   314,   299,   315,   300,   301,   302,   303,   305,
     304,   306,   307,   308,   309,   310,   311,   312,   313,   292,
     291,   318,   321,   319,   322,   317,   320,   252,   316,   323,
     324,   295,   288,   290,   296,   289,   286,   287,   284,   285,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   342,
     335,   336,   337,   341,   338,   339,   340,   343,   344,   297,
     345,   346,   347,   348,   349,   283,   294,    72,   293,   248,
     249,   298,   250,   251,   325,   579,   570,   580,   577,   578,
     581,   572,   575,   576,   574,   355,   507,   508,   386,   585,
     586,   513,   584,   587,   582,   583,   377,   515,   516,   371,
     444,     0,     0,     0,     0,     0,     0,     0,   479,   480,
     481,   473,   475,   472,   474,   476,   477,   478,     0,   465,
     461,   462,     0,     0,   466,   467,   468,   469,   470,     0,
       0,   482,   483,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   405,   406,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,   351,     0,     0,     0,   357,   359,
     353,    16,    18,    21,    11,    13,   384,    95,     9,    97,
     375,     0,     0,   500,     0,   378,     0,     0,     0,   502,
       0,     0,   102,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     464,   463,     0,     0,   458,   440,   447,     0,     0,     0,
       0,   451,   624,   621,   622,   625,   623,   619,   620,   442,
     398,   404,   425,   423,     0,   400,   402,   408,   410,   412,
     414,   416,   418,   420,   422,   427,   614,   618,   389,   611,
       0,   616,   617,   391,   395,   393,   396,   102,   361,   363,
     380,     0,   528,   365,   530,   531,   367,   369,   382,   373,
       0,     0,   571,   510,   512,   573,   509,   592,   567,   589,
     590,   593,   591,   588,   518,   569,   514,   522,   520,   517,
     445,     0,     0,   435,   429,   431,   434,   432,   428,   439,
       0,   437,   436,   457,   449,   452,   448,   450,     0,   424,
     612,   610,   613,   615,   232,     0,   379,     0,     0,     0,
     104,     0,     0,     0,     0,     0,     0,     0,   430,   433,
     438,     0,   231,   114,   381,   604,   601,   602,   535,   603,
     600,   533,   529,   539,   537,   532,     0,   101,   107,   106,
       0,     0,    73,    31,    33,    34,    36,    35,    37,   511,
     568,   598,   596,   523,   599,   597,   594,   595,   519,   527,
     525,   521,     0,   443,     0,     0,     0,     0,   103,   105,
       0,     0,     0,     0,    39,     0,     0,   446,     0,    98,
       0,   116,   118,   120,     0,     0,     0,     0,   152,   125,
     128,   132,   135,   139,   142,   146,   113,   557,   536,   561,
     559,   609,   607,   540,   542,   608,   605,   606,   534,   546,
     544,   538,    41,    32,     0,    44,    38,     0,   524,   526,
     154,   252,   254,   255,    30,   115,     0,   234,   234,   234,
     234,     0,   253,   256,   257,   258,   236,     0,     0,   264,
     267,   265,   268,   266,   262,   263,   165,     0,   163,   164,
     259,   260,   261,     0,     0,   161,   150,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      74,     0,     0,    43,     0,   233,    27,    28,    26,    24,
      25,     0,   278,   276,   277,   272,   273,   279,   280,   274,
     275,   269,     0,   239,   241,   242,   270,   271,   121,     0,
     162,     0,    79,   160,     0,     0,   126,   129,   133,   136,
     140,   143,   147,   562,   558,   566,   564,   560,   541,   547,
     543,   551,   549,   545,   232,    48,     0,    53,     0,    45,
      47,    50,     0,    40,    29,    32,    99,   100,   117,   235,
     238,   119,   237,   236,   243,     0,   177,   178,   182,   108,
     168,   123,   167,     0,   180,   181,   149,    78,     0,   155,
       0,   153,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    77,     0,    58,     0,     0,     0,    42,
       0,     0,   122,     0,   176,   174,   187,   170,   110,   124,
     234,   254,   255,   156,   157,     0,   253,   256,   257,   258,
     151,   127,   130,   134,   137,   141,   144,   148,   563,   565,
     552,   548,   556,   554,   550,     0,     0,    56,     0,     0,
      46,    49,   240,   247,     0,     0,   185,     0,   234,   169,
     172,   173,     0,   166,   179,   109,     0,     0,     0,     0,
       0,     0,     0,     0,    75,     0,    82,     0,    76,    51,
     180,    54,    52,   245,   244,   183,     0,   193,   194,   190,
     197,   198,     0,     0,     0,     0,     0,     0,     0,   208,
     210,   209,     0,     0,   213,   214,   215,   220,     0,     0,
     195,   189,   175,   171,   158,   159,   131,   138,   145,   553,
     555,    84,     0,     0,     0,     0,    61,     0,   184,   191,
     192,   199,   200,   216,   204,   206,   211,   217,     0,   202,
     186,   188,   222,     0,    87,    90,    83,    79,     0,    59,
      55,   246,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   226,     0,   221,   196,    86,    80,   234,   180,     0,
       0,    57,     0,   224,     0,     0,     0,     0,     0,   227,
       0,     0,   219,   225,     0,    85,     0,    91,    81,     0,
       0,     0,     0,     0,    63,   201,   223,   205,   207,   212,
     218,   228,   229,   230,   203,     0,    64,    65,    69,    67,
      60,    62,     0,     0,     0,     0,    88,     0,     0,     0,
       0,    66,    70,    68,    89
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
     -37,  -447,  -796,  -796,  -479,  -478,  -796,  -477,  -796,  -796,
    -796,  -796,  -796,  -796,  -140,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -323,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -276,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
     -91,     0,  -796,  -796,  -796,  -796,  -579,   195,  -796,   107,
    -796,  -670,  -296,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -191,  -201,    63,  -438,  -604,  -796,  -796,  -183,  -796,
    -795,  -760,  -610,  -796,  -796,  -796,  -796,  -796,  -229,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -536,  -796,  -278,
     -57,  -552,   -73,    12,  -796,  -502,  -714,  -796,  -796,  -796,
     -34,  -663,  -518,  -796,  -796,  -499,  -515,   -45,  -796,  -498,
    -500,  -324,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,   -29,   592,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,   -48,  -796,  -796,  -796,  -101,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -134,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,  -796,
    -796,  -420,  -796,   -50,  -796,  -796,  -796,    32,  -476,  -599,
    -796,  -796,   -69,  -796,   237,  -796,   238,  -226
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    38,    39,    40,   112,   108,   109,   105,   106,   107,
     565,   482,   483,   484,   485,   486,   557,   487,   612,   556,
     611,   717,   669,   670,   671,   715,   836,   758,   790,   860,
     882,   903,   904,   923,   925,   924,    41,    49,   322,   512,
     664,   755,   698,   788,   833,   876,   856,   878,   930,   879,
     415,   377,   111,    43,   114,   387,   678,   401,   449,   450,
     480,   690,   729,    44,   504,   536,   576,   577,   578,   685,
     598,   702,   599,   703,   778,   600,   704,   601,   705,   779,
     602,   706,   603,   707,   780,   604,   708,   644,   597,   700,
     733,   734,   594,   587,   588,   691,   772,   773,   692,   725,
     693,   727,   694,   723,   765,   768,   819,   820,   821,   852,
     862,   872,   863,   864,   865,   866,   874,   884,   870,   871,
     463,   616,   621,   632,   683,   695,   634,   721,   764,   837,
     488,   570,   589,   635,   571,   198,   199,   636,   201,   202,
     203,   204,    45,    92,   104,    50,   102,   103,   311,   312,
     316,   317,   318,    53,   321,   115,    52,   391,   445,   110,
      51,   290,   291,   296,   295,   272,   277,   280,   273,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   271,   438,
     328,   457,    48,   259,   248,   263,    90,    91,    95,   130,
      46,    47,   314,   320,    68,   215,   325,   405,   453,   226,
     326,   327,   416,   455,   456,   498,   515,   516,   393,   447,
     448,   472,   506,   505,   507,   548,   607,   608,   609,   660,
     711,   712,   751,   781,   782,   538,   605,   606,   654,   709,
     710,   417,   454,   216,   323,   324,   217,   228,   418,   500,
     474,   550,   378,   441,   379,   380,   381,   382
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   294,   227,   218,   200,   229,   656,   763,   595,   590,
     590,   590,   665,   300,   310,   197,   617,   618,   619,   783,
     575,   572,   574,   724,   592,   592,   592,   473,   728,   540,
     835,   264,   265,   509,   510,   499,   735,   696,   270,   266,
     858,   567,   568,   569,   905,   359,   249,   250,   251,   252,
     292,   253,   275,   681,   298,   299,   478,   771,    97,   666,
     907,   667,   306,   267,   553,   227,   701,   552,   479,   200,
     231,    98,   590,   566,    99,   633,   643,   511,   590,   590,
     309,   722,   254,   726,   906,   539,   549,   592,   730,   668,
     113,   293,   854,   592,   592,   633,   255,   256,   257,   276,
     906,   915,   420,   855,   747,   100,   232,   610,   233,   234,
     235,   236,   753,   735,   575,   572,   574,   774,   896,   679,
     101,   403,   666,   861,   667,   292,   237,   131,   784,   785,
     682,   786,   662,   268,   787,   567,   568,   569,   261,   262,
      86,    87,   230,   682,   908,   260,   476,   116,   117,   640,
     118,   119,   668,   338,   477,   640,   120,   269,   121,   122,
     123,   124,   125,   126,   714,   274,   293,   675,   672,   909,
      93,    94,   914,   931,   258,   933,   673,   278,   775,    88,
      89,   360,   361,   396,   906,   655,   365,   399,   661,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   739,
     736,   738,   301,   331,   332,   333,   334,   335,   336,   906,
     728,   389,   906,   906,   892,   906,   822,   397,   679,   337,
     793,   766,   383,   339,   279,   774,   384,   385,   794,   297,
     342,   343,   402,   404,   406,  -282,   419,   347,   348,   349,
     350,   127,   128,   129,  -294,  -294,  -294,   302,   881,   615,
     867,   868,   869,   637,   789,   615,   303,   920,   -36,   -36,
     -36,   304,   227,   388,   -34,   -34,   394,   394,   227,   595,
     730,   394,   590,   637,   899,   900,   739,   736,   738,  -295,
    -295,  -295,   901,   832,   899,   900,   774,   592,   902,   305,
     446,   752,   901,   686,   838,   687,   352,   637,   902,   308,
     421,   422,   353,   354,   355,   922,    86,    87,   637,   430,
      54,   261,   262,   615,   307,   637,   643,   637,   313,   590,
     315,   637,   -35,   -35,   -35,   895,   319,   885,   886,   887,
     329,   330,    55,   518,   592,    56,   894,   344,   356,   357,
     358,   227,   637,    57,   464,  -296,  -296,  -296,   395,   395,
     345,   877,    58,   395,    59,   200,   346,   475,    60,   340,
     341,   637,   489,   490,   390,   501,   451,   795,   796,   637,
    -293,  -293,  -293,    61,   637,    62,    63,    64,    65,  -249,
    -249,  -249,    66,   351,   637,   362,   363,   927,   364,   929,
    -250,  -250,  -250,    67,   386,   227,   637,   637,   398,   637,
    -251,  -251,  -251,   637,   867,   868,   869,   200,   400,   461,
     423,   424,   425,   426,   452,   205,   551,   206,   427,   207,
     428,   208,   209,   210,   429,   559,   431,   432,   502,   554,
     433,   200,   637,   434,   435,   200,   200,   511,   462,   436,
     519,   520,   451,   521,   522,   523,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   533,   534,   211,   212,   213,
     214,   637,   437,   439,   458,   459,   200,   558,   238,   460,
     503,   513,   637,   517,   555,   573,   638,   514,   646,   591,
     591,   591,   200,   560,  -281,   615,   614,   620,   647,   239,
     240,   241,   242,   596,   243,   244,   219,   652,   206,   535,
     220,   648,   221,   222,   223,   649,   650,   651,   684,   200,
     697,   637,   200,   637,   699,   657,   718,   740,   663,   742,
     245,   246,   247,   613,   586,   586,   744,   746,   756,   637,
     757,   767,   759,   777,   637,   760,   873,   776,   211,   224,
     225,   214,   591,   831,    69,   791,    70,   842,   591,   591,
     839,   844,   845,   846,   840,   841,   637,   847,   843,   658,
     848,   849,   859,   853,   888,    71,   883,   910,    36,   573,
     917,   918,   637,   919,   926,   928,   932,   674,   761,   916,
     921,   880,   444,   508,   898,   824,   834,   586,   593,   823,
     851,   637,   893,   586,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,   713,    96,   645,
     720,   741,     0,   743,     0,   745,     0,   442,   443,   749,
       0,   754,     0,   200,     0,     0,     0,   200,    85,   219,
       0,   206,     0,   220,   716,   392,   222,   223,   719,   407,
       0,   408,     0,   292,     0,   409,   410,   411,     0,     0,
       0,   352,     0,    36,   737,   440,     0,   353,   354,   355,
     465,   748,   408,     0,   292,     0,   466,   467,   468,     0,
       0,   211,   224,   225,   214,     0,     0,     0,     0,     0,
       0,   412,   413,   414,   293,     0,     0,   826,   827,   828,
       0,   830,     0,   356,   357,   358,   376,     0,     0,     0,
       0,     0,   469,   470,   471,   293,     0,     0,     0,     0,
       0,     0,     0,     0,   200,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   792,     0,     0,     0,     0,
       0,   737,   200,   829,     0,     0,     0,     0,     0,     0,
       0,     0,   591,   825,     0,     0,     0,     0,     0,     0,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   622,     0,   686,
     623,   687,   624,     0,   625,     0,     0,     0,     0,   688,
       0,     0,   626,   627,   628,     0,   629,   630,   897,   591,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,     0,   631,     0,   689,     0,
       0,     0,   200,   200,   200,     0,     0,     0,     0,     0,
       0,     0,     0,   889,   890,   891,     0,    -3,     1,   934,
       0,     0,     0,     2,     3,     4,     5,     0,  -499,  -499,
       6,  -499,  -499,     7,     8,     9,    10,  -499,     0,  -499,
    -499,  -499,  -499,  -499,  -499,    11,    12,     0,   491,    13,
     408,     0,   292,    14,   492,   493,   494,    15,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   911,
     912,   913,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    17,     0,
     495,   496,   497,   293,     0,     0,     0,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,     0,     0,     0,
       0,     0,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,  -499,  -499,  -499,   797,   798,   799,   800,   801,
     802,   803,   804,   805,   806,   807,   808,   809,   810,   811,
     812,   813,   814,   815,   816,   817,   818,     0,     0,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   481,
      37,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,     0,
     195,     0,   196,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   481,     0,
     561,   158,   159,   160,   562,   162,   163,   563,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,     0,   195,
       0,   564,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   579,   580,   581,   582,   639,   583,    36,     0,   584,
     585,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,     0,   195,     0,
     196,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     579,   580,   581,   582,   641,   583,    36,     0,   584,   585,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,     0,   195,     0,   196,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,     0,     0,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,     0,   195,     0,   196,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   579,   580,
     581,   582,     0,   583,    36,     0,   584,   585,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,     0,   195,     0,   196,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   579,   580,   581,
     582,   642,   583,     0,     0,   584,   585,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,     0,   195,     0,   196,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   579,   580,   581,   582,
     857,   583,     0,     0,   584,   585,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,     0,   195,     0,   196,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   579,   580,   581,   582,     0,
     583,     0,     0,   584,   585,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,     0,   195,     0,   196,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   622,     0,     0,   623,     0,   624,   769,   625,
       0,     0,   770,     0,   688,     0,     0,   626,   627,   628,
       0,   629,   630,     0,     0,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
       0,   631,     0,   689,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   622,     0,     0,   623,     0,   624,     0,   625,     0,
       0,   676,   680,     0,     0,     0,   626,   627,   628,     0,
     629,   630,     0,     0,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,     0,
     631,     0,   677,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     622,     0,     0,   623,     0,   624,     0,   625,     0,     0,
     676,     0,     0,     0,     0,   626,   627,   628,     0,   629,
     630,     0,     0,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,     0,   631,
       0,   677,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   622,
       0,     0,   623,     0,   624,     0,   625,     0,     0,     0,
       0,   688,     0,     0,   626,   627,   628,     0,   629,   630,
       0,     0,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,     0,   631,     0,
     689,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   622,     0,
       0,   623,     0,   624,     0,   625,     0,     0,     0,     0,
       0,     0,     0,   626,   627,   628,     0,   629,   630,     0,
       0,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,     0,   631,     0,   875,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,     0,     0,     0,   158,   159,
     160,   731,   162,   163,   732,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,     0,   195,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   622,     0,   686,   623,   687,   624,
       0,   625,     0,     0,     0,     0,   688,     0,     0,   626,
     627,   628,     0,   629,   630,     0,     0,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,     0,   631,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   622,     0,     0,   623,     0,   624,     0,   625,     0,
       0,     0,   762,     0,     0,     0,   626,   627,   628,     0,
     629,   630,     0,     0,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,     0,
     631,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   622,     0,
       0,   623,     0,   624,     0,   625,     0,     0,     0,     0,
     688,     0,     0,   626,   627,   628,     0,   629,   630,     0,
       0,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,     0,   631,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   622,     0,     0,   623,     0,
     624,     0,   625,     0,     0,     0,     0,     0,     0,     0,
     626,   627,   628,     0,   629,   630,     0,     0,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   407,   631,   408,     0,   292,     0,   409,
     410,   411,   541,     0,   408,     0,   292,     0,   542,   543,
     544,   491,     0,   408,     0,   292,     0,   492,   653,   494,
     407,     0,   408,     0,   292,     0,   409,   410,   411,     0,
       0,     0,     0,     0,     0,   412,   413,   537,   293,     0,
       0,     0,     0,     0,   545,   546,   547,   293,     0,     0,
       0,     0,     0,   495,   496,   497,   293,     0,     0,     0,
       0,     0,   412,   413,   659,   293,   491,     0,   408,     0,
     292,     0,   492,   750,   494,   352,   850,    36,     0,     0,
       0,   353,   354,   355,     0,     0,   797,   798,   799,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,   810,
     811,   812,   813,   814,   815,   816,   817,   818,   495,   496,
     497,   293,     0,     0,     0,     0,     0,   356,   357,   358,
     376
};

static const yytype_int16 yycheck[] =
{
       0,    92,    52,    51,    49,    53,   605,   721,   526,   524,
     525,   526,     1,   104,   115,    49,   568,   569,   570,     1,
     520,   520,   520,   693,   524,   525,   526,   447,   698,   505,
     790,    60,    61,   480,   481,   455,   699,   641,    67,    66,
     835,   520,   520,   520,   134,   271,    41,    42,    43,    44,
     135,    46,    94,   632,   102,   103,   131,   727,    72,   111,
     134,   113,   110,    90,   511,   115,   645,   134,   143,   114,
      13,   135,   587,   520,   135,   577,   594,   144,   593,   594,
     114,   685,    77,   693,   174,   505,   506,   587,   698,   141,
       0,   176,   131,   593,   594,   597,    91,    92,    93,   141,
     174,   896,   328,   142,   708,   135,    49,   554,    51,    52,
      53,    54,   711,   776,   614,   614,   614,   727,   878,   621,
     135,   131,   111,   837,   113,   135,    69,   135,   110,   111,
     632,   113,   608,   160,   116,   614,   614,   614,    30,    31,
      36,    37,    75,   645,   134,    29,   139,    11,    12,   587,
      14,    15,   141,    45,   147,   593,    20,    80,    22,    23,
      24,    25,    26,    27,   666,    41,   176,   614,   139,   134,
      77,    78,   134,   134,   169,   134,   147,   135,   730,    75,
      76,   272,   273,   317,   174,   605,   277,   321,   608,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   699,
     699,   699,     3,   232,   233,   234,   235,   236,   237,   174,
     880,   312,   174,   174,   134,   174,   768,   318,   720,   248,
     139,   723,   291,   252,   135,   835,   295,   296,   147,   160,
     259,   260,   323,   324,   325,   135,   327,   266,   267,   268,
     269,   105,   106,   107,   144,   145,   146,     3,   858,   135,
     170,   171,   172,   577,   756,   135,     4,   138,   144,   145,
     146,     3,   312,   311,   144,   145,   316,   317,   318,   787,
     880,   321,   787,   597,   155,   156,   776,   776,   776,   144,
     145,   146,   163,   785,   155,   156,   896,   787,   169,     3,
     391,   711,   163,   127,   796,   129,   131,   621,   169,     3,
     329,   330,   137,   138,   139,   915,    36,    37,   632,   338,
       7,    30,    31,   135,   134,   639,   834,   641,    82,   834,
      18,   645,   144,   145,   146,   877,    94,   863,   864,   865,
      51,    52,    29,     1,   834,    32,   872,   135,   173,   174,
     175,   391,   666,    40,   445,   144,   145,   146,   316,   317,
     135,   853,    49,   321,    51,   400,   135,   448,    55,    47,
      48,   685,   453,   454,    21,   456,   400,   138,   139,   693,
     144,   145,   146,    70,   698,    72,    73,    74,    75,   144,
     145,   146,    79,   135,   708,   135,   135,   923,    94,   925,
     144,   145,   146,    90,   135,   445,   720,   721,   135,   723,
     144,   145,   146,   727,   170,   171,   172,   452,   146,   438,
     135,   135,   135,   135,   126,   131,   507,   133,   135,   135,
     135,   137,   138,   139,   135,   516,   135,   135,   457,   142,
     135,   476,   756,   135,   135,   480,   481,   144,   125,   135,
     108,   109,   476,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   173,   174,   175,
     176,   785,   135,   135,   135,   135,   511,   515,    12,   135,
     135,   145,   796,   135,   133,   520,   131,   146,   131,   524,
     525,   526,   527,   135,   135,   135,   144,   135,   131,    33,
      34,    35,    36,   527,    38,    39,   131,   131,   133,   167,
     135,   132,   137,   138,   139,   132,   128,   128,   146,   554,
     127,   835,   557,   837,   140,   606,   139,   135,   609,   130,
      64,    65,    66,   557,   524,   525,   130,   130,   142,   853,
     131,   137,   142,   131,   858,   134,   177,   139,   173,   174,
     175,   176,   587,   135,     6,   142,     8,   133,   593,   594,
     150,   133,   133,   133,   150,   174,   880,   133,   174,   607,
     133,   133,   137,   142,     5,    27,   174,   134,   133,   614,
     133,   133,   896,   133,   142,     5,   134,   614,   718,   174,
     903,   857,   387,   476,   880,   776,   787,   587,   525,   772,
     819,   915,   870,   593,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,   664,    16,   597,
     683,   702,    -1,   704,    -1,   706,    -1,   380,   380,   710,
      -1,   712,    -1,   668,    -1,    -1,    -1,   672,    90,   131,
      -1,   133,    -1,   135,   668,   137,   138,   139,   672,   131,
      -1,   133,    -1,   135,    -1,   137,   138,   139,    -1,    -1,
      -1,   131,    -1,   133,   699,   135,    -1,   137,   138,   139,
     131,   709,   133,    -1,   135,    -1,   137,   138,   139,    -1,
      -1,   173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   173,   174,   175,   176,    -1,    -1,   778,   779,   780,
      -1,   782,    -1,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,   173,   174,   175,   176,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   759,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   759,    -1,    -1,    -1,    -1,
      -1,   776,   777,   781,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   787,   777,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,   127,
     128,   129,   130,    -1,   132,    -1,    -1,    -1,    -1,   137,
      -1,    -1,   140,   141,   142,    -1,   144,   145,   879,   834,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,    -1,   174,    -1,   176,    -1,
      -1,    -1,   867,   868,   869,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   867,   868,   869,    -1,     0,     1,   930,
      -1,    -1,    -1,     6,     7,     8,     9,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,   131,    32,
     133,    -1,   135,    36,   137,   138,   139,    40,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   889,
     890,   891,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    81,    -1,
      83,    84,    85,    86,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     173,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,    -1,
     174,    -1,   176,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,    -1,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,    -1,   174,
      -1,   176,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,    -1,   174,    -1,
     176,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,    -1,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,    -1,   174,    -1,   176,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,    -1,    -1,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,    -1,   174,    -1,   176,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,    -1,   132,   133,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,    -1,   174,    -1,   176,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,    -1,   174,    -1,   176,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,    -1,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,    -1,   174,    -1,   176,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,    -1,
     132,    -1,    -1,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,    -1,   174,    -1,   176,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,    -1,   128,    -1,   130,   131,   132,
      -1,    -1,   135,    -1,   137,    -1,    -1,   140,   141,   142,
      -1,   144,   145,    -1,    -1,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
      -1,   174,    -1,   176,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,    -1,   128,    -1,   130,    -1,   132,    -1,
      -1,   135,   136,    -1,    -1,    -1,   140,   141,   142,    -1,
     144,   145,    -1,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,    -1,
     174,    -1,   176,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,    -1,   128,    -1,   130,    -1,   132,    -1,    -1,
     135,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,   144,
     145,    -1,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,    -1,   174,
      -1,   176,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,    -1,   128,    -1,   130,    -1,   132,    -1,    -1,    -1,
      -1,   137,    -1,    -1,   140,   141,   142,    -1,   144,   145,
      -1,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,    -1,   174,    -1,
     176,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
      -1,   128,    -1,   130,    -1,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,    -1,   144,   145,    -1,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,    -1,   174,    -1,   176,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,    -1,    -1,    -1,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,    -1,   174,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,   127,   128,   129,   130,
      -1,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,   140,
     141,   142,    -1,   144,   145,    -1,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,    -1,   174,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,    -1,    -1,   128,    -1,   130,    -1,   132,    -1,
      -1,    -1,   136,    -1,    -1,    -1,   140,   141,   142,    -1,
     144,   145,    -1,    -1,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,    -1,
     174,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
      -1,   128,    -1,   130,    -1,   132,    -1,    -1,    -1,    -1,
     137,    -1,    -1,   140,   141,   142,    -1,   144,   145,    -1,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,    -1,   174,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,    -1,   128,    -1,
     130,    -1,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,   144,   145,    -1,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   131,   174,   133,    -1,   135,    -1,   137,
     138,   139,   131,    -1,   133,    -1,   135,    -1,   137,   138,
     139,   131,    -1,   133,    -1,   135,    -1,   137,   138,   139,
     131,    -1,   133,    -1,   135,    -1,   137,   138,   139,    -1,
      -1,    -1,    -1,    -1,    -1,   173,   174,   175,   176,    -1,
      -1,    -1,    -1,    -1,   173,   174,   175,   176,    -1,    -1,
      -1,    -1,    -1,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,   173,   174,   175,   176,   131,    -1,   133,    -1,
     135,    -1,   137,   138,   139,   131,   138,   133,    -1,    -1,
      -1,   137,   138,   139,    -1,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,   173,   174,   175,
     176
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     6,     7,     8,     9,    13,    16,    17,    18,
      19,    28,    29,    32,    36,    40,    55,    71,    81,    83,
      84,    85,    86,    87,    88,    89,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   133,   173,   179,   180,
     181,   214,   229,   231,   241,   320,   368,   369,   360,   215,
     323,   338,   334,   331,     7,    29,    32,    40,    49,    51,
      55,    70,    72,    73,    74,    75,    79,    90,   372,     6,
       8,    27,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    90,    36,    37,    75,    76,
     364,   365,   321,    77,    78,   366,   364,    72,   135,   135,
     135,   135,   324,   325,   322,   185,   186,   187,   183,   184,
     337,   230,   182,     0,   232,   333,    11,    12,    14,    15,
      20,    22,    23,    24,    25,    26,    27,   105,   106,   107,
     367,   135,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   174,   176,   308,   313,   314,
     315,   316,   317,   318,   319,   131,   133,   135,   137,   138,
     139,   173,   174,   175,   176,   373,   411,   414,   373,   131,
     135,   137,   138,   139,   174,   175,   377,   411,   415,   373,
      75,    13,    49,    51,    52,    53,    54,    69,    12,    33,
      34,    35,    36,    38,    39,    64,    65,    66,   362,    41,
      42,    43,    44,    46,    77,    91,    92,    93,   169,   361,
      29,    30,    31,   363,   363,   363,    66,    90,   160,    80,
     363,   356,   343,   346,    41,    94,   141,   344,   135,   135,
     345,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     339,   340,   135,   176,   228,   342,   341,   160,   373,   373,
     228,     3,     3,     4,     3,     3,   373,   134,     3,   308,
     377,   326,   327,    82,   370,    18,   328,   329,   330,    94,
     371,   332,   216,   412,   413,   374,   378,   379,   358,    51,
      52,   363,   363,   363,   363,   363,   363,   363,    45,   363,
      47,    48,   363,   363,   135,   135,   135,   363,   363,   363,
     363,   135,   131,   137,   138,   139,   173,   174,   175,   425,
     228,   228,   135,   135,    94,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   176,   229,   420,   422,
     423,   424,   425,   420,   420,   420,   135,   233,   373,   377,
      21,   335,   137,   386,   411,   415,   386,   377,   135,   386,
     146,   235,   228,   131,   228,   375,   228,   131,   133,   137,
     138,   139,   173,   174,   175,   228,   380,   409,   416,   228,
     425,   363,   363,   135,   135,   135,   135,   135,   135,   135,
     363,   135,   135,   135,   135,   135,   135,   135,   357,   135,
     135,   421,   422,   424,   235,   336,   377,   387,   388,   236,
     237,   308,   126,   376,   410,   381,   382,   359,   135,   135,
     135,   363,   125,   298,   377,   131,   137,   138,   139,   173,
     174,   175,   389,   409,   418,   228,   139,   147,   131,   143,
     238,   133,   189,   190,   191,   192,   193,   195,   308,   228,
     228,   131,   137,   138,   139,   173,   174,   175,   383,   409,
     417,   228,   363,   135,   242,   391,   390,   392,   237,   189,
     189,   144,   217,   145,   146,   384,   385,   135,     1,   108,
     109,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   167,   243,   175,   403,   409,
     416,   131,   137,   138,   139,   173,   174,   175,   393,   409,
     419,   228,   134,   189,   142,   133,   197,   194,   373,   228,
     135,   135,   139,   142,   176,   188,   189,   192,   193,   195,
     309,   312,   313,   315,   317,   318,   244,   245,   246,   127,
     128,   129,   130,   132,   135,   136,   229,   271,   272,   310,
     314,   315,   318,   271,   270,   310,   308,   266,   248,   250,
     253,   255,   258,   260,   263,   404,   405,   394,   395,   396,
     189,   198,   196,   308,   144,   135,   299,   299,   299,   299,
     135,   300,   125,   128,   130,   132,   140,   141,   142,   144,
     145,   174,   301,   303,   304,   311,   315,   319,   131,   131,
     272,   131,   131,   310,   265,   301,   131,   131,   132,   132,
     128,   128,   131,   138,   406,   409,   417,   228,   373,   175,
     397,   409,   416,   228,   218,     1,   111,   113,   141,   200,
     201,   202,   139,   147,   188,   189,   135,   176,   234,   303,
     136,   234,   303,   302,   146,   247,   127,   129,   137,   176,
     239,   273,   276,   278,   280,   303,   273,   127,   220,   140,
     267,   234,   249,   251,   254,   256,   259,   261,   264,   407,
     408,   398,   399,   298,   303,   203,   308,   199,   139,   308,
     300,   305,   273,   281,   239,   277,   280,   279,   239,   240,
     280,   139,   142,   268,   269,   309,   313,   315,   317,   318,
     135,   228,   130,   228,   130,   228,   130,   273,   373,   228,
     138,   400,   409,   417,   228,   219,   142,   131,   205,   142,
     134,   202,   136,   304,   306,   282,   303,   137,   283,   131,
     135,   239,   274,   275,   280,   299,   139,   131,   252,   257,
     262,   401,   402,     1,   110,   111,   113,   116,   221,   303,
     206,   142,   308,   139,   147,   138,   139,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   284,
     285,   286,   299,   276,   269,   308,   228,   228,   228,   373,
     228,   135,   303,   222,   270,   279,   204,   307,   303,   150,
     150,   174,   133,   174,   133,   133,   133,   133,   133,   133,
     138,   286,   287,   142,   131,   142,   224,   131,   278,   137,
     207,   304,   288,   290,   291,   292,   293,   170,   171,   172,
     296,   297,   289,   177,   294,   176,   223,   303,   225,   227,
     220,   280,   208,   174,   295,   295,   295,   295,     5,   308,
     308,   308,   134,   297,   295,   299,   279,   228,   240,   155,
     156,   163,   169,   209,   210,   134,   174,   134,   134,   134,
     134,   229,   229,   229,   134,   278,   174,   133,   133,   133,
     138,   210,   280,   211,   213,   212,   142,   295,     5,   295,
     226,   134,   134,   134,   228
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   178,   179,   179,   180,   180,   180,   180,   182,   181,
     183,   181,   184,   181,   181,   185,   181,   186,   181,   181,
     187,   181,   181,   181,   188,   188,   188,   188,   188,   188,
     188,   189,   189,   190,   190,   191,   191,   191,   192,   194,
     193,   195,   196,   196,   198,   199,   197,   200,   200,   201,
     201,   202,   202,   203,   204,   202,   206,   205,   205,   208,
     207,   207,   209,   209,   210,   211,   210,   212,   210,   213,
     210,   215,   216,   217,   218,   214,   219,   219,   220,   220,
     221,   221,   222,   221,   221,   223,   223,   225,   226,   224,
     227,   224,   228,   228,   230,   229,   232,   233,   231,   234,
     234,   235,   235,   236,   236,   237,   238,   238,   239,   240,
     240,   241,   241,   242,   242,   243,   244,   243,   245,   243,
     246,   247,   243,   243,   243,   248,   249,   243,   250,   251,
     252,   243,   253,   254,   243,   255,   256,   257,   243,   258,
     259,   243,   260,   261,   262,   243,   263,   264,   243,   243,
     265,   243,   266,   243,   243,   267,   267,   268,   268,   269,
     270,   270,   271,   271,   272,   272,   273,   273,   273,   274,
     274,   275,   275,   275,   276,   277,   277,   278,   278,   279,
     279,   280,   281,   280,   282,   282,   283,   283,   284,   284,
     285,   285,   285,   286,   286,   287,   286,   286,   286,   286,
     288,   286,   289,   286,   290,   286,   291,   286,   286,   286,
     286,   292,   286,   286,   286,   286,   286,   293,   286,   286,
     286,   294,   294,   295,   295,   296,   296,   297,   297,   297,
     297,   298,   298,   299,   299,   300,   300,   301,   301,   302,
     301,   303,   304,   305,   304,   307,   306,   306,   308,   308,
     308,   308,   308,   309,   309,   309,   309,   309,   309,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   312,   312,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   314,   314,   314,   314,   315,   315,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   317,   317,   317,   317,
     317,   317,   317,   318,   318,   318,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     321,   320,   322,   320,   323,   320,   324,   320,   325,   320,
     326,   320,   327,   320,   328,   320,   329,   320,   330,   320,
     331,   320,   332,   320,   333,   320,   334,   320,   335,   320,
     336,   320,   320,   337,   320,   338,   320,   320,   339,   320,
     340,   320,   341,   320,   342,   320,   320,   343,   320,   344,
     320,   345,   320,   346,   320,   320,   320,   347,   320,   348,
     320,   349,   320,   350,   320,   351,   320,   352,   320,   353,
     320,   354,   320,   320,   320,   320,   355,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   356,   357,   320,   358,   359,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   360,
     320,   361,   361,   361,   361,   361,   361,   361,   361,   361,
     361,   362,   362,   362,   362,   362,   362,   362,   362,   362,
     362,   362,   363,   363,   364,   364,   365,   365,   366,   366,
     367,   367,   367,   367,   367,   367,   368,   368,   369,   369,
     370,   370,   371,   371,   372,   372,   372,   373,   374,   373,
     376,   375,   375,   378,   377,   377,   379,   377,   381,   380,
     382,   380,   380,   384,   383,   385,   383,   383,   387,   386,
     386,   388,   386,   390,   389,   391,   389,   392,   389,   389,
     394,   393,   395,   393,   396,   393,   393,   398,   397,   399,
     397,   397,   401,   400,   402,   400,   400,   404,   403,   405,
     403,   403,   407,   406,   408,   406,   406,   410,   409,   409,
     412,   411,   413,   411,   411,   414,   414,   414,   414,   414,
     414,   414,   415,   415,   415,   415,   415,   415,   416,   416,
     416,   416,   416,   416,   417,   417,   417,   417,   417,   417,
     418,   418,   418,   418,   418,   419,   419,   419,   419,   419,
     420,   420,   421,   421,   422,   423,   423,   424,   424,   425,
     425,   425,   425,   425,   425,   425
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     1,     1,     0,     3,
       0,     3,     0,     3,     1,     0,     3,     0,     3,     1,
       0,     3,     1,     1,     2,     2,     2,     2,     2,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     3,     0,
       5,     3,     3,     1,     0,     0,     5,     1,     1,     3,
       1,     4,     4,     0,     0,     6,     0,     5,     0,     0,
       4,     0,     2,     1,     2,     0,     5,     0,     5,     0,
       5,     0,     0,     0,     0,    14,     2,     0,     1,     0,
       4,     5,     0,     3,     2,     2,     1,     0,     0,     8,
       0,     3,     1,     1,     0,     3,     0,     0,     8,     1,
       1,     3,     0,     3,     1,     3,     1,     1,     1,     2,
       1,     1,     1,     2,     0,     2,     0,     4,     0,     4,
       0,     0,     5,     4,     5,     0,     0,     5,     0,     0,
       0,     7,     0,     0,     5,     0,     0,     0,     7,     0,
       0,     5,     0,     0,     0,     7,     0,     0,     5,     4,
       0,     5,     0,     4,     2,     1,     2,     1,     3,     3,
       2,     1,     2,     1,     1,     1,     3,     1,     1,     1,
       0,     2,     1,     1,     2,     3,     1,     1,     1,     2,
       0,     1,     0,     4,     3,     1,     3,     0,     2,     1,
       1,     2,     2,     1,     1,     0,     3,     1,     1,     2,
       0,     5,     0,     5,     0,     5,     0,     5,     1,     1,
       1,     0,     5,     1,     1,     1,     2,     0,     5,     4,
       1,     1,     0,     2,     1,     2,     1,     2,     3,     3,
       3,     1,     0,     1,     0,     2,     0,     2,     2,     0,
       4,     1,     1,     0,     5,     0,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     3,     0,     4,     0,     3,     0,     3,     0,     5,
       0,     6,     4,     0,     3,     0,     3,     1,     0,     4,
       0,     4,     0,     4,     0,     4,     4,     0,     4,     0,
       4,     0,     4,     0,     4,     3,     3,     0,     4,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       4,     0,     4,     4,     5,     4,     0,     4,     5,     5,
       6,     5,     5,     6,     5,     5,     5,     5,     6,     5,
       4,     0,     0,     7,     0,     0,     8,     4,     5,     5,
       5,     4,     5,     2,     2,     2,     2,     5,     4,     0,
       3,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     0,     1,     0,     1,     1,     1,     1,     0,     3,
       0,     3,     1,     0,     3,     1,     0,     3,     0,     3,
       0,     3,     1,     0,     3,     0,     3,     1,     0,     3,
       1,     0,     3,     0,     3,     0,     3,     0,     3,     1,
       0,     3,     0,     3,     0,     3,     1,     0,     3,     0,
       3,     1,     0,     3,     0,     3,     1,     0,     3,     0,
       3,     1,     0,     3,     0,     3,     1,     0,     3,     1,
       0,     3,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
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
      yyerror (parseResult, YY_("syntax error: cannot back up")); \
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
                  Type, Value, parseResult); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, UserLevelRewritingContext::ParseResult* parseResult)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (parseResult);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, UserLevelRewritingContext::ParseResult* parseResult)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, parseResult);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, UserLevelRewritingContext::ParseResult* parseResult)
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
                                              , parseResult);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, parseResult); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, UserLevelRewritingContext::ParseResult* parseResult)
{
  YYUSE (yyvaluep);
  YYUSE (parseResult);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (UserLevelRewritingContext::ParseResult* parseResult)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

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
      yychar = yylex (&yylval);
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
#line 235 "surface.yy" /* yacc.c:1646  */
    { YYACCEPT; }
#line 2542 "surface.c" /* yacc.c:1646  */
    break;

  case 3:
#line 237 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			}
#line 2550 "surface.c" /* yacc.c:1646  */
    break;

  case 8:
#line 251 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2556 "surface.c" /* yacc.c:1646  */
    break;

  case 9:
#line 253 "surface.yy" /* yacc.c:1646  */
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, false, lineNr);
			}
#line 2569 "surface.c" /* yacc.c:1646  */
    break;

  case 10:
#line 261 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2575 "surface.c" /* yacc.c:1646  */
    break;

  case 11:
#line 263 "surface.yy" /* yacc.c:1646  */
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, true, lineNr);
			}
#line 2588 "surface.c" /* yacc.c:1646  */
    break;

  case 12:
#line 271 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2594 "surface.c" /* yacc.c:1646  */
    break;

  case 13:
#line 273 "surface.yy" /* yacc.c:1646  */
    {
			  int lineNr = lineNumber;
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr) &&
			      !directoryManager.alreadySeen(directory, fileName))
			    includeFile(directory, fileName, true, lineNr);
			}
#line 2607 "surface.c" /* yacc.c:1646  */
    break;

  case 14:
#line 282 "surface.yy" /* yacc.c:1646  */
    {
			  cout << directoryManager.getCwd() << '\n';
			}
#line 2615 "surface.c" /* yacc.c:1646  */
    break;

  case 15:
#line 285 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2621 "surface.c" /* yacc.c:1646  */
    break;

  case 16:
#line 287 "surface.yy" /* yacc.c:1646  */
    {
			  string directory;
			  directoryManager.realPath((yyvsp[0].yyString), directory);
			  if (!directoryManager.cd(directory))
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": cd failed");
			    }
			}
#line 2635 "surface.c" /* yacc.c:1646  */
    break;

  case 17:
#line 296 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2641 "surface.c" /* yacc.c:1646  */
    break;

  case 18:
#line 298 "surface.yy" /* yacc.c:1646  */
    {
			  string directory;
			  directoryManager.realPath((yyvsp[0].yyString), directory);
			  if (directoryManager.pushd(directory) == UNDEFINED)
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": pushd failed");
			    }
			}
#line 2655 "surface.c" /* yacc.c:1646  */
    break;

  case 19:
#line 308 "surface.yy" /* yacc.c:1646  */
    {
			  const char* path = directoryManager.popd();
			  if (path != 0)
			    cout << path << '\n';
			  else
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": popd failed");
			    }
			}
#line 2670 "surface.c" /* yacc.c:1646  */
    break;

  case 20:
#line 318 "surface.yy" /* yacc.c:1646  */
    { lexerStringMode(); }
#line 2676 "surface.c" /* yacc.c:1646  */
    break;

  case 21:
#line 320 "surface.yy" /* yacc.c:1646  */
    {
			  system((string("ls") + (yyvsp[0].yyString)).c_str());
			}
#line 2684 "surface.c" /* yacc.c:1646  */
    break;

  case 22:
#line 324 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			  YYACCEPT;
			}
#line 2693 "surface.c" /* yacc.c:1646  */
    break;

  case 23:
#line 329 "surface.yy" /* yacc.c:1646  */
    {
			  if(!handleEof())
			    {
			      PARSE_RESULT = UserLevelRewritingContext::QUIT;
			    }
			}
#line 2704 "surface.c" /* yacc.c:1646  */
    break;

  case 24:
#line 362 "surface.yy" /* yacc.c:1646  */
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[-1].yyToken)));
                        }
#line 2712 "surface.c" /* yacc.c:1646  */
    break;

  case 25:
#line 366 "surface.yy" /* yacc.c:1646  */
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[-1].yyToken)));
                        }
#line 2720 "surface.c" /* yacc.c:1646  */
    break;

  case 29:
#line 373 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			}
#line 2732 "surface.c" /* yacc.c:1646  */
    break;

  case 30:
#line 381 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  moduleExpressions.push(new ModuleExpression(t));
			}
#line 2743 "surface.c" /* yacc.c:1646  */
    break;

  case 32:
#line 391 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			}
#line 2755 "surface.c" /* yacc.c:1646  */
    break;

  case 37:
#line 407 "surface.yy" /* yacc.c:1646  */
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[0].yyToken)));
                        }
#line 2763 "surface.c" /* yacc.c:1646  */
    break;

  case 38:
#line 414 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, currentRenaming));
			  currentRenaming = 0;
			}
#line 2774 "surface.c" /* yacc.c:1646  */
    break;

  case 39:
#line 422 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 2780 "surface.c" /* yacc.c:1646  */
    break;

  case 40:
#line 424 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, tokenSequence));
			}
#line 2790 "surface.c" /* yacc.c:1646  */
    break;

  case 41:
#line 431 "surface.yy" /* yacc.c:1646  */
    {}
#line 2796 "surface.c" /* yacc.c:1646  */
    break;

  case 42:
#line 434 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 2802 "surface.c" /* yacc.c:1646  */
    break;

  case 43:
#line 435 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 2808 "surface.c" /* yacc.c:1646  */
    break;

  case 44:
#line 442 "surface.yy" /* yacc.c:1646  */
    {
			  oldSyntaxContainer = currentSyntaxContainer;
			  currentSyntaxContainer = currentRenaming = new Renaming;
			}
#line 2817 "surface.c" /* yacc.c:1646  */
    break;

  case 45:
#line 447 "surface.yy" /* yacc.c:1646  */
    {
			  currentSyntaxContainer = oldSyntaxContainer;
			}
#line 2825 "surface.c" /* yacc.c:1646  */
    break;

  case 51:
#line 462 "surface.yy" /* yacc.c:1646  */
    {
			  currentRenaming->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 2833 "surface.c" /* yacc.c:1646  */
    break;

  case 52:
#line 466 "surface.yy" /* yacc.c:1646  */
    {
			  currentRenaming->addLabelMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 2841 "surface.c" /* yacc.c:1646  */
    break;

  case 53:
#line 469 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON | BAR_TO, 1); }
#line 2847 "surface.c" /* yacc.c:1646  */
    break;

  case 54:
#line 470 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COMMA | BAR_LEFT_BRACKET | BAR_RIGHT_PAREN, 1); }
#line 2853 "surface.c" /* yacc.c:1646  */
    break;

  case 55:
#line 471 "surface.yy" /* yacc.c:1646  */
    {}
#line 2859 "surface.c" /* yacc.c:1646  */
    break;

  case 56:
#line 476 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
#line 2865 "surface.c" /* yacc.c:1646  */
    break;

  case 57:
#line 477 "surface.yy" /* yacc.c:1646  */
    {}
#line 2871 "surface.c" /* yacc.c:1646  */
    break;

  case 58:
#line 478 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
#line 2877 "surface.c" /* yacc.c:1646  */
    break;

  case 59:
#line 484 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
#line 2883 "surface.c" /* yacc.c:1646  */
    break;

  case 60:
#line 485 "surface.yy" /* yacc.c:1646  */
    {}
#line 2889 "surface.c" /* yacc.c:1646  */
    break;

  case 61:
#line 486 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
#line 2895 "surface.c" /* yacc.c:1646  */
    break;

  case 64:
#line 493 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setPrec((yyvsp[0].yyToken)); }
#line 2901 "surface.c" /* yacc.c:1646  */
    break;

  case 65:
#line 494 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 2907 "surface.c" /* yacc.c:1646  */
    break;

  case 66:
#line 495 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setGather(tokenSequence); }
#line 2913 "surface.c" /* yacc.c:1646  */
    break;

  case 67:
#line 496 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 2919 "surface.c" /* yacc.c:1646  */
    break;

  case 68:
#line 497 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setFormat(tokenSequence); }
#line 2925 "surface.c" /* yacc.c:1646  */
    break;

  case 69:
#line 498 "surface.yy" /* yacc.c:1646  */
    { lexerLatexMode(); }
#line 2931 "surface.c" /* yacc.c:1646  */
    break;

  case 70:
#line 499 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setLatexMacro((yyvsp[-1].yyString)); }
#line 2937 "surface.c" /* yacc.c:1646  */
    break;

  case 71:
#line 505 "surface.yy" /* yacc.c:1646  */
    { lexerIdMode(); }
#line 2943 "surface.c" /* yacc.c:1646  */
    break;

  case 72:
#line 507 "surface.yy" /* yacc.c:1646  */
    {
			  fileTable.beginModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			  interpreter.setCurrentView(new SyntacticView((yyvsp[0].yyToken), &interpreter));
			  currentSyntaxContainer = CV;
			}
#line 2953 "surface.c" /* yacc.c:1646  */
    break;

  case 73:
#line 513 "surface.yy" /* yacc.c:1646  */
    {
			  CV->addFrom(moduleExpressions.top());
			  moduleExpressions.pop();
			}
#line 2962 "surface.c" /* yacc.c:1646  */
    break;

  case 74:
#line 518 "surface.yy" /* yacc.c:1646  */
    {
			  CV->addTo(moduleExpressions.top());
			  moduleExpressions.pop();
			}
#line 2971 "surface.c" /* yacc.c:1646  */
    break;

  case 75:
#line 523 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  interpreter.insertView(((yyvsp[-11].yyToken)).code(), CV);
			  CV->finishView();
			}
#line 2982 "surface.c" /* yacc.c:1646  */
    break;

  case 77:
#line 532 "surface.yy" /* yacc.c:1646  */
    {}
#line 2988 "surface.c" /* yacc.c:1646  */
    break;

  case 78:
#line 536 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
				       ": skipping " << QUOTE("->") << " in variable declaration.");
			}
#line 2997 "surface.c" /* yacc.c:1646  */
    break;

  case 79:
#line 540 "surface.yy" /* yacc.c:1646  */
    {}
#line 3003 "surface.c" /* yacc.c:1646  */
    break;

  case 80:
#line 544 "surface.yy" /* yacc.c:1646  */
    {
			  CV->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3011 "surface.c" /* yacc.c:1646  */
    break;

  case 81:
#line 547 "surface.yy" /* yacc.c:1646  */
    {}
#line 3017 "surface.c" /* yacc.c:1646  */
    break;

  case 82:
#line 548 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON | BAR_TO, 1); }
#line 3023 "surface.c" /* yacc.c:1646  */
    break;

  case 85:
#line 553 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyToken) = (yyvsp[-1].yyToken); }
#line 3029 "surface.c" /* yacc.c:1646  */
    break;

  case 86:
#line 555 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  (yyval.yyToken) = t;
			}
#line 3040 "surface.c" /* yacc.c:1646  */
    break;

  case 87:
#line 564 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	Specific op->op mapping.
			  //
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpMapping(lexerBubble);
			}
#line 3052 "surface.c" /* yacc.c:1646  */
    break;

  case 88:
#line 572 "surface.yy" /* yacc.c:1646  */
    {
			  lexBubble(END_STATEMENT, 1);
			}
#line 3060 "surface.c" /* yacc.c:1646  */
    break;

  case 89:
#line 576 "surface.yy" /* yacc.c:1646  */
    {
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpTarget(lexerBubble);
			}
#line 3069 "surface.c" /* yacc.c:1646  */
    break;

  case 90:
#line 581 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	At this point we don't know if we have an op->term mapping
			  //	or a generic op->op mapping so we save the from description and
			  //	press on.
			  //
			  opDescription = lexerBubble;
			  lexBubble(END_STATEMENT, 1);
			}
#line 3083 "surface.c" /* yacc.c:1646  */
    break;

  case 91:
#line 591 "surface.yy" /* yacc.c:1646  */
    {
			  if (lexerBubble[0].code() == Token::encode("term"))
			    {
			      //
			      //	Op->term mapping.
			      //
			      CV->addOpTermMapping(opDescription, lexerBubble);
			    }
			  else
			    {
			      //
			      //	Generic op->op mapping.
			      //
			      Token::peelParens(opDescription);  // remove any enclosing parens from op name
			      CV->addOpMapping(opDescription);
			      Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			      CV->addOpTarget(lexerBubble);
			    }
			}
#line 3107 "surface.c" /* yacc.c:1646  */
    break;

  case 92:
#line 613 "surface.yy" /* yacc.c:1646  */
    {}
#line 3113 "surface.c" /* yacc.c:1646  */
    break;

  case 93:
#line 615 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  lexerBubble.append(t);
			}
#line 3124 "surface.c" /* yacc.c:1646  */
    break;

  case 94:
#line 623 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_RIGHT_PAREN, 1); }
#line 3130 "surface.c" /* yacc.c:1646  */
    break;

  case 95:
#line 624 "surface.yy" /* yacc.c:1646  */
    {}
#line 3136 "surface.c" /* yacc.c:1646  */
    break;

  case 96:
#line 630 "surface.yy" /* yacc.c:1646  */
    { lexerIdMode(); }
#line 3142 "surface.c" /* yacc.c:1646  */
    break;

  case 97:
#line 632 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setCurrentModule(new SyntacticPreModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken), &interpreter));
			  currentSyntaxContainer = CM;
			  fileTable.beginModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3152 "surface.c" /* yacc.c:1646  */
    break;

  case 98:
#line 638 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule((yyvsp[0].yyToken));
			}
#line 3162 "surface.c" /* yacc.c:1646  */
    break;

  case 99:
#line 645 "surface.yy" /* yacc.c:1646  */
    {}
#line 3168 "surface.c" /* yacc.c:1646  */
    break;

  case 100:
#line 647 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  store(t);
			}
#line 3179 "surface.c" /* yacc.c:1646  */
    break;

  case 101:
#line 655 "surface.yy" /* yacc.c:1646  */
    {}
#line 3185 "surface.c" /* yacc.c:1646  */
    break;

  case 102:
#line 656 "surface.yy" /* yacc.c:1646  */
    {}
#line 3191 "surface.c" /* yacc.c:1646  */
    break;

  case 105:
#line 664 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  currentSyntaxContainer->addParameter2((yyvsp[-2].yyToken), me);
			}
#line 3201 "surface.c" /* yacc.c:1646  */
    break;

  case 106:
#line 671 "surface.yy" /* yacc.c:1646  */
    {}
#line 3207 "surface.c" /* yacc.c:1646  */
    break;

  case 107:
#line 673 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			    ": saw " << QUOTE(':') << " instead of " <<
			    QUOTE("::") << " in parameter declaration.");
			}
#line 3217 "surface.c" /* yacc.c:1646  */
    break;

  case 108:
#line 681 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  currentSyntaxContainer->addType(false, singleton);
			  (yyval.yyToken) = (yyvsp[0].yyToken);  // needed for line number
			}
#line 3228 "surface.c" /* yacc.c:1646  */
    break;

  case 110:
#line 690 "surface.yy" /* yacc.c:1646  */
    {}
#line 3234 "surface.c" /* yacc.c:1646  */
    break;

  case 114:
#line 697 "surface.yy" /* yacc.c:1646  */
    {}
#line 3240 "surface.c" /* yacc.c:1646  */
    break;

  case 115:
#line 701 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addImport((yyvsp[-1].yyToken), me);
			}
#line 3250 "surface.c" /* yacc.c:1646  */
    break;

  case 116:
#line 707 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3256 "surface.c" /* yacc.c:1646  */
    break;

  case 117:
#line 708 "surface.yy" /* yacc.c:1646  */
    { CM->addSortDecl(tokenSequence); }
#line 3262 "surface.c" /* yacc.c:1646  */
    break;

  case 118:
#line 710 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3268 "surface.c" /* yacc.c:1646  */
    break;

  case 119:
#line 711 "surface.yy" /* yacc.c:1646  */
    { CM->addSubsortDecl(tokenSequence); }
#line 3274 "surface.c" /* yacc.c:1646  */
    break;

  case 120:
#line 713 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON, 1); }
#line 3280 "surface.c" /* yacc.c:1646  */
    break;

  case 121:
#line 714 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
#line 3286 "surface.c" /* yacc.c:1646  */
    break;

  case 122:
#line 715 "surface.yy" /* yacc.c:1646  */
    {}
#line 3292 "surface.c" /* yacc.c:1646  */
    break;

  case 123:
#line 717 "surface.yy" /* yacc.c:1646  */
    {}
#line 3298 "surface.c" /* yacc.c:1646  */
    break;

  case 124:
#line 719 "surface.yy" /* yacc.c:1646  */
    {}
#line 3304 "surface.c" /* yacc.c:1646  */
    break;

  case 125:
#line 721 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1); }
#line 3310 "surface.c" /* yacc.c:1646  */
    break;

  case 126:
#line 722 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3316 "surface.c" /* yacc.c:1646  */
    break;

  case 127:
#line 723 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3322 "surface.c" /* yacc.c:1646  */
    break;

  case 128:
#line 725 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1);  }
#line 3328 "surface.c" /* yacc.c:1646  */
    break;

  case 129:
#line 726 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3334 "surface.c" /* yacc.c:1646  */
    break;

  case 130:
#line 727 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3340 "surface.c" /* yacc.c:1646  */
    break;

  case 131:
#line 728 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3346 "surface.c" /* yacc.c:1646  */
    break;

  case 132:
#line 730 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); }
#line 3352 "surface.c" /* yacc.c:1646  */
    break;

  case 133:
#line 731 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3358 "surface.c" /* yacc.c:1646  */
    break;

  case 134:
#line 732 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3364 "surface.c" /* yacc.c:1646  */
    break;

  case 135:
#line 734 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); }
#line 3370 "surface.c" /* yacc.c:1646  */
    break;

  case 136:
#line 735 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3376 "surface.c" /* yacc.c:1646  */
    break;

  case 137:
#line 736 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3382 "surface.c" /* yacc.c:1646  */
    break;

  case 138:
#line 737 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3388 "surface.c" /* yacc.c:1646  */
    break;

  case 139:
#line 739 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); }
#line 3394 "surface.c" /* yacc.c:1646  */
    break;

  case 140:
#line 740 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3400 "surface.c" /* yacc.c:1646  */
    break;

  case 141:
#line 741 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3406 "surface.c" /* yacc.c:1646  */
    break;

  case 142:
#line 743 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); }
#line 3412 "surface.c" /* yacc.c:1646  */
    break;

  case 143:
#line 744 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3418 "surface.c" /* yacc.c:1646  */
    break;

  case 144:
#line 745 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3424 "surface.c" /* yacc.c:1646  */
    break;

  case 145:
#line 746 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3430 "surface.c" /* yacc.c:1646  */
    break;

  case 146:
#line 748 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON, 1); }
#line 3436 "surface.c" /* yacc.c:1646  */
    break;

  case 147:
#line 749 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
#line 3442 "surface.c" /* yacc.c:1646  */
    break;

  case 148:
#line 750 "surface.yy" /* yacc.c:1646  */
    { CM->setFlag(SymbolType::MESSAGE); }
#line 3448 "surface.c" /* yacc.c:1646  */
    break;

  case 149:
#line 753 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
#line 3456 "surface.c" /* yacc.c:1646  */
    break;

  case 150:
#line 758 "surface.yy" /* yacc.c:1646  */
    {
			}
#line 3463 "surface.c" /* yacc.c:1646  */
    break;

  case 151:
#line 761 "surface.yy" /* yacc.c:1646  */
    {
			}
#line 3470 "surface.c" /* yacc.c:1646  */
    break;

  case 152:
#line 764 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3476 "surface.c" /* yacc.c:1646  */
    break;

  case 153:
#line 765 "surface.yy" /* yacc.c:1646  */
    { CM->addSubsortDecl(tokenSequence); }
#line 3482 "surface.c" /* yacc.c:1646  */
    break;

  case 154:
#line 768 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	Fix things that might be in a bad state due
			  //	to a partially processed declaration.
			  //
			  cleanUpModuleExpression();
			  CM->makeOpDeclsConsistent();
			}
#line 3495 "surface.c" /* yacc.c:1646  */
    break;

  case 155:
#line 778 "surface.yy" /* yacc.c:1646  */
    {}
#line 3501 "surface.c" /* yacc.c:1646  */
    break;

  case 156:
#line 779 "surface.yy" /* yacc.c:1646  */
    {}
#line 3507 "surface.c" /* yacc.c:1646  */
    break;

  case 159:
#line 787 "surface.yy" /* yacc.c:1646  */
    {
			}
#line 3514 "surface.c" /* yacc.c:1646  */
    break;

  case 160:
#line 791 "surface.yy" /* yacc.c:1646  */
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); }
#line 3520 "surface.c" /* yacc.c:1646  */
    break;

  case 161:
#line 792 "surface.yy" /* yacc.c:1646  */
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); }
#line 3526 "surface.c" /* yacc.c:1646  */
    break;

  case 164:
#line 799 "surface.yy" /* yacc.c:1646  */
    { singleton[0] = (yyvsp[0].yyToken); CM->addOpDecl(singleton); }
#line 3532 "surface.c" /* yacc.c:1646  */
    break;

  case 165:
#line 800 "surface.yy" /* yacc.c:1646  */
    { CM->addOpDecl(lexerBubble); }
#line 3538 "surface.c" /* yacc.c:1646  */
    break;

  case 168:
#line 806 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber(lineNumber) <<
				       ": missing " << QUOTE("->") << " in constant declaration.");
			}
#line 3547 "surface.c" /* yacc.c:1646  */
    break;

  case 169:
#line 813 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
				       ": skipping stray " << QUOTE(":") << " in operator declaration.");

			}
#line 3557 "surface.c" /* yacc.c:1646  */
    break;

  case 170:
#line 818 "surface.yy" /* yacc.c:1646  */
    {}
#line 3563 "surface.c" /* yacc.c:1646  */
    break;

  case 172:
#line 823 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
#line 3572 "surface.c" /* yacc.c:1646  */
    break;

  case 173:
#line 828 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
#line 3581 "surface.c" /* yacc.c:1646  */
    break;

  case 174:
#line 835 "surface.yy" /* yacc.c:1646  */
    {
			  if ((yyvsp[-1].yyBool))
			    CM->convertSortsToKinds();
			}
#line 3590 "surface.c" /* yacc.c:1646  */
    break;

  case 176:
#line 842 "surface.yy" /* yacc.c:1646  */
    {}
#line 3596 "surface.c" /* yacc.c:1646  */
    break;

  case 177:
#line 845 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 3602 "surface.c" /* yacc.c:1646  */
    break;

  case 178:
#line 846 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 3608 "surface.c" /* yacc.c:1646  */
    break;

  case 180:
#line 850 "surface.yy" /* yacc.c:1646  */
    {}
#line 3614 "surface.c" /* yacc.c:1646  */
    break;

  case 181:
#line 854 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  currentSyntaxContainer->addType(false, singleton);
			}
#line 3623 "surface.c" /* yacc.c:1646  */
    break;

  case 182:
#line 858 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3629 "surface.c" /* yacc.c:1646  */
    break;

  case 183:
#line 860 "surface.yy" /* yacc.c:1646  */
    {
			  currentSyntaxContainer->addType(true, tokenSequence);
			}
#line 3637 "surface.c" /* yacc.c:1646  */
    break;

  case 184:
#line 865 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3643 "surface.c" /* yacc.c:1646  */
    break;

  case 185:
#line 866 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3649 "surface.c" /* yacc.c:1646  */
    break;

  case 186:
#line 869 "surface.yy" /* yacc.c:1646  */
    {}
#line 3655 "surface.c" /* yacc.c:1646  */
    break;

  case 187:
#line 870 "surface.yy" /* yacc.c:1646  */
    {}
#line 3661 "surface.c" /* yacc.c:1646  */
    break;

  case 190:
#line 878 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
			}
#line 3669 "surface.c" /* yacc.c:1646  */
    break;

  case 191:
#line 882 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::LEFT_ID);
			}
#line 3677 "surface.c" /* yacc.c:1646  */
    break;

  case 192:
#line 886 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::RIGHT_ID);
			}
#line 3685 "surface.c" /* yacc.c:1646  */
    break;

  case 193:
#line 892 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::ASSOC);
			}
#line 3693 "surface.c" /* yacc.c:1646  */
    break;

  case 194:
#line 896 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::COMM);
			}
#line 3701 "surface.c" /* yacc.c:1646  */
    break;

  case 195:
#line 899 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_RIGHT_BRACKET | BAR_OP_ATTRIBUTE, 1); }
#line 3707 "surface.c" /* yacc.c:1646  */
    break;

  case 196:
#line 900 "surface.yy" /* yacc.c:1646  */
    { CM->setIdentity(lexerBubble); }
#line 3713 "surface.c" /* yacc.c:1646  */
    break;

  case 197:
#line 902 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::IDEM);
			}
#line 3721 "surface.c" /* yacc.c:1646  */
    break;

  case 198:
#line 906 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::ITER);
			}
#line 3729 "surface.c" /* yacc.c:1646  */
    break;

  case 199:
#line 909 "surface.yy" /* yacc.c:1646  */
    { CM->setPrec((yyvsp[0].yyToken)); }
#line 3735 "surface.c" /* yacc.c:1646  */
    break;

  case 200:
#line 910 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3741 "surface.c" /* yacc.c:1646  */
    break;

  case 201:
#line 911 "surface.yy" /* yacc.c:1646  */
    { CM->setGather(tokenSequence); }
#line 3747 "surface.c" /* yacc.c:1646  */
    break;

  case 202:
#line 912 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3753 "surface.c" /* yacc.c:1646  */
    break;

  case 203:
#line 913 "surface.yy" /* yacc.c:1646  */
    { CM->setFormat(tokenSequence); }
#line 3759 "surface.c" /* yacc.c:1646  */
    break;

  case 204:
#line 914 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3765 "surface.c" /* yacc.c:1646  */
    break;

  case 205:
#line 915 "surface.yy" /* yacc.c:1646  */
    { CM->setStrat(tokenSequence); }
#line 3771 "surface.c" /* yacc.c:1646  */
    break;

  case 206:
#line 916 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3777 "surface.c" /* yacc.c:1646  */
    break;

  case 207:
#line 917 "surface.yy" /* yacc.c:1646  */
    { CM->setPoly(tokenSequence); }
#line 3783 "surface.c" /* yacc.c:1646  */
    break;

  case 208:
#line 919 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::MEMO);
			}
#line 3791 "surface.c" /* yacc.c:1646  */
    break;

  case 209:
#line 923 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::CTOR);
			}
#line 3799 "surface.c" /* yacc.c:1646  */
    break;

  case 210:
#line 927 "surface.yy" /* yacc.c:1646  */
    {
			  clear();
			  CM->setFrozen(tokenSequence);
			}
#line 3808 "surface.c" /* yacc.c:1646  */
    break;

  case 211:
#line 931 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3814 "surface.c" /* yacc.c:1646  */
    break;

  case 212:
#line 932 "surface.yy" /* yacc.c:1646  */
    { CM->setFrozen(tokenSequence); }
#line 3820 "surface.c" /* yacc.c:1646  */
    break;

  case 213:
#line 934 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::CONFIG);
			}
#line 3828 "surface.c" /* yacc.c:1646  */
    break;

  case 214:
#line 938 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::OBJECT);
			}
#line 3836 "surface.c" /* yacc.c:1646  */
    break;

  case 215:
#line 942 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
#line 3844 "surface.c" /* yacc.c:1646  */
    break;

  case 216:
#line 946 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setMetadata((yyvsp[0].yyToken));
			}
#line 3852 "surface.c" /* yacc.c:1646  */
    break;

  case 217:
#line 949 "surface.yy" /* yacc.c:1646  */
    { lexerLatexMode(); }
#line 3858 "surface.c" /* yacc.c:1646  */
    break;

  case 218:
#line 950 "surface.yy" /* yacc.c:1646  */
    { CM->setLatexMacro((yyvsp[-1].yyString)); }
#line 3864 "surface.c" /* yacc.c:1646  */
    break;

  case 219:
#line 951 "surface.yy" /* yacc.c:1646  */
    {}
#line 3870 "surface.c" /* yacc.c:1646  */
    break;

  case 220:
#line 953 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::DITTO);
			}
#line 3878 "surface.c" /* yacc.c:1646  */
    break;

  case 222:
#line 963 "surface.yy" /* yacc.c:1646  */
    {}
#line 3884 "surface.c" /* yacc.c:1646  */
    break;

  case 223:
#line 966 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3890 "surface.c" /* yacc.c:1646  */
    break;

  case 224:
#line 967 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3896 "surface.c" /* yacc.c:1646  */
    break;

  case 225:
#line 970 "surface.yy" /* yacc.c:1646  */
    {}
#line 3902 "surface.c" /* yacc.c:1646  */
    break;

  case 226:
#line 971 "surface.yy" /* yacc.c:1646  */
    {}
#line 3908 "surface.c" /* yacc.c:1646  */
    break;

  case 227:
#line 974 "surface.yy" /* yacc.c:1646  */
    { clear(); CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[0].yyToken), tokenSequence); }
#line 3914 "surface.c" /* yacc.c:1646  */
    break;

  case 228:
#line 975 "surface.yy" /* yacc.c:1646  */
    { CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 3920 "surface.c" /* yacc.c:1646  */
    break;

  case 229:
#line 976 "surface.yy" /* yacc.c:1646  */
    { CM->addHook(SyntacticPreModule::OP_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 3926 "surface.c" /* yacc.c:1646  */
    break;

  case 230:
#line 977 "surface.yy" /* yacc.c:1646  */
    { CM->addHook(SyntacticPreModule::TERM_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 3932 "surface.c" /* yacc.c:1646  */
    break;

  case 231:
#line 983 "surface.yy" /* yacc.c:1646  */
    {}
#line 3938 "surface.c" /* yacc.c:1646  */
    break;

  case 232:
#line 985 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing " <<
				       QUOTE("is") << " keyword.");
			}
#line 3947 "surface.c" /* yacc.c:1646  */
    break;

  case 233:
#line 991 "surface.yy" /* yacc.c:1646  */
    {}
#line 3953 "surface.c" /* yacc.c:1646  */
    break;

  case 234:
#line 993 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			}
#line 3961 "surface.c" /* yacc.c:1646  */
    break;

  case 235:
#line 1001 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3967 "surface.c" /* yacc.c:1646  */
    break;

  case 236:
#line 1002 "surface.yy" /* yacc.c:1646  */
    {}
#line 3973 "surface.c" /* yacc.c:1646  */
    break;

  case 237:
#line 1005 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3979 "surface.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1006 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3985 "surface.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1007 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3991 "surface.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1008 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3997 "surface.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1015 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  if (fragments.size() == 1)
			    t = fragments[0];
			  else
			    t.tokenize(Token::bubbleToPrefixNameCode(fragments), fragments[0].lineNumber());
			  fragClear();
			  (yyval.yyToken) = t;
			}
#line 4011 "surface.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1026 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4017 "surface.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1027 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4023 "surface.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1028 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4029 "surface.c" /* yacc.c:1646  */
    break;

  case 245:
#line 1031 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4035 "surface.c" /* yacc.c:1646  */
    break;

  case 246:
#line 1032 "surface.yy" /* yacc.c:1646  */
    {}
#line 4041 "surface.c" /* yacc.c:1646  */
    break;

  case 247:
#line 1033 "surface.yy" /* yacc.c:1646  */
    {}
#line 4047 "surface.c" /* yacc.c:1646  */
    break;

  case 350:
#line 1113 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 1); }
#line 4053 "surface.c" /* yacc.c:1646  */
    break;

  case 351:
#line 1115 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setCurrentModule(lexerBubble);
			}
#line 4061 "surface.c" /* yacc.c:1646  */
    break;

  case 352:
#line 1118 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 1); }
#line 4067 "surface.c" /* yacc.c:1646  */
    break;

  case 353:
#line 1120 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->dump();
			}
#line 4076 "surface.c" /* yacc.c:1646  */
    break;

  case 354:
#line 1125 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4085 "surface.c" /* yacc.c:1646  */
    break;

  case 355:
#line 1130 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.parse(lexerBubble);
			}
#line 4095 "surface.c" /* yacc.c:1646  */
    break;

  case 356:
#line 1137 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4104 "surface.c" /* yacc.c:1646  */
    break;

  case 357:
#line 1142 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.creduce(lexerBubble);
			}
#line 4114 "surface.c" /* yacc.c:1646  */
    break;

  case 358:
#line 1149 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4123 "surface.c" /* yacc.c:1646  */
    break;

  case 359:
#line 1154 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sreduce(lexerBubble);
			}
#line 4133 "surface.c" /* yacc.c:1646  */
    break;

  case 360:
#line 1161 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4142 "surface.c" /* yacc.c:1646  */
    break;

  case 361:
#line 1166 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.reduce(lexerBubble, (yyvsp[-3].yyBool));
			}
#line 4152 "surface.c" /* yacc.c:1646  */
    break;

  case 362:
#line 1173 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4162 "surface.c" /* yacc.c:1646  */
    break;

  case 363:
#line 1179 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.rewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4172 "surface.c" /* yacc.c:1646  */
    break;

  case 364:
#line 1185 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4183 "surface.c" /* yacc.c:1646  */
    break;

  case 365:
#line 1192 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.eRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			}
#line 4193 "surface.c" /* yacc.c:1646  */
    break;

  case 366:
#line 1198 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4204 "surface.c" /* yacc.c:1646  */
    break;

  case 367:
#line 1205 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.fRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			}
#line 4214 "surface.c" /* yacc.c:1646  */
    break;

  case 368:
#line 1211 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4224 "surface.c" /* yacc.c:1646  */
    break;

  case 369:
#line 1217 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4234 "surface.c" /* yacc.c:1646  */
    break;

  case 370:
#line 1224 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4243 "surface.c" /* yacc.c:1646  */
    break;

  case 371:
#line 1229 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.check(lexerBubble);
			}
#line 4253 "surface.c" /* yacc.c:1646  */
    break;

  case 372:
#line 1236 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4264 "surface.c" /* yacc.c:1646  */
    break;

  case 373:
#line 1243 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(lexerBubble, number, number2, (yyvsp[-2].yySearchKind), (yyvsp[-3].yyBool));
			}
#line 4274 "surface.c" /* yacc.c:1646  */
    break;

  case 374:
#line 1249 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4284 "surface.c" /* yacc.c:1646  */
    break;

  case 375:
#line 1255 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.match(lexerBubble, (yyvsp[-2].yyBool), number);
			}
#line 4294 "surface.c" /* yacc.c:1646  */
    break;

  case 376:
#line 1261 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4304 "surface.c" /* yacc.c:1646  */
    break;

  case 377:
#line 1267 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.unify(lexerBubble, number);
			}
#line 4314 "surface.c" /* yacc.c:1646  */
    break;

  case 378:
#line 1273 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4324 "surface.c" /* yacc.c:1646  */
    break;

  case 379:
#line 1279 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantUnify(lexerBubble, number, (yyvsp[-4].yyBool));
			}
#line 4334 "surface.c" /* yacc.c:1646  */
    break;

  case 380:
#line 1286 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4344 "surface.c" /* yacc.c:1646  */
    break;

  case 381:
#line 1292 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.getVariants(lexerBubble, number, (yyvsp[-3].yyBool), (yyvsp[-5].yyBool));
			}
#line 4354 "surface.c" /* yacc.c:1646  */
    break;

  case 382:
#line 1298 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.cont((yyvsp[-1].yyInt64), (yyvsp[-3].yyBool));
			}
#line 4362 "surface.c" /* yacc.c:1646  */
    break;

  case 383:
#line 1302 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	test is a generic command to call code with a term for development purposes.
			  //
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4374 "surface.c" /* yacc.c:1646  */
    break;

  case 384:
#line 1310 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.test(lexerBubble);
			    //interpreter.newNarrow(lexerBubble);

			}
#line 4386 "surface.c" /* yacc.c:1646  */
    break;

  case 385:
#line 1318 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4395 "surface.c" /* yacc.c:1646  */
    break;

  case 386:
#line 1323 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.loop(lexerBubble);
			}
#line 4405 "surface.c" /* yacc.c:1646  */
    break;

  case 387:
#line 1329 "surface.yy" /* yacc.c:1646  */
    {
			  moduleExpr.contractTo(0);
			  if (interpreter.setCurrentModule(moduleExpr))  // HACK
			    interpreter.contLoop(lexerBubble);
			}
#line 4415 "surface.c" /* yacc.c:1646  */
    break;

  case 388:
#line 1334 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4421 "surface.c" /* yacc.c:1646  */
    break;

  case 389:
#line 1336 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.traceSelect((yyvsp[-2].yyBool));
			}
#line 4430 "surface.c" /* yacc.c:1646  */
    break;

  case 390:
#line 1340 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4436 "surface.c" /* yacc.c:1646  */
    break;

  case 391:
#line 1342 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.traceExclude((yyvsp[-2].yyBool));
			}
#line 4445 "surface.c" /* yacc.c:1646  */
    break;

  case 392:
#line 1346 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4451 "surface.c" /* yacc.c:1646  */
    break;

  case 393:
#line 1348 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.breakSelect((yyvsp[-2].yyBool));
			}
#line 4460 "surface.c" /* yacc.c:1646  */
    break;

  case 394:
#line 1352 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4466 "surface.c" /* yacc.c:1646  */
    break;

  case 395:
#line 1354 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.printConceal((yyvsp[-2].yyBool));
			}
#line 4475 "surface.c" /* yacc.c:1646  */
    break;

  case 396:
#line 1359 "surface.yy" /* yacc.c:1646  */
    {
			  if (CM != 0)  // HACK
			    CM->getFlatSignature()->clearMemo();
			}
#line 4484 "surface.c" /* yacc.c:1646  */
    break;

  case 397:
#line 1366 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4490 "surface.c" /* yacc.c:1646  */
    break;

  case 398:
#line 1368 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
#line 4499 "surface.c" /* yacc.c:1646  */
    break;

  case 399:
#line 1372 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4505 "surface.c" /* yacc.c:1646  */
    break;

  case 400:
#line 1374 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
#line 4514 "surface.c" /* yacc.c:1646  */
    break;

  case 401:
#line 1378 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4520 "surface.c" /* yacc.c:1646  */
    break;

  case 402:
#line 1380 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showModule(true);
			}
#line 4529 "surface.c" /* yacc.c:1646  */
    break;

  case 403:
#line 1384 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4535 "surface.c" /* yacc.c:1646  */
    break;

  case 404:
#line 1386 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentView(lexerBubble))
			    interpreter.showView();
			}
#line 4544 "surface.c" /* yacc.c:1646  */
    break;

  case 405:
#line 1391 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showModules(true);
			}
#line 4552 "surface.c" /* yacc.c:1646  */
    break;

  case 406:
#line 1395 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showNamedViews();
			}
#line 4560 "surface.c" /* yacc.c:1646  */
    break;

  case 407:
#line 1398 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4566 "surface.c" /* yacc.c:1646  */
    break;

  case 408:
#line 1400 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSortsAndSubsorts();
			}
#line 4575 "surface.c" /* yacc.c:1646  */
    break;

  case 409:
#line 1404 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4581 "surface.c" /* yacc.c:1646  */
    break;

  case 410:
#line 1406 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showOps();
			}
#line 4590 "surface.c" /* yacc.c:1646  */
    break;

  case 411:
#line 1410 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4596 "surface.c" /* yacc.c:1646  */
    break;

  case 412:
#line 1412 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showVars();
			}
#line 4605 "surface.c" /* yacc.c:1646  */
    break;

  case 413:
#line 1416 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4611 "surface.c" /* yacc.c:1646  */
    break;

  case 414:
#line 1418 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showMbs();
			}
#line 4620 "surface.c" /* yacc.c:1646  */
    break;

  case 415:
#line 1422 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4626 "surface.c" /* yacc.c:1646  */
    break;

  case 416:
#line 1424 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showEqs();
			}
#line 4635 "surface.c" /* yacc.c:1646  */
    break;

  case 417:
#line 1428 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4641 "surface.c" /* yacc.c:1646  */
    break;

  case 418:
#line 1430 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showRls();
			}
#line 4650 "surface.c" /* yacc.c:1646  */
    break;

  case 419:
#line 1434 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4656 "surface.c" /* yacc.c:1646  */
    break;

  case 420:
#line 1436 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSummary();
			}
#line 4665 "surface.c" /* yacc.c:1646  */
    break;

  case 421:
#line 1440 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4671 "surface.c" /* yacc.c:1646  */
    break;

  case 422:
#line 1442 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showKinds();
			}
#line 4680 "surface.c" /* yacc.c:1646  */
    break;

  case 423:
#line 1447 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showSearchPath((yyvsp[-1].yyInt64));
			}
#line 4688 "surface.c" /* yacc.c:1646  */
    break;

  case 424:
#line 1451 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showSearchPathLabels((yyvsp[-1].yyInt64));
			}
#line 4696 "surface.c" /* yacc.c:1646  */
    break;

  case 425:
#line 1455 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showSearchGraph();
			}
#line 4704 "surface.c" /* yacc.c:1646  */
    break;

  case 426:
#line 1458 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4710 "surface.c" /* yacc.c:1646  */
    break;

  case 427:
#line 1460 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showProfile();
			}
#line 4719 "surface.c" /* yacc.c:1646  */
    break;

  case 428:
#line 1468 "surface.yy" /* yacc.c:1646  */
    {
			  globalAdvisoryFlag = alwaysAdviseFlag ? true : (yyvsp[-1].yyBool);
			}
#line 4727 "surface.c" /* yacc.c:1646  */
    break;

  case 429:
#line 1472 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			}
#line 4735 "surface.c" /* yacc.c:1646  */
    break;

  case 430:
#line 1476 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_STATS, (yyvsp[-1].yyBool));
			}
#line 4743 "surface.c" /* yacc.c:1646  */
    break;

  case 431:
#line 1480 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_TIMING, (yyvsp[-1].yyBool));
			}
#line 4751 "surface.c" /* yacc.c:1646  */
    break;

  case 432:
#line 1484 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_BREAKDOWN, (yyvsp[-1].yyBool));
			}
#line 4759 "surface.c" /* yacc.c:1646  */
    break;

  case 433:
#line 1488 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_TIMING, (yyvsp[-1].yyBool));
			}
#line 4767 "surface.c" /* yacc.c:1646  */
    break;

  case 434:
#line 1492 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_COMMAND, (yyvsp[-1].yyBool));
			}
#line 4775 "surface.c" /* yacc.c:1646  */
    break;

  case 435:
#line 1496 "surface.yy" /* yacc.c:1646  */
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			}
#line 4783 "surface.c" /* yacc.c:1646  */
    break;

  case 436:
#line 1500 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setPrintFlag((yyvsp[-2].yyPrintFlags), (yyvsp[-1].yyBool));
			}
#line 4791 "surface.c" /* yacc.c:1646  */
    break;

  case 437:
#line 1504 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE, (yyvsp[-1].yyBool));
			}
#line 4799 "surface.c" /* yacc.c:1646  */
    break;

  case 438:
#line 1508 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE_NEWLINE, (yyvsp[-1].yyBool));
			}
#line 4807 "surface.c" /* yacc.c:1646  */
    break;

  case 439:
#line 1512 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag((yyvsp[-2].yyFlags), (yyvsp[-1].yyBool));
			}
#line 4815 "surface.c" /* yacc.c:1646  */
    break;

  case 440:
#line 1516 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::BREAK, (yyvsp[-1].yyBool));
			}
#line 4823 "surface.c" /* yacc.c:1646  */
    break;

  case 441:
#line 1519 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4829 "surface.c" /* yacc.c:1646  */
    break;

  case 442:
#line 1520 "surface.yy" /* yacc.c:1646  */
    { lexerInitialMode(); }
#line 4835 "surface.c" /* yacc.c:1646  */
    break;

  case 443:
#line 1522 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setAutoImport((yyvsp[-5].yyImportMode), (yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			}
#line 4843 "surface.c" /* yacc.c:1646  */
    break;

  case 444:
#line 1525 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4849 "surface.c" /* yacc.c:1646  */
    break;

  case 445:
#line 1526 "surface.yy" /* yacc.c:1646  */
    { lexerInitialMode(); }
#line 4855 "surface.c" /* yacc.c:1646  */
    break;

  case 446:
#line 1528 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setOmodInclude((yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			}
#line 4863 "surface.c" /* yacc.c:1646  */
    break;

  case 447:
#line 1532 "surface.yy" /* yacc.c:1646  */
    {
			  globalVerboseFlag = (yyvsp[-1].yyBool);
			}
#line 4871 "surface.c" /* yacc.c:1646  */
    break;

  case 448:
#line 1536 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_MEMO, (yyvsp[-1].yyBool));
			}
#line 4879 "surface.c" /* yacc.c:1646  */
    break;

  case 449:
#line 1540 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_RULES, (yyvsp[-1].yyBool));
			}
#line 4887 "surface.c" /* yacc.c:1646  */
    break;

  case 450:
#line 1544 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::COMPILE_COUNT, (yyvsp[-1].yyBool));
			}
#line 4895 "surface.c" /* yacc.c:1646  */
    break;

  case 451:
#line 1548 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::PROFILE, (yyvsp[-1].yyBool));
			}
#line 4903 "surface.c" /* yacc.c:1646  */
    break;

  case 452:
#line 1552 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_PROFILE, (yyvsp[-1].yyBool));
			}
#line 4911 "surface.c" /* yacc.c:1646  */
    break;

  case 453:
#line 1559 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::RESUME;
			}
#line 4919 "surface.c" /* yacc.c:1646  */
    break;

  case 454:
#line 1563 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::ABORT;
			}
#line 4927 "surface.c" /* yacc.c:1646  */
    break;

  case 455:
#line 1567 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			}
#line 4935 "surface.c" /* yacc.c:1646  */
    break;

  case 456:
#line 1571 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::WHERE;
			}
#line 4943 "surface.c" /* yacc.c:1646  */
    break;

  case 457:
#line 1578 "surface.yy" /* yacc.c:1646  */
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			}
#line 4951 "surface.c" /* yacc.c:1646  */
    break;

  case 458:
#line 1582 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			}
#line 4959 "surface.c" /* yacc.c:1646  */
    break;

  case 459:
#line 1588 "surface.yy" /* yacc.c:1646  */
    { lexerInitialMode(); }
#line 4965 "surface.c" /* yacc.c:1646  */
    break;

  case 461:
#line 1595 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_MIXFIX; }
#line 4971 "surface.c" /* yacc.c:1646  */
    break;

  case 462:
#line 1596 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FLAT; }
#line 4977 "surface.c" /* yacc.c:1646  */
    break;

  case 463:
#line 1597 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_ALIASES; }
#line 4983 "surface.c" /* yacc.c:1646  */
    break;

  case 464:
#line 1598 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_PARENS; }
#line 4989 "surface.c" /* yacc.c:1646  */
    break;

  case 465:
#line 1599 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_GRAPH; }
#line 4995 "surface.c" /* yacc.c:1646  */
    break;

  case 466:
#line 1600 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_CONCEAL; }
#line 5001 "surface.c" /* yacc.c:1646  */
    break;

  case 467:
#line 1601 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_NUMBER; }
#line 5007 "surface.c" /* yacc.c:1646  */
    break;

  case 468:
#line 1602 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_RAT; }
#line 5013 "surface.c" /* yacc.c:1646  */
    break;

  case 469:
#line 1603 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_COLOR; }
#line 5019 "surface.c" /* yacc.c:1646  */
    break;

  case 470:
#line 1604 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FORMAT; }
#line 5025 "surface.c" /* yacc.c:1646  */
    break;

  case 471:
#line 1607 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE; }
#line 5031 "surface.c" /* yacc.c:1646  */
    break;

  case 472:
#line 1608 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_CONDITION; }
#line 5037 "surface.c" /* yacc.c:1646  */
    break;

  case 473:
#line 1609 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_WHOLE; }
#line 5043 "surface.c" /* yacc.c:1646  */
    break;

  case 474:
#line 1610 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_SUBSTITUTION; }
#line 5049 "surface.c" /* yacc.c:1646  */
    break;

  case 475:
#line 1611 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_SELECT; }
#line 5055 "surface.c" /* yacc.c:1646  */
    break;

  case 476:
#line 1612 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_MB; }
#line 5061 "surface.c" /* yacc.c:1646  */
    break;

  case 477:
#line 1613 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_EQ; }
#line 5067 "surface.c" /* yacc.c:1646  */
    break;

  case 478:
#line 1614 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_RL; }
#line 5073 "surface.c" /* yacc.c:1646  */
    break;

  case 479:
#line 1615 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_REWRITE; }
#line 5079 "surface.c" /* yacc.c:1646  */
    break;

  case 480:
#line 1616 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_BODY; }
#line 5085 "surface.c" /* yacc.c:1646  */
    break;

  case 481:
#line 1617 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_BUILTIN; }
#line 5091 "surface.c" /* yacc.c:1646  */
    break;

  case 482:
#line 1620 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5097 "surface.c" /* yacc.c:1646  */
    break;

  case 483:
#line 1621 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5103 "surface.c" /* yacc.c:1646  */
    break;

  case 484:
#line 1624 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5109 "surface.c" /* yacc.c:1646  */
    break;

  case 485:
#line 1625 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5115 "surface.c" /* yacc.c:1646  */
    break;

  case 486:
#line 1628 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5121 "surface.c" /* yacc.c:1646  */
    break;

  case 487:
#line 1629 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5127 "surface.c" /* yacc.c:1646  */
    break;

  case 488:
#line 1632 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5133 "surface.c" /* yacc.c:1646  */
    break;

  case 489:
#line 1633 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5139 "surface.c" /* yacc.c:1646  */
    break;

  case 490:
#line 1638 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::NARROW; }
#line 5145 "surface.c" /* yacc.c:1646  */
    break;

  case 491:
#line 1639 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::XG_NARROW; }
#line 5151 "surface.c" /* yacc.c:1646  */
    break;

  case 492:
#line 1640 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::SEARCH; }
#line 5157 "surface.c" /* yacc.c:1646  */
    break;

  case 493:
#line 1641 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::SMT_SEARCH; }
#line 5163 "surface.c" /* yacc.c:1646  */
    break;

  case 494:
#line 1642 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::VU_NARROW; }
#line 5169 "surface.c" /* yacc.c:1646  */
    break;

  case 495:
#line 1643 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::FVU_NARROW; }
#line 5175 "surface.c" /* yacc.c:1646  */
    break;

  case 496:
#line 1646 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5181 "surface.c" /* yacc.c:1646  */
    break;

  case 497:
#line 1647 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5187 "surface.c" /* yacc.c:1646  */
    break;

  case 498:
#line 1650 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5193 "surface.c" /* yacc.c:1646  */
    break;

  case 499:
#line 1651 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5199 "surface.c" /* yacc.c:1646  */
    break;

  case 500:
#line 1654 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5205 "surface.c" /* yacc.c:1646  */
    break;

  case 501:
#line 1655 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5211 "surface.c" /* yacc.c:1646  */
    break;

  case 502:
#line 1658 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyInt64) = (yyvsp[0].yyInt64); }
#line 5217 "surface.c" /* yacc.c:1646  */
    break;

  case 503:
#line 1659 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyInt64) = NONE; }
#line 5223 "surface.c" /* yacc.c:1646  */
    break;

  case 504:
#line 1662 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyImportMode) = ImportModule::PROTECTING; }
#line 5229 "surface.c" /* yacc.c:1646  */
    break;

  case 505:
#line 1663 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyImportMode) = ImportModule::EXTENDING; }
#line 5235 "surface.c" /* yacc.c:1646  */
    break;

  case 506:
#line 1664 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyImportMode) = ImportModule::INCLUDING; }
#line 5241 "surface.c" /* yacc.c:1646  */
    break;

  case 508:
#line 1674 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5247 "surface.c" /* yacc.c:1646  */
    break;

  case 510:
#line 1678 "surface.yy" /* yacc.c:1646  */
    { moduleExpr = lexerBubble; lexBubble(END_COMMAND, 1); }
#line 5253 "surface.c" /* yacc.c:1646  */
    break;

  case 513:
#line 1690 "surface.yy" /* yacc.c:1646  */
    { lexSave((yyvsp[0].yyToken)); }
#line 5259 "surface.c" /* yacc.c:1646  */
    break;

  case 516:
#line 1693 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0);  }
#line 5265 "surface.c" /* yacc.c:1646  */
    break;

  case 518:
#line 1698 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5271 "surface.c" /* yacc.c:1646  */
    break;

  case 520:
#line 1700 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5277 "surface.c" /* yacc.c:1646  */
    break;

  case 523:
#line 1706 "surface.yy" /* yacc.c:1646  */
    { number = Token::codeToInt64(lexerBubble[1].code()); }
#line 5283 "surface.c" /* yacc.c:1646  */
    break;

  case 525:
#line 1708 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5289 "surface.c" /* yacc.c:1646  */
    break;

  case 528:
#line 1723 "surface.yy" /* yacc.c:1646  */
    { lexSave((yyvsp[0].yyToken)); }
#line 5295 "surface.c" /* yacc.c:1646  */
    break;

  case 531:
#line 1726 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5301 "surface.c" /* yacc.c:1646  */
    break;

  case 533:
#line 1735 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5307 "surface.c" /* yacc.c:1646  */
    break;

  case 535:
#line 1737 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5313 "surface.c" /* yacc.c:1646  */
    break;

  case 537:
#line 1739 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5319 "surface.c" /* yacc.c:1646  */
    break;

  case 540:
#line 1749 "surface.yy" /* yacc.c:1646  */
    { number = Token::codeToInt64(lexerBubble[1].code()); }
#line 5325 "surface.c" /* yacc.c:1646  */
    break;

  case 542:
#line 1751 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5331 "surface.c" /* yacc.c:1646  */
    break;

  case 544:
#line 1753 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5337 "surface.c" /* yacc.c:1646  */
    break;

  case 547:
#line 1763 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5343 "surface.c" /* yacc.c:1646  */
    break;

  case 549:
#line 1765 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5349 "surface.c" /* yacc.c:1646  */
    break;

  case 552:
#line 1776 "surface.yy" /* yacc.c:1646  */
    {
			  number = Token::codeToInt64(lexerBubble[1].code());
			  number2 = Token::codeToInt64(lexerBubble[3].code());
			  clear();
			}
#line 5359 "surface.c" /* yacc.c:1646  */
    break;

  case 554:
#line 1782 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5365 "surface.c" /* yacc.c:1646  */
    break;

  case 557:
#line 1792 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5371 "surface.c" /* yacc.c:1646  */
    break;

  case 559:
#line 1794 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5377 "surface.c" /* yacc.c:1646  */
    break;

  case 562:
#line 1804 "surface.yy" /* yacc.c:1646  */
    { number2 = Token::codeToInt64(lexerBubble[2].code()); }
#line 5383 "surface.c" /* yacc.c:1646  */
    break;

  case 564:
#line 1806 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5389 "surface.c" /* yacc.c:1646  */
    break;

  case 567:
#line 1811 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0, 1); }
#line 5395 "surface.c" /* yacc.c:1646  */
    break;

  case 570:
#line 1817 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 1, 1); }
#line 5401 "surface.c" /* yacc.c:1646  */
    break;

  case 572:
#line 1819 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON | END_COMMAND, 0); }
#line 5407 "surface.c" /* yacc.c:1646  */
    break;

  case 574:
#line 1822 "surface.yy" /* yacc.c:1646  */
    {
			  lexerBubble.resize(1);
			  lexerBubble[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(lexerBubble[0]);
			}
#line 5417 "surface.c" /* yacc.c:1646  */
    break;

  case 612:
#line 1861 "surface.yy" /* yacc.c:1646  */
    {}
#line 5423 "surface.c" /* yacc.c:1646  */
    break;

  case 614:
#line 1866 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  interpreter.addSelected(singleton);
			}
#line 5433 "surface.c" /* yacc.c:1646  */
    break;

  case 617:
#line 1877 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  interpreter.addSelected(singleton);
			}
#line 5442 "surface.c" /* yacc.c:1646  */
    break;

  case 618:
#line 1882 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.addSelected(lexerBubble);
			}
#line 5450 "surface.c" /* yacc.c:1646  */
    break;


#line 5454 "surface.c" /* yacc.c:1646  */
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
      yyerror (parseResult, YY_("syntax error"));
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
        yyerror (parseResult, yymsgp);
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
                      yytoken, &yylval, parseResult);
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
                  yystos[yystate], yyvsp, parseResult);
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
  yyerror (parseResult, YY_("memory exhausted"));
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
                  yytoken, &yylval, parseResult);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, parseResult);
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
#line 1912 "surface.yy" /* yacc.c:1906  */


static void
yyerror(UserLevelRewritingContext::ParseResult* /*parseResult*/, char *s)
{
  if (!(UserLevelRewritingContext::interrupted()))
    IssueWarning(LineNumber(lineNumber) << ": " << s);
}

void
cleanUpModuleExpression()
{
  //
  //	Delete pieces of a partly built module expression.
  //
  delete currentRenaming;
  currentRenaming = 0;
  while (!moduleExpressions.empty())
    {
      moduleExpressions.top()->deepSelfDestruct();
      moduleExpressions.pop();
    }
}

void
cleanUpParser()
{
  interpreter.makeClean(lineNumber);
}

void
missingSpace(const Token& token)
{
  IssueWarning(LineNumber(token.lineNumber()) << ": missing space between " <<
	       QUOTE(token) << " and period.");
}

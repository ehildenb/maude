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
#define YYLAST   2437

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  178
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  247
/* YYNRULES -- Number of rules.  */
#define YYNRULES  624
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  933

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
     498,   505,   507,   515,   505,   528,   529,   532,   537,   540,
     544,   545,   545,   547,   550,   551,   561,   569,   560,   578,
     577,   610,   611,   620,   620,   627,   629,   627,   642,   643,
     652,   653,   656,   657,   660,   668,   669,   677,   686,   687,
     690,   690,   693,   694,   697,   704,   704,   707,   707,   710,
     711,   710,   714,   716,   718,   719,   718,   722,   723,   724,
     722,   727,   728,   727,   731,   732,   733,   731,   736,   737,
     736,   740,   741,   742,   740,   745,   746,   745,   749,   755,
     754,   761,   761,   764,   775,   776,   779,   780,   783,   788,
     789,   792,   793,   796,   797,   800,   801,   802,   809,   815,
     818,   819,   824,   831,   838,   839,   842,   843,   846,   847,
     850,   855,   855,   862,   863,   866,   867,   870,   871,   874,
     878,   882,   888,   892,   896,   896,   898,   902,   906,   907,
     907,   909,   909,   911,   911,   913,   913,   915,   919,   923,
     928,   928,   930,   934,   938,   942,   946,   946,   948,   949,
     959,   960,   963,   964,   967,   968,   971,   972,   973,   974,
     980,   982,   988,   990,   998,   999,  1002,  1003,  1004,  1004,
    1011,  1023,  1024,  1024,  1028,  1028,  1030,  1036,  1036,  1036,
    1036,  1036,  1039,  1039,  1039,  1040,  1040,  1040,  1043,  1043,
    1043,  1043,  1044,  1044,  1044,  1044,  1044,  1044,  1047,  1047,
    1047,  1048,  1048,  1048,  1048,  1049,  1049,  1049,  1049,  1049,
    1052,  1052,  1058,  1058,  1058,  1058,  1058,  1058,  1058,  1058,
    1059,  1059,  1062,  1062,  1062,  1062,  1065,  1065,  1068,  1068,
    1068,  1068,  1068,  1068,  1069,  1069,  1069,  1070,  1070,  1070,
    1070,  1070,  1070,  1070,  1070,  1073,  1073,  1073,  1073,  1073,
    1073,  1073,  1076,  1076,  1076,  1079,  1079,  1079,  1079,  1079,
    1079,  1079,  1080,  1080,  1080,  1080,  1080,  1080,  1081,  1081,
    1081,  1081,  1082,  1082,  1082,  1082,  1083,  1083,  1083,  1110,
    1110,  1115,  1115,  1122,  1121,  1134,  1133,  1146,  1145,  1158,
    1157,  1170,  1169,  1182,  1181,  1195,  1194,  1208,  1207,  1221,
    1220,  1233,  1232,  1246,  1245,  1258,  1257,  1270,  1269,  1283,
    1282,  1294,  1299,  1298,  1315,  1314,  1325,  1331,  1331,  1337,
    1337,  1343,  1343,  1349,  1349,  1355,  1363,  1363,  1369,  1369,
    1375,  1375,  1381,  1381,  1387,  1391,  1395,  1395,  1401,  1401,
    1407,  1407,  1413,  1413,  1419,  1419,  1425,  1425,  1431,  1431,
    1437,  1437,  1443,  1447,  1451,  1455,  1455,  1464,  1468,  1472,
    1476,  1480,  1484,  1488,  1492,  1496,  1500,  1504,  1508,  1512,
    1516,  1517,  1516,  1522,  1523,  1522,  1528,  1532,  1536,  1540,
    1544,  1548,  1555,  1559,  1563,  1567,  1574,  1578,  1585,  1585,
    1592,  1593,  1594,  1595,  1596,  1597,  1598,  1599,  1600,  1601,
    1604,  1605,  1606,  1607,  1608,  1609,  1610,  1611,  1612,  1613,
    1614,  1617,  1618,  1621,  1622,  1625,  1626,  1629,  1630,  1635,
    1636,  1637,  1638,  1639,  1640,  1643,  1644,  1647,  1648,  1651,
    1652,  1655,  1656,  1659,  1660,  1661,  1670,  1671,  1671,  1675,
    1675,  1677,  1687,  1687,  1689,  1690,  1690,  1695,  1695,  1697,
    1697,  1699,  1703,  1703,  1705,  1705,  1707,  1720,  1720,  1722,
    1723,  1723,  1732,  1732,  1734,  1734,  1736,  1736,  1738,  1746,
    1746,  1748,  1748,  1750,  1750,  1752,  1760,  1760,  1762,  1762,
    1764,  1773,  1772,  1779,  1779,  1781,  1789,  1789,  1791,  1791,
    1793,  1801,  1801,  1803,  1803,  1805,  1808,  1808,  1810,  1814,
    1814,  1816,  1816,  1818,  1829,  1829,  1829,  1829,  1829,  1829,
    1829,  1832,  1832,  1832,  1832,  1832,  1832,  1836,  1836,  1836,
    1836,  1836,  1836,  1840,  1840,  1840,  1840,  1840,  1840,  1844,
    1844,  1844,  1844,  1844,  1848,  1848,  1848,  1848,  1848,  1854,
    1855,  1858,  1859,  1862,  1869,  1870,  1873,  1878,  1885,  1885,
    1885,  1885,  1885,  1885,  1885
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
  "$@18", "$@19", "viewDecList", "skipStrayArrow", "viewDeclaration",
  "$@20", "sortDot", "viewEndOpMap", "$@21", "$@22", "$@23", "endBubble",
  "parenBubble", "$@24", "module", "$@25", "$@26", "dot", "parameters",
  "parameterList", "parameter", "colon2", "badType", "typeDot",
  "startModule", "decList", "declaration", "$@27", "$@28", "$@29", "$@30",
  "$@31", "$@32", "$@33", "$@34", "$@35", "$@36", "$@37", "$@38", "$@39",
  "$@40", "$@41", "$@42", "$@43", "$@44", "$@45", "$@46", "$@47", "$@48",
  "$@49", "classDef", "cPairList", "cPair", "varNameList", "opNameList",
  "simpleOpName", "domainRangeAttr", "skipStrayColon", "dra2", "rangeAttr",
  "typeAttr", "arrow", "typeList", "typeName", "$@50", "sortNames",
  "attributes", "attributeList", "idKeyword", "attribute", "$@51", "$@52",
  "$@53", "$@54", "$@55", "$@56", "$@57", "identity", "idList", "hookList",
  "hook", "expectedIs", "expectedDot", "sortNameList", "subsortList",
  "$@58", "sortName", "sortNameFrag", "$@59", "sortNameFrags", "$@60",
  "token", "tokenBarDot", "tokenBarColon", "sortToken", "endsInDot",
  "inert", "identifier", "startKeyword", "startKeyword2", "midKeyword",
  "attrKeyword", "attrKeyword2", "command", "$@61", "$@62", "$@63", "$@64",
  "$@65", "$@66", "$@67", "$@68", "$@69", "$@70", "$@71", "$@72", "$@73",
  "$@74", "$@75", "$@76", "$@77", "$@78", "$@79", "$@80", "$@81", "$@82",
  "$@83", "$@84", "$@85", "$@86", "$@87", "$@88", "$@89", "$@90", "$@91",
  "$@92", "$@93", "$@94", "$@95", "$@96", "$@97", "$@98", "$@99", "$@100",
  "printOption", "traceOption", "polarity", "select", "exclude", "conceal",
  "search", "match", "optDebug", "optIrredundant", "optNumber",
  "importMode", "moduleAndTerm", "$@101", "inEnd", "$@102",
  "numberModuleTerm", "$@103", "$@104", "numberModuleTerm1", "$@105",
  "$@106", "numberModuleTerm2", "$@107", "$@108", "numbersModuleTerm",
  "$@109", "$@110", "numbersModuleTerm1", "$@111", "$@112", "$@113",
  "numbersModuleTerm2", "$@114", "$@115", "$@116", "numbersModuleTerm3",
  "$@117", "$@118", "numbersModuleTerm4", "$@119", "$@120",
  "numbersModuleTerm5", "$@121", "$@122", "numbersModuleTerm6", "$@123",
  "$@124", "miscEndBubble", "$@125", "initialEndBubble", "$@126", "$@127",
  "cTokenBarIn", "cTokenBarLeftIn", "cTokenBarDotNumber",
  "cTokenBarDotRight", "cTokenBarDotCommaNumber", "cTokenBarDotCommaRight",
  "opSelect", "endSelect", "badSelect", "cOpNameList", "cSimpleOpName",
  "cSimpleTokenBarDot", YY_NULLPTR
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

#define YYPACT_NINF -748

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-748)))

#define YYTABLE_NINF -499

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     763,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,   253,   588,   151,  -748,     5,    75,   -19,  -748,   -40,
      -5,    11,    50,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,   128,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,   486,    90,  1078,
     247,   247,   341,   247,   153,   109,   404,    60,   192,   239,
     239,   239,   -20,  -748,  -748,  -748,   155,   239,  -748,  -748,
    -748,   205,   -49,  -748,   115,   118,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,   -78,  -748,  -748,  -748,  -748,    99,  -748,  -748,
    -748,  -748,   247,   247,   -78,   271,   281,   285,   302,   308,
     247,   187,   321,  -748,  1078,   341,  -748,  -748,  -748,  -748,
     260,   326,  -748,  -748,  -748,   252,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,   222,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,   283,   239,   239,   239,   239,   239,   239,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,   239,  -748,
    -748,  -748,    30,   292,  -748,  -748,  -748,  -748,  -748,   239,
     239,  -748,  -748,   219,   227,   228,   239,   239,   239,   239,
     230,   -58,   -78,   -78,   237,   240,   280,   -78,  -748,  -748,
     -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,
    2238,  2238,  -748,  -748,  -748,  2238,  2238,   242,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,   247,   341,  -748,   358,  -748,   413,   413,   341,  -748,
     246,   413,   802,   -78,   -88,   -78,   537,   -78,   -58,   239,
     239,   258,   268,   269,   272,   274,   275,   276,   239,   278,
    -748,  -748,   279,   282,  -748,  -748,  -748,   284,   289,   291,
     293,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,   294,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
     548,  -748,  -748,  -748,  -748,  -748,  -748,   241,  -748,  -748,
    -748,   341,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
     802,   262,   273,   287,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,   295,   299,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,   300,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,   239,  -748,  -748,  -748,  -748,  -748,
    1078,   316,   341,  -748,   557,   -78,   -24,   802,   303,   313,
    -748,   -78,   -78,  2147,   -78,   239,  -748,  -748,  -748,   314,
      29,  -748,   -80,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,   802,
    -748,  -748,  1078,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,   317,  -748,  1078,  -748,
    -748,  -748,   802,   461,  2156,  2165,   -78,   262,    25,   147,
    -748,   247,   -78,  -748,  -748,   262,   319,  -748,   871,  -748,
    -748,  -748,  1147,  1147,  1354,  1078,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
     316,  -748,  2105,  -748,  1078,  -748,   288,  -748,  1078,  -748,
    -748,  -748,  -748,   320,   173,   206,   146,  -748,   304,    35,
     152,   185,   322,   323,   212,   223,   245,   251,  -748,  2105,
     325,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,   940,
    -748,  -748,  -748,  -748,  -748,  1009,  1216,  -748,  -748,  2105,
     328,   330,   333,   339,   347,   349,   350,  2174,   -78,   247,
    2220,   -78,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,   309,   336,  -748,  -748,  -748,   353,   343,
     352,   -55,  -748,   871,  -748,  -748,  -748,  -748,  -748,  -748,
    1630,  1561,  -748,  -748,  1423,  -748,  1423,   364,  -748,   354,
    1561,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,    32,
    2105,  -748,  -748,   357,  1078,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  1423,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  1699,  -748,  -748,  -748,  -748,
    1699,  1837,   360,  -748,   -78,   362,   -78,   366,   -78,   372,
    1423,   247,   -78,  2229,   -78,   368,  -748,  2105,  -748,  1354,
    -748,  -748,  2105,  -748,  -748,  -748,  1971,  -748,  2105,  -748,
    -748,   370,  1492,  -748,  -748,   322,  -748,  -748,   379,  -748,
     389,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,   383,    23,  1285,   336,   156,  1904,   384,  -748,   221,
    -748,   723,   322,  -748,  -748,  -748,   -77,  -748,  -748,  -748,
    1837,  1078,   -78,   -78,   -78,   247,   -78,  1768,  -748,  -748,
    -748,   364,  -748,  -748,  2038,  -748,  -748,  -748,  2105,  -748,
    -748,  -748,  -748,  -748,   378,   381,   355,   401,   361,   403,
     405,  -748,   406,  -748,   407,   408,  -748,  -748,  -748,  -748,
     409,  2268,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,   322,  -748,   -78,  1699,  2105,
    -748,    88,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,   229,  -748,  -748,  -748,   376,  -748,  1904,  -748,
    -748,   336,   363,   410,   412,   416,    86,  -748,   382,   382,
     382,   382,   550,  1078,  1078,  1078,   -94,  -748,   382,  -748,
    -748,  2038,  -748,  -748,  -748,  -748,  -748,  -748,  -748,   -85,
     -75,   -60,   -27,   423,   425,   425,   425,  -748,  -748,   -15,
     417,   382,   558,   382,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,   -10,   428,    -7,   -78,  -748,
    -748,  -748,  -748
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,   458,   110,   111,    71,   353,   384,   496,   495,   375,
     369,     0,     0,     0,   349,     0,     0,     0,   497,     0,
       0,     0,     0,   355,   357,   351,    14,    15,    17,    19,
      20,    10,    12,    22,    23,   382,    93,     8,     0,     2,
       6,     5,   386,     4,    95,     7,   373,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   470,     0,     0,     0,
       0,     0,     0,   503,   504,   505,     0,     0,   440,   396,
     402,     0,     0,   398,     0,     0,   400,   406,   408,   410,
     412,   414,   416,   418,   420,   425,   483,   484,   486,   485,
     387,   389,     0,   487,   488,   393,   391,     0,   452,   453,
     454,   455,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     0,     0,   359,   361,   489,   490,
     500,     0,   363,   365,   367,   502,   491,   492,   493,   494,
     371,   459,   313,   298,   314,   299,   300,   301,   302,   304,
     303,   305,   306,   307,   308,   309,   310,   311,   312,   291,
     290,   317,   320,   318,   321,   316,   319,   251,   315,   322,
     323,   294,   287,   289,   295,   288,   285,   286,   283,   284,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   341,
     334,   335,   336,   340,   337,   338,   339,   342,   343,   296,
     344,   345,   346,   347,   348,   282,   293,     0,   292,   247,
     248,   297,   249,   250,   324,   578,   569,   579,   576,   577,
     580,   571,   574,   575,   573,   354,   506,   507,   385,   584,
     585,   512,   583,   586,   581,   582,   376,   514,   515,   370,
     443,     0,     0,     0,     0,     0,     0,     0,   478,   479,
     480,   472,   474,   471,   473,   475,   476,   477,     0,   464,
     460,   461,     0,     0,   465,   466,   467,   468,   469,     0,
       0,   481,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    92,   350,     0,     0,     0,   356,   358,
     352,    16,    18,    21,    11,    13,   383,    94,     9,    96,
     374,     0,     0,   499,     0,   377,     0,     0,     0,   501,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     463,   462,     0,     0,   457,   439,   446,     0,     0,     0,
       0,   450,   623,   620,   621,   624,   622,   618,   619,   441,
     397,   403,   424,   422,     0,   399,   401,   407,   409,   411,
     413,   415,   417,   419,   421,   426,   613,   617,   388,   610,
       0,   615,   616,   390,   394,   392,   395,   101,   360,   362,
     379,     0,   527,   364,   529,   530,   366,   368,   381,   372,
       0,    72,    31,    33,    34,    36,    35,    37,   570,   509,
     511,   572,   508,   591,   566,   588,   589,   592,   590,   587,
     517,   568,   513,   521,   519,   516,   444,     0,     0,   434,
     428,   430,   433,   431,   427,   438,     0,   436,   435,   456,
     448,   451,   447,   449,     0,   423,   611,   609,   612,   614,
       0,   231,     0,   378,     0,     0,     0,     0,     0,     0,
      39,     0,     0,     0,     0,     0,   429,   432,   437,     0,
       0,   103,     0,   230,   113,   380,   603,   600,   601,   534,
     602,   599,   532,   528,   538,   536,   531,    41,    32,     0,
      44,    38,     0,   510,   567,   597,   595,   522,   598,   596,
     593,   594,   518,   526,   524,   520,     0,   442,     0,   100,
     106,   105,     0,     0,     0,     0,     0,    73,     0,     0,
      43,     0,     0,   445,   102,   104,     0,    97,     0,   115,
     117,   119,     0,     0,     0,     0,   151,   124,   127,   131,
     134,   138,   141,   145,   112,   556,   535,   560,   558,   608,
     606,   539,   541,   607,   604,   605,   533,   545,   543,   537,
     231,    48,     0,    53,     0,    45,    47,    50,     0,    40,
     523,   525,   153,   251,   253,   254,    30,   114,     0,   233,
     233,   233,   233,     0,   252,   255,   256,   257,   235,     0,
       0,   263,   266,   264,   267,   265,   261,   262,   164,     0,
     162,   163,   258,   259,   260,     0,     0,   160,   149,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    76,   277,   275,   276,   271,   272,   278,   279,
     273,   274,   268,     0,   240,   241,   269,   270,    58,     0,
       0,     0,    42,     0,   232,    27,    28,    26,    24,    25,
       0,     0,   238,   120,     0,   161,     0,    78,   159,     0,
       0,   125,   128,   132,   135,   139,   142,   146,   561,   557,
     565,   563,   559,   540,   546,   542,   550,   548,   544,     0,
       0,   242,    56,     0,     0,    46,    49,    29,    32,    98,
      99,   116,   234,   237,   118,   236,   235,     0,   176,   177,
     181,   107,   167,   122,   166,     0,   179,   180,   148,    77,
       0,   154,     0,   152,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,    81,     0,
      75,    51,     0,   179,    54,    52,     0,   121,     0,   175,
     173,   186,   169,   109,   123,   233,   253,   254,   155,   156,
       0,   252,   255,   256,   257,   150,   126,   129,   133,   136,
     140,   143,   147,   562,   564,   551,   547,   555,   553,   549,
      83,     0,     0,     0,   246,     0,     0,    61,   239,     0,
     184,     0,   233,   168,   171,   172,     0,   165,   178,   108,
       0,     0,     0,     0,     0,     0,     0,     0,    86,    89,
      82,    78,   244,   243,     0,    59,    55,   182,     0,   192,
     193,   189,   196,   197,     0,     0,     0,     0,     0,     0,
       0,   207,   209,   208,     0,     0,   212,   213,   214,   219,
       0,     0,   194,   188,   174,   170,   157,   158,   130,   137,
     144,   552,   554,    85,    79,   233,   179,     0,     0,     0,
      57,     0,   183,   190,   191,   198,   199,   215,   203,   205,
     210,   216,     0,   201,   185,   187,   221,    84,     0,    90,
      80,   245,     0,     0,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   225,     0,   220,
     195,     0,    64,    65,    69,    67,    60,    62,   223,     0,
       0,     0,     0,     0,   226,     0,     0,   218,   224,     0,
       0,     0,     0,     0,   200,   222,   204,   206,   211,   217,
     227,   228,   229,   202,    87,     0,     0,     0,     0,    66,
      70,    68,    88
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
     -79,  -389,  -748,  -748,  -514,  -487,  -748,  -486,  -748,  -748,
    -748,  -748,  -748,  -748,   -76,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -310,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -234,  -748,  -748,  -748,  -748,  -748,  -748,  -748,   -91,
       0,  -748,  -748,  -748,  -748,  -477,  -748,  -748,    82,  -748,
    -671,  -280,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -195,  -132,    67,  -383,  -612,  -748,  -748,  -185,  -748,  -747,
    -697,  -592,  -748,  -748,  -748,  -748,  -748,  -229,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -566,  -748,  -283,    44,
    -572,   -90,    -2,  -748,  -525,  -705,  -748,  -748,  -748,   -42,
    -681,  -515,  -748,  -748,  -511,  -509,   -45,  -748,  -510,  -512,
    -344,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,   -29,   592,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,   -48,  -748,  -748,  -748,  -103,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -146,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -448,  -748,   -50,  -748,  -748,  -748,   -69,  -479,  -589,  -748,
    -748,    42,  -748,   231,  -748,   232,  -223
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    38,    39,    40,   112,   108,   109,   105,   106,   107,
     577,   401,   402,   403,   404,   405,   492,   406,   519,   491,
     518,   640,   565,   566,   567,   638,   777,   683,   733,   806,
     851,   876,   877,   911,   913,   912,    41,    49,   458,   560,
     679,   710,   730,   772,   844,   800,   846,   928,   847,   421,
     377,   111,    43,   114,   387,   691,   451,   470,   471,   512,
     702,   744,    44,   513,   544,   588,   589,   590,   697,   610,
     714,   611,   715,   792,   612,   716,   613,   717,   793,   614,
     718,   615,   719,   794,   616,   720,   659,   609,   712,   748,
     749,   606,   599,   600,   703,   786,   787,   704,   740,   705,
     742,   706,   738,   779,   782,   831,   832,   833,   866,   878,
     888,   879,   880,   881,   882,   890,   899,   886,   887,   474,
     645,   650,   651,   696,   707,   634,   732,   775,   849,   407,
     582,   601,   635,   583,   198,   199,   636,   201,   202,   203,
     204,    45,    92,   104,    50,   102,   103,   311,   312,   316,
     317,   318,    53,   321,   115,    52,   391,   452,   110,    51,
     290,   291,   296,   295,   272,   277,   280,   273,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   271,   444,   328,
     465,    48,   259,   248,   263,    90,    91,    95,   130,    46,
      47,   314,   320,    68,   215,   325,   411,   461,   226,   326,
     327,   422,   463,   464,   502,   521,   522,   393,   454,   455,
     483,   515,   514,   516,   556,   619,   620,   621,   675,   723,
     724,   766,   795,   796,   546,   617,   618,   669,   721,   722,
     423,   462,   216,   323,   324,   217,   228,   424,   504,   485,
     558,   378,   447,   379,   380,   381,   382
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      42,   294,   227,   218,   200,   229,   484,   197,   646,   647,
     648,   456,   310,   300,   579,   503,   587,   584,   586,   607,
     604,   604,   604,   602,   602,   602,   561,   774,   671,   804,
     750,   264,   265,   725,   739,   548,   776,   633,   270,   743,
     907,   580,   581,   409,   708,   275,   266,   292,   359,   914,
     698,   510,   699,    97,   298,   299,   562,   292,   563,   916,
     261,   262,   306,   511,   652,   227,   547,   557,   488,   200,
     267,   785,   309,   352,   917,   338,   883,   884,   885,   353,
     354,   355,    93,    94,   652,   737,   564,   604,   293,   915,
     602,   658,   276,   604,   604,    98,   602,   602,   293,   915,
     517,   249,   250,   251,   252,   426,   253,   918,   762,   750,
     487,    86,    87,   741,   915,   356,   357,   358,   745,   923,
     457,   891,   231,   525,   929,   692,   695,   931,   113,   579,
      99,   587,   584,   586,   768,   695,   562,   254,   563,   578,
     268,   677,   726,   727,   871,   728,   100,   915,   729,   868,
     788,   255,   256,   257,   798,   731,   580,   581,   232,   915,
     233,   234,   235,   236,   915,   799,   564,   915,   508,   670,
     644,   396,   676,   789,   694,   399,   509,   743,   237,   -34,
     -34,   360,   361,   713,   788,   101,   365,    86,    87,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   754,
     751,   753,   771,   331,   332,   333,   334,   335,   336,   389,
     834,   692,   850,   780,   607,   397,   655,   604,   637,   337,
     602,   260,   655,   339,   896,   131,    88,    89,   230,   258,
     342,   343,   408,   410,   412,   269,   425,   347,   348,   349,
     350,   872,   873,   872,   873,   637,   274,   395,   395,   874,
     278,   874,   395,   279,   688,   875,   745,   875,   658,   297,
      54,   604,   227,   388,   602,   637,   394,   394,   227,   261,
     262,   394,   845,   867,   301,   767,   788,   200,   754,   751,
     753,  -281,    55,   852,   302,    56,   568,   644,   453,   303,
    -293,  -293,  -293,    57,   569,   802,   -36,   -36,   -36,   910,
     427,   428,    58,   803,    59,   304,   637,   637,    60,   436,
     637,   305,   637,   900,   901,   902,   637,  -294,  -294,  -294,
     644,   307,   909,    61,   308,    62,    63,    64,    65,   -35,
     -35,   -35,    66,   383,   329,   330,   637,   384,   385,   340,
     341,   227,   313,    67,   315,   925,   319,   927,   322,   475,
    -295,  -295,  -295,   637,   344,   200,  -292,  -292,  -292,   807,
     808,   637,   345,   346,   486,   351,   637,  -248,  -248,  -248,
     493,   494,   362,   505,   364,   363,   637,   386,   205,   390,
     206,   398,   207,   637,   208,   209,   210,   450,   637,  -249,
    -249,  -249,   637,   429,   637,  -250,  -250,  -250,   637,   883,
     884,   885,   227,   430,   431,   200,   457,   432,   472,   433,
     434,   435,   200,   437,   438,   469,   238,   439,   459,   440,
     211,   212,   213,   214,   441,   559,   442,   641,   443,   445,
     466,   571,   637,   460,   467,   468,   506,   239,   240,   241,
     242,   473,   243,   244,   200,   489,   490,   200,   643,   507,
     520,   680,   523,   637,   572,  -280,   653,   644,   649,   661,
     637,   662,   526,   200,   637,   663,   472,   200,   245,   246,
     247,   664,   219,   570,   206,   665,   220,   666,   221,   222,
     223,   667,   681,   585,   682,   684,   685,   603,   603,   603,
     200,   709,   757,   608,   711,   755,   759,   116,   117,   734,
     118,   119,   761,   770,   637,   637,   120,   781,   121,   122,
     123,   124,   125,   126,   211,   224,   225,   214,   790,   200,
     791,   805,   639,   200,   637,   797,   642,   672,   853,   855,
     678,   854,   598,   598,   856,   857,   858,   892,   859,   860,
     861,   862,   863,   893,   219,   894,   206,   637,   220,   895,
     392,   222,   223,   889,   603,   903,   898,   919,    36,   924,
     603,   603,   930,   926,   687,   686,   897,   848,   870,   527,
     528,   673,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,   542,   211,   224,   225,   214,
     524,   127,   128,   129,    69,   836,    70,   773,   585,   598,
     605,   835,   865,   908,   622,   598,   736,   660,    96,     0,
       0,   448,   449,     0,     0,    71,     0,     0,     0,     0,
       0,     0,     0,   756,     0,   758,     0,   760,   543,     0,
       0,   764,     0,   769,     0,     0,     0,     0,     0,   200,
       0,     0,   735,     0,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   752,     0,   413,     0,
     414,     0,   292,   763,   415,   416,   417,     0,    85,   352,
       0,    36,     0,   446,   603,   353,   354,   355,   476,     0,
     414,     0,   292,     0,   477,   478,   479,     0,     0,     0,
       0,   838,   839,   840,     0,   842,     0,     0,     0,     0,
     418,   419,   420,   293,     0,     0,     0,     0,     0,     0,
       0,   356,   357,   358,   376,     0,     0,     0,   603,     0,
     480,   481,   482,   293,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   752,   200,   841,     0,   837,
       0,     0,     0,     0,     0,     0,   869,     0,     0,     0,
       0,     0,     0,    -3,     1,     0,     0,     0,     0,     2,
       3,     4,     5,     0,  -498,  -498,     6,  -498,  -498,     7,
       8,     9,    10,  -498,     0,  -498,  -498,  -498,  -498,  -498,
    -498,    11,    12,     0,     0,    13,     0,     0,     0,    14,
       0,     0,     0,    15,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    17,     0,     0,   932,   200,   200,
     200,   904,   905,   906,    18,     0,    19,    20,    21,    22,
      23,    24,    25,     0,     0,     0,     0,     0,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,  -498,  -498,
    -498,   809,   810,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,   821,   822,   823,   824,   825,   826,   827,
     828,   829,   830,     0,     0,     0,    36,     0,     0,     0,
       0,     0,     0,     0,   920,   921,   922,     0,     0,     0,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   400,    37,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,     0,   195,     0,   196,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   400,     0,   573,   158,   159,   160,
     574,   162,   163,   575,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,     0,   195,     0,   576,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   591,   592,   593,
     594,   654,   595,    36,     0,   596,   597,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,     0,   195,     0,   196,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   591,   592,   593,   594,
     656,   595,    36,     0,   596,   597,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,     0,   195,     0,   196,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,     0,     0,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,     0,   195,     0,   196,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   591,   592,   593,   594,     0,   595,
      36,     0,   596,   597,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
       0,   195,     0,   196,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   591,   592,   593,   594,   657,   595,     0,
       0,   596,   597,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,     0,
     195,     0,   196,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   591,   592,   593,   594,   801,   595,     0,     0,
     596,   597,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,     0,   195,
       0,   196,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   591,   592,   593,   594,     0,   595,     0,     0,   596,
     597,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,     0,   195,     0,
     196,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   623,     0,
     698,   624,   699,   625,     0,   626,     0,     0,     0,     0,
     700,     0,     0,   627,   628,   629,     0,   630,   631,     0,
       0,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,     0,   632,     0,   701,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   623,     0,     0,
     624,     0,   625,   783,   626,     0,     0,   784,     0,   700,
       0,     0,   627,   628,   629,     0,   630,   631,     0,     0,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,     0,   632,     0,   701,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   623,     0,     0,   624,
       0,   625,     0,   626,     0,     0,   689,   693,     0,     0,
       0,   627,   628,   629,     0,   630,   631,     0,     0,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,     0,   632,     0,   690,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   623,     0,     0,   624,     0,
     625,     0,   626,     0,     0,   689,     0,     0,     0,     0,
     627,   628,   629,     0,   630,   631,     0,     0,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,     0,   632,     0,   690,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   623,     0,     0,   624,     0,   625,
       0,   626,     0,     0,     0,     0,   700,     0,     0,   627,
     628,   629,     0,   630,   631,     0,     0,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,     0,   632,     0,   701,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   623,     0,     0,   624,     0,   625,     0,
     626,     0,     0,     0,     0,     0,     0,     0,   627,   628,
     629,     0,   630,   631,     0,     0,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,     0,   632,     0,   843,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
       0,     0,     0,   158,   159,   160,   746,   162,   163,   747,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
       0,   195,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   623,
       0,   698,   624,   699,   625,     0,   626,     0,     0,     0,
       0,   700,     0,     0,   627,   628,   629,     0,   630,   631,
       0,     0,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,     0,   632,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   623,     0,     0,   624,
       0,   625,     0,   626,     0,     0,     0,   778,     0,     0,
       0,   627,   628,   629,     0,   630,   631,     0,     0,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,     0,   632,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   623,     0,     0,   624,     0,   625,     0,
     626,     0,     0,     0,     0,   700,     0,     0,   627,   628,
     629,     0,   630,   631,     0,     0,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,     0,   632,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     623,     0,     0,   624,     0,   625,     0,   626,     0,     0,
       0,     0,     0,     0,     0,   627,   628,   629,     0,   630,
     631,     0,     0,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   495,   632,
     414,     0,   292,     0,   496,   497,   498,   413,     0,   414,
       0,   292,     0,   415,   416,   417,   549,     0,   414,     0,
     292,     0,   550,   551,   552,   495,     0,   414,     0,   292,
       0,   496,   668,   498,     0,     0,     0,     0,     0,     0,
     499,   500,   501,   293,     0,     0,     0,     0,     0,   418,
     419,   545,   293,     0,     0,     0,     0,     0,   553,   554,
     555,   293,     0,     0,     0,     0,     0,   499,   500,   501,
     293,   413,     0,   414,     0,   292,     0,   415,   416,   417,
     495,     0,   414,     0,   292,     0,   496,   765,   498,   352,
       0,    36,     0,     0,     0,   353,   354,   355,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   418,   419,   674,   293,     0,     0,     0,
       0,     0,   499,   500,   501,   293,   864,     0,     0,     0,
       0,   356,   357,   358,   376,     0,   809,   810,   811,   812,
     813,   814,   815,   816,   817,   818,   819,   820,   821,   822,
     823,   824,   825,   826,   827,   828,   829,   830
};

static const yytype_int16 yycheck[] =
{
       0,    92,    52,    51,    49,    53,   454,    49,   580,   581,
     582,   400,   115,   104,   528,   463,   528,   528,   528,   534,
     532,   533,   534,   532,   533,   534,     1,   732,   617,   776,
     711,    60,    61,     1,   705,   514,   733,   562,    67,   710,
     134,   528,   528,   131,   656,    94,    66,   135,   271,   134,
     127,   131,   129,    72,   102,   103,   111,   135,   113,   134,
      30,    31,   110,   143,   589,   115,   514,   515,   457,   114,
      90,   742,   114,   131,   134,    45,   170,   171,   172,   137,
     138,   139,    77,    78,   609,   697,   141,   599,   176,   174,
     599,   606,   141,   605,   606,   135,   605,   606,   176,   174,
     489,    41,    42,    43,    44,   328,    46,   134,   720,   790,
     134,    36,    37,   705,   174,   173,   174,   175,   710,   134,
     144,   868,    13,   512,   134,   650,   651,   134,     0,   643,
     135,   643,   643,   643,   723,   660,   111,    77,   113,   528,
     160,   620,   110,   111,   849,   113,   135,   174,   116,   846,
     742,    91,    92,    93,   131,   680,   643,   643,    49,   174,
      51,    52,    53,    54,   174,   142,   141,   174,   139,   617,
     135,   317,   620,   745,   651,   321,   147,   848,    69,   144,
     145,   272,   273,   660,   776,   135,   277,    36,    37,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   711,
     711,   711,   727,   232,   233,   234,   235,   236,   237,   312,
     782,   736,   804,   738,   729,   318,   599,   729,   562,   248,
     729,    29,   605,   252,   138,   135,    75,    76,    75,   169,
     259,   260,   323,   324,   325,    80,   327,   266,   267,   268,
     269,   155,   156,   155,   156,   589,    41,   316,   317,   163,
     135,   163,   321,   135,   643,   169,   848,   169,   773,   160,
       7,   773,   312,   311,   773,   609,   316,   317,   318,    30,
      31,   321,   797,   845,     3,   723,   868,   322,   790,   790,
     790,   135,    29,   808,     3,    32,   139,   135,   391,     4,
     144,   145,   146,    40,   147,   139,   144,   145,   146,   891,
     329,   330,    49,   147,    51,     3,   650,   651,    55,   338,
     654,     3,   656,   879,   880,   881,   660,   144,   145,   146,
     135,   134,   888,    70,     3,    72,    73,    74,    75,   144,
     145,   146,    79,   291,    51,    52,   680,   295,   296,    47,
      48,   391,    82,    90,    18,   911,    94,   913,   126,   452,
     144,   145,   146,   697,   135,   400,   144,   145,   146,   138,
     139,   705,   135,   135,   455,   135,   710,   144,   145,   146,
     461,   462,   135,   464,    94,   135,   720,   135,   131,    21,
     133,   135,   135,   727,   137,   138,   139,   146,   732,   144,
     145,   146,   736,   135,   738,   144,   145,   146,   742,   170,
     171,   172,   452,   135,   135,   450,   144,   135,   450,   135,
     135,   135,   457,   135,   135,   444,    12,   135,   145,   135,
     173,   174,   175,   176,   135,   516,   135,   139,   135,   135,
     135,   522,   776,   146,   135,   135,   465,    33,    34,    35,
      36,   125,    38,    39,   489,   142,   133,   492,   144,   135,
     492,   142,   135,   797,   135,   135,   131,   135,   135,   131,
     804,   131,     1,   508,   808,   132,   508,   512,    64,    65,
      66,   132,   131,   521,   133,   128,   135,   128,   137,   138,
     139,   131,   146,   528,   131,   142,   134,   532,   533,   534,
     535,   127,   130,   535,   140,   135,   130,    11,    12,   142,
      14,    15,   130,   135,   848,   849,    20,   137,    22,    23,
      24,    25,    26,    27,   173,   174,   175,   176,   139,   564,
     131,   137,   564,   568,   868,   142,   568,   618,   150,   174,
     621,   150,   532,   533,   133,   174,   133,   174,   133,   133,
     133,   133,   133,   133,   131,   133,   133,   891,   135,   133,
     137,   138,   139,   177,   599,     5,   174,   134,   133,   142,
     605,   606,   134,     5,   643,   641,   876,   801,   848,   108,
     109,   619,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   173,   174,   175,   176,
     508,   105,   106,   107,     6,   790,     8,   729,   643,   599,
     533,   786,   831,   886,   560,   605,   696,   609,    16,    -1,
      -1,   380,   380,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   714,    -1,   716,    -1,   718,   167,    -1,
      -1,   722,    -1,   724,    -1,    -1,    -1,    -1,    -1,   684,
      -1,    -1,   684,    -1,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   711,    -1,   131,    -1,
     133,    -1,   135,   721,   137,   138,   139,    -1,    90,   131,
      -1,   133,    -1,   135,   729,   137,   138,   139,   131,    -1,
     133,    -1,   135,    -1,   137,   138,   139,    -1,    -1,    -1,
      -1,   792,   793,   794,    -1,   796,    -1,    -1,    -1,    -1,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   173,   174,   175,   176,    -1,    -1,    -1,   773,    -1,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   790,   791,   795,    -1,   791,
      -1,    -1,    -1,    -1,    -1,    -1,   847,    -1,    -1,    -1,
      -1,    -1,    -1,     0,     1,    -1,    -1,    -1,    -1,     6,
       7,     8,     9,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    -1,    36,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    -1,   928,   883,   884,
     885,   883,   884,   885,    81,    -1,    83,    84,    85,    86,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   904,   905,   906,    -1,    -1,    -1,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   173,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,    -1,   174,    -1,   176,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,    -1,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,    -1,   174,    -1,   176,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,    -1,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,    -1,   174,    -1,   176,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,    -1,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,    -1,   174,    -1,   176,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,    -1,    -1,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,    -1,   174,    -1,   176,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,   132,
     133,    -1,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
      -1,   174,    -1,   176,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,    -1,
      -1,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,    -1,
     174,    -1,   176,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,    -1,    -1,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,    -1,   174,
      -1,   176,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,    -1,    -1,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,    -1,   174,    -1,
     176,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,    -1,
     127,   128,   129,   130,    -1,   132,    -1,    -1,    -1,    -1,
     137,    -1,    -1,   140,   141,   142,    -1,   144,   145,    -1,
      -1,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,    -1,   174,    -1,   176,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,    -1,    -1,
     128,    -1,   130,   131,   132,    -1,    -1,   135,    -1,   137,
      -1,    -1,   140,   141,   142,    -1,   144,   145,    -1,    -1,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,    -1,   174,    -1,   176,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,    -1,   128,
      -1,   130,    -1,   132,    -1,    -1,   135,   136,    -1,    -1,
      -1,   140,   141,   142,    -1,   144,   145,    -1,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,    -1,   174,    -1,   176,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,    -1,    -1,   128,    -1,
     130,    -1,   132,    -1,    -1,   135,    -1,    -1,    -1,    -1,
     140,   141,   142,    -1,   144,   145,    -1,    -1,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,    -1,   174,    -1,   176,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,    -1,   128,    -1,   130,
      -1,   132,    -1,    -1,    -1,    -1,   137,    -1,    -1,   140,
     141,   142,    -1,   144,   145,    -1,    -1,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,    -1,   174,    -1,   176,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,    -1,   128,    -1,   130,    -1,
     132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,    -1,   144,   145,    -1,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,    -1,   174,    -1,   176,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
      -1,    -1,    -1,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
      -1,   174,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,   127,   128,   129,   130,    -1,   132,    -1,    -1,    -1,
      -1,   137,    -1,    -1,   140,   141,   142,    -1,   144,   145,
      -1,    -1,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,    -1,   174,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,    -1,    -1,   128,
      -1,   130,    -1,   132,    -1,    -1,    -1,   136,    -1,    -1,
      -1,   140,   141,   142,    -1,   144,   145,    -1,    -1,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,    -1,   174,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,    -1,   128,    -1,   130,    -1,
     132,    -1,    -1,    -1,    -1,   137,    -1,    -1,   140,   141,
     142,    -1,   144,   145,    -1,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,    -1,   174,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,    -1,    -1,   128,    -1,   130,    -1,   132,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,    -1,   144,
     145,    -1,    -1,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   131,   174,
     133,    -1,   135,    -1,   137,   138,   139,   131,    -1,   133,
      -1,   135,    -1,   137,   138,   139,   131,    -1,   133,    -1,
     135,    -1,   137,   138,   139,   131,    -1,   133,    -1,   135,
      -1,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,    -1,
     173,   174,   175,   176,    -1,    -1,    -1,    -1,    -1,   173,
     174,   175,   176,    -1,    -1,    -1,    -1,    -1,   173,   174,
     175,   176,    -1,    -1,    -1,    -1,    -1,   173,   174,   175,
     176,   131,    -1,   133,    -1,   135,    -1,   137,   138,   139,
     131,    -1,   133,    -1,   135,    -1,   137,   138,   139,   131,
      -1,   133,    -1,    -1,    -1,   137,   138,   139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   173,   174,   175,   176,    -1,    -1,    -1,
      -1,    -1,   173,   174,   175,   176,   138,    -1,    -1,    -1,
      -1,   173,   174,   175,   176,    -1,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     6,     7,     8,     9,    13,    16,    17,    18,
      19,    28,    29,    32,    36,    40,    55,    71,    81,    83,
      84,    85,    86,    87,    88,    89,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   133,   173,   179,   180,
     181,   214,   228,   230,   240,   319,   367,   368,   359,   215,
     322,   337,   333,   330,     7,    29,    32,    40,    49,    51,
      55,    70,    72,    73,    74,    75,    79,    90,   371,     6,
       8,    27,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    90,    36,    37,    75,    76,
     363,   364,   320,    77,    78,   365,   363,    72,   135,   135,
     135,   135,   323,   324,   321,   185,   186,   187,   183,   184,
     336,   229,   182,     0,   231,   332,    11,    12,    14,    15,
      20,    22,    23,    24,    25,    26,    27,   105,   106,   107,
     366,   135,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   174,   176,   307,   312,   313,
     314,   315,   316,   317,   318,   131,   133,   135,   137,   138,
     139,   173,   174,   175,   176,   372,   410,   413,   372,   131,
     135,   137,   138,   139,   174,   175,   376,   410,   414,   372,
      75,    13,    49,    51,    52,    53,    54,    69,    12,    33,
      34,    35,    36,    38,    39,    64,    65,    66,   361,    41,
      42,    43,    44,    46,    77,    91,    92,    93,   169,   360,
      29,    30,    31,   362,   362,   362,    66,    90,   160,    80,
     362,   355,   342,   345,    41,    94,   141,   343,   135,   135,
     344,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     338,   339,   135,   176,   227,   341,   340,   160,   372,   372,
     227,     3,     3,     4,     3,     3,   372,   134,     3,   307,
     376,   325,   326,    82,   369,    18,   327,   328,   329,    94,
     370,   331,   126,   411,   412,   373,   377,   378,   357,    51,
      52,   362,   362,   362,   362,   362,   362,   362,    45,   362,
      47,    48,   362,   362,   135,   135,   135,   362,   362,   362,
     362,   135,   131,   137,   138,   139,   173,   174,   175,   424,
     227,   227,   135,   135,    94,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   176,   228,   419,   421,
     422,   423,   424,   419,   419,   419,   135,   232,   372,   376,
      21,   334,   137,   385,   410,   414,   385,   376,   135,   385,
     133,   189,   190,   191,   192,   193,   195,   307,   227,   131,
     227,   374,   227,   131,   133,   137,   138,   139,   173,   174,
     175,   227,   379,   408,   415,   227,   424,   362,   362,   135,
     135,   135,   135,   135,   135,   135,   362,   135,   135,   135,
     135,   135,   135,   135,   356,   135,   135,   420,   421,   423,
     146,   234,   335,   376,   386,   387,   189,   144,   216,   145,
     146,   375,   409,   380,   381,   358,   135,   135,   135,   362,
     235,   236,   307,   125,   297,   376,   131,   137,   138,   139,
     173,   174,   175,   388,   408,   417,   227,   134,   189,   142,
     133,   197,   194,   227,   227,   131,   137,   138,   139,   173,
     174,   175,   382,   408,   416,   227,   362,   135,   139,   147,
     131,   143,   237,   241,   390,   389,   391,   189,   198,   196,
     307,   383,   384,   135,   236,   189,     1,   108,   109,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   167,   242,   175,   402,   408,   415,   131,
     137,   138,   139,   173,   174,   175,   392,   408,   418,   227,
     217,     1,   111,   113,   141,   200,   201,   202,   139,   147,
     372,   227,   135,   135,   139,   142,   176,   188,   189,   192,
     193,   195,   308,   311,   312,   314,   316,   317,   243,   244,
     245,   127,   128,   129,   130,   132,   135,   136,   228,   270,
     271,   309,   313,   314,   317,   270,   269,   309,   307,   265,
     247,   249,   252,   254,   257,   259,   262,   403,   404,   393,
     394,   395,   297,   125,   128,   130,   132,   140,   141,   142,
     144,   145,   174,   302,   303,   310,   314,   318,   203,   307,
     199,   139,   307,   144,   135,   298,   298,   298,   298,   135,
     299,   300,   302,   131,   131,   271,   131,   131,   309,   264,
     300,   131,   131,   132,   132,   128,   128,   131,   138,   405,
     408,   416,   227,   372,   175,   396,   408,   415,   227,   218,
     142,   146,   131,   205,   142,   134,   202,   188,   189,   135,
     176,   233,   302,   136,   233,   302,   301,   246,   127,   129,
     137,   176,   238,   272,   275,   277,   279,   302,   272,   127,
     219,   140,   266,   233,   248,   250,   253,   255,   258,   260,
     263,   406,   407,   397,   398,     1,   110,   111,   113,   116,
     220,   302,   304,   206,   142,   307,   299,   272,   280,   238,
     276,   279,   278,   238,   239,   279,   139,   142,   267,   268,
     308,   312,   314,   316,   317,   135,   227,   130,   227,   130,
     227,   130,   272,   372,   227,   138,   399,   408,   416,   227,
     135,   302,   221,   269,   303,   305,   278,   204,   136,   281,
     302,   137,   282,   131,   135,   238,   273,   274,   279,   298,
     139,   131,   251,   256,   261,   400,   401,   142,   131,   142,
     223,   131,   139,   147,   277,   137,   207,   138,   139,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   283,   284,   285,   298,   275,   268,   307,   227,   227,
     227,   372,   227,   176,   222,   302,   224,   226,   219,   306,
     279,   208,   302,   150,   150,   174,   133,   174,   133,   133,
     133,   133,   133,   133,   138,   285,   286,   298,   278,   227,
     239,   303,   155,   156,   163,   169,   209,   210,   287,   289,
     290,   291,   292,   170,   171,   172,   295,   296,   288,   177,
     293,   277,   174,   133,   133,   133,   138,   210,   174,   294,
     294,   294,   294,     5,   307,   307,   307,   134,   296,   294,
     279,   211,   213,   212,   134,   174,   134,   134,   134,   134,
     228,   228,   228,   134,   142,   294,     5,   294,   225,   134,
     134,   134,   227
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
     210,   215,   216,   217,   214,   218,   218,   219,   219,   220,
     220,   221,   220,   220,   222,   222,   224,   225,   223,   226,
     223,   227,   227,   229,   228,   231,   232,   230,   233,   233,
     234,   234,   235,   235,   236,   237,   237,   238,   239,   239,
     240,   240,   241,   241,   242,   243,   242,   244,   242,   245,
     246,   242,   242,   242,   247,   248,   242,   249,   250,   251,
     242,   252,   253,   242,   254,   255,   256,   242,   257,   258,
     242,   259,   260,   261,   242,   262,   263,   242,   242,   264,
     242,   265,   242,   242,   266,   266,   267,   267,   268,   269,
     269,   270,   270,   271,   271,   272,   272,   272,   273,   273,
     274,   274,   274,   275,   276,   276,   277,   277,   278,   278,
     279,   280,   279,   281,   281,   282,   282,   283,   283,   284,
     284,   284,   285,   285,   286,   285,   285,   285,   285,   287,
     285,   288,   285,   289,   285,   290,   285,   285,   285,   285,
     291,   285,   285,   285,   285,   285,   292,   285,   285,   285,
     293,   293,   294,   294,   295,   295,   296,   296,   296,   296,
     297,   297,   298,   298,   299,   299,   300,   300,   301,   300,
     302,   303,   304,   303,   306,   305,   305,   307,   307,   307,
     307,   307,   308,   308,   308,   308,   308,   308,   309,   309,
     309,   309,   309,   309,   309,   309,   309,   309,   310,   310,
     310,   310,   310,   310,   310,   310,   310,   310,   310,   310,
     311,   311,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   313,   313,   313,   313,   314,   314,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   316,   316,   316,   316,   316,
     316,   316,   317,   317,   317,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   318,   318,   320,
     319,   321,   319,   322,   319,   323,   319,   324,   319,   325,
     319,   326,   319,   327,   319,   328,   319,   329,   319,   330,
     319,   331,   319,   332,   319,   333,   319,   334,   319,   335,
     319,   319,   336,   319,   337,   319,   319,   338,   319,   339,
     319,   340,   319,   341,   319,   319,   342,   319,   343,   319,
     344,   319,   345,   319,   319,   319,   346,   319,   347,   319,
     348,   319,   349,   319,   350,   319,   351,   319,   352,   319,
     353,   319,   319,   319,   319,   354,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   319,   319,
     355,   356,   319,   357,   358,   319,   319,   319,   319,   319,
     319,   319,   319,   319,   319,   319,   319,   319,   359,   319,
     360,   360,   360,   360,   360,   360,   360,   360,   360,   360,
     361,   361,   361,   361,   361,   361,   361,   361,   361,   361,
     361,   362,   362,   363,   363,   364,   364,   365,   365,   366,
     366,   366,   366,   366,   366,   367,   367,   368,   368,   369,
     369,   370,   370,   371,   371,   371,   372,   373,   372,   375,
     374,   374,   377,   376,   376,   378,   376,   380,   379,   381,
     379,   379,   383,   382,   384,   382,   382,   386,   385,   385,
     387,   385,   389,   388,   390,   388,   391,   388,   388,   393,
     392,   394,   392,   395,   392,   392,   397,   396,   398,   396,
     396,   400,   399,   401,   399,   399,   403,   402,   404,   402,
     402,   406,   405,   407,   405,   405,   409,   408,   408,   411,
     410,   412,   410,   410,   413,   413,   413,   413,   413,   413,
     413,   414,   414,   414,   414,   414,   414,   415,   415,   415,
     415,   415,   415,   416,   416,   416,   416,   416,   416,   417,
     417,   417,   417,   417,   418,   418,   418,   418,   418,   419,
     419,   420,   420,   421,   422,   422,   423,   423,   424,   424,
     424,   424,   424,   424,   424
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
       5,     0,     0,     0,    12,     2,     0,     1,     0,     4,
       5,     0,     3,     2,     2,     1,     0,     0,     8,     0,
       3,     1,     1,     0,     3,     0,     0,     8,     1,     1,
       3,     0,     3,     1,     3,     1,     1,     1,     2,     1,
       1,     1,     2,     0,     2,     0,     4,     0,     4,     0,
       0,     5,     4,     5,     0,     0,     5,     0,     0,     0,
       7,     0,     0,     5,     0,     0,     0,     7,     0,     0,
       5,     0,     0,     0,     7,     0,     0,     5,     4,     0,
       5,     0,     4,     2,     1,     2,     1,     3,     3,     2,
       1,     2,     1,     1,     1,     3,     1,     1,     1,     0,
       2,     1,     1,     2,     3,     1,     1,     1,     2,     0,
       1,     0,     4,     3,     1,     3,     0,     2,     1,     1,
       2,     2,     1,     1,     0,     3,     1,     1,     2,     0,
       5,     0,     5,     0,     5,     0,     5,     1,     1,     1,
       0,     5,     1,     1,     1,     2,     0,     5,     4,     1,
       1,     0,     2,     1,     2,     1,     2,     3,     3,     3,
       1,     0,     1,     0,     2,     0,     2,     2,     0,     4,
       1,     1,     0,     5,     0,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       3,     0,     4,     0,     3,     0,     3,     0,     5,     0,
       6,     4,     0,     3,     0,     3,     1,     0,     4,     0,
       4,     0,     4,     0,     4,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     3,     3,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     4,     5,     4,     0,     4,     5,     5,     6,
       5,     5,     6,     5,     5,     5,     5,     6,     5,     4,
       0,     0,     7,     0,     0,     8,     4,     5,     5,     5,
       4,     5,     2,     2,     2,     2,     5,     4,     0,     3,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       0,     1,     0,     1,     1,     1,     1,     0,     3,     0,
       3,     1,     0,     3,     1,     0,     3,     0,     3,     0,
       3,     1,     0,     3,     0,     3,     1,     0,     3,     1,
       0,     3,     0,     3,     0,     3,     0,     3,     1,     0,
       3,     0,     3,     0,     3,     1,     0,     3,     0,     3,
       1,     0,     3,     0,     3,     1,     0,     3,     0,     3,
       1,     0,     3,     0,     3,     1,     0,     3,     1,     0,
       3,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
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
#line 2546 "surface.c" /* yacc.c:1646  */
    break;

  case 3:
#line 237 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			}
#line 2554 "surface.c" /* yacc.c:1646  */
    break;

  case 8:
#line 251 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2560 "surface.c" /* yacc.c:1646  */
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
#line 2573 "surface.c" /* yacc.c:1646  */
    break;

  case 10:
#line 261 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2579 "surface.c" /* yacc.c:1646  */
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
#line 2592 "surface.c" /* yacc.c:1646  */
    break;

  case 12:
#line 271 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2598 "surface.c" /* yacc.c:1646  */
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
#line 2611 "surface.c" /* yacc.c:1646  */
    break;

  case 14:
#line 282 "surface.yy" /* yacc.c:1646  */
    {
			  cout << directoryManager.getCwd() << '\n';
			}
#line 2619 "surface.c" /* yacc.c:1646  */
    break;

  case 15:
#line 285 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2625 "surface.c" /* yacc.c:1646  */
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
#line 2639 "surface.c" /* yacc.c:1646  */
    break;

  case 17:
#line 296 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2645 "surface.c" /* yacc.c:1646  */
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
#line 2659 "surface.c" /* yacc.c:1646  */
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
#line 2674 "surface.c" /* yacc.c:1646  */
    break;

  case 20:
#line 318 "surface.yy" /* yacc.c:1646  */
    { lexerStringMode(); }
#line 2680 "surface.c" /* yacc.c:1646  */
    break;

  case 21:
#line 320 "surface.yy" /* yacc.c:1646  */
    {
			  system((string("ls") + (yyvsp[0].yyString)).c_str());
			}
#line 2688 "surface.c" /* yacc.c:1646  */
    break;

  case 22:
#line 324 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			  YYACCEPT;
			}
#line 2697 "surface.c" /* yacc.c:1646  */
    break;

  case 23:
#line 329 "surface.yy" /* yacc.c:1646  */
    {
			  if(!handleEof())
			    {
			      PARSE_RESULT = UserLevelRewritingContext::QUIT;
			    }
			}
#line 2708 "surface.c" /* yacc.c:1646  */
    break;

  case 24:
#line 362 "surface.yy" /* yacc.c:1646  */
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[-1].yyToken)));
                        }
#line 2716 "surface.c" /* yacc.c:1646  */
    break;

  case 25:
#line 366 "surface.yy" /* yacc.c:1646  */
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[-1].yyToken)));
                        }
#line 2724 "surface.c" /* yacc.c:1646  */
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
#line 2736 "surface.c" /* yacc.c:1646  */
    break;

  case 30:
#line 381 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  moduleExpressions.push(new ModuleExpression(t));
			}
#line 2747 "surface.c" /* yacc.c:1646  */
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
#line 2759 "surface.c" /* yacc.c:1646  */
    break;

  case 37:
#line 407 "surface.yy" /* yacc.c:1646  */
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[0].yyToken)));
                        }
#line 2767 "surface.c" /* yacc.c:1646  */
    break;

  case 38:
#line 414 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, currentRenaming));
			  currentRenaming = 0;
			}
#line 2778 "surface.c" /* yacc.c:1646  */
    break;

  case 39:
#line 422 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 2784 "surface.c" /* yacc.c:1646  */
    break;

  case 40:
#line 424 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, tokenSequence));
			}
#line 2794 "surface.c" /* yacc.c:1646  */
    break;

  case 41:
#line 431 "surface.yy" /* yacc.c:1646  */
    {}
#line 2800 "surface.c" /* yacc.c:1646  */
    break;

  case 42:
#line 434 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 2806 "surface.c" /* yacc.c:1646  */
    break;

  case 43:
#line 435 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 2812 "surface.c" /* yacc.c:1646  */
    break;

  case 44:
#line 442 "surface.yy" /* yacc.c:1646  */
    {
			  oldSyntaxContainer = currentSyntaxContainer;
			  currentSyntaxContainer = currentRenaming = new Renaming;
			}
#line 2821 "surface.c" /* yacc.c:1646  */
    break;

  case 45:
#line 447 "surface.yy" /* yacc.c:1646  */
    {
			  currentSyntaxContainer = oldSyntaxContainer;
			}
#line 2829 "surface.c" /* yacc.c:1646  */
    break;

  case 51:
#line 462 "surface.yy" /* yacc.c:1646  */
    {
			  currentRenaming->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 2837 "surface.c" /* yacc.c:1646  */
    break;

  case 52:
#line 466 "surface.yy" /* yacc.c:1646  */
    {
			  currentRenaming->addLabelMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 2845 "surface.c" /* yacc.c:1646  */
    break;

  case 53:
#line 469 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON | BAR_TO, 1); }
#line 2851 "surface.c" /* yacc.c:1646  */
    break;

  case 54:
#line 470 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COMMA | BAR_LEFT_BRACKET | BAR_RIGHT_PAREN, 1); }
#line 2857 "surface.c" /* yacc.c:1646  */
    break;

  case 55:
#line 471 "surface.yy" /* yacc.c:1646  */
    {}
#line 2863 "surface.c" /* yacc.c:1646  */
    break;

  case 56:
#line 476 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
#line 2869 "surface.c" /* yacc.c:1646  */
    break;

  case 57:
#line 477 "surface.yy" /* yacc.c:1646  */
    {}
#line 2875 "surface.c" /* yacc.c:1646  */
    break;

  case 58:
#line 478 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
#line 2881 "surface.c" /* yacc.c:1646  */
    break;

  case 59:
#line 484 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
#line 2887 "surface.c" /* yacc.c:1646  */
    break;

  case 60:
#line 485 "surface.yy" /* yacc.c:1646  */
    {}
#line 2893 "surface.c" /* yacc.c:1646  */
    break;

  case 61:
#line 486 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
#line 2899 "surface.c" /* yacc.c:1646  */
    break;

  case 64:
#line 493 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setPrec((yyvsp[0].yyToken)); }
#line 2905 "surface.c" /* yacc.c:1646  */
    break;

  case 65:
#line 494 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 2911 "surface.c" /* yacc.c:1646  */
    break;

  case 66:
#line 495 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setGather(tokenSequence); }
#line 2917 "surface.c" /* yacc.c:1646  */
    break;

  case 67:
#line 496 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 2923 "surface.c" /* yacc.c:1646  */
    break;

  case 68:
#line 497 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setFormat(tokenSequence); }
#line 2929 "surface.c" /* yacc.c:1646  */
    break;

  case 69:
#line 498 "surface.yy" /* yacc.c:1646  */
    { lexerLatexMode(); }
#line 2935 "surface.c" /* yacc.c:1646  */
    break;

  case 70:
#line 499 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setLatexMacro((yyvsp[-1].yyString)); }
#line 2941 "surface.c" /* yacc.c:1646  */
    break;

  case 71:
#line 505 "surface.yy" /* yacc.c:1646  */
    { lexerIdMode(); }
#line 2947 "surface.c" /* yacc.c:1646  */
    break;

  case 72:
#line 507 "surface.yy" /* yacc.c:1646  */
    {
			  fileTable.beginModule((yyvsp[-4].yyToken), (yyvsp[-2].yyToken));
			  interpreter.setCurrentView(new SyntacticView((yyvsp[-2].yyToken), &interpreter));
			  currentSyntaxContainer = CV;
			  CV->addFrom(moduleExpressions.top());
			  moduleExpressions.pop();
			}
#line 2959 "surface.c" /* yacc.c:1646  */
    break;

  case 73:
#line 515 "surface.yy" /* yacc.c:1646  */
    {
			  CV->addTo(moduleExpressions.top());
			  moduleExpressions.pop();
			}
#line 2968 "surface.c" /* yacc.c:1646  */
    break;

  case 74:
#line 520 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  interpreter.insertView(((yyvsp[-9].yyToken)).code(), CV);
			  CV->finishView();
			}
#line 2979 "surface.c" /* yacc.c:1646  */
    break;

  case 76:
#line 529 "surface.yy" /* yacc.c:1646  */
    {}
#line 2985 "surface.c" /* yacc.c:1646  */
    break;

  case 77:
#line 533 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
				       ": skipping " << QUOTE("->") << " in variable declaration.");
			}
#line 2994 "surface.c" /* yacc.c:1646  */
    break;

  case 78:
#line 537 "surface.yy" /* yacc.c:1646  */
    {}
#line 3000 "surface.c" /* yacc.c:1646  */
    break;

  case 79:
#line 541 "surface.yy" /* yacc.c:1646  */
    {
			  CV->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3008 "surface.c" /* yacc.c:1646  */
    break;

  case 80:
#line 544 "surface.yy" /* yacc.c:1646  */
    {}
#line 3014 "surface.c" /* yacc.c:1646  */
    break;

  case 81:
#line 545 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON | BAR_TO, 1); }
#line 3020 "surface.c" /* yacc.c:1646  */
    break;

  case 84:
#line 550 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyToken) = (yyvsp[-1].yyToken); }
#line 3026 "surface.c" /* yacc.c:1646  */
    break;

  case 85:
#line 552 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  (yyval.yyToken) = t;
			}
#line 3037 "surface.c" /* yacc.c:1646  */
    break;

  case 86:
#line 561 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	Specific op->op mapping.
			  //
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpMapping(lexerBubble);
			}
#line 3049 "surface.c" /* yacc.c:1646  */
    break;

  case 87:
#line 569 "surface.yy" /* yacc.c:1646  */
    {
			  lexBubble(END_STATEMENT, 1);
			}
#line 3057 "surface.c" /* yacc.c:1646  */
    break;

  case 88:
#line 573 "surface.yy" /* yacc.c:1646  */
    {
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpTarget(lexerBubble);
			}
#line 3066 "surface.c" /* yacc.c:1646  */
    break;

  case 89:
#line 578 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	At this point we don't know if we have an op->term mapping
			  //	or a generic op->op mapping so we save the from description and
			  //	press on.
			  //
			  opDescription = lexerBubble;
			  lexBubble(END_STATEMENT, 1);
			}
#line 3080 "surface.c" /* yacc.c:1646  */
    break;

  case 90:
#line 588 "surface.yy" /* yacc.c:1646  */
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
#line 3104 "surface.c" /* yacc.c:1646  */
    break;

  case 91:
#line 610 "surface.yy" /* yacc.c:1646  */
    {}
#line 3110 "surface.c" /* yacc.c:1646  */
    break;

  case 92:
#line 612 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  lexerBubble.append(t);
			}
#line 3121 "surface.c" /* yacc.c:1646  */
    break;

  case 93:
#line 620 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_RIGHT_PAREN, 1); }
#line 3127 "surface.c" /* yacc.c:1646  */
    break;

  case 94:
#line 621 "surface.yy" /* yacc.c:1646  */
    {}
#line 3133 "surface.c" /* yacc.c:1646  */
    break;

  case 95:
#line 627 "surface.yy" /* yacc.c:1646  */
    { lexerIdMode(); }
#line 3139 "surface.c" /* yacc.c:1646  */
    break;

  case 96:
#line 629 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setCurrentModule(new SyntacticPreModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken), &interpreter));
			  currentSyntaxContainer = CM;
			  fileTable.beginModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3149 "surface.c" /* yacc.c:1646  */
    break;

  case 97:
#line 635 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule((yyvsp[0].yyToken));
			}
#line 3159 "surface.c" /* yacc.c:1646  */
    break;

  case 98:
#line 642 "surface.yy" /* yacc.c:1646  */
    {}
#line 3165 "surface.c" /* yacc.c:1646  */
    break;

  case 99:
#line 644 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  store(t);
			}
#line 3176 "surface.c" /* yacc.c:1646  */
    break;

  case 100:
#line 652 "surface.yy" /* yacc.c:1646  */
    {}
#line 3182 "surface.c" /* yacc.c:1646  */
    break;

  case 101:
#line 653 "surface.yy" /* yacc.c:1646  */
    {}
#line 3188 "surface.c" /* yacc.c:1646  */
    break;

  case 104:
#line 661 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addParameter((yyvsp[-2].yyToken), me);
			}
#line 3198 "surface.c" /* yacc.c:1646  */
    break;

  case 105:
#line 668 "surface.yy" /* yacc.c:1646  */
    {}
#line 3204 "surface.c" /* yacc.c:1646  */
    break;

  case 106:
#line 670 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			    ": saw " << QUOTE(':') << " instead of " <<
			    QUOTE("::") << " in parameter declaration.");
			}
#line 3214 "surface.c" /* yacc.c:1646  */
    break;

  case 107:
#line 678 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  currentSyntaxContainer->addType(false, singleton);
			  (yyval.yyToken) = (yyvsp[0].yyToken);  // needed for line number
			}
#line 3225 "surface.c" /* yacc.c:1646  */
    break;

  case 109:
#line 687 "surface.yy" /* yacc.c:1646  */
    {}
#line 3231 "surface.c" /* yacc.c:1646  */
    break;

  case 113:
#line 694 "surface.yy" /* yacc.c:1646  */
    {}
#line 3237 "surface.c" /* yacc.c:1646  */
    break;

  case 114:
#line 698 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addImport((yyvsp[-1].yyToken), me);
			}
#line 3247 "surface.c" /* yacc.c:1646  */
    break;

  case 115:
#line 704 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3253 "surface.c" /* yacc.c:1646  */
    break;

  case 116:
#line 705 "surface.yy" /* yacc.c:1646  */
    { CM->addSortDecl(tokenSequence); }
#line 3259 "surface.c" /* yacc.c:1646  */
    break;

  case 117:
#line 707 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3265 "surface.c" /* yacc.c:1646  */
    break;

  case 118:
#line 708 "surface.yy" /* yacc.c:1646  */
    { CM->addSubsortDecl(tokenSequence); }
#line 3271 "surface.c" /* yacc.c:1646  */
    break;

  case 119:
#line 710 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON, 1); }
#line 3277 "surface.c" /* yacc.c:1646  */
    break;

  case 120:
#line 711 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
#line 3283 "surface.c" /* yacc.c:1646  */
    break;

  case 121:
#line 712 "surface.yy" /* yacc.c:1646  */
    {}
#line 3289 "surface.c" /* yacc.c:1646  */
    break;

  case 122:
#line 714 "surface.yy" /* yacc.c:1646  */
    {}
#line 3295 "surface.c" /* yacc.c:1646  */
    break;

  case 123:
#line 716 "surface.yy" /* yacc.c:1646  */
    {}
#line 3301 "surface.c" /* yacc.c:1646  */
    break;

  case 124:
#line 718 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1); }
#line 3307 "surface.c" /* yacc.c:1646  */
    break;

  case 125:
#line 719 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3313 "surface.c" /* yacc.c:1646  */
    break;

  case 126:
#line 720 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3319 "surface.c" /* yacc.c:1646  */
    break;

  case 127:
#line 722 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1);  }
#line 3325 "surface.c" /* yacc.c:1646  */
    break;

  case 128:
#line 723 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3331 "surface.c" /* yacc.c:1646  */
    break;

  case 129:
#line 724 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3337 "surface.c" /* yacc.c:1646  */
    break;

  case 130:
#line 725 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3343 "surface.c" /* yacc.c:1646  */
    break;

  case 131:
#line 727 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); }
#line 3349 "surface.c" /* yacc.c:1646  */
    break;

  case 132:
#line 728 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3355 "surface.c" /* yacc.c:1646  */
    break;

  case 133:
#line 729 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3361 "surface.c" /* yacc.c:1646  */
    break;

  case 134:
#line 731 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); }
#line 3367 "surface.c" /* yacc.c:1646  */
    break;

  case 135:
#line 732 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3373 "surface.c" /* yacc.c:1646  */
    break;

  case 136:
#line 733 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3379 "surface.c" /* yacc.c:1646  */
    break;

  case 137:
#line 734 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3385 "surface.c" /* yacc.c:1646  */
    break;

  case 138:
#line 736 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); }
#line 3391 "surface.c" /* yacc.c:1646  */
    break;

  case 139:
#line 737 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3397 "surface.c" /* yacc.c:1646  */
    break;

  case 140:
#line 738 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3403 "surface.c" /* yacc.c:1646  */
    break;

  case 141:
#line 740 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); }
#line 3409 "surface.c" /* yacc.c:1646  */
    break;

  case 142:
#line 741 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3415 "surface.c" /* yacc.c:1646  */
    break;

  case 143:
#line 742 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3421 "surface.c" /* yacc.c:1646  */
    break;

  case 144:
#line 743 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3427 "surface.c" /* yacc.c:1646  */
    break;

  case 145:
#line 745 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON, 1); }
#line 3433 "surface.c" /* yacc.c:1646  */
    break;

  case 146:
#line 746 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
#line 3439 "surface.c" /* yacc.c:1646  */
    break;

  case 147:
#line 747 "surface.yy" /* yacc.c:1646  */
    { CM->setFlag(SymbolType::MESSAGE); }
#line 3445 "surface.c" /* yacc.c:1646  */
    break;

  case 148:
#line 750 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
#line 3453 "surface.c" /* yacc.c:1646  */
    break;

  case 149:
#line 755 "surface.yy" /* yacc.c:1646  */
    {
			}
#line 3460 "surface.c" /* yacc.c:1646  */
    break;

  case 150:
#line 758 "surface.yy" /* yacc.c:1646  */
    {
			}
#line 3467 "surface.c" /* yacc.c:1646  */
    break;

  case 151:
#line 761 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3473 "surface.c" /* yacc.c:1646  */
    break;

  case 152:
#line 762 "surface.yy" /* yacc.c:1646  */
    { CM->addSubsortDecl(tokenSequence); }
#line 3479 "surface.c" /* yacc.c:1646  */
    break;

  case 153:
#line 765 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	Fix things that might be in a bad state due
			  //	to a partially processed declaration.
			  //
			  cleanUpModuleExpression();
			  CM->makeOpDeclsConsistent();
			}
#line 3492 "surface.c" /* yacc.c:1646  */
    break;

  case 154:
#line 775 "surface.yy" /* yacc.c:1646  */
    {}
#line 3498 "surface.c" /* yacc.c:1646  */
    break;

  case 155:
#line 776 "surface.yy" /* yacc.c:1646  */
    {}
#line 3504 "surface.c" /* yacc.c:1646  */
    break;

  case 158:
#line 784 "surface.yy" /* yacc.c:1646  */
    {
			}
#line 3511 "surface.c" /* yacc.c:1646  */
    break;

  case 159:
#line 788 "surface.yy" /* yacc.c:1646  */
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); }
#line 3517 "surface.c" /* yacc.c:1646  */
    break;

  case 160:
#line 789 "surface.yy" /* yacc.c:1646  */
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); }
#line 3523 "surface.c" /* yacc.c:1646  */
    break;

  case 163:
#line 796 "surface.yy" /* yacc.c:1646  */
    { singleton[0] = (yyvsp[0].yyToken); CM->addOpDecl(singleton); }
#line 3529 "surface.c" /* yacc.c:1646  */
    break;

  case 164:
#line 797 "surface.yy" /* yacc.c:1646  */
    { CM->addOpDecl(lexerBubble); }
#line 3535 "surface.c" /* yacc.c:1646  */
    break;

  case 167:
#line 803 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber(lineNumber) <<
				       ": missing " << QUOTE("->") << " in constant declaration.");
			}
#line 3544 "surface.c" /* yacc.c:1646  */
    break;

  case 168:
#line 810 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
				       ": skipping stray " << QUOTE(":") << " in operator declaration.");

			}
#line 3554 "surface.c" /* yacc.c:1646  */
    break;

  case 169:
#line 815 "surface.yy" /* yacc.c:1646  */
    {}
#line 3560 "surface.c" /* yacc.c:1646  */
    break;

  case 171:
#line 820 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
#line 3569 "surface.c" /* yacc.c:1646  */
    break;

  case 172:
#line 825 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
#line 3578 "surface.c" /* yacc.c:1646  */
    break;

  case 173:
#line 832 "surface.yy" /* yacc.c:1646  */
    {
			  if ((yyvsp[-1].yyBool))
			    CM->convertSortsToKinds();
			}
#line 3587 "surface.c" /* yacc.c:1646  */
    break;

  case 175:
#line 839 "surface.yy" /* yacc.c:1646  */
    {}
#line 3593 "surface.c" /* yacc.c:1646  */
    break;

  case 176:
#line 842 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 3599 "surface.c" /* yacc.c:1646  */
    break;

  case 177:
#line 843 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 3605 "surface.c" /* yacc.c:1646  */
    break;

  case 179:
#line 847 "surface.yy" /* yacc.c:1646  */
    {}
#line 3611 "surface.c" /* yacc.c:1646  */
    break;

  case 180:
#line 851 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  currentSyntaxContainer->addType(false, singleton);
			}
#line 3620 "surface.c" /* yacc.c:1646  */
    break;

  case 181:
#line 855 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3626 "surface.c" /* yacc.c:1646  */
    break;

  case 182:
#line 857 "surface.yy" /* yacc.c:1646  */
    {
			  currentSyntaxContainer->addType(true, tokenSequence);
			}
#line 3634 "surface.c" /* yacc.c:1646  */
    break;

  case 183:
#line 862 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3640 "surface.c" /* yacc.c:1646  */
    break;

  case 184:
#line 863 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3646 "surface.c" /* yacc.c:1646  */
    break;

  case 185:
#line 866 "surface.yy" /* yacc.c:1646  */
    {}
#line 3652 "surface.c" /* yacc.c:1646  */
    break;

  case 186:
#line 867 "surface.yy" /* yacc.c:1646  */
    {}
#line 3658 "surface.c" /* yacc.c:1646  */
    break;

  case 189:
#line 875 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
			}
#line 3666 "surface.c" /* yacc.c:1646  */
    break;

  case 190:
#line 879 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::LEFT_ID);
			}
#line 3674 "surface.c" /* yacc.c:1646  */
    break;

  case 191:
#line 883 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::RIGHT_ID);
			}
#line 3682 "surface.c" /* yacc.c:1646  */
    break;

  case 192:
#line 889 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::ASSOC);
			}
#line 3690 "surface.c" /* yacc.c:1646  */
    break;

  case 193:
#line 893 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::COMM);
			}
#line 3698 "surface.c" /* yacc.c:1646  */
    break;

  case 194:
#line 896 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_RIGHT_BRACKET | BAR_OP_ATTRIBUTE, 1); }
#line 3704 "surface.c" /* yacc.c:1646  */
    break;

  case 195:
#line 897 "surface.yy" /* yacc.c:1646  */
    { CM->setIdentity(lexerBubble); }
#line 3710 "surface.c" /* yacc.c:1646  */
    break;

  case 196:
#line 899 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::IDEM);
			}
#line 3718 "surface.c" /* yacc.c:1646  */
    break;

  case 197:
#line 903 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::ITER);
			}
#line 3726 "surface.c" /* yacc.c:1646  */
    break;

  case 198:
#line 906 "surface.yy" /* yacc.c:1646  */
    { CM->setPrec((yyvsp[0].yyToken)); }
#line 3732 "surface.c" /* yacc.c:1646  */
    break;

  case 199:
#line 907 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3738 "surface.c" /* yacc.c:1646  */
    break;

  case 200:
#line 908 "surface.yy" /* yacc.c:1646  */
    { CM->setGather(tokenSequence); }
#line 3744 "surface.c" /* yacc.c:1646  */
    break;

  case 201:
#line 909 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3750 "surface.c" /* yacc.c:1646  */
    break;

  case 202:
#line 910 "surface.yy" /* yacc.c:1646  */
    { CM->setFormat(tokenSequence); }
#line 3756 "surface.c" /* yacc.c:1646  */
    break;

  case 203:
#line 911 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3762 "surface.c" /* yacc.c:1646  */
    break;

  case 204:
#line 912 "surface.yy" /* yacc.c:1646  */
    { CM->setStrat(tokenSequence); }
#line 3768 "surface.c" /* yacc.c:1646  */
    break;

  case 205:
#line 913 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3774 "surface.c" /* yacc.c:1646  */
    break;

  case 206:
#line 914 "surface.yy" /* yacc.c:1646  */
    { CM->setPoly(tokenSequence); }
#line 3780 "surface.c" /* yacc.c:1646  */
    break;

  case 207:
#line 916 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::MEMO);
			}
#line 3788 "surface.c" /* yacc.c:1646  */
    break;

  case 208:
#line 920 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::CTOR);
			}
#line 3796 "surface.c" /* yacc.c:1646  */
    break;

  case 209:
#line 924 "surface.yy" /* yacc.c:1646  */
    {
			  clear();
			  CM->setFrozen(tokenSequence);
			}
#line 3805 "surface.c" /* yacc.c:1646  */
    break;

  case 210:
#line 928 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3811 "surface.c" /* yacc.c:1646  */
    break;

  case 211:
#line 929 "surface.yy" /* yacc.c:1646  */
    { CM->setFrozen(tokenSequence); }
#line 3817 "surface.c" /* yacc.c:1646  */
    break;

  case 212:
#line 931 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::CONFIG);
			}
#line 3825 "surface.c" /* yacc.c:1646  */
    break;

  case 213:
#line 935 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::OBJECT);
			}
#line 3833 "surface.c" /* yacc.c:1646  */
    break;

  case 214:
#line 939 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
#line 3841 "surface.c" /* yacc.c:1646  */
    break;

  case 215:
#line 943 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setMetadata((yyvsp[0].yyToken));
			}
#line 3849 "surface.c" /* yacc.c:1646  */
    break;

  case 216:
#line 946 "surface.yy" /* yacc.c:1646  */
    { lexerLatexMode(); }
#line 3855 "surface.c" /* yacc.c:1646  */
    break;

  case 217:
#line 947 "surface.yy" /* yacc.c:1646  */
    { CM->setLatexMacro((yyvsp[-1].yyString)); }
#line 3861 "surface.c" /* yacc.c:1646  */
    break;

  case 218:
#line 948 "surface.yy" /* yacc.c:1646  */
    {}
#line 3867 "surface.c" /* yacc.c:1646  */
    break;

  case 219:
#line 950 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::DITTO);
			}
#line 3875 "surface.c" /* yacc.c:1646  */
    break;

  case 221:
#line 960 "surface.yy" /* yacc.c:1646  */
    {}
#line 3881 "surface.c" /* yacc.c:1646  */
    break;

  case 222:
#line 963 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3887 "surface.c" /* yacc.c:1646  */
    break;

  case 223:
#line 964 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3893 "surface.c" /* yacc.c:1646  */
    break;

  case 224:
#line 967 "surface.yy" /* yacc.c:1646  */
    {}
#line 3899 "surface.c" /* yacc.c:1646  */
    break;

  case 225:
#line 968 "surface.yy" /* yacc.c:1646  */
    {}
#line 3905 "surface.c" /* yacc.c:1646  */
    break;

  case 226:
#line 971 "surface.yy" /* yacc.c:1646  */
    { clear(); CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[0].yyToken), tokenSequence); }
#line 3911 "surface.c" /* yacc.c:1646  */
    break;

  case 227:
#line 972 "surface.yy" /* yacc.c:1646  */
    { CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 3917 "surface.c" /* yacc.c:1646  */
    break;

  case 228:
#line 973 "surface.yy" /* yacc.c:1646  */
    { CM->addHook(SyntacticPreModule::OP_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 3923 "surface.c" /* yacc.c:1646  */
    break;

  case 229:
#line 974 "surface.yy" /* yacc.c:1646  */
    { CM->addHook(SyntacticPreModule::TERM_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 3929 "surface.c" /* yacc.c:1646  */
    break;

  case 230:
#line 980 "surface.yy" /* yacc.c:1646  */
    {}
#line 3935 "surface.c" /* yacc.c:1646  */
    break;

  case 231:
#line 982 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing " <<
				       QUOTE("is") << " keyword.");
			}
#line 3944 "surface.c" /* yacc.c:1646  */
    break;

  case 232:
#line 988 "surface.yy" /* yacc.c:1646  */
    {}
#line 3950 "surface.c" /* yacc.c:1646  */
    break;

  case 233:
#line 990 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			}
#line 3958 "surface.c" /* yacc.c:1646  */
    break;

  case 234:
#line 998 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3964 "surface.c" /* yacc.c:1646  */
    break;

  case 235:
#line 999 "surface.yy" /* yacc.c:1646  */
    {}
#line 3970 "surface.c" /* yacc.c:1646  */
    break;

  case 236:
#line 1002 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3976 "surface.c" /* yacc.c:1646  */
    break;

  case 237:
#line 1003 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3982 "surface.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1004 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3988 "surface.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1005 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3994 "surface.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1012 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  if (fragments.size() == 1)
			    t = fragments[0];
			  else
			    t.tokenize(Token::bubbleToPrefixNameCode(fragments), fragments[0].lineNumber());
			  fragClear();
			  (yyval.yyToken) = t;
			}
#line 4008 "surface.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1023 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4014 "surface.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1024 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4020 "surface.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1025 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4026 "surface.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1028 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4032 "surface.c" /* yacc.c:1646  */
    break;

  case 245:
#line 1029 "surface.yy" /* yacc.c:1646  */
    {}
#line 4038 "surface.c" /* yacc.c:1646  */
    break;

  case 246:
#line 1030 "surface.yy" /* yacc.c:1646  */
    {}
#line 4044 "surface.c" /* yacc.c:1646  */
    break;

  case 349:
#line 1110 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 1); }
#line 4050 "surface.c" /* yacc.c:1646  */
    break;

  case 350:
#line 1112 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setCurrentModule(lexerBubble);
			}
#line 4058 "surface.c" /* yacc.c:1646  */
    break;

  case 351:
#line 1115 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 1); }
#line 4064 "surface.c" /* yacc.c:1646  */
    break;

  case 352:
#line 1117 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->dump();
			}
#line 4073 "surface.c" /* yacc.c:1646  */
    break;

  case 353:
#line 1122 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4082 "surface.c" /* yacc.c:1646  */
    break;

  case 354:
#line 1127 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.parse(lexerBubble);
			}
#line 4092 "surface.c" /* yacc.c:1646  */
    break;

  case 355:
#line 1134 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4101 "surface.c" /* yacc.c:1646  */
    break;

  case 356:
#line 1139 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.creduce(lexerBubble);
			}
#line 4111 "surface.c" /* yacc.c:1646  */
    break;

  case 357:
#line 1146 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4120 "surface.c" /* yacc.c:1646  */
    break;

  case 358:
#line 1151 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sreduce(lexerBubble);
			}
#line 4130 "surface.c" /* yacc.c:1646  */
    break;

  case 359:
#line 1158 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4139 "surface.c" /* yacc.c:1646  */
    break;

  case 360:
#line 1163 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.reduce(lexerBubble, (yyvsp[-3].yyBool));
			}
#line 4149 "surface.c" /* yacc.c:1646  */
    break;

  case 361:
#line 1170 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4159 "surface.c" /* yacc.c:1646  */
    break;

  case 362:
#line 1176 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.rewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4169 "surface.c" /* yacc.c:1646  */
    break;

  case 363:
#line 1182 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4180 "surface.c" /* yacc.c:1646  */
    break;

  case 364:
#line 1189 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.eRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			}
#line 4190 "surface.c" /* yacc.c:1646  */
    break;

  case 365:
#line 1195 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4201 "surface.c" /* yacc.c:1646  */
    break;

  case 366:
#line 1202 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.fRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			}
#line 4211 "surface.c" /* yacc.c:1646  */
    break;

  case 367:
#line 1208 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4221 "surface.c" /* yacc.c:1646  */
    break;

  case 368:
#line 1214 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4231 "surface.c" /* yacc.c:1646  */
    break;

  case 369:
#line 1221 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4240 "surface.c" /* yacc.c:1646  */
    break;

  case 370:
#line 1226 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.check(lexerBubble);
			}
#line 4250 "surface.c" /* yacc.c:1646  */
    break;

  case 371:
#line 1233 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4261 "surface.c" /* yacc.c:1646  */
    break;

  case 372:
#line 1240 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(lexerBubble, number, number2, (yyvsp[-2].yySearchKind), (yyvsp[-3].yyBool));
			}
#line 4271 "surface.c" /* yacc.c:1646  */
    break;

  case 373:
#line 1246 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4281 "surface.c" /* yacc.c:1646  */
    break;

  case 374:
#line 1252 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.match(lexerBubble, (yyvsp[-2].yyBool), number);
			}
#line 4291 "surface.c" /* yacc.c:1646  */
    break;

  case 375:
#line 1258 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4301 "surface.c" /* yacc.c:1646  */
    break;

  case 376:
#line 1264 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.unify(lexerBubble, number);
			}
#line 4311 "surface.c" /* yacc.c:1646  */
    break;

  case 377:
#line 1270 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4321 "surface.c" /* yacc.c:1646  */
    break;

  case 378:
#line 1276 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantUnify(lexerBubble, number, (yyvsp[-4].yyBool));
			}
#line 4331 "surface.c" /* yacc.c:1646  */
    break;

  case 379:
#line 1283 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4341 "surface.c" /* yacc.c:1646  */
    break;

  case 380:
#line 1289 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.getVariants(lexerBubble, number, (yyvsp[-3].yyBool), (yyvsp[-5].yyBool));
			}
#line 4351 "surface.c" /* yacc.c:1646  */
    break;

  case 381:
#line 1295 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.cont((yyvsp[-1].yyInt64), (yyvsp[-3].yyBool));
			}
#line 4359 "surface.c" /* yacc.c:1646  */
    break;

  case 382:
#line 1299 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	test is a generic command to call code with a term for development purposes.
			  //
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4371 "surface.c" /* yacc.c:1646  */
    break;

  case 383:
#line 1307 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.test(lexerBubble);
			    //interpreter.newNarrow(lexerBubble);

			}
#line 4383 "surface.c" /* yacc.c:1646  */
    break;

  case 384:
#line 1315 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4392 "surface.c" /* yacc.c:1646  */
    break;

  case 385:
#line 1320 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.loop(lexerBubble);
			}
#line 4402 "surface.c" /* yacc.c:1646  */
    break;

  case 386:
#line 1326 "surface.yy" /* yacc.c:1646  */
    {
			  moduleExpr.contractTo(0);
			  if (interpreter.setCurrentModule(moduleExpr))  // HACK
			    interpreter.contLoop(lexerBubble);
			}
#line 4412 "surface.c" /* yacc.c:1646  */
    break;

  case 387:
#line 1331 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4418 "surface.c" /* yacc.c:1646  */
    break;

  case 388:
#line 1333 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.traceSelect((yyvsp[-2].yyBool));
			}
#line 4427 "surface.c" /* yacc.c:1646  */
    break;

  case 389:
#line 1337 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4433 "surface.c" /* yacc.c:1646  */
    break;

  case 390:
#line 1339 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.traceExclude((yyvsp[-2].yyBool));
			}
#line 4442 "surface.c" /* yacc.c:1646  */
    break;

  case 391:
#line 1343 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4448 "surface.c" /* yacc.c:1646  */
    break;

  case 392:
#line 1345 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.breakSelect((yyvsp[-2].yyBool));
			}
#line 4457 "surface.c" /* yacc.c:1646  */
    break;

  case 393:
#line 1349 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4463 "surface.c" /* yacc.c:1646  */
    break;

  case 394:
#line 1351 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.printConceal((yyvsp[-2].yyBool));
			}
#line 4472 "surface.c" /* yacc.c:1646  */
    break;

  case 395:
#line 1356 "surface.yy" /* yacc.c:1646  */
    {
			  if (CM != 0)  // HACK
			    CM->getFlatSignature()->clearMemo();
			}
#line 4481 "surface.c" /* yacc.c:1646  */
    break;

  case 396:
#line 1363 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4487 "surface.c" /* yacc.c:1646  */
    break;

  case 397:
#line 1365 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
#line 4496 "surface.c" /* yacc.c:1646  */
    break;

  case 398:
#line 1369 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4502 "surface.c" /* yacc.c:1646  */
    break;

  case 399:
#line 1371 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
#line 4511 "surface.c" /* yacc.c:1646  */
    break;

  case 400:
#line 1375 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4517 "surface.c" /* yacc.c:1646  */
    break;

  case 401:
#line 1377 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showModule(true);
			}
#line 4526 "surface.c" /* yacc.c:1646  */
    break;

  case 402:
#line 1381 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4532 "surface.c" /* yacc.c:1646  */
    break;

  case 403:
#line 1383 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentView(lexerBubble))
			    interpreter.showView();
			}
#line 4541 "surface.c" /* yacc.c:1646  */
    break;

  case 404:
#line 1388 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showModules(true);
			}
#line 4549 "surface.c" /* yacc.c:1646  */
    break;

  case 405:
#line 1392 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showNamedViews();
			}
#line 4557 "surface.c" /* yacc.c:1646  */
    break;

  case 406:
#line 1395 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4563 "surface.c" /* yacc.c:1646  */
    break;

  case 407:
#line 1397 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSortsAndSubsorts();
			}
#line 4572 "surface.c" /* yacc.c:1646  */
    break;

  case 408:
#line 1401 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4578 "surface.c" /* yacc.c:1646  */
    break;

  case 409:
#line 1403 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showOps();
			}
#line 4587 "surface.c" /* yacc.c:1646  */
    break;

  case 410:
#line 1407 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4593 "surface.c" /* yacc.c:1646  */
    break;

  case 411:
#line 1409 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showVars();
			}
#line 4602 "surface.c" /* yacc.c:1646  */
    break;

  case 412:
#line 1413 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4608 "surface.c" /* yacc.c:1646  */
    break;

  case 413:
#line 1415 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showMbs();
			}
#line 4617 "surface.c" /* yacc.c:1646  */
    break;

  case 414:
#line 1419 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4623 "surface.c" /* yacc.c:1646  */
    break;

  case 415:
#line 1421 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showEqs();
			}
#line 4632 "surface.c" /* yacc.c:1646  */
    break;

  case 416:
#line 1425 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4638 "surface.c" /* yacc.c:1646  */
    break;

  case 417:
#line 1427 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showRls();
			}
#line 4647 "surface.c" /* yacc.c:1646  */
    break;

  case 418:
#line 1431 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4653 "surface.c" /* yacc.c:1646  */
    break;

  case 419:
#line 1433 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSummary();
			}
#line 4662 "surface.c" /* yacc.c:1646  */
    break;

  case 420:
#line 1437 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4668 "surface.c" /* yacc.c:1646  */
    break;

  case 421:
#line 1439 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showKinds();
			}
#line 4677 "surface.c" /* yacc.c:1646  */
    break;

  case 422:
#line 1444 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showSearchPath((yyvsp[-1].yyInt64));
			}
#line 4685 "surface.c" /* yacc.c:1646  */
    break;

  case 423:
#line 1448 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showSearchPathLabels((yyvsp[-1].yyInt64));
			}
#line 4693 "surface.c" /* yacc.c:1646  */
    break;

  case 424:
#line 1452 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showSearchGraph();
			}
#line 4701 "surface.c" /* yacc.c:1646  */
    break;

  case 425:
#line 1455 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4707 "surface.c" /* yacc.c:1646  */
    break;

  case 426:
#line 1457 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showProfile();
			}
#line 4716 "surface.c" /* yacc.c:1646  */
    break;

  case 427:
#line 1465 "surface.yy" /* yacc.c:1646  */
    {
			  globalAdvisoryFlag = alwaysAdviseFlag ? true : (yyvsp[-1].yyBool);
			}
#line 4724 "surface.c" /* yacc.c:1646  */
    break;

  case 428:
#line 1469 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			}
#line 4732 "surface.c" /* yacc.c:1646  */
    break;

  case 429:
#line 1473 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_STATS, (yyvsp[-1].yyBool));
			}
#line 4740 "surface.c" /* yacc.c:1646  */
    break;

  case 430:
#line 1477 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_TIMING, (yyvsp[-1].yyBool));
			}
#line 4748 "surface.c" /* yacc.c:1646  */
    break;

  case 431:
#line 1481 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_BREAKDOWN, (yyvsp[-1].yyBool));
			}
#line 4756 "surface.c" /* yacc.c:1646  */
    break;

  case 432:
#line 1485 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_TIMING, (yyvsp[-1].yyBool));
			}
#line 4764 "surface.c" /* yacc.c:1646  */
    break;

  case 433:
#line 1489 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_COMMAND, (yyvsp[-1].yyBool));
			}
#line 4772 "surface.c" /* yacc.c:1646  */
    break;

  case 434:
#line 1493 "surface.yy" /* yacc.c:1646  */
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			}
#line 4780 "surface.c" /* yacc.c:1646  */
    break;

  case 435:
#line 1497 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setPrintFlag((yyvsp[-2].yyPrintFlags), (yyvsp[-1].yyBool));
			}
#line 4788 "surface.c" /* yacc.c:1646  */
    break;

  case 436:
#line 1501 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE, (yyvsp[-1].yyBool));
			}
#line 4796 "surface.c" /* yacc.c:1646  */
    break;

  case 437:
#line 1505 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE_NEWLINE, (yyvsp[-1].yyBool));
			}
#line 4804 "surface.c" /* yacc.c:1646  */
    break;

  case 438:
#line 1509 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag((yyvsp[-2].yyFlags), (yyvsp[-1].yyBool));
			}
#line 4812 "surface.c" /* yacc.c:1646  */
    break;

  case 439:
#line 1513 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::BREAK, (yyvsp[-1].yyBool));
			}
#line 4820 "surface.c" /* yacc.c:1646  */
    break;

  case 440:
#line 1516 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4826 "surface.c" /* yacc.c:1646  */
    break;

  case 441:
#line 1517 "surface.yy" /* yacc.c:1646  */
    { lexerInitialMode(); }
#line 4832 "surface.c" /* yacc.c:1646  */
    break;

  case 442:
#line 1519 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setAutoImport((yyvsp[-5].yyImportMode), (yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			}
#line 4840 "surface.c" /* yacc.c:1646  */
    break;

  case 443:
#line 1522 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4846 "surface.c" /* yacc.c:1646  */
    break;

  case 444:
#line 1523 "surface.yy" /* yacc.c:1646  */
    { lexerInitialMode(); }
#line 4852 "surface.c" /* yacc.c:1646  */
    break;

  case 445:
#line 1525 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setOmodInclude((yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			}
#line 4860 "surface.c" /* yacc.c:1646  */
    break;

  case 446:
#line 1529 "surface.yy" /* yacc.c:1646  */
    {
			  globalVerboseFlag = (yyvsp[-1].yyBool);
			}
#line 4868 "surface.c" /* yacc.c:1646  */
    break;

  case 447:
#line 1533 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_MEMO, (yyvsp[-1].yyBool));
			}
#line 4876 "surface.c" /* yacc.c:1646  */
    break;

  case 448:
#line 1537 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_RULES, (yyvsp[-1].yyBool));
			}
#line 4884 "surface.c" /* yacc.c:1646  */
    break;

  case 449:
#line 1541 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::COMPILE_COUNT, (yyvsp[-1].yyBool));
			}
#line 4892 "surface.c" /* yacc.c:1646  */
    break;

  case 450:
#line 1545 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::PROFILE, (yyvsp[-1].yyBool));
			}
#line 4900 "surface.c" /* yacc.c:1646  */
    break;

  case 451:
#line 1549 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_PROFILE, (yyvsp[-1].yyBool));
			}
#line 4908 "surface.c" /* yacc.c:1646  */
    break;

  case 452:
#line 1556 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::RESUME;
			}
#line 4916 "surface.c" /* yacc.c:1646  */
    break;

  case 453:
#line 1560 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::ABORT;
			}
#line 4924 "surface.c" /* yacc.c:1646  */
    break;

  case 454:
#line 1564 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			}
#line 4932 "surface.c" /* yacc.c:1646  */
    break;

  case 455:
#line 1568 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::WHERE;
			}
#line 4940 "surface.c" /* yacc.c:1646  */
    break;

  case 456:
#line 1575 "surface.yy" /* yacc.c:1646  */
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			}
#line 4948 "surface.c" /* yacc.c:1646  */
    break;

  case 457:
#line 1579 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			}
#line 4956 "surface.c" /* yacc.c:1646  */
    break;

  case 458:
#line 1585 "surface.yy" /* yacc.c:1646  */
    { lexerInitialMode(); }
#line 4962 "surface.c" /* yacc.c:1646  */
    break;

  case 460:
#line 1592 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_MIXFIX; }
#line 4968 "surface.c" /* yacc.c:1646  */
    break;

  case 461:
#line 1593 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FLAT; }
#line 4974 "surface.c" /* yacc.c:1646  */
    break;

  case 462:
#line 1594 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_ALIASES; }
#line 4980 "surface.c" /* yacc.c:1646  */
    break;

  case 463:
#line 1595 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_PARENS; }
#line 4986 "surface.c" /* yacc.c:1646  */
    break;

  case 464:
#line 1596 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_GRAPH; }
#line 4992 "surface.c" /* yacc.c:1646  */
    break;

  case 465:
#line 1597 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_CONCEAL; }
#line 4998 "surface.c" /* yacc.c:1646  */
    break;

  case 466:
#line 1598 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_NUMBER; }
#line 5004 "surface.c" /* yacc.c:1646  */
    break;

  case 467:
#line 1599 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_RAT; }
#line 5010 "surface.c" /* yacc.c:1646  */
    break;

  case 468:
#line 1600 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_COLOR; }
#line 5016 "surface.c" /* yacc.c:1646  */
    break;

  case 469:
#line 1601 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FORMAT; }
#line 5022 "surface.c" /* yacc.c:1646  */
    break;

  case 470:
#line 1604 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE; }
#line 5028 "surface.c" /* yacc.c:1646  */
    break;

  case 471:
#line 1605 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_CONDITION; }
#line 5034 "surface.c" /* yacc.c:1646  */
    break;

  case 472:
#line 1606 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_WHOLE; }
#line 5040 "surface.c" /* yacc.c:1646  */
    break;

  case 473:
#line 1607 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_SUBSTITUTION; }
#line 5046 "surface.c" /* yacc.c:1646  */
    break;

  case 474:
#line 1608 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_SELECT; }
#line 5052 "surface.c" /* yacc.c:1646  */
    break;

  case 475:
#line 1609 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_MB; }
#line 5058 "surface.c" /* yacc.c:1646  */
    break;

  case 476:
#line 1610 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_EQ; }
#line 5064 "surface.c" /* yacc.c:1646  */
    break;

  case 477:
#line 1611 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_RL; }
#line 5070 "surface.c" /* yacc.c:1646  */
    break;

  case 478:
#line 1612 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_REWRITE; }
#line 5076 "surface.c" /* yacc.c:1646  */
    break;

  case 479:
#line 1613 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_BODY; }
#line 5082 "surface.c" /* yacc.c:1646  */
    break;

  case 480:
#line 1614 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_BUILTIN; }
#line 5088 "surface.c" /* yacc.c:1646  */
    break;

  case 481:
#line 1617 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5094 "surface.c" /* yacc.c:1646  */
    break;

  case 482:
#line 1618 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5100 "surface.c" /* yacc.c:1646  */
    break;

  case 483:
#line 1621 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5106 "surface.c" /* yacc.c:1646  */
    break;

  case 484:
#line 1622 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5112 "surface.c" /* yacc.c:1646  */
    break;

  case 485:
#line 1625 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5118 "surface.c" /* yacc.c:1646  */
    break;

  case 486:
#line 1626 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5124 "surface.c" /* yacc.c:1646  */
    break;

  case 487:
#line 1629 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5130 "surface.c" /* yacc.c:1646  */
    break;

  case 488:
#line 1630 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5136 "surface.c" /* yacc.c:1646  */
    break;

  case 489:
#line 1635 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::NARROW; }
#line 5142 "surface.c" /* yacc.c:1646  */
    break;

  case 490:
#line 1636 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::XG_NARROW; }
#line 5148 "surface.c" /* yacc.c:1646  */
    break;

  case 491:
#line 1637 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::SEARCH; }
#line 5154 "surface.c" /* yacc.c:1646  */
    break;

  case 492:
#line 1638 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::SMT_SEARCH; }
#line 5160 "surface.c" /* yacc.c:1646  */
    break;

  case 493:
#line 1639 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::VU_NARROW; }
#line 5166 "surface.c" /* yacc.c:1646  */
    break;

  case 494:
#line 1640 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::FVU_NARROW; }
#line 5172 "surface.c" /* yacc.c:1646  */
    break;

  case 495:
#line 1643 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5178 "surface.c" /* yacc.c:1646  */
    break;

  case 496:
#line 1644 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5184 "surface.c" /* yacc.c:1646  */
    break;

  case 497:
#line 1647 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5190 "surface.c" /* yacc.c:1646  */
    break;

  case 498:
#line 1648 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5196 "surface.c" /* yacc.c:1646  */
    break;

  case 499:
#line 1651 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5202 "surface.c" /* yacc.c:1646  */
    break;

  case 500:
#line 1652 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5208 "surface.c" /* yacc.c:1646  */
    break;

  case 501:
#line 1655 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyInt64) = (yyvsp[0].yyInt64); }
#line 5214 "surface.c" /* yacc.c:1646  */
    break;

  case 502:
#line 1656 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyInt64) = NONE; }
#line 5220 "surface.c" /* yacc.c:1646  */
    break;

  case 503:
#line 1659 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyImportMode) = ImportModule::PROTECTING; }
#line 5226 "surface.c" /* yacc.c:1646  */
    break;

  case 504:
#line 1660 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyImportMode) = ImportModule::EXTENDING; }
#line 5232 "surface.c" /* yacc.c:1646  */
    break;

  case 505:
#line 1661 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyImportMode) = ImportModule::INCLUDING; }
#line 5238 "surface.c" /* yacc.c:1646  */
    break;

  case 507:
#line 1671 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5244 "surface.c" /* yacc.c:1646  */
    break;

  case 509:
#line 1675 "surface.yy" /* yacc.c:1646  */
    { moduleExpr = lexerBubble; lexBubble(END_COMMAND, 1); }
#line 5250 "surface.c" /* yacc.c:1646  */
    break;

  case 512:
#line 1687 "surface.yy" /* yacc.c:1646  */
    { lexSave((yyvsp[0].yyToken)); }
#line 5256 "surface.c" /* yacc.c:1646  */
    break;

  case 515:
#line 1690 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0);  }
#line 5262 "surface.c" /* yacc.c:1646  */
    break;

  case 517:
#line 1695 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5268 "surface.c" /* yacc.c:1646  */
    break;

  case 519:
#line 1697 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5274 "surface.c" /* yacc.c:1646  */
    break;

  case 522:
#line 1703 "surface.yy" /* yacc.c:1646  */
    { number = Token::codeToInt64(lexerBubble[1].code()); }
#line 5280 "surface.c" /* yacc.c:1646  */
    break;

  case 524:
#line 1705 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5286 "surface.c" /* yacc.c:1646  */
    break;

  case 527:
#line 1720 "surface.yy" /* yacc.c:1646  */
    { lexSave((yyvsp[0].yyToken)); }
#line 5292 "surface.c" /* yacc.c:1646  */
    break;

  case 530:
#line 1723 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5298 "surface.c" /* yacc.c:1646  */
    break;

  case 532:
#line 1732 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5304 "surface.c" /* yacc.c:1646  */
    break;

  case 534:
#line 1734 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5310 "surface.c" /* yacc.c:1646  */
    break;

  case 536:
#line 1736 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5316 "surface.c" /* yacc.c:1646  */
    break;

  case 539:
#line 1746 "surface.yy" /* yacc.c:1646  */
    { number = Token::codeToInt64(lexerBubble[1].code()); }
#line 5322 "surface.c" /* yacc.c:1646  */
    break;

  case 541:
#line 1748 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5328 "surface.c" /* yacc.c:1646  */
    break;

  case 543:
#line 1750 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5334 "surface.c" /* yacc.c:1646  */
    break;

  case 546:
#line 1760 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5340 "surface.c" /* yacc.c:1646  */
    break;

  case 548:
#line 1762 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5346 "surface.c" /* yacc.c:1646  */
    break;

  case 551:
#line 1773 "surface.yy" /* yacc.c:1646  */
    {
			  number = Token::codeToInt64(lexerBubble[1].code());
			  number2 = Token::codeToInt64(lexerBubble[3].code());
			  clear();
			}
#line 5356 "surface.c" /* yacc.c:1646  */
    break;

  case 553:
#line 1779 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5362 "surface.c" /* yacc.c:1646  */
    break;

  case 556:
#line 1789 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5368 "surface.c" /* yacc.c:1646  */
    break;

  case 558:
#line 1791 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5374 "surface.c" /* yacc.c:1646  */
    break;

  case 561:
#line 1801 "surface.yy" /* yacc.c:1646  */
    { number2 = Token::codeToInt64(lexerBubble[2].code()); }
#line 5380 "surface.c" /* yacc.c:1646  */
    break;

  case 563:
#line 1803 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5386 "surface.c" /* yacc.c:1646  */
    break;

  case 566:
#line 1808 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0, 1); }
#line 5392 "surface.c" /* yacc.c:1646  */
    break;

  case 569:
#line 1814 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 1, 1); }
#line 5398 "surface.c" /* yacc.c:1646  */
    break;

  case 571:
#line 1816 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON | END_COMMAND, 0); }
#line 5404 "surface.c" /* yacc.c:1646  */
    break;

  case 573:
#line 1819 "surface.yy" /* yacc.c:1646  */
    {
			  lexerBubble.resize(1);
			  lexerBubble[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(lexerBubble[0]);
			}
#line 5414 "surface.c" /* yacc.c:1646  */
    break;

  case 611:
#line 1858 "surface.yy" /* yacc.c:1646  */
    {}
#line 5420 "surface.c" /* yacc.c:1646  */
    break;

  case 613:
#line 1863 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  interpreter.addSelected(singleton);
			}
#line 5430 "surface.c" /* yacc.c:1646  */
    break;

  case 616:
#line 1874 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  interpreter.addSelected(singleton);
			}
#line 5439 "surface.c" /* yacc.c:1646  */
    break;

  case 617:
#line 1879 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.addSelected(lexerBubble);
			}
#line 5447 "surface.c" /* yacc.c:1646  */
    break;


#line 5451 "surface.c" /* yacc.c:1646  */
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
#line 1909 "surface.yy" /* yacc.c:1906  */


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

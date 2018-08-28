/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.

*/

//
//      Class for modules with mixfix syntax.
//
#ifndef _mixfixModule_hh_
#define _mixfixModule_hh_
#include <gmpxx.h>
#include <set>
#include <map>
#include <list>
#include "profileModule.hh"
#include "metadataStore.hh"
#include "sharedTokens.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "token.hh"
#include "pointerSet.hh"
#include "symbolType.hh"
#include "SMT_Info.hh"
#include "SMT_NumberSymbol.hh"

class MixfixModule : public ProfileModule, public MetadataStore, protected SharedTokens
{
  NO_COPYING(MixfixModule);

  enum Bits
  {
    SYSTEM = 1,
    THEORY = 2
  };

public:
  enum ModuleType
  {
    FUNCTIONAL_MODULE = 0,
    SYSTEM_MODULE = SYSTEM,
    FUNCTIONAL_THEORY = THEORY,
    SYSTEM_THEORY = SYSTEM | THEORY
  };

  enum GatherSymbols
  {
    GATHER_e = -1,
    GATHER_E = 0,
    GATHER_AMP = 1
  };

  typedef map<int, Sort*> AliasMap;

  MixfixModule(int name, ModuleType moduleType);
  ~MixfixModule();

  void closeSignature();
  void economize();
  //
  //	Functions to insert stuff.
  //
  void insertPotentialLabels(const set<int>& l);
  Sort* addSort(int name);
  Symbol* addOpDeclaration(Token prefixName,
			   const Vector<Sort*>& domainAndRange,
			   SymbolType symbolType,
			   const Vector<int>& strategy,
			   const NatSet& frozen,
			   int prec,
			   const Vector<int>& gather,
			   const Vector<int>& format,
			   int metadata,
			   bool& firstDecl);
  void addVariableAlias(Token name, Sort* sort);
  int addPolymorph(Token prefixName,
		   const Vector<Sort*>& domainAndRange,
		   SymbolType symbolType,
		   const Vector<int>& strategy,
		   const NatSet& frozen,
		   int prec,
		   const Vector<int>& gather,
		   const Vector<int>& format,
		   int metadata);
  void addIdentityToPolymorph(int polymorphIndex,
			      Term* identity);
  void addIdHookToPolymorph(int polymorphIndex,
			    int purpose,
			    const Vector<int>& data);
  void addOpHookToPolymorph(int polymorphIndex,
			    int purpose,
			    Symbol* symbol);
  void addTermHookToPolymorph(int polymorphIndex,
			      int purpose,
			      Term* term);
  //
  //	Functions to make things.
  //
  Symbol* instantiateSortTest(Sort* sort, bool eager);
  Symbol* instantiateVariable(Sort* sort);
  Symbol* instantiatePolymorph(int polymorphIndex, int kindIndex);
  Term* makeTrueTerm();
  Term* makeBubble(int bubbleSpecIndex, const Vector<Token>& tokens, int first, int last);
  DagNode* makeUnificationProblemDag(Vector<Term*>& lhs, Vector<Term*>& rhs);

  static void printCondition(ostream& s, const Vector<ConditionFragment*>& condition);
  static void printCondition(ostream& s, const PreEquation* pe);
  void printAttributes(ostream& s, const PreEquation* pe, ItemType itemType);
  //
  //	Parsing functions.
  //
  Term* parseTerm(const Vector<Token>& bubble,
		  ConnectedComponent* component = 0,
		  int begin = 0,
		  int end = DEFAULT);
  int parseTerm2(const Vector<Token>& bubble,
		 ConnectedComponent* component,
		 Term*& parse1,
		 Term*& parse2,
		 int& firstBad);
  void parseStatement(const Vector<Token>& bubble);
  bool parseSearchCommand(const Vector<Token>& bubble,
			  Term*& initial,
			  int& searchType,
			  Term*& target,
			  Vector<ConditionFragment*>& condition);
  bool parseMatchCommand(const Vector<Token>& bubble,
			 Term*& pattern,
			 Term*& subject,
			 Vector<ConditionFragment*>& condition);
  bool parseUnifyCommand(const Vector<Token>& bubble,
			 Vector<Term*>& lhs,
			 Vector<Term*>& rhs);
  bool parseStrategyCommand(const Vector<Token>& bubble,
			    Term*& subject,
			    StrategyExpression*& strategy);
  bool parseGetVariantsCommand(const Vector<Token>& bubble,
			       Term*& initial,
			       Vector<Term*>& constraints);
  bool parseVariantUnifyCommand(const Vector<Token>& bubble,
				Vector<Term*>& lhs,
				Vector<Term*>& rhs,
				Vector<Term*>& constraints);
  //
  //	Get functions.
  //
  ModuleType getModuleType() const;
  static const char* moduleTypeString(ModuleType type);
  static const char* moduleEndString(ModuleType type);
  SymbolType getSymbolType(Symbol* symbol) const;
  int getPrec(Symbol* symbol) const;
  void getGather(Symbol* symbol, Vector<int>& gather) const;
  const Vector<int>& getFormat(Symbol* symbol) const;
  const AliasMap& getVariableAliases() const;
  void getParserStats(int& nrNonterminals, int& nrTerminals, int& nrProductions);
  void getDataAttachments(Symbol* symbol,
			  const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data) const;
  void getSymbolAttachments(Symbol* symbol,
			    Vector<const char*>& purposes,
			    Vector<Symbol*>& symbols) const;
  void getTermAttachments(Symbol* symbol,
			  Vector<const char*>& purposes,
			  Vector<Term*>& terms) const;
  //
  //	Find functions.
  //
  Sort* findSort(int name) const;
  Symbol* findSymbol(int name,
		     const Vector<ConnectedComponent*>& domainComponents,
		     ConnectedComponent* rangeComponent);
  QuotedIdentifierSymbol* findQuotedIdentifierSymbol(const ConnectedComponent* component) const;
  StringSymbol* findStringSymbol(const ConnectedComponent* component) const;
  FloatSymbol* findFloatSymbol(const ConnectedComponent* component) const;
  SMT_NumberSymbol* findSMT_NumberSymbol(const ConnectedComponent* component, SMT_Info::SMT_Type type);
  int findPolymorphIndex(int polymorphName, const Vector<Sort*>& domainAndRange) const;
  //
  //	Polymorph functions.
  //
  void copyFixUpPolymorph(int polymorphIndex,
			  const MixfixModule* originalModule,
			  int originalPolymorphIndex,
			  SymbolMap* map);
  int getNrPolymorphs() const;
  Token getPolymorphName(int index) const;
  SymbolType getPolymorphType(int index) const;
  const Vector<Sort*>& getPolymorphDomainAndRange(int index) const;
  Term* getPolymorphIdentity(int index) const;
  const Vector<int>& getPolymorphStrategy(int index) const;
  const NatSet& getPolymorphFrozen(int index) const;
  int getPolymorphPrec(int index) const;
  void getPolymorphGather(int index, Vector<int>& gather) const;
  const Vector<int>& getPolymorphFormat(int index) const;
  int getPolymorphMetadata(int index) const;
  bool getPolymorphDataAttachment(int index, int nr, int& purpose, Vector<int>& items) const;
  bool getPolymorphSymbolAttachment(int index, int nr, int& purpose, Symbol*& op) const;
  bool getPolymorphTermAttachment(int index, int nr, int& purpose, Term*& term) const;
  //
  //	Bubble functions.
  //
  int addBubbleSpec(Symbol* topSymbol,
		    int lowerBound,
		    int upperBound,
		    int leftParenToken,
		    int rightParenToken,
		    const Vector<int>& excludedTokens);
  void copyBubbleSpec(Symbol* originalSymbol, Symbol* newSymbol);
  void copyFixUpBubbleSpec(Symbol* originalSymbol, SymbolMap* map);
  void fixUpBubbleSpec(int bubbleSpecIndex,
		       Symbol* qidSymbol,
		       Symbol* nilQidListSymbol,
		       Symbol* qidListSymbol);
  //
  //	Pretty print functions.
  //
  void bufferPrint(Vector<int>& buffer, Term* term, int printFlags);
  static Sort* disambiguatorSort(const Term* term);
  int getSMT_NumberToken(const mpq_class& value, Sort* sort);
  //
  //	Misc.
  //
  static Sort* hookSort(Sort* sort);
  static ModuleType join(ModuleType t1, ModuleType t2);
  static bool isTheory(ModuleType t);
  bool isTheory() const;
  static bool canImport(ModuleType t1, ModuleType t2);
  static bool canHaveAsParameter(ModuleType t1, ModuleType t2);
  const SMT_Info& getSMT_Info();
  bool validForSMT_Rewriting();
  void checkFreshVariableNames();
  static Term* findNonlinearVariable(Term* term, VariableInfo& variableInfo);
  Symbol* findSMT_Symbol(Term* term);

protected:
  static int findMatchingParen(const Vector<Token>& tokens, int pos);
  
public:  // HACK
  enum Precedence
  {
    MIN_PREC = 0,
    MAX_PREC = 127,
    ANY = 127,
    PREFIX_PREC = 0,
    PREFIX_GATHER = 95,		// to allow _,_ to work correctly
    UNARY_PREC = 15,		// backward compatibility with OBJ3 defaults
    INFIX_PREC = 41,		// backward compatibility with OBJ3 defaults
    //
    //	Precedences for complex syntax
    //
    ASSIGNMENT_PREC = 75,
    //
    //	Precedences for strategy language.
    //
    STRAT_BASIC_PREC = 0,
    STRAT_TEST_PREC = 21,
    STRAT_REW_PREC = 21,
    STRAT_SEQ_PREC = 39,
    STRAT_UNION_PREC = 41,
    STRAT_ORELSE_PREC = 43,
    STRAT_BRANCH_PREC = 55,
    STRAT_USING_LIST_PREC = 61,
    STRAT_USING_PREC = 21
  };

private:
  enum InternalFlags
  {
    LEFT_BARE = 0x1,		// operator name has leading argument underscore
    RIGHT_BARE = 0x2,		// operator name has trailing argument underscore
    ADHOC_OVERLOADED = 0x4,	// another operator with the same name exists
    DOMAIN_OVERLOADED = 0x8,	// another operator with the same name and domain kinds exists
    RANGE_OVERLOADED = 0x10,	// another operator with the same name and range kind exists
    
    PSEUDO_STRING = 0x40,	// nullary operator looks like a built-in string
    PSEUDO_QUOTED_IDENTIFIER = 0x80,	// nullary operator looks like a built-in quoted identifier
    PSEUDO_FLOAT = 0x100,	// nullary operator looks like a built-in float
    PSEUDO_VARIABLE = 0x200,	// nullary operator looks like a explicit variable (i.e. X:Foo)

    PSEUDO_NAT = 0x1000,	// nullary operator looks like a positive integer
    PSEUDO_NEG = 0x2000,	// nullary operator looks like a negative integer
    PSEUDO_RAT = 0x4000,	// nullary operator looks like a rational
    PSEUDO_ZERO = 0x8000,	// nullary operator looks like zero integer
    
    PSEUDOS = PSEUDO_VARIABLE | PSEUDO_FLOAT | PSEUDO_STRING | PSEUDO_QUOTED_IDENTIFIER |
      PSEUDO_NAT | PSEUDO_NEG | PSEUDO_RAT | PSEUDO_ZERO
  };

  enum NonTerminal
  {
    //
    //	Nonterminals needed to parse modules and simple commands.
    //
    TERM = -1,
    SMALL_NAT = -2,
    SMALL_NEG = -3,
    QUOTED_ID = -4,
    STRING_NT = -5,
    FLOAT_NT = -6,
    ENDS_IN_COLON_NT = -7,
    
    COLON_PAIR = -8,
    COLON2_PAIR = -9,
    COLON3_PAIR = -10,
    EQUALITY_PAIR = -11,
    ARROW_PAIR = -12,
    ASSIGN_PAIR = -13,

    LABEL = -14,
    CONDITION_FRAGMENT = -15,
    RULE_CONDITION_FRAGMENT = -16,
    CONDITION = -17,
    RULE_CONDITION = -18,

    MB_BODY = -19,
    EQ_BODY = -20,
    RL_BODY = -21,
    
    STATEMENT_PART = -22,
    ATTRIBUTE_PART = -23,
    ATTRIBUTE_LIST = -24,
    ATTRIBUTE = -25,
    STATEMENT = -26,

    ZERO = -27,
    RATIONAL = -28,

    VARIABLE = -29,
    PRINT_ITEM = -30,
    PRINT_LIST = -31,

    SIMPLE_BASE = -32,
    //
    //	Extra nonterminals needed to parse complex commands.
    //
    SEARCH_CONNECTIVE = SIMPLE_BASE,
    MATCH_PAIR = SIMPLE_BASE - 1,
    SEARCH_PAIR = SIMPLE_BASE - 2,
    SUCH_THAT = SIMPLE_BASE - 3,
    STRATEGY_EXPRESSION = SIMPLE_BASE - 4,

    MATCH_COMMAND = SIMPLE_BASE - 5,
    SEARCH_COMMAND = SIMPLE_BASE - 6,
    STRATEGY_COMMAND = SIMPLE_BASE - 7,

    ASSIGNMENT = SIMPLE_BASE - 8,
    SUBSTITUTION = SIMPLE_BASE - 9,

    STRATEGY_LIST = SIMPLE_BASE - 10,

    UNIFY_PAIR = SIMPLE_BASE - 11,
    UNIFY_COMMAND = SIMPLE_BASE - 12,

    GET_VARIANTS_COMMAND = SIMPLE_BASE - 13,
    TERM_LIST = SIMPLE_BASE - 14,  // for command separated list of hetrogeneous terms

    VARIANT_UNIFY_COMMAND = SIMPLE_BASE - 15,

    USING_PAIR = SIMPLE_BASE - 16,
    USING_LIST = SIMPLE_BASE - 17,

    COMPLEX_BASE = SIMPLE_BASE - 18
  };

  enum NonTerminalType
  {
    NUMBER_OF_TYPES = 5,
    TERM_TYPE = 0,
    SORT_TYPE = 1,
    DOT_SORT_TYPE = 2,
    ASSOC_LIST_TYPE = 3,
    SORT_LIST_TYPE = 4
  };

  enum SMT_Status
    {
      UNCHECKED = -1,
      BAD = 0,
      GOOD = 1
    };

  struct SymbolInfo
  {
    void revertGather(Vector<int>& gatherSymbols) const;

    Vector<int> mixfixSyntax;
    Vector<int> gather;
    Vector<int> format;
    int prec;
    SymbolType symbolType;
    int iflags;
    int next;
  };

  struct IdHook
  {
    int purpose;
    Vector<int> data;
  };

  struct OpHook
  {
    int purpose;
    Symbol* symbol;
  };

  struct TermHook
  {
    int purpose;
    Term* term;
  };

  struct Polymorph
  {
    Token name;
    Vector<Sort*> domainAndRange;
    Vector<int> strategy;
    NatSet frozen;
    Term* identity;
    int metadata;
    Vector<IdHook> idHooks;
    Vector<OpHook> opHooks;
    Vector<TermHook> termHooks;
    Vector<Symbol*> instantiations;
    SymbolInfo symbolInfo;
  };

  //
  //	Information needed for node coloring in pretty printer.
  //
  enum DagNodeStatus
  {
    CONSTRUCTOR = 1,
    STRANGENESS_BELOW = 2
  };

  struct ColoringInfo
  {
    PointerSet visited;		// set of all nodes visited on 1st pass
    Vector<int> statusVec;	// status computed on 1st pass
    bool reducedAbove;		// updated on 2nd pass
    bool reducedDirectlyAbove;  // updated on 2nd pass
  };

  int nonTerminal(int componentIndex, NonTerminalType type);
  int nonTerminal(const Sort* sort, NonTerminalType type);
  int newNonTerminal();

  static int domainComponentIndex(const Symbol* symbol, int argNr);
  static int mayAssoc(Symbol* symbol, int argNr);

  Symbol* newFancySymbol(Token prefixName,
			 const Vector<Sort*>& domainAndRange,
			 SymbolType symbolType,
			 const Vector<int>& strategy);

  void makeGrammar(bool complexFlag = false);
  void makeComplexProductions();
  void makeStrategyLanguageProductions();
  void makeStatementProductions();
  void makeConditionProductions();
  void makeAttributeProductions();
  void makeParameterizedSortProductions();
  void makeComponentProductions();
  void makeSymbolProductions();
  void makeVariableProductions();
  void makeBoolProductions();
  void makeLabelProductions();
  void makeSpecialProductions();
  void makePolymorphProductions();
  void makeBubbleProductions();
  void computePrecAndGather(int nrArgs, SymbolInfo& si, Symbol* symbol = 0);

  static Term* findNonlinearVariable(Term* term, NatSet& seenIndices);

  void printVariable(ostream& s, int name, Sort* sort) const;
  void graphPrint(ostream& s, DagNode* dagNode);

  static void printPrefixName(ostream& s, const char* prefixName, SymbolInfo& si);
  static int printTokens(ostream& s,
			 const SymbolInfo& si,
			 int pos,
			 const char* color = 0);
  static void printTails(ostream& s,
			 const SymbolInfo& si,
			 int pos,
			 int nrTails,
			 bool needAssocParen,
			 bool checkForInterrupt = false,
			 const char* color = 0);
  static bool fancySpace(ostream& s, int spaceToken);
  static int chooseDisambiguator(Symbol* s);
  static void graphCount(DagNode* dagNode, PointerSet& visited, Vector<mpz_class>& counts);
  static void makeIterName(string& name, int id, const mpz_class& number);
  static int computeGraphStatus(DagNode* dagNode,
				PointerSet& visited,
				Vector<int>& statusVec);
  //
  //	Member functions for Term* -> Vector<int>&  pretty printer.
  //
  static void prefix(Vector<int>& buffer, bool needDisambig);
  static void suffix(Vector<int>& buffer, Term* term, bool needDisambig, int printFlags);
  void prettyPrint(Vector<int>& buffer,
		   Term* term,
		   int requiredPrec,
		   int leftCapture,
		   const ConnectedComponent* leftCaptureComponent,
		   int rightCapture,
		   const ConnectedComponent* rightCaptureComponent,
		   bool rangeKnown,
		   int printFlags);

  static void handleVariable(Vector<int>& buffer, Term* term, int printFlags);
  bool handleIter(Vector<int>& buffer, Term* term, SymbolInfo& si, bool rangeKnown, int printFlags);
  bool handleMinus(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags);
  bool handleDivision(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags);
  void handleFloat(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags);
  void handleString(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags);
  void handleQuotedIdentifier(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags);
  void handleSMT_NumberSymbol(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags);

  static void printKind(Vector<int>& buffer, const Sort* kind, int printFlags);
  static void printSort(Vector<int>& buffer, const Sort* sort, int printFlags);
  static void printDotSort(Vector<int>& buffer, const Sort* sort, int printFlags);
  static void printVarSort(Vector<int>& buffer, string& fullName, const Sort* sort, int printFlags);

  int printTokens(Vector<int>& buffer, const SymbolInfo& si, int pos, int printFlags);
  void printTails(Vector<int>& buffer,
		  const SymbolInfo& si,
		  int pos,
		  int nrTails,
		  bool needAssocParen,
		  int printFlags);
  void printPrefixName(Vector<int>& buffer, int prefixName, SymbolInfo& si, int printFlags);
  void handleFormat(Vector<int>& buffer, int spaceToken);

  static Vector<int> emptyGather;
  static Vector<int> gatherAny;
  static Vector<int> gatherAnyAny;
  static Vector<int> gatherAnyAnyAny;
  static Vector<int> gatherAny4;
  static Vector<int> gatherPrefix;
  static Vector<int> gatherPrefixPrefix;
  static Vector<int> gatherAny0;
  static int globalIndent;
  static bool attributeUsed;

  ModuleType moduleType;
  Sort* boolSort;
  FreeSymbol* trueSymbol;
  FreeSymbol* falseSymbol;

  Vector<Symbol*> variableBase;  // for each sort
  Vector<Symbol*> eagerSortTests;
  Vector<Symbol*> lazySortTests;

  AliasMap variableAliases;

  Vector<SymbolInfo> symbolInfo;
  Vector<Polymorph> polymorphs;
  //
  //	Mixfix parsing stuff.
  //
  int parseSentence(const Vector<Token>& bubble,
		    int root,
		    int begin = 0,
		    int end = DEFAULT);
  int parseSentence2(const Vector<Token>& bubble,
		     int root,
		     int& firstBad,
		     int begin,
		     int nrTokens);
  Term* makeTerm(int node);
  void makeStatement(int node);
  void makeCondition(int node, Vector<ConditionFragment*>& condition);
  ConditionFragment* makeConditionFragment(int node);
  void makeCommand(int node, Vector<Term*>& terms);
  MixfixParser *parser;
  bool complexParser;
  int componentNonTerminalBase;
  int nextNonTerminal;

  typedef map<int, int> IntMap;
  IntMap iterSymbols;  // maps from name code to unique nonterminals
  IntMap leadTokens;  // maps from lead tokens of structured sort to unique nonterminals

  typedef map<int, Sort*> SortMap;
  SortMap sortNames;  // maps from name codes to sort pointers

  IntMap firstSymbols;  // names from name codes to index of first symbol with that name

  struct BubbleSpec
  {
    QuotedIdentifierSymbol* qidSymbol;
    Symbol* nilQidListSymbol;
    Symbol* qidListSymbol;
    Symbol* topSymbol;
    int lowerBound;
    int upperBound;
    int leftParenToken;
    int rightParenToken;
    int componentIndex;
    Vector<int> excludedTokens;
  };

  int findBubbleSpecIndex(Symbol* topSymbol) const;

  set<int> potentialLabels;
  set<int> bubbleComponents;
  Vector<BubbleSpec> bubbleSpecs;

  //
  //	These sets and maps are needed to decided when to
  //	disambiguate by sort in the pretty printer and to
  //	find builtin symbols from their kind index.
  //
  set<double> overloadedFloats;
  map<int, Symbol*> floatSymbols;
  set<string> overloadedStrings;
  map<int, Symbol*> stringSymbols;
  set<int> overloadedQuotedIdentifiers;
  map<int, Symbol*> quotedIdentifierSymbols;
  map<int, Symbol*> SMT_NumberSymbols;
  //
  //	We keep sets of integers and rationals whose decimal i/o
  //	representation has been used for user symbols of arity 0.
  //
  set<mpz_class> overloadedIntegers;
  set<pair<mpz_class, mpz_class> > overloadedRationals;
  //
  //	We also keep sets of kinds that have succ, minus and division
  //	symbols defined on them, or that have an non-explicit zero.
  //
  set<int> kindsWithSucc;  // also holds kinds with built-in constants that look like positive integers
  set<int> kindsWithMinus;  // also holds kinds with built-in constants that look like negative integers
  set<int> kindsWithDivision;  // also holds kinds with built-in that look like rationals
  set<int> kindsWithZero;   // holds kinds with a built-in constnat that look like zero

  set<pair<int, int> > overloadedVariables;

  //
  //	We keep track of symbols whose name looks like iterated notation
  //
  typedef multimap<mpz_class, Symbol*> NumberToSymbolMap;
  typedef map<int, NumberToSymbolMap> PseudoIteratedMap;
  PseudoIteratedMap pseudoIteratedMap;
  //
  //	We also keep track of iterated symbols.
  //
  typedef multimap<int, Symbol*> IteratedMap;
  IteratedMap iteratedMap;

  static bool hasSameDomain(const Vector<Sort*>& domainAndRange1,
			    bool assoc1,
			    const Vector<Sort*>& domainAndRange2,
			    bool assoc2);
  static bool domainAndRangeMatch(const Vector<Sort*>& domainAndRange1,
				  const Vector<Sort*>& domainAndRange2);

  bool ambiguous(int iflags);
  static bool rangeOfArgumentsKnown(int iflags, bool rangeKnown, bool rangeDisambiguated);
  void decideIteratedAmbiguity(bool rangeKnown,
			       Symbol* symbol,
			       const mpz_class& number,
			       bool& needToDisambiguate,
			       bool& argumentRangeKnown);
  int checkPseudoIterated(Symbol* symbol, const Vector<Sort*>& domainAndRange);
  void checkIterated(Symbol* symbol, const Vector<Sort*>& domainAndRange);
  //
  //	Member functions for DagNode* -> ostream& pretty printer.
  //
  const char* computeColor(ColoringInfo& coloringInfo, DagNode* dagNode);
  static void prefix(ostream& s, bool needDisambig, const char* color);
  static void suffix(ostream& s, DagNode* dagNode, bool needDisambig, const char* color);
  void handleFloat(ostream& s,
		   DagNode* dagNode,
		   bool rangeKnown,
		   const char* color);
  void handleString(ostream& s,
		    DagNode* dagNode,
		    bool rangeKnown,
		    const char* color);
  void handleQuotedIdentifier(ostream& s,
			      DagNode* dagNode,
			      bool rangeKnown,
			      const char* color);
  void handleVariable(ostream& s,
		      DagNode* dagNode,
		      bool rangeKnown,
		      const char* color);
  bool handleIter(ostream& s,
		  ColoringInfo& coloringInfo,
		  DagNode* dagNode,
		  SymbolInfo& si,
		  bool rangeKnown,
		  const char* color);
  bool handleMinus(ostream& s,
		   DagNode* dagNode,
		   bool rangeKnown,
		   const char* color);
  bool handleDivision(ostream& s,
		      DagNode* dagNode,
		      bool rangeKnown,
		      const char* color);
  void handleSMT_Number(ostream& s,
			DagNode* dagNode,
			bool rangeKnown,
			const char* color);
  void prettyPrint(ostream& s,
		   ColoringInfo& coloringInfo,
		   DagNode* dagNode,
		   int requiredPrec,
		   int leftCapture,
		   const ConnectedComponent* leftCaptureComponent,
		   int rightCapture,
		   const ConnectedComponent* rightCaptureComponent,
		   bool rangeKnown);
  //
  //	Member functions for Term* -> ostream& pretty printer.
  //
  static const char* computeColor(SymbolType st);
  static void suffix(ostream& s, Term* term, bool needDisambig, const char* color);
  bool handleIter(ostream& s,
		  Term* term,
		  SymbolInfo& si,
		  bool rangeKnown,
		  const char* color);
  bool handleMinus(ostream& s, Term* term,
		   bool rangeKnown,
		   const char* color);
  bool handleDivision(ostream& s,
		      Term* term,
		      bool rangeKnown,
		      const char* color);
  void handleFloat(ostream& s,
		   Term* term,
		   bool rangeKnown,
		   const char* color);
  void handleString(ostream& s,
		    Term* term,
		    bool rangeKnown,
		    const char* color);
  void handleQuotedIdentifier(ostream& s,
			      Term* term,
			      bool rangeKnown,
			      const char* color);
  void handleVariable(ostream& s,
		      Term* term,
		      bool rangeKnown,
		      const char* color);
  void handleSMT_Number(ostream& s,
			Term* term,
			bool rangeKnown,
			const char* color);
  void prettyPrint(ostream& s,
		   Term* term,
		   int requiredPrec,
		   int leftCapture,
		   const ConnectedComponent* leftCaptureComponent,
		   int rightCapture,
		   const ConnectedComponent* rightCaptureComponent,
		   bool rangeKnown);

  static bool prettyPrint(ostream& s, StrategyExpression* strategy, int requiredPrec);

  NatSet objectSymbols;
  NatSet messageSymbols;

  //
  //	Stuff for internal tuples.
  //
  Symbol* createInternalTupleSymbol(const Vector<ConnectedComponent*>& domain, ConnectedComponent* range);
  typedef list<int> IntList;
  typedef map<IntList, Symbol*> InternalTupleMap;
  InternalTupleMap tupleSymbols;

  SMT_Info smtInfo;
  SMT_Status smtStatus;

  friend ostream& operator<<(ostream& s, const Term* term);
  friend ostream& operator<<(ostream& s, DagNode* dagNode);
  friend ostream& operator<<(ostream& s, StrategyExpression* strategy);
};

inline SymbolType
MixfixModule::getSymbolType(Symbol* symbol) const
{
  Assert(symbol->getModule() == this, "symbol belongs to " << symbol->getModule() << " and not " << this);
  return symbolInfo[symbol->getIndexWithinModule()].symbolType;
}

inline Token
MixfixModule::getPolymorphName(int index) const
{
  return polymorphs[index].name;
}

inline SymbolType
MixfixModule::getPolymorphType(int index) const
{
  return polymorphs[index].symbolInfo.symbolType;
}

inline const Vector<Sort*>&
MixfixModule::getPolymorphDomainAndRange(int index) const
{
  return polymorphs[index].domainAndRange;
}

inline Term*
MixfixModule::getPolymorphIdentity(int index) const
{
  return polymorphs[index].identity;
}

inline const Vector<int>&
MixfixModule::getPolymorphStrategy(int index) const
{
  return polymorphs[index].strategy;
}

inline const NatSet&
MixfixModule::getPolymorphFrozen(int index) const
{
  return polymorphs[index].frozen;
}

inline int
MixfixModule::getPolymorphPrec(int index) const
{
  return polymorphs[index].symbolInfo.prec;
}

inline void
MixfixModule::getPolymorphGather(int index, Vector<int>& gather) const
{
  polymorphs[index].symbolInfo.revertGather(gather);
}

inline const Vector<int>&
MixfixModule::getPolymorphFormat(int index) const
{
  return polymorphs[index].symbolInfo.format;
}

inline int
MixfixModule::getPolymorphMetadata(int index) const
{
  return polymorphs[index].metadata;
}

inline int
MixfixModule::getPrec(Symbol* symbol) const
{
  return symbolInfo[symbol->getIndexWithinModule()].prec;
}

inline void
MixfixModule::getGather(Symbol* symbol, Vector<int>& gather) const
{
  symbolInfo[symbol->getIndexWithinModule()].revertGather(gather);
}

inline const Vector<int>&
MixfixModule::getFormat(Symbol* symbol) const
{
  return symbolInfo[symbol->getIndexWithinModule()].format;
}

inline MixfixModule::ModuleType
MixfixModule::getModuleType() const
{
  return moduleType;
}
inline const MixfixModule::AliasMap&
MixfixModule::getVariableAliases() const
{
  return variableAliases;
}

inline void
MixfixModule::insertPotentialLabels(const set<int>& l)
{
  potentialLabels = l;
}

inline int
MixfixModule::getNrPolymorphs() const
{
  return polymorphs.length();
}

inline Sort*
MixfixModule::hookSort(Sort* sort)
{
  return (sort->index() == Sort::KIND) ? sort->component()->sort(1) : sort;
}

inline MixfixModule::ModuleType
MixfixModule::join(ModuleType t1, ModuleType t2)
{
  return static_cast<ModuleType>(t1 | t2);
}

inline bool
MixfixModule::isTheory(ModuleType t)
{
  return t & THEORY;
}

inline bool
MixfixModule::isTheory() const
{
  return isTheory(getModuleType()); 
}

inline bool
MixfixModule::canImport(ModuleType t1, ModuleType t2)
{
  //
  //	System can import anything; funtional can only import functional.
  //	Theory can import anything; module can only import module.
  //
  return (t1 | t2) == t1;
}

inline bool
MixfixModule::canHaveAsParameter(ModuleType t1, ModuleType t2)
{
  //
  //	System can be parameterized by anything; functional can only be parameterized by functional.
  //	Only theories can be parameters.
  //
  return isTheory(t2) && (((t1 | t2) & SYSTEM) == (t1 & SYSTEM));
}

#endif

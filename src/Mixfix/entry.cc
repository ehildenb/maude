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
//	Code for entering stuff into a MixfixModule.
//

Sort*
MixfixModule::addSort(int name)
{
  Assert(findSort(name) == 0, "redeclaration of sort" << Token::name(name));
  Sort* s = new Sort(name);
  Module::insertSort(s);
  (void) sortNames.insert(SortMap::value_type(name, s));
  return s;
}

bool
MixfixModule::hasSameDomain(const Vector<Sort*>& domainAndRange1,
			    bool assoc1,
			    const Vector<Sort*>& domainAndRange2,
			    bool assoc2)
{
  int nrArgs1 = domainAndRange1.length() - 1;
  int nrArgs2 = domainAndRange2.length() - 1;
  if (nrArgs1 == nrArgs2)
    {
      for (int i = 0; i < nrArgs1; i++)
	{
	  if (domainAndRange1[i]->component() != domainAndRange2[i]->component())
	    return false;
	}
    }
  else if (assoc1)
    {
      Assert(nrArgs1 == 2 && nrArgs2 > 2,
	     "shouldn't have been called");
      for (int i = 0; i < nrArgs2; i++)
	{
	  if (domainAndRange1[0]->component() != domainAndRange2[i]->component())
	    return false;
	}
    }
  else
    {
      Assert(assoc2 && nrArgs2 == 2 && nrArgs1 > 2,
	     "shouldn't have been called");
      for (int i = 0; i < nrArgs1; i++)
	{
	  if (domainAndRange1[i]->component() != domainAndRange2[0]->component())
	    return false;
	}
    }
  return true;
}

int
MixfixModule::checkPseudoIterated(Symbol* symbol, const Vector<Sort*>& domainAndRange)
{
  //
  //	A symbol with a prefix name that looks like f^n can be confused with
  //	an iterated symbol f when parsing and pretty printing.
  //	We keep track of this extra overloading here.
  //
  int baseName;
  mpz_class number;
  Token::split(symbol->id(), baseName, number);
  pseudoIteratedMap[baseName].insert(NumberToSymbolMap::value_type(number, symbol));
  int overloadType = 0;
  //
  //	Now can we find an iterated symbol with baseName?
  //
  for (IteratedMap::const_iterator i = iteratedMap.lower_bound(baseName);
       i != iteratedMap.end() && i->first == baseName; ++i)
    {
      Symbol* iSymbol = i->second;
      const Vector<Sort*>& iDomainAndRange = iSymbol->getOpDeclarations()[0].getDomainAndRange();
      overloadType |= ADHOC_OVERLOADED;

      bool sameRange = (domainAndRange[1]->component() == iDomainAndRange[1]->component());
      if (sameRange)
	overloadType |= RANGE_OVERLOADED;

      bool sameDomain (domainAndRange[0]->component() == iDomainAndRange[0]->component());
      if (sameDomain)
	{
	  overloadType |= DOMAIN_OVERLOADED;
	  if (sameRange)
	    {
	      IssueWarning(*symbol << ": declaration for operator " << QUOTE(symbol) <<
			   " clashes with declaration for iterated operator " << QUOTE(iSymbol) <<
			   " on " << *iSymbol <<
			   " because of iterated notation.");
	    }
	  else
	    {
	      IssueWarning(*symbol << ": declaration for operator " << QUOTE(symbol) <<
			   " clashes with declaration for iterated operator " << QUOTE(iSymbol) <<
			   " on " << *iSymbol <<
			   ", which has a different range kind, because of iterated notation.");
	    }
	}
    }
  return overloadType;
}

void
MixfixModule::checkIterated(Symbol* symbol, const Vector<Sort*>& domainAndRange)
{
  int name = symbol->id();
  iteratedMap.insert(IteratedMap::value_type(name, symbol));
  //
  //	An iterated symbol f can be confused with a pseudo iterated symbol
  //	with name f^n when parsing and pretty printing.
  //	We keep track of this extra overloading here.
  //
  PseudoIteratedMap::const_iterator i = pseudoIteratedMap.find(name);
  if (i != pseudoIteratedMap.end())
    {
      //
      //	At least one symbol aliasing the iterated forms of symbol.
      //
      FOR_EACH_CONST(j, NumberToSymbolMap, i->second)
	{
	  int overloadType = ADHOC_OVERLOADED;
	  Symbol* pSymbol = j->second;
	  const Vector<Sort*>& pDomainAndRange = pSymbol->getOpDeclarations()[0].getDomainAndRange();

	  bool sameRange = (domainAndRange[1]->component() == pDomainAndRange[1]->component());
	  if (sameRange)
	    overloadType |= RANGE_OVERLOADED;

	  bool sameDomain (domainAndRange[0]->component() == pDomainAndRange[0]->component());
	  if (sameDomain)
	    {
	      overloadType |= DOMAIN_OVERLOADED;
	      if (sameRange)
		{
		  IssueWarning(*symbol << ": declaration for operator " << QUOTE(symbol) <<
			       " clashes with declaration for iterated operator " << QUOTE(pSymbol) <<
			       " on " << *pSymbol <<
			       " because of iterated notation.");
		}
	      else
		{
		  IssueWarning(*symbol << ": declaration for operator " << QUOTE(symbol) <<
			       " clashes with declaration for iterated " << QUOTE(pSymbol) <<
			       " on " << *pSymbol <<
			       ", which has a different range kind, because of iterated notation.");
		}
	    }
	  symbolInfo[pSymbol->getIndexWithinModule()].iflags |= overloadType;
	}
    }
}

Symbol*
MixfixModule::addOpDeclaration(Token prefixName,
			       const Vector<Sort*>& domainAndRange,
			       SymbolType symbolType,
			       const Vector<int>& strategy,
			       const NatSet& frozen,
			       int prec,
			       const Vector<int>& gather,
			       const Vector<int>& format,
			       int metadata,
			       bool& firstDecl)
{
  Assert(symbolType.getBasicType() != SymbolType::VARIABLE,
	 "we don't handle variables anymore");

  if (symbolType.dittoProblem())
    {
      IssueWarning(LineNumber(prefixName.lineNumber()) <<
		   ": declaration for " << QUOTE(prefixName) <<
		   " combines ditto with attributes other than ctor.");
    }

  int nrArgs = domainAndRange.length() - 1;
  int name = prefixName.code();
  int iflags = 0;
  IntMap::const_iterator first = firstSymbols.find(name);
  if (first != firstSymbols.end())
    {
      //
      //	Examine all existing symbols with same name.
      //
      for (int i = first->second; i != NONE; i = symbolInfo[i].next)
	{
	  Symbol* s = getSymbols()[i];
	  int iNrArgs = s->arity();
	  bool assoc = symbolType.hasFlag(SymbolType::ASSOC);
	  bool iAssoc = symbolInfo[i].symbolType.hasFlag(SymbolType::ASSOC);
	  if (nrArgs == iNrArgs ||
	      (assoc && nrArgs == 2 && iNrArgs > 2) ||
	      (iAssoc && nrArgs > 2))
	    {
	      //
	      //	We have the same name and could appear to have
	      //	the same number of arguments. Thus ambiguity is possible
	      //	and we have to rely on context or actual arguments for
	      //	disambiguation.
	      //
	      const Vector<Sort*>& iDomainAndRange =
		s->getOpDeclarations()[0].getDomainAndRange();
	      bool sameDomain =
		hasSameDomain(domainAndRange, assoc, iDomainAndRange, iAssoc);
	      bool sameRange = (domainAndRange[nrArgs]->component() ==
				iDomainAndRange[iNrArgs]->component());
	      int overloadType;
	      if (sameDomain)
		{
		  if (sameRange)
		    {
		      if (nrArgs == iNrArgs)
			{
			  firstDecl = false;
			  if (!(symbolType.hasFlag(SymbolType::DITTO)) &&
			      !(getSymbolType(s).compatible(symbolType)))
			    {
			      IssueWarning(LineNumber(prefixName.lineNumber()) <<
					   ": declaration for " << QUOTE(s) <<
					   " has different attributes from declaration on " <<
					   *s << '.');
			      DebugAdvisory("orginal symbol type: " << getSymbolType(s) <<
					    "\tnew symbol type: " << symbolType <<
					    "\tflag difference: " <<
					    (getSymbolType(s).getFlags() ^ symbolType.getFlags()));
			      markAsBad();
			    }
			  if (metadata != NONE)
			    insertMetadata(s, s->getOpDeclarations().length(), metadata);
			  s->addOpDeclaration(domainAndRange,
					      symbolType.hasFlag(SymbolType::CTOR));
			  return s;
			}
		      else
			{
			  IssueWarning(LineNumber(prefixName.lineNumber()) <<
				       ": declaration for " << QUOTE(s) <<
				       " clashes with declaration on " << *s <<
				       " because of associativity.");
			  overloadType =
			    ADHOC_OVERLOADED | DOMAIN_OVERLOADED | RANGE_OVERLOADED;
			}
		    }
		  else
		    {
		      if (nrArgs > 0)
			{
			  if (nrArgs == iNrArgs)
			    {
			      IssueWarning(LineNumber(prefixName.lineNumber()) <<
					   ": declaration for " << QUOTE(s) <<
					   " has the same domain kinds as the declaration on " <<
					   *s << " but a different range kind.");
			    }
			  else
			    {
			      IssueWarning(LineNumber(prefixName.lineNumber()) <<
					   ": declaration for " << QUOTE(s) <<
					   " clashes with declaration on " << *s <<
					   ", which has a different range kind, because of associativity.");
			    }
			}
		      overloadType = ADHOC_OVERLOADED | DOMAIN_OVERLOADED;
		    }
		}
	      else
		{
		  overloadType = sameRange ? (ADHOC_OVERLOADED | RANGE_OVERLOADED) :
		    ADHOC_OVERLOADED;
		}
	      iflags |= overloadType;
	      symbolInfo[s->getIndexWithinModule()].iflags |= overloadType;
	    }
	}
    }
  //
  //	Need to create a new symbol.
  //
  if (symbolType.hasFlag(SymbolType::DITTO))
    {
      IssueWarning(LineNumber(prefixName.lineNumber()) <<
		   ": declaration for " << QUOTE(prefixName) <<
		   " uses ditto without a previous declaration to refer to.");
      symbolType.clearFlags(SymbolType::DITTO);
    }	   

  Symbol* symbol = newFancySymbol(prefixName, domainAndRange, symbolType, strategy);
  if (symbol == 0)
    {
      symbol = FreeSymbol::newFreeSymbol(name, nrArgs, strategy,
					 symbolType.hasFlag(SymbolType::MEMO));
      symbolType.clearFlags(SymbolType::AXIOMS);
    }
  if (symbolType.hasFlag(SymbolType::FROZEN))
    symbol->setFrozen(frozen);
  symbol->setLineNumber(prefixName.lineNumber());

  //
  //	Fill in symbolInfo entry with syntactic info.
  //
  if (nrArgs == 0)
    {
      int sp = prefixName.specialProperty();
      if (sp == Token::CONTAINS_COLON)
	{
	  pair<int, int> p;
	  Token::split(prefixName.code(), p.first, p.second);
	  if (findSort(p.second))  // not perfect since we don't handle kinds
	    {
	      WarningCheck(domainAndRange[0]->id() != p.second,
			   LineNumber(prefixName.lineNumber()) <<
			   ": operator " << QUOTE(prefixName) <<
			   " is indistinguishable from a variable.");
	      overloadedVariables.insert(p);
	      iflags |= PSEUDO_VARIABLE;
	    }
	}
      else if (sp == Token::FLOAT)
	{
	  //
	  //	Maybe we should check that it's actually in canonical form before we
	  //	insert it. Maybe we could store strings rather than doubles.
	  //	Either way we always need to set the PSEUDO_FLOAT flag.
	  //
	  overloadedFloats.insert(Token::codeToDouble(prefixName.code()));
	  iflags |= PSEUDO_FLOAT;
	} 
      else if (sp == Token::STRING)
	{
	  //
	  //	Maybe we should check that it's actually in canonical form before we
	  //	insert it. Either way we always need to set the PSEUDO_STRING flag.
	  //
	  overloadedStrings.insert(prefixName.name());
	  iflags |= PSEUDO_STRING;
	}
      else if (sp == Token::QUOTED_IDENTIFIER)
	{
	  overloadedQuotedIdentifiers.insert(Token::encode(prefixName.name() + 1));
	  iflags |= PSEUDO_QUOTED_IDENTIFIER;
	}
      else if (sp == Token::SMALL_NAT)
	{
	  mpz_class nat(prefixName.name(), 10);
	  overloadedIntegers.insert(nat);
	  iflags |= PSEUDO_NAT;
	}
      else if (sp == Token::ZERO)
	{
	  //
	  //	This is not needed for built-in Nats because 0 appears as a regular constant
	  //	and the standard operator pretty printing will handle the disambiguation.
	  //	It is needed for SMT Integers, because here 0 is a built in non-algebraic
	  //	constant, and the SMT pretty printer needs to know that another 0 exists.
	  //
	  //
	  overloadedIntegers.insert(0);
	  //
	  //	Also when we print this constant we need to force a check of kindsWithZero
	  //	when we decide whether disambiguation is needed. In the case of build-in
	  //	Nats, the Nat constant zero will already set the abiguity flag, but
	  //	in the case of SMT Integers, the constant zero is implicit.
	  //
	  iflags |= PSEUDO_ZERO;
	}
      else if (sp == Token::SMALL_NEG)
	{
	  mpz_class neg(prefixName.name(), 10);
	  overloadedIntegers.insert(neg);
	  iflags |= PSEUDO_NEG;
	}
      else if (sp == Token::RATIONAL)
	{
	  pair<mpz_class, mpz_class> rat;
	  prefixName.getRational(rat.first, rat.second);
	  overloadedRationals.insert(rat);
	  iflags |= PSEUDO_RAT;
	}
    }
  //
  //	Need to worry about unary operators whose prefix name looks like f^2; i.e.
  //	iterated notation.
  //
  if (nrArgs == 1 && prefixName.specialProperty() == Token::ITER_SYMBOL)
    iflags |= checkPseudoIterated(symbol, domainAndRange);
  //
  //	We also need the check iterated symbols against any symbols with prefix
  //	names like f^n.
  if (symbolType.hasFlag(SymbolType::ITER))
    checkIterated(symbol, domainAndRange);

  int nrSymbols = symbolInfo.length();
  symbolInfo.expandBy(1);
  SymbolInfo& si = symbolInfo[nrSymbols];
  int nrUnderscores = Token::extractMixfix(name, si.mixfixSyntax);
  if (si.mixfixSyntax.length() == 0)
    {
      si.prec = 0;
      WarningCheck(!(symbolType.hasFlag(SymbolType::PREC)),
		   *symbol <<
		   ": prec attribute without mixfix syntax for operator " <<
		   QUOTE(symbol) << '.');
      WarningCheck(!(symbolType.hasFlag(SymbolType::GATHER)),
		   *symbol <<
		   ": gather attribute without mixfix syntax for operator " <<
		   QUOTE(symbol) << '.');
      symbolType.clearFlags(SymbolType::PREC | SymbolType::GATHER);
      if (symbolType.hasFlag(SymbolType::FORMAT))
	{
	  if (format.length() == 2)
	    si.format = format;  // deep copy
	  else
	    {
	      IssueWarning(LineNumber(prefixName.lineNumber()) <<
			   ": bad format length of " <<
			   format.length() <<
			   ", should be 2.");
	      symbolType.clearFlags(SymbolType::FORMAT);
	    }
	}
    }
  else
    {
      if (si.mixfixSyntax.length() == 1 && nrUnderscores == 1)
	{
	  IssueWarning(*symbol <<
		       ": empty syntax not allowed for single argument operators.");
	  si.mixfixSyntax.contractTo(0);
	  si.prec = 0;
	  symbolType.clearFlags(SymbolType::PREC | SymbolType::GATHER | SymbolType::FORMAT);
	}
      if (nrArgs != nrUnderscores)
	{
	  IssueWarning(*symbol <<
		       ": number of underscores (" << nrUnderscores << ") does not match " <<
		       "number of arguments (" << nrArgs << ") for operator " <<
		       QUOTE(symbol) << '.');
	  si.mixfixSyntax.contractTo(0);
	  si.prec = 0;
	  symbolType.clearFlags(SymbolType::PREC | SymbolType::GATHER | SymbolType::FORMAT);
	}
      else
	{
	  si.prec = prec;
	  si.gather = gather;  // deep copy
	  if (symbolType.hasFlag(SymbolType::FORMAT))
	    {
	      if (si.mixfixSyntax.length() + 1 != format.length())
		{
		  IssueWarning(LineNumber(prefixName.lineNumber()) <<
			       ": bad format length of " <<
			       format.length() <<
			       ", should be " <<
			       si.mixfixSyntax.length() + 1 << '.');
		  symbolType.clearFlags(SymbolType::FORMAT);
		}
	      else
		si.format = format;  // deep copy
	    }
	}
    }
  si.symbolType = symbolType;
  si.symbolType.clearFlags(SymbolType::CTOR);  // don't store ctor flag in per-symbol struct
  si.iflags = iflags;
  si.next = (first == firstSymbols.end()) ? NONE : first->second;
  firstSymbols[name] = nrSymbols;

  Module::insertSymbol(symbol);
  if (metadata != NONE)
    insertMetadata(symbol, symbol->getOpDeclarations().length(), metadata);
  symbol->addOpDeclaration(domainAndRange, symbolType.hasFlag(SymbolType::CTOR));
  firstDecl = true;
  switch (symbolType.getBasicType())
    {
    case SymbolType::SYSTEM_TRUE:
      {
	if (trueSymbol == 0)
	  {
	    trueSymbol = safeCast(FreeSymbol*, symbol);  // HACK
	    boolSort = domainAndRange[0];
	  }
	else
	  {
	    IssueWarning(*symbol << ": declaration for SystemTrue symbol " <<
			 QUOTE(symbol) << " conflicts with SystemTrue symbol " <<
			 QUOTE(trueSymbol) << " on " << *trueSymbol << '.');
	  }
	break;
      }
    case SymbolType::SYSTEM_FALSE:
      {
	if (falseSymbol == 0)
	  {
	    falseSymbol = safeCast(FreeSymbol*, symbol);  // HACK
	    boolSort = domainAndRange[0];
	  }
	else
	  {
	    IssueWarning(*symbol << ": declaration for SystemFalse symbol " <<
			 QUOTE(symbol) << " conflicts with SystemFalse symbol " <<
			 QUOTE(falseSymbol) << " on " << *falseSymbol << '.');
	  }
	break;
      }

  //  if (flags & BUBBLE)
  //  bubbleComponents.insert(domainAndRange[nrArgs]->component()->indexWithinModule());

    case SymbolType::FLOAT:
      {
	ConnectedComponent* c = domainAndRange[nrArgs]->component();
	floatSymbols[c->getIndexWithinModule()] = symbol;
	break;
      }
    case SymbolType::STRING:
      {
	ConnectedComponent* c = domainAndRange[nrArgs]->component();
	stringSymbols[c->getIndexWithinModule()] = symbol;
	break;
      }
    case SymbolType::QUOTED_IDENTIFIER:
      {
	ConnectedComponent* c = domainAndRange[nrArgs]->component();
	quotedIdentifierSymbols[c->getIndexWithinModule()] = symbol;
	break;
      }
    case SymbolType::SMT_NUMBER_SYMBOL:
      {
	ConnectedComponent* c = domainAndRange[nrArgs]->component();
	SMT_NumberSymbols[c->getIndexWithinModule()] = symbol;
      }
    }
  if (symbolType.hasFlag(SymbolType::CONFIG))
    {
      if (dynamic_cast<ConfigSymbol*>(symbol) == 0)
	{
	  IssueWarning(LineNumber(prefixName.lineNumber()) <<
		       ": operator " <<
		       BEGIN_QUOTE << symbol << END_QUOTE <<
		       " cannot take config attribute.");
	}
    }
  if (symbolType.hasFlag(SymbolType::OBJECT))
    {
      WarningCheck(!(symbolType.hasFlag(SymbolType::MESSAGE)),
		   LineNumber(prefixName.lineNumber()) <<
		   ": operator " <<
		   BEGIN_QUOTE << symbol << END_QUOTE <<
		   " is not allowed to have both object and msg attributes.");

      if (nrArgs == 0)
	{
	  IssueWarning(LineNumber(prefixName.lineNumber()) <<
		       ": object attribute for operator " <<
		       BEGIN_QUOTE << symbol << END_QUOTE <<
		       " which lacks arguments.");
	}
      else
	objectSymbols.insert(symbol->getIndexWithinModule());
    }
  else
    {
      if (symbolType.hasFlag(SymbolType::MESSAGE))
	{
	  if (nrArgs == 0)
	    {
	      IssueWarning(LineNumber(prefixName.lineNumber()) <<
			   ": msg attribute for operator " <<
			   BEGIN_QUOTE << symbol << END_QUOTE <<
			   " which lacks arguments.");
	    }
	  else
	    messageSymbols.insert(symbol->getIndexWithinModule());
	}
    }
  return symbol;
}

void
MixfixModule::addVariableAlias(Token name, Sort* sort)
{
  pair<AliasMap::iterator, bool> r =
    variableAliases.insert(AliasMap::value_type(name.code(), sort));
  WarningCheck(r.second, LineNumber(name.lineNumber()) <<
	       ": redeclaration of variable alias " << QUOTE(name) << '.');
}

Symbol*
MixfixModule::newFancySymbol(Token prefixName,
			     const Vector<Sort*>& domainAndRange,
			     SymbolType symbolType,
			     const Vector<int>& strategy)
{
  int name = prefixName.code();
  int nrArgs = domainAndRange.length() - 1;
  int kindIndex = domainAndRange[nrArgs]->component()->getIndexWithinModule();
  switch (symbolType.getBasicType())
    {
    case SymbolType::BRANCH_SYMBOL:
      return new BranchSymbol(name, nrArgs);
    case SymbolType::EQUALITY_SYMBOL:
      return new EqualitySymbol(name, strategy);
    case SymbolType::FLOAT:
      return new FloatSymbol(name);
    case SymbolType::STRING:
      return new StringSymbol(name);
    case SymbolType::QUOTED_IDENTIFIER:
      return new QuotedIdentifierSymbol(name);
    case SymbolType::LOOP_SYMBOL:
      return new LoopSymbol(name);
    case SymbolType::FLOAT_OP:
      return new FloatOpSymbol(name, nrArgs);
    case SymbolType::STRING_OP:
      return new StringOpSymbol(name, nrArgs);
    case SymbolType::QUOTED_IDENTIFIER_OP:
      return new QuotedIdentifierOpSymbol(name, nrArgs);
    case SymbolType::META_LEVEL_OP_SYMBOL:
      return new MetaLevelOpSymbol(name, nrArgs, strategy);
    case SymbolType::MODEL_CHECKER_SYMBOL:
      return new ModelCheckerSymbol(name);
    case SymbolType::SAT_SOLVER_SYMBOL:
      return new SatSolverSymbol(name);
    case SymbolType::SUCC_SYMBOL:
      {
	pair<set<int>::iterator, bool> p = kindsWithSucc.insert(kindIndex);
	if (!(p.second))
	  {
	    IssueWarning(LineNumber(prefixName.lineNumber()) <<
	      ": multiple SuccSymbols in same kind will cause pretty printing problems.");
	  }
	return new SuccSymbol(name);
      }
    case SymbolType::MINUS_SYMBOL:
      {
	pair<set<int>::iterator, bool> p = kindsWithMinus.insert(kindIndex);
	if (!(p.second))
	  {
	    IssueWarning(LineNumber(prefixName.lineNumber()) <<
	      ": multiple MinusSymbols in same kind will cause pretty printing problems.");
	  }
	return new MinusSymbol(name);
      }
    case SymbolType::NUMBER_OP_SYMBOL:
      return new NumberOpSymbol(name, nrArgs);
    case SymbolType::ACU_NUMBER_OP_SYMBOL:
      return new ACU_NumberOpSymbol(name);
    case SymbolType::DIVISION_SYMBOL:
      {
	pair<set<int>::iterator, bool> p = kindsWithDivision.insert(kindIndex);
	if (!(p.second))
	  {
	    IssueWarning(LineNumber(prefixName.lineNumber()) <<
	    ": multiple DivisionSymbols in same kind will cause pretty printing problems.");
	  }
	return new DivisionSymbol(name);
      }
    case SymbolType::RANDOM_OP_SYMBOL:
      return new RandomOpSymbol(name);
    case SymbolType::MATRIX_OP_SYMBOL:
      return new MatrixOpSymbol(name, nrArgs);
    case SymbolType::COUNTER_SYMBOL:
      return new CounterSymbol(name);
    case SymbolType::SOCKET_MANAGER_SYMBOL:
      return new SocketManagerSymbol(name);
    case SymbolType::INTERPRETER_MANAGER_SYMBOL:
      return new InterpreterManagerSymbol(name);
    case SymbolType::SMT_SYMBOL:
      return new SMT_Symbol(name, nrArgs);
    case SymbolType::SMT_NUMBER_SYMBOL:
      return new SMT_NumberSymbol(name);
    case SymbolType::FILE_MANAGER_SYMBOL:
      return new FileManagerSymbol(name);
    case SymbolType::STREAM_MANAGER_SYMBOL:
      return new StreamManagerSymbol(name);
    case SymbolType::TERMINATION_CHECKER_SYMBOL:
       return new TerminationCheckerSymbol(name,nrArgs);
    }

  int lineNr = prefixName.lineNumber();
  if (symbolType.hasFlag(SymbolType::ASSOC))
    {
      if (nrArgs == 2)
	{
	  bool leftOK = domainAndRange[0]->component() == domainAndRange[2]->component();
	  if (leftOK && domainAndRange[1]->component() == domainAndRange[2]->component())
	    {
	      if (symbolType.hasFlag(SymbolType::COMM))
		{
		  if (symbolType.hasFlag(SymbolType::CONFIG))
		    return new ConfigSymbol(name, strategy, symbolType.hasFlag(SymbolType::MEMO));
		  else
		    return new ACU_Symbol(name, strategy, symbolType.hasFlag(SymbolType::MEMO));
		}
	      else
		{
		  return new AU_Symbol(name,
				       strategy,
				       symbolType.hasFlag(SymbolType::MEMO),
				       symbolType.hasFlag(SymbolType::LEFT_ID),
				       symbolType.hasFlag(SymbolType::RIGHT_ID));
		}
	    }
	  else
	    {
	      IssueWarning(LineNumber(lineNr) <<
			   ": assoc operator " << QUOTE(prefixName) <<
			   " has a domain sort " <<
			   QUOTE((leftOK ? domainAndRange[1] : domainAndRange[0])) <<
			   " in a different connected component from its range sort " <<
			   QUOTE(domainAndRange[2]) << '.');
	    }     
	}
      else
	{
	  IssueWarning(LineNumber(lineNr) <<
		       ": assoc operator " << QUOTE(prefixName) <<
		       " has " << nrArgs << " rather than 2 domain sorts.");

	}
    }
  else if (symbolType.hasFlag(SymbolType::ITER))
    {
      if (nrArgs == 1)
	return new S_Symbol(name, strategy, symbolType.hasFlag(SymbolType::MEMO));
      else
	{
	  IssueWarning(LineNumber(lineNr) <<
		       ": iter operator " << QUOTE(prefixName) <<
		       " has " << QUOTE(nrArgs) << " domain sorts rather than 1.");
	}
    }
  else if (symbolType.hasFlag(SymbolType::AXIOMS))
    {
      if (nrArgs == 2)
	{
	  int axioms = 0;
	  if (symbolType.hasFlag(SymbolType::COMM))
	    {
	      if (domainAndRange[0]->component() == domainAndRange[1]->component())
		axioms |= CUI_Symbol::COMM;
	      else
		{
		  IssueWarning(LineNumber(lineNr) <<
			       ": comm operator " << QUOTE(prefixName) <<
			       " has a domain sorts " << QUOTE(domainAndRange[0]) <<
			       " and " << QUOTE(domainAndRange[1]) << 
			       " in different connected components.");
		  return 0;
		}
	    }
	  if (symbolType.hasFlag(SymbolType::IDEM))
	    {
	      bool leftOK = domainAndRange[0]->component() == domainAndRange[2]->component();
	      if (leftOK && domainAndRange[1]->component() == domainAndRange[2]->component())
		axioms |= CUI_Symbol::IDEM;
	      else
		{
		  IssueWarning(LineNumber(lineNr) <<
			       ": idem operator " << QUOTE(prefixName) <<
			       " has a domain sort " <<
			       QUOTE((leftOK ? domainAndRange[1] : domainAndRange[0])) <<
			       " in a different connected component from its range sort " <<
			       QUOTE(domainAndRange[2]) << '.');
		  return 0;
		}  
	    }
	  if (symbolType.hasFlag(SymbolType::LEFT_ID) ||
	      (symbolType.hasAllFlags(SymbolType::RIGHT_ID | SymbolType::COMM)))
	    {
	      if (domainAndRange[1]->component() == domainAndRange[2]->component())
		axioms |= CUI_Symbol::LEFT_ID;
	      else
		{
		  IssueWarning(LineNumber(lineNr) <<
			       ": id operator " << QUOTE(prefixName) <<
			       " has a domain sort " << QUOTE(domainAndRange[1]) <<
			       " in a different connected component from its range sort " <<
			       QUOTE(domainAndRange[2]) << '.');
		  return 0;
		}
	    }
	  if (symbolType.hasFlag(SymbolType::RIGHT_ID) ||
	      (symbolType.hasAllFlags(SymbolType::LEFT_ID | SymbolType::COMM)))
	    {
	      if (domainAndRange[0]->component() == domainAndRange[2]->component())
		axioms |= CUI_Symbol::RIGHT_ID;
	      else
		{
		  IssueWarning(LineNumber(lineNr) <<
			       ": id operator " << QUOTE(prefixName) <<
			       " has a domain sort " << QUOTE(domainAndRange[0]) <<
			       " in a different connected component from its range sort " <<
			       domainAndRange[2] << '.');
		  return 0;
		}
	    }
	  return (symbolType.getBasicType() == SymbolType::CUI_NUMBER_OP_SYMBOL) ?
	    new CUI_NumberOpSymbol(name, static_cast<CUI_Symbol::Axioms>(axioms)) :
	    new CUI_Symbol(name,
			   strategy,
			   symbolType.hasFlag(SymbolType::MEMO),
			   static_cast<CUI_Symbol::Axioms>(axioms));
	}
      else
	IssueWarning(LineNumber(lineNr) << ": " <<
		     (symbolType.hasFlag(SymbolType::COMM) ? "comm" :
		     (symbolType.hasFlag(SymbolType::IDEM) ? "idem" : "id")) <<
		     " operator " << QUOTE(prefixName) <<
		     " has " << nrArgs << " rather than 2 domain sorts.");
    }
  return 0;
}

int
MixfixModule::addPolymorph(Token prefixName,
			   const Vector<Sort*>& domainAndRange,
			   SymbolType symbolType,
			   const Vector<int>& strategy,
			   const NatSet& frozen,
			   int prec,
			   const Vector<int>& gather,
			   const Vector<int>& format,
			   int metadata)
{
  int index = findPolymorphIndex(prefixName.code(), domainAndRange);
  if (index != NONE)
    {
      IssueWarning(LineNumber(prefixName.lineNumber()) <<
		   ": declaration for polymorphic operator " << QUOTE(prefixName) <<
		   " duplicates declaration on " <<
		   LineNumber(polymorphs[index].name.lineNumber()) << '.');
      return index;
    }

  int nrPolymorphs = polymorphs.length();
  polymorphs.expandBy(1);
  Polymorph& p = polymorphs[nrPolymorphs];
  p.name = prefixName;
  p.domainAndRange = domainAndRange;  // deep copy
  p.strategy = strategy;  // deep copy
  p.frozen = frozen;  // deep copy
  p.identity = 0;
  p.metadata = metadata;
  int nrUnderscores = Token::extractMixfix(prefixName.code(), p.symbolInfo.mixfixSyntax);
  if (p.symbolInfo.mixfixSyntax.length() == 0)
    {
      p.symbolInfo.prec = 0;
      WarningCheck(!(symbolType.hasFlag(SymbolType::PREC)),
		   LineNumber(prefixName.lineNumber()) <<
		   ": prec attribute without mixfix syntax for operator " <<
		   QUOTE(prefixName) << '.');
      WarningCheck(!(symbolType.hasFlag(SymbolType::GATHER)),
		   LineNumber(prefixName.lineNumber()) <<
		   ": gather attribute without mixfix syntax for operator " <<
		   QUOTE(prefixName) << '.');
      symbolType.clearFlags(SymbolType::PREC | SymbolType::GATHER);
      if (symbolType.hasFlag(SymbolType::FORMAT))
	{
	  if (format.length() == 2)
	    p.symbolInfo.format = format;  // deep copy
	  else
	    {
	      IssueWarning(LineNumber(prefixName.lineNumber()) <<
			   ": bad format length of " <<
			   format.length() <<
			   ", should be 2.");
	      symbolType.clearFlags(SymbolType::FORMAT);
	    }
	}
    }
  else
    {
      if (domainAndRange.length() - 1 != nrUnderscores)
	{
	  IssueWarning(LineNumber(prefixName.lineNumber()) <<
		       ": number of underscores does not match (" << nrUnderscores <<
		       ") number of arguments (" << domainAndRange.length() - 1 <<
		       ") for operator " <<
		       QUOTE(prefixName) << '.');
	  p.symbolInfo.mixfixSyntax.contractTo(0);
	  p.symbolInfo.prec = 0;
	  symbolType.clearFlags(SymbolType::PREC | SymbolType::GATHER | SymbolType::FORMAT);
	}
      else
	{
	  p.symbolInfo.prec = prec;
	  p.symbolInfo.gather = gather;  // deep copy
	  if (symbolType.hasFlag(SymbolType::FORMAT))
	    {
	      if (p.symbolInfo.mixfixSyntax.length() + 1 != format.length())
		{
		  IssueWarning(LineNumber(prefixName.lineNumber()) <<
			       ": bad format length of " <<
			       format.length() <<
			       ", should be " <<
			       p.symbolInfo.mixfixSyntax.length() + 1 << '.');
		  symbolType.clearFlags(SymbolType::FORMAT);
		}
	      else
		p.symbolInfo.format = format;  // deep copy
	    }
	}
    }
  p.symbolInfo.symbolType = symbolType;
  p.symbolInfo.next = NONE;
  {
    p.symbolInfo.iflags = ADHOC_OVERLOADED | DOMAIN_OVERLOADED;
    int nrArgs = domainAndRange.length() - 1;
    for (int i = 0; i < nrArgs; i++)
      {
	if (domainAndRange[i] == 0)
	  {
	    p.symbolInfo.iflags &= ~DOMAIN_OVERLOADED;
	    break;
	  }
      }
    if (domainAndRange[nrArgs] != 0)
      p.symbolInfo.iflags |= RANGE_OVERLOADED;
  }
  return nrPolymorphs;
}

int
MixfixModule::addBubbleSpec(Symbol* topSymbol,
			    int lowerBound,
			    int upperBound,
			    int leftParenToken,
			    int rightParenToken,
			    const Vector<int>& excludedTokens)
{
  int nrBubbleSpecs = bubbleSpecs.length();
  bubbleSpecs.expandBy(1);
  BubbleSpec& b = bubbleSpecs[nrBubbleSpecs];
  b.topSymbol = topSymbol;
  b.lowerBound = lowerBound;
  b.upperBound = upperBound;
  b.leftParenToken = leftParenToken;
  b.rightParenToken = rightParenToken;
  b.excludedTokens = excludedTokens;
  const Vector<Sort*>& domainAndRange =
    topSymbol->getOpDeclarations()[0].getDomainAndRange();
  b.componentIndex =
    domainAndRange[domainAndRange.length() - 1]->component()->getIndexWithinModule();
  bubbleComponents.insert(b.componentIndex);
  return nrBubbleSpecs;
}

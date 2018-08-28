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
//      Implementation for class View.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "core.hh"
#include "interface.hh"
#include "variable.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "argumentIterator.hh"

//      variable class definitions
#include "variableTerm.hh"

//	front end class definitions
#include "token.hh"
#include "moduleExpression.hh"
#include "interpreter.hh"
#include "view.hh"

View::View(Token viewName, Interpreter* owner)
  : NamedEntity(viewName.code()),
    LineNumber(viewName.lineNumber()),
    owner(owner)
{
  fromTheory = 0;
  toModule = 0;
  status = INITIAL;
}

View::~View()
{
  clearOpTermMap();
  if (fromTheory != 0)
    fromTheory->removeUser(this);
  if (toModule != 0)
    toModule->removeUser(this);
  fromExpr->deepSelfDestruct();
  toExpr->deepSelfDestruct();
  informUsers();
}

void
View::clearOpTermMap()
{
  FOR_EACH_CONST(i, OpTermMap, opTermMap)
    {
      i->second.first->deepSelfDestruct();
      i->second.second->deepSelfDestruct();
    }
  opTermMap.clear();
}

void
View::regretToInform(Entity* doomedEntity)
{
  if (doomedEntity == fromTheory)
    fromTheory = 0;
  else if(doomedEntity == toModule)
    toModule = 0;
  else
    CantHappen("unexpected regretToInform()");
  //
  //	Something we depended on changed so self destruct all calculated stuff.
  //
  clearOpTermMap();
  if (fromTheory != 0)
    {
      fromTheory->removeUser(this);
      fromTheory = 0;
    }
  if (toModule != 0)
    {
      toModule->removeUser(this);
      toModule = 0;
    }
  status = STALE;
}

ConnectedComponent*
View::mapComponent(const ConnectedComponent* component, ImportModule* module) const
{
  Sort* sort = module->findSort(renameSort(component->sort(1)->id()));
  Assert(sort != 0, "translation for sort failed");
  return sort->component();
}

bool
View::checkSorts()
{
  //
  //	We check that for each pair of sorts (A, B) lying in the same kind in
  //	fromTheory, there exists under our mapping sorts (A', B') that are in 
  //	the same kind in toModule, and further more if A <= B then A' <= B'.
  //
  const Vector<ConnectedComponent*> kinds = fromTheory->getConnectedComponents();
  //
  //	We examine the sort structure of the from theory one connected component at a time.
  //
  FOR_EACH_CONST(i, Vector<ConnectedComponent*>, kinds)
    {
      ConnectedComponent* kind = (*i);
      //
      //	For each kind, we examine its sorts.
      //
      int nrSorts = kind->nrSorts();
      for (int j = 1; j < nrSorts; ++j)
	{
	  Sort* jSort = kind->sort(j);
	  int jId = jSort->id();
	  int jTrans = renameSort(jSort->id());
	  if (fromTheory->moduleDeclared(jSort) && jTrans != jId)
	    {
	      IssueAdvisory(*this << ": sort mapping not applied to sort " << QUOTE(jSort) <<
			    " as it was declared in a module rather than in a theory.");
	      jTrans = jId;
	    }
	  Sort* jTransSort = toModule->findSort(jTrans);
	  if (jTransSort == 0)
	    {
	      IssueWarning(*this << ": failed to find sort " << QUOTE(Token::sortName(jTrans)) <<
			   " in " << QUOTE(toModule) << " to represent sort " << QUOTE(jSort) <<
			   " from " << QUOTE(fromTheory) << '.');
	      return false;
	    }
	  ConnectedComponent* transKind = jTransSort->component();
	  //
	  //	Now we compare the sort to earlier sorts which have already been
	  //	check for a valid translation.
	  //
	  for (int k = 1; k < j; ++k)
	    {
	      Sort* kSort = kind->sort(k);
	      int kId = kSort->id();
	      if (!(fromTheory->moduleDeclared(kSort)))
		kId = renameSort(kId);
	      Sort* kTransSort = toModule->findSort(kId);
	      if (kTransSort->component() != transKind)
		{
		  IssueWarning(*this << ": sorts " << QUOTE(jSort) << " and " << QUOTE(kSort) << " from " <<
			       QUOTE(fromTheory) << " are in the same kind but " <<
			       QUOTE(jTransSort) << " and " << QUOTE(kTransSort) << " from " <<
			       QUOTE(toModule) << " are in different kinds.");
		  return false;
		}
	      if (leq(jSort, kSort) && !(leq(jTransSort, kTransSort)))
		{
		  //
		  //	<= is not preserved but this case doesn't lead to serious
		  //	inconsistancies later on so we keep going.
		  //
		  IssueWarning(*this << ": " << QUOTE(jSort) << " is a subsort of " << QUOTE(kSort) <<
			       " in " << QUOTE(fromTheory) << " but " << QUOTE(jTransSort) <<
			       " is not a subsort of " << QUOTE(kTransSort) << " in " <<
			       QUOTE(toModule) << '.');
		}			      
	    }
	}
    }
  return true;
}


bool
View::typeMatch(const ConnectedComponent* c1, const ConnectedComponent* c2)
{
  //
  //	c1 and c2 are in different theories but they typeMatch if they have sorts with the same name.
  //	Actually we only need check for one regular sort from one component, if a similarly named
  //	sort exists in the other.
  //
  int nrSorts = c1->nrSorts();
  int sortName = c2->sort(1)->id();
  for (int i = 0; i < nrSorts; ++i)
    {
      if (c1->sort(i)->id() == sortName)
	return true;
    }
  return false;
}

bool
View::typeMatch(const Symbol* s1, const Symbol* s2)
{
  //
  //	s1 and s2 are in different theories but they typeMatch if their respective components all typeMatch.
  //
  int nrArgs = s1->arity();
  if (s2->arity() != nrArgs)
    return false;
  for (int i = 0; i < nrArgs; ++i)
    {
      if (!typeMatch(s1->domainComponent(i), s2->domainComponent(i)))
	return false;
    }
  return typeMatch(s1->rangeComponent(), s2->rangeComponent());
}

Term*
View::getOpMapTerm(Symbol* symbol) const
{
  int id = symbol->id();
  const OpTermMap::const_iterator e = opTermMap.end();
  for (OpTermMap::const_iterator i = opTermMap.find(id); i != e && i->first == id; ++i)
    {
      if (typeMatch(i->second.first->symbol(), symbol))
	return i->second.second;
    }
  return 0;
}

bool
View::checkOps()
{
  //
  //	Now we check that for each operator
  //	  f : D1 ... Dn -> R
  //	in fromTheory, there exists (or can be instantiated) an operator
  //	  f' : D1' ... Dn' -> R'
  //	in toModule where the prime mappings are obtained from the view.
  //
  const Vector<Symbol*>& symbols = fromTheory->getSymbols();
  int nrUserSymbols =  fromTheory->getNrUserSymbols();
  for (int i = 0; i < nrUserSymbols; ++i)
    {
      Symbol* s = symbols[i];
      Assert(fromTheory->getSymbolType(s).getBasicType() != SymbolType::VARIABLE,
	     "didn't expect variable");
      Assert(fromTheory->getSymbolType(s).getBasicType() != SymbolType::SORT_TEST,
	     "didn't expect sort test");
      //
      //	Translate the name for those operators that have no module declarations.
      //
      int id = s->id();
      if (fromTheory->moduleDeclared(s))
	{
	  if (getOpMapTerm(s) != 0 || renameOp(s) != NONE)
	    {
	      IssueAdvisory(*this << ": op mapping not applied to operator " << QUOTE(s) <<
			    " as it has at least one declaration in a module.");
	    }
	}
      else
	{
	  if (getOpMapTerm(s) != 0)
	    continue;  // op is being mapped to a term which parses and therefore whose ops must exist
	  int index = renameOp(s);
	  if (index != NONE)
	    id = getOpTo(index);
	}
      //
      //	Translate the domain and range components.
      //
      int nrArgs = s->arity();
      Vector<ConnectedComponent*> domainComponents(nrArgs);
      for (int j = 0; j < nrArgs; ++j)
	domainComponents[j] = mapComponent(s->domainComponent(j), toModule);
      ConnectedComponent* rangeComponent = mapComponent(s->rangeComponent(), toModule);
      //
      //	Check to see that a suitable operator exists or can be made in toModule.
      //
      Symbol* t = toModule->findSymbol(id, domainComponents, rangeComponent);
      if (t == 0)
	{
	  IssueWarning(*this << ": failed to find suitable operator " << QUOTE(Token::name(id)) <<
		       " in " << QUOTE(toModule) << " to represent operator " << QUOTE(s) <<
		       " from " << QUOTE(fromTheory) << '.');
	  return false;
	}
      //
      //	Need to check that t can actually replace s. This is an issue if s or t are special
      //	symbols used to represent non-algebraic data types.
      //
      SymbolType st = fromTheory->getSymbolType(s);
      SymbolType tt = toModule->getSymbolType(t);
      if ((st.nonAlgebraic() || tt.nonAlgebraic()) &&
	  st.getBasicType() != tt.getBasicType())
	{
	  IssueWarning(*this << ": operator " << QUOTE(s) << " from " << QUOTE(fromTheory) <<
		       " maps to incompatible operator " << QUOTE(t) << " in " <<
		       QUOTE(toModule) << '.');
	  return false;
	}
    }
  return true;
}

bool
View::checkPolymorphicOps()
{
  //
  //	Now we check that for each polymorphic operator
  //	  f : D1 ... Dn -> R
  //	in fromTheory, there exists (or can be instantiated) an operator
  //	  f' : D1' ... Dn' -> R'
  //	in toModule where the prime mappings are obtained from the view, and
  //	the null sort maps to the null sort.
  //
  int nrPolymorphs = fromTheory->getNrPolymorphs();
  for (int i = 0; i < nrPolymorphs; ++i)
    {
      //
      //	Translate the name.
      //
      int id = fromTheory->getPolymorphName(i).code();
      int index = renamePolymorph(id);
      if (index != NONE)
	{
	  if (fromTheory->moduleDeclaredPolymorph(i))
	    {
	      IssueAdvisory(*this << ": op mapping not applied to polymorphic operator " <<
			    QUOTE(Token::name(id)) << " as it was declared in a module.");
	    }
	  else
	    id = getOpTo(index);
	}
      //
      //	Translate the domain and range components.
      //
      const Vector<Sort*>& domainAndRange = fromTheory->getPolymorphDomainAndRange(i);
      int nrSorts = domainAndRange.size();
      Vector<Sort*> newDomainAndRange(nrSorts);
      for (int j = 0; j < nrSorts; ++j)
	{
	  Sort* s = domainAndRange[j];
	  newDomainAndRange[j] = (s == 0) ? 0 : toModule->findSort(renameSort(s->id()));
	}
      //
      //	Check to see that a suitable polymorphic operator exists in toModule.
      //
      if (toModule->findPolymorphIndex(id, newDomainAndRange) == NONE)
	{
	  IssueWarning(*this << ": failed to find suitable polymorphic operator " << QUOTE(Token::name(id)) <<
		       " in " << QUOTE(toModule) << " to represent polymorphic operator " <<
		       QUOTE(fromTheory->getPolymorphName(i)) << " from " << QUOTE(fromTheory) << '.');
	  return false;
	}
    }
  return true;
}

bool
View::evaluate()
{
  //
  //	See if view needs to be evaluated.
  //
  switch (status)
    {
    case INITIAL:
      break;
    case GOOD:
      return true;
    case BAD:
      return false;
    case STALE:
      {
	IssueAdvisory("reexamining view " << static_cast<NamedEntity*>(this) << '.');
	break;
      }
    }
  //
  //	Evaluate "from" part.
  //
  fromTheory = owner->makeModule(fromExpr);
  if (fromTheory != 0)
    {
      fromTheory->addUser(this);
      if (!(fromTheory->isTheory()))
	{
	  IssueWarning(*this << ": from part of a view must be a theory.");
	  status = BAD;
	}
    }
  else
    {
      DebugAdvisory("view " << this << " couldn't make fromTheory");
      status = BAD;
    }
  //
  //	Evaluate "to" part.
  //
  toModule = owner->makeModule(toExpr);
  if (toModule != 0)
    {
      toModule->addUser(this);
      if (toModule->getNrParameters() > 0)
	{
	  IssueWarning(*this << ": target of a view cannot have free parameters.");
	  status = BAD;
	}
    }
  else
    {
      DebugAdvisory("view " << this << " couldn't make toModule");
      status = BAD;
    }
  if (status == BAD)
    return false;

  status = GOOD;  // until proven otherwise

  if (!checkSorts() ||
      !handleOpTermMappings() ||
      !checkOps() ||
      !checkPolymorphicOps())
    {
      status = BAD;
      return false;
    }
  return true;
}

Sort*
View::mapSort(const Sort* sort, ImportModule* module) const
{
  if (sort->index() == Sort::KIND)
    return mapComponent(sort->component(), module)->sort(Sort::KIND);
  Sort* transSort = module->findSort(renameSort(sort->id()));
  Assert(transSort != 0, "translation for sort failed");
  return transSort;
}

bool
View::indexRhsVariables(Term* term, const VarMap& varMap, int lineNr)
{
  //
  //	We recurse through term, looking up each variable we encounter
  //	in varMap, and setting its index to the corresponding argument
  //	index obtained from varMap.
  //
  if (VariableTerm* vt = dynamic_cast<VariableTerm*>(term))
    {
      //
      //	We look the variable up by name.
      //
      const VarMap::const_iterator i = varMap.find(vt->id());
      if (i == varMap.end() || i->second.first != vt->getSort())
	{
	  IssueWarning(LineNumber(lineNr) <<
		       ": rhs of operator mapping contains a variable " <<
		       QUOTE(term) << " which is not the mapping of a lhs variable.");
	  return false;
	}
      vt->setIndex(i->second.second);
    }
  else
    { 
      for (ArgumentIterator i(*term); i.valid(); i.next())
	{
	  if (!indexRhsVariables(i.argument(), varMap, lineNr))
	    return false;
	}
    }
  return true;
}

bool
View::insertOpToTermMapping(int fromLineNumber,
			    Term* fromTerm,
			    int toLineNumber,
			    Term* toTerm,
			    ImportModule* targetModule)
{
  VarMap varMap;
  //
  //	First we check the arguments under fromTerm so see that
  //	they are all variables, and for X:Foo at index p, we make an entry
  //	X |-> (Bar, p) in varMap.
  //
  int argNr = 0;
  for (ArgumentIterator i(*fromTerm); i.valid(); i.next())
    {
      VariableTerm* vt = dynamic_cast<VariableTerm*>(i.argument());
      if (vt == 0)
	{
	  IssueWarning(LineNumber(fromLineNumber) <<
		       ": lhs of operator mapping has non-variable argument " <<
		       QUOTE(i.argument()) << '.');
	  fromTerm->deepSelfDestruct();
	  toTerm->deepSelfDestruct();
	  return false;
	}
      int base = vt->id();
      Sort* sort = mapSort(vt->getSort(), targetModule);
      pair<VarMap::iterator, bool> p = varMap.insert(VarMap::value_type(base, make_pair(sort, argNr)));
      if (!p.second)
	{
	  IssueWarning(LineNumber(fromLineNumber) <<
		       ": using the same variable base name " << QUOTE(Token::name(base)) <<
		       " for two left hand side variables in an operator mapping is not allowed.");
	  fromTerm->deepSelfDestruct();
	  toTerm->deepSelfDestruct();
	  return false;
	}
      ++argNr;
    }
  //
  //	It could be that fromTerm parses but has to many arguements because of flattening.
  //
  Symbol* fromSymbol = fromTerm->symbol();
  if (fromSymbol->arity() != argNr)
    {
      Assert(fromSymbol->arity() < argNr, "too few args");
      IssueWarning(LineNumber(fromLineNumber) << ": lhs of operator mapping has too many arguments.");
      fromTerm->deepSelfDestruct();
      toTerm->deepSelfDestruct();
      return false;
    }
  //
  //	Check that each variable in toTerm has a matching entry in varMap. 
  //
  if (!indexRhsVariables(toTerm, varMap, toLineNumber))
    {
      fromTerm->deepSelfDestruct();
      toTerm->deepSelfDestruct();
      return false;
    }
  //
  //	For op f(...) to term g(...) we insert
  //	f |-> (f(...), g(...)) into opTermMap.
  //
  OpTermMap::iterator i = opTermMap.insert(OpTermMap::value_type(fromSymbol->id(), make_pair(fromTerm, toTerm)));
  return true;
}

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
//      Implementation for class CUI_DagNode.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "CUI_Theory.hh"
 
//      interface class definitions
#include "binarySymbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "rawDagArgumentIterator.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"
#include "subproblem.hh"
#include "extensionInfo.hh"
 
//      core class definitions
#include "substitution.hh"
#include "pendingUnificationStack.hh"
#include "unificationContext.hh"

//	variable class definitions
#include "variableDagNode.hh"

//      CUI theory class definitions
#include "CUI_Symbol.hh"
#include "CUI_DagNode.hh"
#include "CUI_DagArgumentIterator.hh"

RawDagArgumentIterator*
CUI_DagNode::arguments()
{
  return new CUI_DagArgumentIterator(argArray);
}

size_t
CUI_DagNode::getHashValue()
{
  if (isHashValid())
    return hashCache;
  size_t hashValue = hash(hash(symbol()->getHashValue(), argArray[0]->getHashValue()),
			  argArray[1]->getHashValue());
  hashCache = hashValue;
  setHashValid();
  return hashValue;
}

int
CUI_DagNode::compareArguments(const DagNode* other) const
{
  const CUI_DagNode* d = static_cast<const CUI_DagNode*>(other);
  int r = argArray[0]->compare(d->argArray[0]);
  if (r != 0)
    return r;
  return argArray[1]->compare(d->argArray[1]);
}

DagNode*
CUI_DagNode::markArguments()
{
  argArray[0]->mark();
  return argArray[1];
}

DagNode*
CUI_DagNode::copyEagerUptoReduced2()
{
  CUI_Symbol* s = symbol();
  CUI_DagNode* n = new CUI_DagNode(s);
  n->argArray[0] = s->eagerArgument(0) ? argArray[0]->copyEagerUptoReduced() : argArray[0];
  n->argArray[1] = s->eagerArgument(1) ? argArray[1]->copyEagerUptoReduced() : argArray[1];
  return n;
}

DagNode*
CUI_DagNode::copyAll2()
{
  CUI_Symbol* s = symbol();
  CUI_DagNode* n = new CUI_DagNode(s);
  n->argArray[0] = argArray[0]->copyAll();
  n->argArray[1] = argArray[1]->copyAll();
  return n;
}

void
CUI_DagNode::clearCopyPointers2()
{
  argArray[0]->clearCopyPointers();
  argArray[1]->clearCopyPointers();
}

void
CUI_DagNode::overwriteWithClone(DagNode* old)
{
  CUI_DagNode* d = new(old) CUI_DagNode(symbol());
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
  d->argArray[0] = argArray[0];
  d->argArray[1] = argArray[1];
}

DagNode*
CUI_DagNode::makeClone()
{
  CUI_DagNode* d = new CUI_DagNode(symbol());
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
  d->argArray[0] = argArray[0];
  d->argArray[1] = argArray[1];
  return d;
}

DagNode*
CUI_DagNode::copyWithReplacement(int argIndex, DagNode* replacement)
{
  CUI_DagNode* d = new CUI_DagNode(symbol());
  if (argIndex == 0)
    {
      d->argArray[0] = replacement;
      d->argArray[1] = argArray[1];
    }
  else
    {
      Assert(argIndex == 1, "bad argument index");
      d->argArray[0] = argArray[0];
      d->argArray[1] = replacement;
    }
  return d;
}

DagNode*
CUI_DagNode::copyWithReplacement(Vector<RedexPosition>& redexStack,
				 int first,
				 int last)
{
  if (first == last)
    return copyWithReplacement(redexStack[first].argIndex(), redexStack[first].node());

  Assert(first + 1 == last, "nrArgs clash");
  CUI_DagNode* d = new CUI_DagNode(symbol());
  d->argArray[0] = redexStack[first].node();
  d->argArray[1] = redexStack[last].node();
  return d;
}

void
CUI_DagNode::collapseTo(int argNr)
{
  DagNode* remaining = (symbol()->eagerArgument(argNr)) ?
    argArray[argNr] : argArray[argNr]->copyReducible();
  remaining->overwriteWithClone(this);
}

bool
CUI_DagNode::normalizeAtTop()
{
  CUI_Symbol* s = symbol();
  Term* identity = s->getIdentity();
  if (identity != 0)
    {
      if (s->leftId() && identity->equal(argArray[0]))
	{
	  collapseTo(1);
	  return true;
	}
      if (s->rightId() && identity->equal(argArray[1]))
	{
	  collapseTo(0);
	  return true;
	}
    }
  if (s->comm() || s->idem())
    {
      int r = argArray[0]->compare(argArray[1]);
      if (s->idem() && r == 0)
	{
	  collapseTo(0);
	  return true;
	}
      if (s->comm() && r > 0)
	{
	  DagNode* t = argArray[0];
	  argArray[0] = argArray[1];
	  argArray[1] = t;
	}
    }
  return false;
}

//
//	Unification code.
//

DagNode::ReturnResult
CUI_DagNode::computeBaseSortForGroundSubterms()
{
  CUI_Symbol* s = symbol();
  if (/* s->leftId() || s->rightId() ||*/ s->idem())
    {
      //
      //	We only support unification for commutativity at the moment
      //	so let the backstop version handle it.
      //
      return DagNode::computeBaseSortForGroundSubterms();
    }
  ReturnResult r0 = argArray[0]->computeBaseSortForGroundSubterms();
  if (r0 == UNIMPLEMENTED)
    return UNIMPLEMENTED;
  ReturnResult r1 = argArray[1]->computeBaseSortForGroundSubterms();
  if (r1 == UNIMPLEMENTED)
    return UNIMPLEMENTED;
  if (r0 == GROUND && r1 == GROUND)
    {
      s->computeBaseSort(this);
      setGround();
      return GROUND;
    }
  return NONGROUND;
}

bool
CUI_DagNode::computeSolvedFormCommutativeCase(CUI_DagNode* rhs, UnificationContext& solution, PendingUnificationStack& pending)
{
  //
  //	We are have a C symbol and the rhs has the same symbol.
  //	Both dagnodes are assumed to have their arguments sorted in ascending order. Equality
  //	between any two of the four arguments eliminates the need for branching.
  //
  DagNode** rhsArgs = rhs->argArray;
  DagNode* l0 = argArray[0];
  DagNode* l1 = argArray[1];
  DagNode* r0 = rhsArgs[0];
  DagNode* r1 = rhsArgs[1];
  //
  //	We know l0 <= l1 and r0 <= r1 because of normal forms.
  //	In the C case we will decide if at least one of the 6 possible equalities holds in at most 4 comparisons.
  //	In the CU case we will postpone the issue of l0 vs l1 and r0 vs r1, and push the problem if none of the other 4 equalities hold.
  //
  int r = l0->compare(r0);
  if (r == 0)
    return l1->computeSolvedForm(r1, solution, pending);
  if (r > 0)
    {
      //
      //	Swap unificands to turn > 0 case in to < 0 case.
      swap(l0, r0);
      swap(l1, r1);
    }

  r = l1->compare(r0);
  if (r == 0)
    return l0->computeSolvedForm(r1, solution, pending);

  if (r < 0)
    {
      //
      //	We have l0 <= l1 < r0 <= r1. If we are in the C case we check both sides for duplicated arguments.
      //
      CUI_Symbol* s = symbol();
      if (!(l0->equal(l1) || r0->equal(r1)))
	{
	  //
	  //	Either we have an identity or there were no equalities. Either way we need to consider multiple
	  //	possibilities.
	  //
	  pending.push(s, this, rhs);
	  return true;
	}
    }
  else
    {
      r = l1->compare(r1);
      if (r == 0)
	return l0->computeSolvedForm(r0, solution, pending);
      if (r < 0)
	{
	  //
	  //	We have l0 < r0 < l1 < r1. No equalities possible; need to consider multiple possibilities.
	  //
	  pending.push(symbol(), this, rhs);
	  return true;
	}
      else
	{
	  //
	  //	We have l0 < r0 < l1 and r1 < l1. So r0 = r1 is our only possible equality.
	  //
	  CUI_Symbol* s = symbol();
 	  if (!(r0->equal(r1)))
	    {
	      //
	      //	Either we have an identity or there were no equalities. Either way we need to consider multiple
	      //	possibilities.
	      //
	      pending.push(s, this, rhs);
	      return true;
	    }
	}
    }
  //
  //	If we got here, we have no identity and duplicated arguments in at least one unificand.
  //	We only need to consider a singe possibility.
  //
  return l0->computeSolvedForm(r0, solution, pending) && l1->computeSolvedForm(r1, solution, pending);
}

bool
CUI_DagNode::computeSolvedForm2(DagNode* rhs, UnificationContext& solution, PendingUnificationStack& pending)
{
  //cout << "CUI_DagNode::computeSolvedForm2() " << (DagNode*) this << " vs " << rhs << endl;
  CUI_Symbol* s = symbol();
  if (s == rhs->symbol())
    {
      if (!(s->leftId() || s->rightId()))
	return computeSolvedFormCommutativeCase(safeCast(CUI_DagNode*, rhs), solution, pending);  // optimized case for C
      pending.push(s, this, rhs); // consider all alternatives
      return true;
    }

  if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(rhs))
    {
      //
      //	Get representative variable.
      //
      VariableDagNode* r = v->lastVariableInChain(solution);
      if (DagNode* value = solution.value(r->getIndex()))
	return computeSolvedForm2(value, solution, pending);
      //
      //	We have a unification problem f(u, v) =? X where X unbound.
      //
      if (s->leftId() || s->rightId())
	{
	  //
	  //	Because we could collapse, potentially to a smaller sort, we consider multiple alternatives.
	  //
	  pending.push(s, this, rhs);
	  return true;
	}
      //
      //	We need to bind the variable to our purified form.
      //
      //	We cut a corner here by treating each commutative symbol as its
      //	own theory, and rely on cycle detection to do occurs checks that
      //	pass through multiple symbols.
      //
      /*
      bool needToRebuild = false;

      DagNode* l0 = argArray[0];
      if (VariableDagNode* a = dynamic_cast<VariableDagNode*>(l0))
	{
	  if (a->lastVariableInChain(solution)->equal(r))
	    return false;  // occurs check fail
	}
      else
	{
	  VariableDagNode* abstractionVariable = solution.makeFreshVariable(s->domainComponent(0));
	  //
	  //	solution.unificationBind(abstractionVariable, l0) would be unsafe since l0 might not be pure.
	  //
	  l0->computeSolvedForm(abstractionVariable, solution, pending);
	  l0 = abstractionVariable;
	  needToRebuild = true;
	}

      DagNode* l1 = argArray[1];
      if (l1->equal(argArray[0]))
	l1 = l0;  // arguments equal so treat them the same in purified version
      else
	{
	  if (VariableDagNode* a = dynamic_cast<VariableDagNode*>(l1))
	    {
	      if (a->lastVariableInChain(solution)->equal(r))
		return false;  // occurs check fail
	    }
	  else
	    {
	      VariableDagNode* abstractionVariable = solution.makeFreshVariable(s->domainComponent(1));
	      //
	      //	solution.unificationBind(abstractionVariable, l1) would be unsafe since l1 might not be pure.
	      //
	      l1->computeSolvedForm(abstractionVariable, solution, pending);
	      l1 = abstractionVariable;
	      needToRebuild = true;
	    }
	}
      
      CUI_DagNode* purified = this;
      if (needToRebuild)
	{
	  purified = new CUI_DagNode(s);
	  if (l0->compare(l1) <= 0)
	    {
	      purified->argArray[0] = l0;
	      purified->argArray[1] = l1;
	    }
	  else
	    {
	      purified->argArray[0] = l1;
	      purified->argArray[1] = l0;
	    }
	}
      //cout << "unification bind " << (DagNode*) r << " vs " << (DagNode*) purified << endl;
      */
      CUI_DagNode* purified = makePurifiedVersion(solution, pending);
      solution.unificationBind(r, purified);
      return true;
    }
  return pending.resolveTheoryClash(this, rhs);
}

CUI_DagNode*
CUI_DagNode::makePurifiedVersion(UnificationContext& solution, PendingUnificationStack& pending)
{
  CUI_Symbol* s = symbol();
  bool needToRebuild = false;

  DagNode* l0 = argArray[0];
  if (dynamic_cast<VariableDagNode*>(l0) == 0)  // need to purify
    {
      VariableDagNode* abstractionVariable = solution.makeFreshVariable(s->domainComponent(0));
      //
      //	solution.unificationBind(abstractionVariable, l0) would be unsafe since l0 might not be pure.
      //
      l0->computeSolvedForm(abstractionVariable, solution, pending);
      l0 = abstractionVariable;
      needToRebuild = true;
    }

  DagNode* l1 = argArray[1];
  if (l1->equal(argArray[0]))
    l1 = l0;  // arguments equal so treat them the same in purified version
  else
    {
      if (dynamic_cast<VariableDagNode*>(l1) == 0)  // need to purify
	{
	  VariableDagNode* abstractionVariable = solution.makeFreshVariable(s->domainComponent(1));
	  //
	  //	solution.unificationBind(abstractionVariable, l1) would be unsafe since l1 might not be pure.
	  //
	  l1->computeSolvedForm(abstractionVariable, solution, pending);
	  l1 = abstractionVariable;
	  needToRebuild = true;
	}
    }
  if (!needToRebuild)
    return this;

  CUI_DagNode* purified = new CUI_DagNode(s);
  if (l0->compare(l1) <= 0)
    {
      purified->argArray[0] = l0;
      purified->argArray[1] = l1;
    }
  else
    {
      purified->argArray[0] = l1;
      purified->argArray[1] = l0;
    }
  return purified;
}

void
CUI_DagNode::insertVariables2(NatSet& occurs)
{
  argArray[0]->insertVariables(occurs);
  argArray[1]->insertVariables(occurs);
}

DagNode*
CUI_DagNode::instantiate2(const Substitution& substitution)
{
  bool changed = false;
  DagNode* a0 = argArray[0];
  if (DagNode* n = a0->instantiate(substitution))
    {
      a0 = n;
      changed = true;
    }
  DagNode* a1 = argArray[1];
  if (DagNode* n = a1->instantiate(substitution))
    {
      a1 = n;
      changed = true;
    }
  if (changed)
    {
      CUI_Symbol* s = symbol();
      CUI_DagNode* d = new CUI_DagNode(s);
      d->argArray[0] = a0;
      d->argArray[1] = a1;
      if(!(d->normalizeAtTop()) && a0->isGround() && a1->isGround())
	{
	  s->computeBaseSort(d);
	  d->setGround();
	}
      return d;
    }
  return 0;
}

//
//	Narrowing code.
//

bool
CUI_DagNode::indexVariables2(NarrowingVariableInfo& indices, int baseIndex)
{
  return argArray[0]->indexVariables(indices, baseIndex) &  // always make both calls
    argArray[1]->indexVariables(indices, baseIndex);
}

DagNode*
CUI_DagNode::instantiateWithReplacement(const Substitution& substitution,
					const Vector<DagNode*>* eagerCopies,
					int argIndex,
					DagNode* replacement)
{
  CUI_DagNode* d = new CUI_DagNode(symbol());
  int other = 1 - argIndex;
  d->argArray[argIndex] = replacement;

  DagNode* n = argArray[other];
  DagNode* c = (eagerCopies != 0) && symbol()->eagerArgument(other) ?
    n->instantiateWithCopies(substitution, *eagerCopies) :
    n->instantiate(substitution);  // lazy case - ok to use original unifier bindings since we won't evaluate them
  if (c != 0)  // changed under substitutition
    n = c;

  d->argArray[other] = n;
  return d;
}

DagNode*
CUI_DagNode::instantiateWithCopies2(const Substitution& substitution, const Vector<DagNode*>& eagerCopies)
{
  bool changed = false;
  DagNode* a0 = argArray[0];
  {
    DagNode* n = symbol()->eagerArgument(0) ?
      a0->instantiateWithCopies(substitution, eagerCopies) :
      a0->instantiate(substitution);
    if (n != 0)
      {
	a0 = n;
	changed = true;
      }
  }
 DagNode* a1 = argArray[1];
  {
    DagNode* n = symbol()->eagerArgument(1) ?
      a1->instantiateWithCopies(substitution, eagerCopies) :
      a1->instantiate(substitution);
    if (n != 0)
      {
	a1 = n;
	changed = true;
      }
  }

  if (changed)
    {
      CUI_Symbol* s = symbol();
      CUI_DagNode* d = new CUI_DagNode(s);
      d->argArray[0] = a0;
      d->argArray[1] = a1;
      //
      //	Currently the only user of this function is PositionState::rebuildAndInstantiateDag()
      //	via instantiateWithCopies(), SAFE_INSTANTIATE() and instantiateWithReplacement(),
      //	and this is only used for various kinds of narrowing steps. These are followed
      //	by reduction so we don't need to worry about:
      //	  normal forms
      //	  sort computations
      //	  ground flags
      //
      //	If this changes in the future the following will be needed:
      //
#if 0
      if(!(d->normalizeAtTop()) && a0->isGround() && a1->isGround())
	{
	  s->computeBaseSort(d);
	  d->setGround();
	}
#endif
      return d;
    }
  return 0;
}

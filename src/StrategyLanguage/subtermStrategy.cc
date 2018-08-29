/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2006 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for abstract class SubtermStrategy.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "strategyLanguage.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	higher class definitions
#include "matchSearchState.hh"

//	strategy language class definitions
#include "decompositionProcess.hh"
#include "strategicSearch.hh"
#include "subtermStrategy.hh"

SubtermStrategy::SubtermStrategy(Term* patternTerm,
				 int depth,
				 const Vector<ConditionFragment*>& condition,
				 const Vector<Term*>& subterms,
				 const Vector<StrategyExpression*>& strategies)
  : pattern(patternTerm, depth >= 0, condition),
    depth(depth),
    subterms(subterms),
    strategies(strategies)
{
  WarningCheck(pattern.getUnboundVariables().empty(),
	       *patternTerm << ": variable " <<
	       QUOTE(pattern.index2Variable(pattern.getUnboundVariables().min())) <<
	       " is used before it is bound in condition of subterm strategy.");
  // need to do something about recovery
}

StrategicExecution::Survival
SubtermStrategy::decompose(StrategicSearch& searchObject, DecompositionProcess* remainder)
{
  /*
  if (!pattern.getUnboundVariables().empty())
    return StrategicExecution::DIE;  // bad condition always fails
  RewritingContext* context = searchObject.getContext();
  RewritingContext* newContext = context->makeSubcontext(searchObject.getCanonical(remainder->getDagIndex()));
  MatchSearchState* state = new MatchSearchState(newContext, &pattern, MatchSearchState::GC_CONTEXT, 0, depth);
  bool result = state->findNextMatch();
  state->transferCount(*context);
  delete state;
  return result ? StrategicExecution::SURVIVE : StrategicExecution::DIE;
  */
  return StrategicExecution::DIE;  // not implemented
}

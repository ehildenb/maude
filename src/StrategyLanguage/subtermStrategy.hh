/*

    This file is part of the Maude 2 interpreter.

    Copyright 2017 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for matchrew/xmatchrew/amatchrew strategies.
//
#ifndef _subtermStrategy_hh_
#define _subtermStrategy_hh_
#include "strategyExpression.hh"
#include "pattern.hh"

class SubtermStrategy : public StrategyExpression
{
public:
  //
  //	depth = -1		means at top, no extension
  //	depth = 0		means at top, with extension
  //	depth = UNBOUNDED	means all the way down to the leaf nodes, with extension
  //
  SubtermStrategy(Term* patternTerm,
		  int depth,
		  const Vector<ConditionFragment*>& condition,
		  const Vector<Term*>& subterms,
		  const Vector<StrategyExpression*>& strategies);

  Term* getPatternTerm() const;
  int getDepth() const;
  const Vector<ConditionFragment*>& getCondition();
  const Vector<Term*>& getSubterms();
  const Vector<StrategyExpression*>& getStrategies();

  StrategicExecution::Survival decompose(StrategicSearch& searchObject, DecompositionProcess* remainder);


private:
  Pattern pattern;
  const int depth;
  const Vector<Term*> subterms;
  const Vector<StrategyExpression*> strategies;
};

inline Term*
SubtermStrategy::getPatternTerm() const
{
  return pattern.getLhs();
}

inline int
SubtermStrategy::getDepth() const
{
  return depth;
}

inline const Vector<ConditionFragment*>&
SubtermStrategy::getCondition()
{
  return pattern.getCondition();
}

inline const Vector<Term*>&
SubtermStrategy::getSubterms()
{
  return subterms;
}

inline const Vector<StrategyExpression*>&
SubtermStrategy::getStrategies()
{
  return strategies;
}

#endif

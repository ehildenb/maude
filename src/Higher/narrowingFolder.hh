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
//	Class for folding and maintaining the history of a narrowing search.
//	Both folding and keeping the history are optional. If neither are used, we simply keep
//	track of unexplored states.
//
//	The caller gives each state an index, and for each new state, holds the index of the parent state or NONE if
//	it is the initial state of the narrowing search. A states index is required to be larger than that of its parent.
//	When an existing state is ejected by the arrival of a new state that is more general, all of its descendents are
//	likewise ejected, just as if the search that has already taken place had been pruned.
//
#ifndef _narrowingFolder_hh_
#define _narrowingFolder_hh_
#include <map>
#include "simpleRootContainer.hh"
#include "narrowingVariableInfo.hh"

class NarrowingFolder : private SimpleRootContainer
{
  NO_COPYING(NarrowingFolder);

public:
  NarrowingFolder(bool fold, bool keepHistory);
  ~NarrowingFolder();

  bool insertState(int index, DagNode* state, int parentIndex);
  void addAccumulatedSubstitution(int index, int variableFamily, Substitution* accumulatedSubstitution);
  void addHistory(int index,
		  Rule* rule,
		  DagNode* narrowingContext,
		  DagNode* narrowingPosition,
		  const Substitution& unifier,
		  const NarrowingVariableInfo& variableInfo);

  bool keepingHistory() const;

  void getState(int index,
		DagNode*& state,
		int& variableFamily,
		Substitution*& accumulatedSubstitution) const;
  int getDepth(int index) const;
  void getHistory(int index,
		  DagNode*& root,
		  DagNode*& position,
		  Rule*& rule,
		  const Substitution*& unifier,
		  const NarrowingVariableInfo*& unifierVariableInfo,
		  int& variableFamily,
		  DagNode*& newDag,
		  const Substitution*& accumulatedSubstitution,
		  int& parentIndex) const;
  //
  //	Returns NONE if there is not next surviving state.
  //
  int getNextSurvivingState(DagNode*& nextState,
			    Substitution*& nextStateAccumulatedSubstitution,
			    int& nextStateVariableFamily,
			    int& nextStateDepth);

private:
  struct RetainedState
  {
    RetainedState(DagNode* state, int parentIndex, bool fold);
    ~RetainedState();
    bool subsumes(DagNode* state) const;

    DagNode* const state;
    Substitution* accumulatedSubstitution;
    const int parentIndex;
    int variableFamily;
    int depth;
    //
    //	Only used for folding.
    //
    Term* stateTerm;
    LhsAutomaton* matchingAutomaton;
    int nrMatchingVariables;  // number of variables needed for matching; includes any abstraction variables
    //
    //	Only used for history.
    //
    Rule* rule;  // rule used for narrowing step that created this state
    DagNode* narrowingContext;
    DagNode* narrowingPosition;  //  pointer into old state
    Substitution* unifier;  // variant unifier between parent state and rule lhs
    NarrowingVariableInfo variableInfo;  // variable info for the parent state part of the unifier
    //
    //	Only used for history if we are not folding.
    //
    int nrDescendants;
  };

  typedef map<int, RetainedState*> RetainedStateMap;

  void markReachableNodes();
  void cleanGraph();

  const bool fold;  // we do folding to prune less general states
  const bool keepHistory;  // we keep the history of how we arrived at each state
  RetainedStateMap mostGeneralSoFar;
  int currentStateIndex;
};

inline void
NarrowingFolder::addAccumulatedSubstitution(int index, int variableFamily, Substitution* accumulatedSubstitution)
{
  RetainedStateMap::iterator i = mostGeneralSoFar.find(index);
  Assert(i != mostGeneralSoFar.end(), "couldn't find state with index " << index);
  i->second->variableFamily = variableFamily;
  i->second->accumulatedSubstitution = accumulatedSubstitution;
}

inline bool
NarrowingFolder::keepingHistory() const
{
  return keepHistory;
}

inline void
NarrowingFolder::getState(int index,
			  DagNode*& state,
			  int& variableFamily,
			  Substitution*& accumulatedSubstitution) const
{
  RetainedStateMap::const_iterator i = mostGeneralSoFar.find(index);
  Assert(i != mostGeneralSoFar.end(), "couldn't find state with index " << index);
  state = i->second->state;
  variableFamily = i->second->variableFamily;
  accumulatedSubstitution = i->second->accumulatedSubstitution;
}

inline int
NarrowingFolder::getDepth(int index) const
{
  RetainedStateMap::const_iterator i = mostGeneralSoFar.find(index);
  Assert(i != mostGeneralSoFar.end(), "couldn't find state with index " << index);
  return i->second->depth;
}

inline void
NarrowingFolder::getHistory(int index,
			    DagNode*& root,
			    DagNode*& position,
			    Rule*& rule,
			    const Substitution*& unifier,
			    const NarrowingVariableInfo*& unifierVariableInfo,
			    int& variableFamily,
			    DagNode*& newDag,
			    const Substitution*& accumulatedSubstitution,
			    int& parentIndex) const
{
  RetainedStateMap::const_iterator i = mostGeneralSoFar.find(index);
  Assert(i != mostGeneralSoFar.end(), "couldn't find state with index " << index);

  root = i->second->narrowingContext;
  position = i->second->narrowingPosition;
  rule = i->second->rule;
  unifier = i->second->unifier;
  unifierVariableInfo = &(i->second->variableInfo);
  variableFamily = i->second->variableFamily;
  newDag = i->second->state;
  accumulatedSubstitution = i->second->accumulatedSubstitution;
  parentIndex = i->second->parentIndex;
}

#endif

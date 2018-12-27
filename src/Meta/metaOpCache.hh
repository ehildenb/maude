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
//      Class for cache of meta-operations.
//
#ifndef _metaOpCache_hh_
#define _metaOpCache_hh_
#include "userLevelRewritingContext.hh"

class MetaOpCache
{
  NO_COPYING(MetaOpCache);

public:
  MetaOpCache(int maxSize = 4);
  ~MetaOpCache();

  void insert(FreeDagNode* metaOp, CacheableState* state, Int64 lastSolutionNr);
  bool remove(FreeDagNode* metaOp, CacheableState*& state, Int64& lastSolutionNr);

  template<class T>
  bool getCachedStateObject(FreeDagNode* subject,
			    Int64 solutionNr,
			    T*& state,
			    Int64& lastSolutionNr);

  template<class T>
  bool getCachedStateObject(FreeDagNode* subject,
			    RewritingContext& context,
			    Int64 solutionNr,
			    T*& state,
			    Int64& lastSolutionNr);

private:
  bool sameProblem(FreeDagNode* m1, DagNode* m2);

  struct Item
  {
    void clear();

    DagRoot* metaOp;
    CacheableState* state;
    Int64 lastSolutionNr;
  };

  const int maxSize;
  Vector<Item> cache;
};

template<class T>
inline bool
MetaOpCache::getCachedStateObject(FreeDagNode* subject,
				  Int64 solutionNr,
				  T*& state,
				  Int64& lastSolutionNr)
{
  CacheableState* cachedState;
  if (remove(subject, cachedState, lastSolutionNr))
    {
      DebugAdvisory("looking for solution #" << solutionNr << " and found cached solution #" << lastSolutionNr);
      if (lastSolutionNr <= solutionNr)
	{
	  state = safeCast(T*, cachedState);
	  return true;
	}
      delete cachedState;
    }
  return false;
}


template<class T>
inline bool
MetaOpCache::getCachedStateObject(FreeDagNode* subject,
				  RewritingContext& context,
				  Int64 solutionNr,
				  T*& state,
				  Int64& lastSolutionNr)
{
  CacheableState* cachedState;
  if (remove(subject, cachedState, lastSolutionNr))
    {
      DebugAdvisory("looking for solution #" << solutionNr << " and found cached solution #" << lastSolutionNr);
      if (lastSolutionNr <= solutionNr)
	{
	  state = safeCast(T*, cachedState);
	  //
	  //	The parent context pointer of the root context in the
	  //	state object is possibly stale.
	  //	So we replace it with a pointer to the current context.
	  //
	  safeCast(UserLevelRewritingContext*, state->getContext())->
	    beAdoptedBy(safeCast(UserLevelRewritingContext*, &context));
	  return true;
	}
      delete cachedState;
    }
  return false;
}

#endif

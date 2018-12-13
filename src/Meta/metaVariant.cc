/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2012 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for variant narrowing descent functions.
//

bool
MetaLevelOpSymbol::metaGetVariant2(FreeDagNode* subject, RewritingContext& context, bool irredundant)
{
  DebugAdvisory(Tty(Tty::CYAN) << "meta variant call: " << Tty(Tty::GREEN) << (DagNode*) subject << Tty(Tty::RESET));
  //
  //	We handle both metaGenerateVariant() and metaGenerateIrredundantVariant().
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      DagNode* metaVarIndex = subject->getArgument(3);
      Int64 solutionNr;
      if (metaLevel->isNat(metaVarIndex) &&
	  metaLevel->downSaturate64(subject->getArgument(4), solutionNr) && solutionNr >= 0)
	{
	  const mpz_class& varIndex = metaLevel->getNat(metaVarIndex);
	  VariantSearch* vs;
	  Int64 lastSolutionNr;
	  if (m->getCachedStateObject(subject, context, solutionNr, vs, lastSolutionNr))
	    m->protect();  // Use cached state
	  else
	    {
	      if (Term* start = metaLevel->downTerm(subject->getArgument(1),  m))
		{
		  Vector<Term*> blockerTerms;
		  if (metaLevel->downTermList(subject->getArgument(2), m, blockerTerms))
		    {
		      m->protect();
		      RewritingContext* startContext = term2RewritingContext(start, context);

		      Vector<DagNode*> blockerDags; 
		      FOR_EACH_CONST(i, Vector<Term*>, blockerTerms)
			{
			  Term* t = *i;
			  t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
			  blockerDags.append(t->term2Dag());
			  t->deepSelfDestruct();
			}
		      vs = new VariantSearch(startContext, blockerDags, new FreshVariableSource(m, varIndex), false, irredundant);
		      lastSolutionNr = -1;
		    }
		  else
		    {
		      delete start;
		      return false;
		    }
		}
	      else
		return false;
	    }

	  DagNode* result;
	  const Vector<DagNode*>* variant;
	  int nrFreeVariables;
	  int parentIndex = -1;  // dummy
	  bool moreInLayer = false;  // dummy

	  if (lastSolutionNr == solutionNr)
	    {
	      //
	      //	So the user can ask for the same variant over and over again without
	      //	a horrible loss of performance.
	      //
	      int dummy;
	      variant = vs->getLastReturnedVariant(nrFreeVariables, dummy, parentIndex, moreInLayer);
	    }
	  else
	    {
	      while (lastSolutionNr < solutionNr)
		{
		  int dummy;
		  variant = vs->getNextVariant(nrFreeVariables, dummy, parentIndex, moreInLayer);
		  if (variant == 0)
		    {
		      bool incomplete = vs->isIncomplete();
		      delete vs;
		      result = metaLevel->upNoVariant(incomplete);
		      goto fail;
		    }
		  
		  context.transferCountFrom(*(vs->getContext()));
		  ++lastSolutionNr;
		}
	    }
	  {
	    m->insert(subject, vs, solutionNr);
	    mpz_class lastVarIndex = varIndex + nrFreeVariables;
	    mpz_class parentIndexBig(parentIndex);
	    result = metaLevel->upVariant(*variant, vs->getVariableInfo(), lastVarIndex, parentIndexBig, moreInLayer, m); // dummy args
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaGetVariant(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaGenerateVariant : Module Term TermList Nat Nat ~> Variant? .
  //
  return metaGetVariant2(subject, context, false);
}

bool
MetaLevelOpSymbol::metaGetIrredundantVariant(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaGenerateIrredundantVariant : Module Term TermList Nat Nat ~> Variant? .
  //
  return metaGetVariant2(subject, context, true);
}

bool
MetaLevelOpSymbol::metaVariantUnify2(FreeDagNode* subject, RewritingContext& context, bool disjoint)
{
  DebugAdvisory(Tty(Tty::CYAN) << "meta variant unify call: " << Tty(Tty::GREEN) << (DagNode*) subject << Tty(Tty::RESET));
  //
  //	We handle both metaVariantUnify() and metaVariantDisjointUnify().
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      DagNode* metaVarIndex = subject->getArgument(3);
      if (metaLevel->isNat(metaVarIndex) &&
	  metaLevel->downSaturate64(subject->getArgument(4), solutionNr) && solutionNr >= 0)
	{
	  const mpz_class& varIndex = metaLevel->getNat(metaVarIndex);
	  VariantSearch* vs;
	  Int64 lastSolutionNr;
	  if (m->getCachedStateObject(subject, context, solutionNr, vs, lastSolutionNr))
	    m->protect();  // Use cached state
	  else
	    {
	      Vector<Term*> lhs;
	      Vector<Term*> rhs;
	      if (!metaLevel->downUnificationProblem(subject->getArgument(1), lhs, rhs, m, disjoint))
		return false;

	      Vector<Term*> blockerTerms;
	      if (!metaLevel->downTermList(subject->getArgument(2), m, blockerTerms))
		{
		  FOR_EACH_CONST(i, Vector<Term*>, lhs)
		    (*i)->deepSelfDestruct();
		  FOR_EACH_CONST(j, Vector<Term*>, rhs)
		    (*j)->deepSelfDestruct();
		  return false;
		}
	      
	      m->protect();
	      DagNode* d = m->makeUnificationProblemDag(lhs, rhs);
	      RewritingContext* startContext = context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);

	      Vector<DagNode*> blockerDags; 
	      FOR_EACH_CONST(i, Vector<Term*>, blockerTerms)
		{
		  Term* t = *i;
		  t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
		  blockerDags.append(t->term2Dag());
		  t->deepSelfDestruct();
		}
	      vs = new VariantSearch(startContext, blockerDags, new FreshVariableSource(m, varIndex), true, false);
	      lastSolutionNr = -1;
	    }

	  DagNode* result;
	  const Vector<DagNode*>* unifier;
	  int nrFreeVariables;
	  if (lastSolutionNr == solutionNr)
	    {
	      //
	      //	So the user can ask for the same unifier over and over again without
	      //	a horrible loss of performance.
	      //
	      int dummy;
	      unifier = vs->getLastReturnedUnifier(nrFreeVariables, dummy);
	    }
	  else
	    {
	      while (lastSolutionNr < solutionNr)
		{
		  int dummy;
		  unifier = vs->getNextUnifier(nrFreeVariables, dummy);
		  if (unifier == 0)
		    {
		      bool incomplete = vs->isIncomplete();
		      delete vs;
		      result = disjoint ? metaLevel->upNoUnifierTriple(incomplete) : metaLevel->upNoUnifierPair(incomplete);
		      goto fail;
		    }

		  context.transferCountFrom(*(vs->getContext()));
		  ++lastSolutionNr;
		}
	    }
	  {
	    m->insert(subject, vs, solutionNr);
	    mpz_class lastVarIndex = varIndex + nrFreeVariables;
	    result = disjoint ?
	      metaLevel->upUnificationTriple(*unifier, vs->getVariableInfo(), lastVarIndex, m) :
	      metaLevel->upUnificationPair(*unifier, vs->getVariableInfo(), lastVarIndex, m);
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaVariantUnify(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaVariantUnify : Module UnificationProblem TermList Nat Nat ~> UnificationPair? .
  //
  return metaVariantUnify2(subject, context, false);
}

bool
MetaLevelOpSymbol::metaVariantDisjointUnify(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaVariantDisjointUnify : Module UnificationProblem TermList Nat Nat ~> UnificationTriple? .
  //
  return metaVariantUnify2(subject, context, true);
}

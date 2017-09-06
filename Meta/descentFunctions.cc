//
//	The descent functions supported by MetaLevelOpSymbol.
//

local_inline bool
MetaLevelOpSymbol::getCachedRewriteSearchState(MetaModule* m,
					       FreeDagNode* subject,
					       RewritingContext& context,
					       Int64 solutionNr,
					       RewriteSearchState*& state,
					       Int64& lastSolutionNr)
{
  if (solutionNr > 0)
    {
      SearchState* s;
      if (m->remove(subject, context, s, lastSolutionNr))
	{
	  if (lastSolutionNr < solutionNr)
	    {
	      state = safeCast(RewriteSearchState*, s);
	      return true;
	    }
	  delete s;
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaSortLeq(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* s1;
      Sort* s2;
      if (metaLevel->downSimpleSort(subject->getArgument(1), m, s1) &&
	  metaLevel->downSimpleSort(subject->getArgument(2), m, s2))
	{
	  return context.builtInReplace(subject,
					metaLevel->upBool(s1->component() == s2->component()
							  && leq(s1, s2)));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaWellFormedModule(FreeDagNode* subject, RewritingContext& context)
{
  MixfixModule* m = metaLevel->downModule(subject->getArgument(0));
  return context.builtInReplace(subject, metaLevel->upBool(m != 0));
}

bool
MetaLevelOpSymbol::metaWellFormedTerm(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      bool result = false;
      if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
	{
	  result = true;
	  t->deepSelfDestruct();
	}
      return context.builtInReplace(subject, metaLevel->upBool(result));
    }
  return false;
}

bool
MetaLevelOpSymbol::metaWellFormedSubstitution(FreeDagNode* subject, RewritingContext& context)
{
  if (ImportModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      bool result = false;
      Vector<Term*> variables;
      Vector<Term*> values;
      if (metaLevel->downSubstitution(subject->getArgument(1), m, variables, values))
	{
	  if (noDuplicates(variables))
	    {
	      Vector<DagRoot*> dags;
	      m->protect();
	      if (dagifySubstitution(variables, values, dags, context))
		{
		  result = true;
		  for (int i = variables.length() - 1; i >= 0; i--)
		    delete dags[i];
		}
	      (void) m->unprotect();
	    }
	  for (int i = variables.length() - 1; i >= 0; i--)
	    {
	      variables[i]->deepSelfDestruct();
	      values[i]->deepSelfDestruct();
	    }
	}
      return context.builtInReplace(subject, metaLevel->upBool(result));
    }
  return false;
}

bool
MetaLevelOpSymbol::noDuplicates(const Vector<Term*>& terms)
{
  for (int i = terms.length() - 1; i >= 1; i--)
    {
      for (int j = i - 1; j >= 0; j--)
	{
	  if (terms[i]->equal(terms[j]))
	    return false;
	}
    }
  return true;
}

bool
MetaLevelOpSymbol::dagifySubstitution(const Vector<Term*>& variables,
				      Vector<Term*>& values,
				      Vector<DagRoot*>& dags,
				      RewritingContext& context)
{
  int nrVars = variables.length();
  dags.resize(nrVars);
  for (int i = 0; i < nrVars; i++)
    {
      values[i] = values[i]->normalize(false);
      DagNode* d = term2Dag(values[i]);
      dags[i] = new DagRoot(d);
      d->computeTrueSort(context);
      VariableTerm* v = static_cast<VariableTerm*>(variables[i]);
      if (!(leq(d->getSortIndex(), v->getSort())))
	{
	  for (int j = 0; j <= i ; j++)
	    delete dags[j];
	  return false;
	}
    }
  return true;
}

RewritingContext*
MetaLevelOpSymbol::term2RewritingContext(Term* term, RewritingContext& context)
{
  term = term->normalize(false);
  DagNode* d = term2Dag(term);
  term->deepSelfDestruct();
  return context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
}

bool
MetaLevelOpSymbol::metaSameKind(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* s1;
      Sort* s2;
      if (metaLevel->downType(subject->getArgument(1), m, s1) &&
	  metaLevel->downType(subject->getArgument(2), m, s2))
	{
	  bool result = s1->component() == s2->component();
	  return context.builtInReplace(subject, metaLevel->upBool(result));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaLeastSort(FreeDagNode* subject, RewritingContext& context)
{
  if (ImportModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
	{
	  t = t->normalize(false);
	  DagNode* d = term2Dag(t);
	  t->deepSelfDestruct();
	  RewritingContext* objectContext =
	    context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
	  m->protect();
	  d->computeTrueSort(*objectContext);
	  context.addInCount(*objectContext);
	  PointerMap qidMap;
	  d = metaLevel->upType(d->getSort(), qidMap);
	  delete objectContext;
	  (void) m->unprotect();
	  return context.builtInReplace(subject, d);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaCompleteName(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* s;
      if (metaLevel->downType(subject->getArgument(1), m, s))
	{
	  PointerMap qidMap;
	  return context.builtInReplace(subject, metaLevel->upType(s, qidMap));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaLesserSorts(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* s;
      if (metaLevel->downSimpleSort(subject->getArgument(1), m, s))
	{
	  ConnectedComponent* component = s->component();
	  int nrSorts = component->nrSorts();
	  Vector<Sort*> lesserSorts;
	  for (int i = s->index() + 1; i < nrSorts; i++)
	    {
	      Sort* s2 = component->sort(i);
	      if (leq(s2, s))
		lesserSorts.append(s2);
	    }
	  return context.builtInReplace(subject, metaLevel->upSortSet(lesserSorts));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaMaximalSorts(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* k;
      if (metaLevel->downType(subject->getArgument(1), m, k) &&
	  k->index() == Sort::ERROR_SORT)
	{
	  ConnectedComponent* component = k->component();
	  int nrMaximalSorts = component->nrMaximalSorts();
	  Vector<Sort*> maximalSorts(nrMaximalSorts);
	  for (int i = 1; i <= nrMaximalSorts; i++)
	    maximalSorts[i - 1] = component->sort(i);
	  return context.builtInReplace(subject, metaLevel->upSortSet(maximalSorts));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaMinimalSorts(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* k;
      if (metaLevel->downType(subject->getArgument(1), m, k) &&
	  k->index() == Sort::ERROR_SORT)
	{
	  ConnectedComponent* component = k->component();
	  int nrSorts = component->nrSorts();
	  Vector<Sort*> minimalSorts;
	  for (int i = 1; i < nrSorts; i++)
	    {
	      Sort* s = component->sort(i);
	      if (s->getSubsorts().length() == 0)
		minimalSorts.append(s);
	    }
	  return context.builtInReplace(subject, metaLevel->upSortSet(minimalSorts));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaGlbSorts(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* s1;
      Sort* s2;
      if (metaLevel->downSimpleSort(subject->getArgument(1), m, s1) &&
	  metaLevel->downSimpleSort(subject->getArgument(2), m, s2))
	{
	  Vector<Sort*> glbSorts;
	  ConnectedComponent* component = s1->component();
	  if (s2->component() == component)
	    {
	      NatSet leqSorts(s1->getLeqSorts());
	      leqSorts.intersect(s2->getLeqSorts());
	      Vector<int> glbIndices;
	      component->findMaximalSorts(leqSorts, glbIndices);
	      int nrSorts = glbIndices.length();
	      glbSorts.expandTo(nrSorts);
	      for (int i = 0; i < nrSorts; i++)
		glbSorts[i] = component->sort(glbIndices[i]);
	    }
	  return context.builtInReplace(subject, metaLevel->upSortSet(glbSorts));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaGetKind(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* s;
      if (metaLevel->downType(subject->getArgument(1), m, s))
	{
	  Sort* kind = s->component()->sort(Sort::ERROR_SORT);
	  PointerMap qidMap;
	  return context.builtInReplace(subject, metaLevel->upType(kind, qidMap));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaGetKinds(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    return context.builtInReplace(subject, metaLevel->upKindSet(m->getConnectedComponents()));
  return false;
}

bool
MetaLevelOpSymbol::metaReduce(FreeDagNode* subject, RewritingContext& context)
{
  if (ImportModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
	{
	  t = t->normalize(false);
	  DagNode* d = term2Dag(t);
	  t->deepSelfDestruct();
	  RewritingContext* objectContext =
	    context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
	  m->protect();
	  objectContext->reduce();
	  context.addInCount(*objectContext);
	  d = metaLevel->upResultPair(objectContext->root(), m);
	  delete objectContext;
	  (void) m->unprotect();
	  return context.builtInReplace(subject, d);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaRewrite(FreeDagNode* subject, RewritingContext& context)
{
  if (ImportModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 limit;
      if (metaLevel->downBound64(subject->getArgument(2), limit) && limit != 0)
        {
          if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
            {
              t = t->normalize(false);
              DagNode* d = term2Dag(t);
              t->deepSelfDestruct();
	      //cerr << "before create\n"; RootContainer::dump(cerr);
              RewritingContext* objectContext =
                context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
	      //cerr << "after create\n"; RootContainer::dump(cerr);
              m->resetRules();
              m->protect();
              objectContext->ruleRewrite(limit);
	      //cerr << "after run\n"; RootContainer::dump(cerr);
              context.addInCount(*objectContext);
              d = metaLevel->upResultPair(objectContext->root(), m);
              delete objectContext;
	      //cerr << "after delete\n"; RootContainer::dump(cerr);
              (void) m->unprotect();
              return context.builtInReplace(subject, d);
            }
        }
    }
  return false;
}

bool
MetaLevelOpSymbol::metaFrewrite(FreeDagNode* subject, RewritingContext& context)
{
  if (ImportModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 limit;
      Int64 gas;
      if (metaLevel->downBound64(subject->getArgument(2), limit) && limit != 0 &&
	  metaLevel->downSaturate64(subject->getArgument(3), gas) && gas != 0)
	{
	  if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
	    {
	      t = t->normalize(false);
	      DagNode* d = term2Dag(t);
	      t->deepSelfDestruct();
	      //cerr << "before create\n"; RootContainer::dump(cerr);
	      RewritingContext* objectContext =
		context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
	      safeCast(ObjectSystemRewritingContext*, objectContext)->
		setObjectMode(ObjectSystemRewritingContext::FAIR);
	      //cerr << "after create\n"; RootContainer::dump(cerr);
	      m->resetRules();
	      m->protect();
	      objectContext->fairRewrite(limit, gas);
	      //cerr << "after run\n"; RootContainer::dump(cerr);
	      objectContext->root()->computeTrueSort(*objectContext);
	      //cerr << "after sort\n"; RootContainer::dump(cerr);
	      context.addInCount(*objectContext);
	      d = metaLevel->upResultPair(objectContext->root(), m);
	      delete objectContext;
	      //cerr << "after delete\n"; RootContainer::dump(cerr);
	      (void) m->unprotect();
	      return context.builtInReplace(subject, d);
	    }
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaParse(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      ConnectedComponent* component;
      Vector<int> metaTokens;
      if (metaLevel->downComponent(subject->getArgument(2), m, component) &&
	  metaLevel->downQidList(subject->getArgument(1), metaTokens))
	{
	  int nrTokens = metaTokens.length();
	  Vector<Token> tokens(nrTokens);
	  for (int i = 0; i < nrTokens; i++)
	    tokens[i].tokenize(metaTokens[i], FileTable::META_LEVEL_CREATED);
	  Term* parse1;
	  Term* parse2;
	  int firstBad;
	  switch(m->parseTerm2(tokens, component, parse1, parse2, firstBad))
	    {
	    case -1: 	// bad token
	    case 0:	// no parse
	      {
		return context.builtInReplace(subject, metaLevel->upNoParse(firstBad));
	      }
	    case 1:	// 1 parse
	      {
		parse1->symbol()->fillInSortInfo(parse1);
		DagNode* d = metaLevel->upResultPair(parse1, m);
		parse1->deepSelfDestruct();
		return context.builtInReplace(subject, d);
	      }
	    default:	// more than 1 parse
	      {
		parse1->symbol()->fillInSortInfo(parse1);
		parse2->symbol()->fillInSortInfo(parse2);
		DagNode* d = metaLevel->upAmbiguity(parse1, parse2, m);
		parse1->deepSelfDestruct();
		parse2->deepSelfDestruct();
		return context.builtInReplace(subject, d);
	      }
	    }
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaPrettyPrint(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
	{
	  Vector<int> buffer;
	  m->bufferPrint(buffer, t);
	  t->deepSelfDestruct();
	  return context.builtInReplace(subject, metaLevel->upQidList(buffer));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaApply(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaApply: Module Term Qid Substitution Nat ~> ResultTriple? .
  //    op {_,_,_} : Term Type Substitution -> ResultTriple [ctor] .
  //
  Int64 solutionNr;
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      if (metaLevel->downSaturate64(subject->getArgument(4), solutionNr))
	{
	  RewriteSearchState* state;
	  Int64 lastSolutionNr;
	  if (getCachedRewriteSearchState(m, subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();  // Use cached state
	  else
	    {
	      //
	      //	Need to create state from scratch
	      //
	      int label;
	      Vector<Term*> variables;
	      Vector<Term*> values;
	      if (metaLevel->downQid(subject->getArgument(2), label) &&
		  metaLevel->downSubstitution(subject->getArgument(3), m, variables, values))
		{
		  Vector<DagRoot*> dags;
		  m->protect();
		  if (noDuplicates(variables) &&
		      dagifySubstitution(variables, values, dags, context))
		    {
		      if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
			{
			  RewritingContext* subjectContext = term2RewritingContext(t, context);
			  subjectContext->reduce();
			  context.addInCount(*subjectContext);
			  state = new RewriteSearchState(subjectContext,
							 label,
							 RewriteSearchState::GC_CONTEXT |
							 RewriteSearchState::GC_SUBSTITUTION |
							 RewriteSearchState::ALLOW_NONEXEC);
			  if (variables.length() > 0)
			    state->setInitialSubstitution(variables, dags);
			  for (int i = values.length() - 1; i >= 0; i--)
			    values[i]->deepSelfDestruct();
			  lastSolutionNr = -1;
			  goto ok;
			}
		      for (int i = dags.length() - 1; i >= 0; i--)
			delete dags[i];
		    }
		  for (int i = variables.length() - 1; i >= 0; i--)
		    {
		      variables[i]->deepSelfDestruct();
		      values[i]->deepSelfDestruct();
		    }
		  m->unprotect();
		}
	      return false;
	    }
	ok:
	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextRewrite();
	      state->transferCount(context);
	      if (!success)
		{
		  delete state;
		  result = metaLevel->upFailureTriple();
		  goto fail;
		}
	      ++lastSolutionNr;
	    }
	  m->insert(subject, state, solutionNr);
	  {
	    Rule* rule = state->getRule();
	    DagNode* replacement = state->getReplacement();
	    Substitution* substitution = state->getContext();
	    DagNode* top = state->rebuildDag(replacement);
	    RewritingContext* resultContext =
	      context.makeSubcontext(top, UserLevelRewritingContext::META_EVAL);
	    resultContext->reduce();
	    context.addInCount(*resultContext);
	    context.incrementRlCount();
	    result = metaLevel->upResultTriple(resultContext->root(),
					       *substitution,
					       *rule,
					       m);
	    delete resultContext;
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaXapply(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaXapply : Module Term Qid Substitution Nat Bound Nat ~> Result4Tuple? .
  //	op {_,_,_,_} : Term Type Substitution Context -> Result4Tuple [ctor] .
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(subject->getArgument(6), solutionNr))
	{
	  RewriteSearchState* state;
	  Int64 lastSolutionNr;
	  if (getCachedRewriteSearchState(m, subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();  // Use cached state
	  else
	    {
	      //
	      //	Need to create state from scratch
	      //
	      int minDepth;
	      int maxDepth;
	      int label;
	      Vector<Term*> variables;
	      Vector<Term*> values;
	      if (metaLevel->downSaturate(subject->getArgument(4), minDepth) &&
		  metaLevel->downBound(subject->getArgument(5), maxDepth) &&
		  metaLevel->downQid(subject->getArgument(2), label) &&
		  metaLevel->downSubstitution(subject->getArgument(3), m, variables, values))
		{
		  if (maxDepth == NONE)
		    maxDepth = UNBOUNDED;  // NONE means no extension for RewriteSearchState
		  Vector<DagRoot*> dags;
		  m->protect();
		  if (noDuplicates(variables) &&
		      dagifySubstitution(variables, values, dags, context))
		    {
		      if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
			{
			  RewritingContext* subjectContext = term2RewritingContext(t, context);
			  subjectContext->reduce();
			  context.addInCount(*subjectContext);
			  state = new RewriteSearchState(subjectContext,
							 label,
							 RewriteSearchState::GC_CONTEXT |
							 RewriteSearchState::GC_SUBSTITUTION |
							 RewriteSearchState::ALLOW_NONEXEC,
							 minDepth,
							 maxDepth);
			  if (variables.length() > 0)
			    state->setInitialSubstitution(variables, dags);
			  for (int i = values.length() - 1; i >= 0; i--)
			    values[i]->deepSelfDestruct();
			  lastSolutionNr = -1;
			  goto ok;
			}
		      for (int i = dags.length() - 1; i >= 0; i--)
			delete dags[i];
		    }
		  for (int i = variables.length() - 1; i >= 0; i--)
		    {
		      variables[i]->deepSelfDestruct();
		      values[i]->deepSelfDestruct();
		    }
		  m->unprotect();
		}
	      return false;
	    }
	ok:
	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      bool success = state->findNextRewrite();
	      state->transferCount(context);
	      if (!success)
		{
		  delete state;
		  result = metaLevel->upFailure4Tuple();
		  goto fail;
		}
	      ++lastSolutionNr;
	    }
	  m->insert(subject, state, solutionNr);
	  {
	    Rule* rule = state->getRule();
	    DagNode* replacement = state->getReplacement()->makeClone();  // for unique ptr
	    Substitution* substitution = state->getContext();
	    DagNode* top = state->rebuildDag(replacement);
	    PointerMap qidMap;
	    PointerMap dagNodeMap;
	    DagRoot metaContext(metaLevel->upContext(top, m, replacement, qidMap, dagNodeMap));
	    RewritingContext* resultContext =
	      context.makeSubcontext(top, UserLevelRewritingContext::META_EVAL);
	    resultContext->reduce();
	    context.addInCount(*resultContext);
	    context.incrementRlCount();
	    result = metaLevel->upResult4Tuple(resultContext->root(),
					       *substitution,
					       *rule,
					       metaContext.getNode(),
					       m);
	    delete resultContext;
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

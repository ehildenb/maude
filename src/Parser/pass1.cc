//
//	First pass: construct a compact representation of all parses.
//

void
Parser::processBubble(int tokenNr, int bubbleNr, const Vector<int>& sentence)
{
  Bubble b = bubbles[bubbleNr];
  int ruleNr = b.ruleNr;
  ParserLog("processBubble tokenNr=" << tokenNr << " ruleNr=" << ruleNr << " bubbleNr=" << bubbleNr);

  int openParens = 0;
  int last = sentence.size();

  for (int i = tokenNr; i < last;)
    {
      int token = sentence[i];
      if (b.upperBound != NONE)
	{
	  //
	  //	Check for syntax errors caused by hitting upper bound.
	  //
	  int tokensLeft = b.upperBound - (i - tokenNr);
	  if (tokensLeft == 0)
	    return;
	  if (token != b.rightParen)
	    {
	      //
	      //	Need to check for syntax errors arising from insufficient upperBounds to
	      //	close parentheses.
	      //
	      int tokensLeft = b.upperBound - (i - tokenNr);
	      if (tokensLeft == openParens)
		return;  // not enough tokens left to close open parens
	      if (tokensLeft == openParens + 1 && token == b.leftParen)
		return;  // no enough tokens left to close parens if we open one more
	    }
	}
      //
      //	Match tokens, keeping in mind that left and right paren could be the same.
      //
      if (token == b.rightParen)
	{
	  if (openParens > 0)
	    --openParens;
	  else if (token == b.leftParen)
	    openParens = 1;
	  else
	    return;  // unmatched closing paren
	}
      else if (token == b.leftParen)
	++openParens;
      else
	{
	  //
	  //	Regular token, check if it is allowed.
	  //
	  const Vector<int>& excluded = b.excludedTerminals;
	  FOR_EACH_CONST(i, Vector<int>, excluded)
	    {
	      if (*i == token)
		return;  // exluded token
	    }
	}
      //
      //	Token was part of a bubble with a valid continuation.
      //
      ++i;
      if (i > badTokenIndex)
	badTokenIndex = i;
      if (openParens == 0 &&
	  (b.lowerBound == NONE || b.lowerBound <= i - tokenNr))
	{
	  //
	  //	Bubble is big enough and has no unclosed parens, so it's
	  //	a valid place to end the bubble.
	  //
	  ParserLog("returning bubble at token " << i);
	  makeReturn(i, ruleNr, tokenNr);
	}
    }
}

void
Parser::doBubbles(int tokenNr, const Vector<int>& sentence)
{
  ParserLog(Tty(Tty::RED) << "doBubble() at token " << tokenNr << Tty(Tty::RESET));
  //
  //	Look at each call for a nonterminal.
  //
  for (int i = firstCalls[tokenNr]; i != NONE;)
    {
      Call& call = calls[i];
      i = call.nextCall;
      //
      //	Look at each bubble that can parse to that nonterminal.
      //
      for (int j = firstBubbles[flip(call.nonTerminal)]; j != NONE; j = bubbles[j].nextBubble)
	processBubble(tokenNr, j, sentence);
    }
}

void
Parser::doEmptyBubbles(int tokenNr)
{
  ParserLog(Tty(Tty::RED) << "doEmptyBubble() at token " << tokenNr << Tty(Tty::RESET));
  //
  //	Look at each call for a nonterminal.
  //
  for (int i = firstCalls[tokenNr]; i != NONE;)
    {
      Call& call = calls[i];
      i = call.nextCall;
      //
      //	Look at each bubble that can parse to that nonterminal.
      //
      for (int j = firstBubbles[flip(call.nonTerminal)]; j != NONE;)
	{
	  Bubble& b = bubbles[j];
	  j = b.nextBubble;
	  if (b.lowerBound == 0)
	    {
	      //
	      //	We have the possibility of this nonterminal going to an empty bubble.
	      //	This generates an immediate return in the same parse list.
	      //
	      ParserLog("did empty bubble return for rule " << b.ruleNr);
	      makeReturn(tokenNr, b.ruleNr, tokenNr);
	    }
	}
    }
}

void
Parser::processReturn(int tokenNr, int startTokenNr, int ruleNr, const Vector<int>& sentence)
{
  //
  //	In list tokenNr we have a return for ruleNr starting at startTokenNr.
  //
  ParserLog("processReturn() tokenNr=" << tokenNr << " startTokenNr=" << startTokenNr <<
	    " ruleNr=" << ruleNr);
  Rule& rule = rules[ruleNr];
  //
  //	Get the (nonterminal, prec) we recognized.
  //
  int nonTerminal = rule.nonTerminal;
  int prec = rule.prec;
  ParserLog("nonterminal=" << nonTerminal << " prec=" << prec);
  //
  //	See if we handled this (nonterminal, prec) at startToken before and
  //	memoized a DRP.
  //
  for (int j = firstMemoItems[startTokenNr]; j != NONE;)
    {
      MemoItem& m = memoItems[j];
      j = m.nextMemoItem;
      if (m.nonTerminal == nonTerminal && m.maxPrec == prec)
	{
	  ParserLog("Found memoized DRP that ends at rule="<< m.ruleNr << " startTokenNr=" << m.startTokenNr);
	  makeReturn(tokenNr, m.ruleNr, m.startTokenNr);
	  return;
	}
    }
  //
  //	No memo applied so we now look for calls satisfied by (nonterminal, prec).
  //
  bool seenContinuationBranch = false;
  Vector<DeferredReturn> deferredReturns;  // FIXME: inefficient
  //
  //	We look at all the calls at startTokenNr.
  //
  for (int j = firstCalls[startTokenNr]; j != NONE;)  // look for what called our nonterminal
    {
      Call& call = calls[j];
      int nonTerminal2 = call.nonTerminal;
      int maxPrec = call.maxPrec;
      j = call.nextCall;
      if (nonTerminal2 == nonTerminal)
	{
	  //
	  //	This is the call for our nonterminal - handle explicit continuations.
	  //
	  for (int k = call.firstContinuation; k != NONE;)
	    {
	      Continuation& cont = continuations[k];
	      int ruleNr2 = cont.ruleNr;
	      int pos = cont.rhsPosition;
	      k = cont.nextContinuation;
	      Rule& rule2 = rules[ruleNr2];
	      if (rule2.rhs[pos].prec >= prec)
		{
		  int rhsRemaining = rule2.rhs.length() - (pos + 1);
		  if (rhsRemaining == 0)
		    {
		      ParserLog("right recursive return for rule=" << ruleNr2);
		      //
		      //	This continuation immediately generates a return.
		      //	We defer processing these until we know if they are unique
		      //	(i.e. a deterministic reduction).
		      //
		      int nrDeferredReturns = deferredReturns.size();
		      deferredReturns.expandBy(1);
		      deferredReturns[nrDeferredReturns].ruleNr = ruleNr2;
		      deferredReturns[nrDeferredReturns].startTokenNr = cont.startTokenNr;
		    }
		  else
		    {
		      ParserLog("continuation for rule=" << ruleNr2);
		      seenContinuationBranch = true;
		      //
		      //	This continuation has more to match.
		      //
		      advanceRule(ruleNr2, pos + 1, cont.startTokenNr, tokenNr, sentence);
		    }
		}
	    }
	}
      //
      //	We see a call for nonTerminal2. Get the tree of implied start-ups for nonTerminal2,
      //	and see if we have nonTerminal2 ::= nonTerminal ...
      //	where the prec for nonTerminal is >= the prec of the rule we just recognized.
      //
      int r = nonTerminalDecisionTrees[flip(nonTerminal2)];
      while (r != NONE)
	{
	  Rule& rule2 = rules[r];
	  int t = nonTerminal - rule2.rhs[0].symbol;
	  if (t == 0)
	    break;
	  r = (t > 0) ? rule2.bigger : rule2.smaller;
	}
      while (r != NONE)
	{
	  Rule& rule2 = rules[r];
	  if (rule2.prec > maxPrec)
	    {
	      //
	      //	rule2 didn't have small enough prec to satisfy call. Rules in the equals path
	      //	are ordered by increasing prec so we are done.
	      //
	      break;
	    }
	  if (rule2.rhs[0].prec < prec)
	    {
	      //
	      //	rule2 required a smaller prec than our return provided.
	      //
	      r = rule2.equal;
	      continue;
	    }
	  ParserLog("saw implied call by rule=" << r);
	  //
	  //	Normal processesing.
	  //
	  int rhsRemaining = rule2.rhs.length() - 1;
	  if (rhsRemaining == 0)
	    {
	      ParserLog("implied right recursive return for rule=" << r);
	      //
	      //	This continuation immediately generates a return.
	      //	We defer processing these until we know if they are unique
	      //	(i.e. a deterministic reduction).
	      //
	      int nrDeferredReturns = deferredReturns.size();
	      deferredReturns.expandBy(1);
	      deferredReturns[nrDeferredReturns].ruleNr = r;
	      deferredReturns[nrDeferredReturns].startTokenNr = startTokenNr;
	    }
	  else
	    {
	      ParserLog("implied continuation for rule=" << r);
	      seenContinuationBranch = true; 
	      advanceRule(r, 1, startTokenNr, tokenNr, sentence);
	    }
	  r = rule2.equal;
	}
    }
  //
  //	We've now processed all the explicit and implicit callers for (nonterminal, prec)
  //	pair we recognized. Now see if we have a DRP.
  //
  ParserLog("After processing callers seenContinuationBranch=" << seenContinuationBranch <<
	    " deferredReturns.size()=" << deferredReturns.size());
  if (!seenContinuationBranch && deferredReturns.size() == 1)
    {
      //
      //	When looking for continuations of (nonTerminal, prec) in calls from
      //	token startTokenNr we found only a single continuation that immediately
      //	generates a return (that we placed in deferredReturns).
      //
      //	We treat this as the bottom of a deterministic reduction path (DRP) and
      //	follow it to the top of the path. Rather than generate a linear number
      //	of returns, we only generate the final return, and memoize this
      //	return at startTokenNr for continuations of (nonTerminal, prec)
      //	so we don't chase this DRP a second time.
      //
      ParserLog("deterministic reduction seen in set " << tokenNr <<
		" from rule " << ruleNr << " starting  at " << startTokenNr <<
		" to rule " << deferredReturns[0].ruleNr <<
		" starting at " << deferredReturns[0].startTokenNr);

      IntPair topMost = chaseDeterministicReductionPath(deferredReturns[0].ruleNr, deferredReturns[0].startTokenNr);
      firstMemoItems[startTokenNr] = makeMemoItem(nonTerminal,
						  prec,
						  topMost.first,
						  topMost.second,
						  firstMemoItems[startTokenNr]);
      ParserLog("made memo item in set " << startTokenNr <<
		" when looking for nonterminal " << nonTerminal << " at prec " << prec <<
		" we saw a DRP that ends at rule " << topMost.first << " to set " << topMost.second);
      makeReturn(tokenNr, topMost.first, topMost.second);
    }
  else
    {
      ParserLog("No DRP seen tokeNr = " << tokenNr << "  ruleNr = " << ruleNr <<
		"  startTokenNr = " << startTokenNr <<
		"  seenContinuationBranch = " << seenContinuationBranch <<
		"  deferredReturns.size() = " << deferredReturns.size());
      //
      //	Process deferred returns normally.
      //
      FOR_EACH_CONST(i, Vector<DeferredReturn>, deferredReturns)
	makeReturn(tokenNr, i->ruleNr, i->startTokenNr);
    }
}

void
Parser::doReturns(int tokenNr, const Vector<int>& sentence)
{
  lastReturnProcessed = NONE;  // in case we don't process any returns for this list
  //
  //	Do all returns.
  //
  for (int i = firstReturns[tokenNr]; i != NONE; i = returns[i].nextReturn)
    {
      lastReturnProcessed = i;
      Return& ret = returns[i];
      processReturn(tokenNr, ret.startTokenNr, ret.ruleNr, sentence);
    }
}

void
Parser::doEmptyBubbleReturns(int tokenNr, const Vector<int>& sentence)
{
  //
  //	We redo old empty bubble returns in case they have more continuations
  //
  int i = firstReturns[tokenNr];
  if (lastReturnProcessed != NONE)
    {
      //
      //	We have already processed some returns. We need to redo any
      //	old empty bubble returns in case they have more continuations.
      //
      for (;; i = returns[i].nextReturn)
	{
	  Return& ret = returns[i];
	  if (tokenNr == ret.startTokenNr)
	    processReturn(tokenNr, ret.startTokenNr, ret.ruleNr, sentence);
	  if (i == lastReturnProcessed)
	    break;  // this was the last of the already processed returns
	}
      i = returns[i].nextReturn;  // first new return
    }
  //
  //	We then do new returns which maybe empty bubble returns or
  //	have arose from processing empty bubble returns.
  //
  for (; i != NONE; i = returns[i].nextReturn)
    {
      lastReturnProcessed = i;
      Return& ret = returns[i];
      processReturn(tokenNr, ret.startTokenNr, ret.ruleNr, sentence);
    }
}

Parser::IntPair
Parser::chaseDeterministicReductionPath(int ruleNr, int startTokenNr)
{
  Rule& rule = rules[ruleNr];
  int nonTerminal = rule.nonTerminal;
  int prec = rule.prec;

  ParserLog("    chaseDeterministicReductionPath() called for ruleNr " << ruleNr <<
	    "  startTokenNr = " << startTokenNr <<
	    "  nonTerminal = " << nonTerminal <<
	    "  prec = " << prec);
  int nextRuleNr = NONE;
  int nextStartTokenNr = NONE;

  for (int i = firstMemoItems[startTokenNr]; i != NONE;)
    {
      MemoItem& m = memoItems[i];
      i = m.nextMemoItem;
      //
      //	We require an exact match for precs. Clearly we couldn't use a
      //	memo item with a lower prec since our rule simply doesn't satisfy it.
      //	Less obviously, using a memo item with a higher prec means we
      //	could lose behavior available with the lower prec that isn't
      //	captured by the memo item.
      //
      if (m.nonTerminal == nonTerminal && m.maxPrec == prec)
	{
	  ParserLog("vvvvvvvvvvvvvvvvvvvvvvvvvv" <<
		    "\n   Using memo item in set " << startTokenNr <<
		    " when looking for nonterminal " << nonTerminal << " at prec " << prec <<
		    " we saw a DRP that ends at rule " << m.ruleNr << " to set " << m.startTokenNr <<
		    "\n^^^^^^^^^^^^^^^^^^^^^");
	  return IntPair(m.ruleNr, m.startTokenNr);
	}
      /* perhaps user could be the same
      Assert(m.nonTerminal != nonTerminal || m.maxPrec < prec,
	     "startTokenNr = " << startTokenNr <<
	     " memo item for " << nonTerminal <<
	     " with prec  " << m.maxPrec <<
	     " should not exist since we have a rule returning prec " << prec <<
	     " which must have some other user, making the memo item not a DRP");
      */
    }
	
  for (int j = firstCalls[startTokenNr]; j != NONE;)
    {
      Call& call = calls[j];
      int nonTerminal2 = call.nonTerminal;
      int maxPrec = call.maxPrec;
      j = call.nextCall;
      if (nonTerminal2 == nonTerminal)
	{
	  if (maxPrec == UNBOUNDED)
	    {
	      ParserLog("----> end of DRP seen because of root object is a caller");
	      return IntPair(ruleNr, startTokenNr);
	    }
	  //
	  //	Handle explicit continuations.
	  //
	  for (int k = call.firstContinuation; k != NONE;)
	    {
	      Continuation& cont = continuations[k];
	      int ruleNr2 = cont.ruleNr;
	      int pos = cont.rhsPosition;
	      k = cont.nextContinuation;
	      Rule& rule2 = rules[ruleNr2];
	      if (rule2.rhs[pos].prec >= prec)
		{
		  //
		  //	Found a continutation.
		  //
		  if (nextRuleNr != NONE)
		    {
		      ParserLog("----> end of DRP seen because of multiple continuations");
		      return IntPair(ruleNr, startTokenNr);
		    }
		  int rhsRemaining = rule2.rhs.length() - (pos + 1);
		  if (rhsRemaining != 0)
		    {
		      ParserLog("----> end of DRP seen because of non return event");
		      return IntPair(ruleNr, startTokenNr);
		    }
		  //
		  //	It generates a return that potentially continues the DRP.
		  //
		  nextRuleNr = ruleNr2;
		  nextStartTokenNr = cont.startTokenNr;
		}
	    }
	}
      //
      //	Check for implied start ups.
      //
      int r = nonTerminalDecisionTrees[flip(nonTerminal2)];
      while (r != NONE)
	{
	  Rule& rule2 = rules[r];
	  int t = nonTerminal - rule2.rhs[0].symbol;
	  if (t == 0)
	    break;
	  r = (t > 0) ? rule2.bigger : rule2.smaller;
	}

      while (r != NONE)
	{
	  Rule& rule2 = rules[r];
	  if (rule2.prec > maxPrec)
	    break;
	  if (rule2.rhs[0].prec >= prec)
	    {
	      //
	      //	Found a continutation.
	      //
	      
	      if (nextRuleNr != NONE)
		{
		  ParserLog("----> end of DRP seen because of multiple continuations - implied start up");
		  return IntPair(ruleNr, startTokenNr);
		}
	      int rhsRemaining = rule2.rhs.length() - 1;
	      if (rhsRemaining != 0)
		{
		  ParserLog("----> end of DRP seen because of non return event - implied start up");
		  return IntPair(ruleNr, startTokenNr);
		}
	      //
	      //	It generates a return that potentially continues the DRP.
	      //	Can only happen for a <nonterminal> ::= <nonterminal> production.
	      //
	      nextRuleNr = r;
	      nextStartTokenNr = startTokenNr;
	    }
	  r = rule2.equal;
	}
    }
  Assert(nextRuleNr != NONE, "didn't find caller for rule " << ruleNr << " at position " << startTokenNr);
  ParserLog("*********** continuing DRP chase with rule " << nextRuleNr << " at position " << nextStartTokenNr);

  IntPair topMost = chaseDeterministicReductionPath(nextRuleNr, nextStartTokenNr);
  firstMemoItems[startTokenNr] = makeMemoItem(nonTerminal,
					      prec,  // must be the same as we're looking for to be effective
					      topMost.first,
					      topMost.second,
					      firstMemoItems[startTokenNr]);
  ParserLog("################" <<
	    "\n   made memo item in set " << startTokenNr <<
	    " when looking for nonterminal " << nonTerminal << " at prec " << prec <<
	    " we saw a DRP that ends at rule " << topMost.first << " to set " << topMost.second <<
	    "\n###################");
  return topMost;
}

void
Parser::expandCalls(int tokenNr)
{
  int firstCall = firstCalls[tokenNr];
  for (int i = firstCall; i != NONE;)
    {
      Call& call = calls[i];
      int nonTerminal = call.nonTerminal;
      int maxPrec = call.maxPrec;
      i = call.nextCall;
      Vector<Expansion>& exs = expansions[flip(nonTerminal)];
      for (int j = exs.length() - 1; j >= 0; j--)
	{
	  Expansion& ex = exs[j];
	  if (ex.prec <= maxPrec)
	    {
	      int nrPairs = ex.expansion.length();
	      for (int k = 0; k < nrPairs; k++)
		{
		  int nt2 = ex.expansion[k].symbol;
		  int prec = ex.expansion[k].prec;
		  for (int l = firstCall; l != NONE;)
		    {
		      Call& call2 = calls[l];
		      if (call2.nonTerminal == nt2)
			{
			  if (call2.maxPrec < prec)
			    call2.maxPrec = prec;
			  goto nextPair;
			}
		      l = call2.nextCall;
		    }
		  {
		    int nrCalls = calls.length();
		    calls.expandBy(1);
		    Call& call2 = calls[nrCalls];
		    call2.nonTerminal = nt2;
		    call2.maxPrec = prec;
		    call2.firstContinuation = NONE;
		    call2.nextCall = firstCall;
		    firstCall = nrCalls;
		    ParserLog("created call for " << nt2 << " from " << nonTerminal);
		  }
		nextPair:
		  ;
		}
	      break;
	    }
	}
    }
  firstCalls[tokenNr] = firstCall;
}

void
Parser::scanCalls(int tokenNr, const Vector<int>& sentence)
{
  int currentToken = sentence[tokenNr];
  int nextTokenNr = tokenNr + 1;
  for (int i = firstCalls[tokenNr]; i != NONE;)
    {
      Call& call = calls[i];
      int maxPrec = call.maxPrec;
      i = call.nextCall;
      int r = terminalDecisionTrees[flip(call.nonTerminal)];
      while (r != NONE)
	{
	  Rule& rule = rules[r];
	  int t = currentToken - rule.rhs[0].symbol;
	  if (t == 0)
	    break;
	  r = (t > 0) ? rule.bigger : rule.smaller;
	}
      if (r != NONE)
	{
	  //
	  //	Implied match can move the badTokenIndex.
	  //
	  if (nextTokenNr > badTokenIndex)
	    badTokenIndex = nextTokenNr;
	}
      while (r != NONE)
	{
	  Rule& rule = rules[r];
	  if (rule.prec > maxPrec)
	    break;
	  advanceRule(r, 1, tokenNr, nextTokenNr, sentence);
	  r = rule.equal;
	}
    }
}

void
Parser::advanceRule(int ruleNr,
		    int pos,
		    int startTokenNr,
		    int tokenNr,
		    const Vector<int>& sentence)
{
  Rule& r = rules[ruleNr];
  int end = r.rhs.length();
  int nrTokens = sentence.size();
  Assert(tokenNr <= nrTokens, "tokenNr past last parse list");
  for (; pos < end; pos++)
    {
      int symbol = r.rhs[pos].symbol;
      if (symbol < 0)
	{
	  makeCall(tokenNr, ruleNr, pos, startTokenNr);
	  return;
	}
      if (tokenNr == nrTokens)
	{
	  //
	  //	Ran out of tokens.
	  //
	  return;
	}
      int token = sentence[tokenNr];
      if (symbol != token)
	return; // die
      ++tokenNr;
      //
      //	Successful match of token. All previous tokens must be part of at least
      //	one legal partial parse so check if we need to update badTokenIndex.
      //
      if (tokenNr > badTokenIndex)
	badTokenIndex = tokenNr;
    }
  makeReturn(tokenNr, ruleNr, startTokenNr);
}

void
Parser::makeCall(int tokenNr, int ruleNr, int rhsPosition, int startTokenNr)
{
  int nonTerminal = rules[ruleNr].rhs[rhsPosition].symbol;
  int prec = rules[ruleNr].rhs[rhsPosition].prec;
  for (int i = firstCalls[tokenNr]; i != NONE;)
    {
      Call& call = calls[i];
      i = call.nextCall;
      if (call.nonTerminal == nonTerminal)
	{
	  for (int j = call.firstContinuation; j != NONE;)
	    {
	      Continuation& cont = continuations[j];
	      if (cont.ruleNr == ruleNr && cont.rhsPosition == rhsPosition &&
		  cont.startTokenNr == startTokenNr)
		return;
	      j = cont.nextContinuation;
	    }
	  if (prec > call.maxPrec)
	    call.maxPrec = prec;
	  int nrContinuations = continuations.length();
	  continuations.expandBy(1);
	  Continuation& cont = continuations[nrContinuations];
	  cont.ruleNr = ruleNr;
	  cont.rhsPosition = rhsPosition;
	  cont.startTokenNr = startTokenNr;
	  cont.nextContinuation = call.firstContinuation;
	  call.firstContinuation = nrContinuations;
	  return;
	}
    }
  int nrContinuations = continuations.length();
  continuations.expandBy(1);
  Continuation& cont = continuations[nrContinuations];
  cont.ruleNr = ruleNr;
  cont.rhsPosition = rhsPosition;
  cont.startTokenNr = startTokenNr;
  cont.nextContinuation = NONE;

  int nrCalls = calls.length();
  calls.expandBy(1);
  Call& call = calls[nrCalls];
  call.nonTerminal = nonTerminal;
  call.maxPrec = prec;
  call.firstContinuation = nrContinuations;
  call.nextCall = firstCalls[tokenNr];
  firstCalls[tokenNr] = nrCalls;
}

bool
Parser::makeReturn(int tokenNr, int ruleNr, int startTokenNr)
{
  int prev = NONE;
  for (int i = firstReturns[tokenNr]; i != NONE;)
    {
      Return& ret = returns[i];
      if (ret.ruleNr == ruleNr && ret.startTokenNr == startTokenNr)
	return false;
      prev = i;
      i = ret.nextReturn;
    }
  ParserLog("makeReturn() << " << tokenNr << "  " << ruleNr << "  " << startTokenNr);
  int nrReturns = returns.length();
  returns.expandBy(1);
  Return& ret = returns[nrReturns];
  ret.ruleNr = ruleNr;
  ret.startTokenNr = startTokenNr;
  ret.nextReturn = NONE;
  if (prev == NONE)
    firstReturns[tokenNr] = nrReturns;
  else
    returns[prev].nextReturn = nrReturns;
  return true;
}

int
Parser::makeMemoItem(int nonTerminal, int maxPrec, int ruleNr, int startTokenNr, int nextMemoItem)
{
  int nrMemoItems = memoItems.size();
  memoItems.expandBy(1);
  MemoItem& m = memoItems[nrMemoItems];
  m.nonTerminal = nonTerminal;
  m.maxPrec = maxPrec;
  m.ruleNr = ruleNr;
  m.startTokenNr = startTokenNr;
  m.nextMemoItem = nextMemoItem;
  return nrMemoItems;
}

//
//	Compile productions rules into tables and ternary trees.
//

void
Parser::buildTerminalDecisionTrees()
{
  Vector<Rule*> ruleTable;
  Vector<int> starts;
  int nrNonTerminals = firstTerminalRules.length();
  terminalDecisionTrees.expandTo(nrNonTerminals);
  for (int i = 0; i < nrNonTerminals; i++)
    {
      //
      //	Collect all the productions that have i as the nonterminal.
      //
      ruleTable.contractTo(0);
      for (int r = firstTerminalRules[i]; r != NONE; r = rules[r].nextRule)
	ruleTable.append(&(rules[r]));
      //
      //	Sort them by rhs start symbol, then prec, smallest first.
      //
      sort(ruleTable.begin(), ruleTable.end(), ruleLt);
      //
      //	Make a list of the index in ruleTable of the first production
      //	for each rhs start symbol.
      //
      int nrRules = ruleTable.length();
      int lastSymbol = UNBOUNDED;  // bigger than any terminal symbol
      starts.contractTo(0);
      for (int j = 0; j < nrRules; j++)
	{
	  int startSymbol = ruleTable[j]->rhs[0].symbol;
	  if (startSymbol == BUBBLE_TERMINAL)
	    {
	      //
	      //	We only have bubble productions left. The should be at most one.
	      //
	      /*
	      Assert(j + 1 == nrRules, "two bubble terminal rules see for the same nonterminal");
	      bubbleRule[i] = ruleTable[j]->index;
	      */
	      ruleTable.contractTo(j);  // don't put bubble production in the tree
	      break;
	    }
	  if (startSymbol != lastSymbol)
	    {
	      lastSymbol = ruleTable[j]->rhs[0].symbol;
	      starts.append(j);
	    }
	}
      //
      //	Make a decision tree by divid-and-conqueor.
      //
      terminalDecisionTrees[i] = buildDecisionTree(ruleTable, starts, 0, starts.length() - 1);
    }
}

void
Parser::buildNonTerminalDecisionTrees()
{
  Vector<Rule*> ruleTable;
  Vector<int> starts;
  int nrNonTerminals = firstTerminalRules.length();
  nonTerminalDecisionTrees.expandTo(nrNonTerminals);
  for (int i = 0; i < nrNonTerminals; i++)
    {
      ruleTable.contractTo(0);
      for (int r = firstNonTerminalRules[i]; r != NONE; r = rules[r].nextRule)
	ruleTable.append(&(rules[r]));

      sort(ruleTable.begin(), ruleTable.end(), ruleLt);
      //ruleTable.sort(ruleCompare);
      int nrRules = ruleTable.length();
      int lastSymbol = UNBOUNDED;
      starts.contractTo(0);
      for (int j = 0; j < nrRules; j++)
	{
	  if (ruleTable[j]->rhs[0].symbol != lastSymbol)
	    {
	      lastSymbol = ruleTable[j]->rhs[0].symbol;
	      starts.append(j);
	    }
	}
      nonTerminalDecisionTrees[i] = buildDecisionTree(ruleTable, starts, 0, starts.length() - 1);
    }
}

int
Parser::buildDecisionTree(Vector<Rule*>& ruleTable, Vector<int>& starts, int first, int last)
{
  if (first > last)
    return NONE;
  int i = (first + last) / 2;
  int j = starts[i]; 
  ruleTable[j]->smaller = buildDecisionTree(ruleTable, starts, first, i - 1);
  int k = j;
  int end = ruleTable.length() - 1;
  for (; k < end; k++)
    {
      if (ruleTable[k]->rhs[0].symbol != ruleTable[k + 1]->rhs[0].symbol)
	break;
      ruleTable[k]->equal = ruleTable[k + 1]->index;
    }
  ruleTable[k]->equal = NONE;
  ruleTable[j]->bigger = buildDecisionTree(ruleTable, starts, i + 1, last);
  return ruleTable[j]->index;
}

void
Parser::buildExpansionTables()
{
  int nrNonTerminals = firstTerminalRules.length();
  expansions.expandTo(nrNonTerminals);
  for (int i = 0; i < nrNonTerminals; i++)
    buildExpansionTable(i);
}

void
Parser::buildExpansionTable(int i)
{
  int lastPrec = -1;
  for(;;)
    {
      int prec = UNBOUNDED;
      for (int r = firstNonTerminalRules[i]; r != NONE; r = rules[r].nextRule)
	{
	  int p = rules[r].prec;
	  if (p > lastPrec && p < prec)
	    prec = p;
	}
      if (prec == UNBOUNDED)
	return;
      buildExpansion(i, prec);
      lastPrec = prec;
    }
}

void
Parser::buildExpansion(int i, int prec)
{
  Vector<Expansion>& exTable = expansions[i];
  int nrEx = exTable.length();
  exTable.expandBy(1);
  Expansion& ex = exTable[nrEx];
  ex.prec = prec;
  ex.expansion.expandTo(1);
  ex.expansion[0].symbol = flip(i);
  ex.expansion[0].prec = prec;
  bool expanded;
  do
    {
      expanded = false;
      int nrPairs = ex.expansion.length();
      for (int i = 0; i < nrPairs; i++)
	{
	  int nonTerminal = ex.expansion[i].symbol;
	  int prec3 = ex.expansion[i].prec;
	  for (int r = firstNonTerminalRules[flip(nonTerminal)]; r != NONE; r = rules[r].nextRule)
	    {
	      if (rules[r].prec <= prec3)
		{
		  int nt2 = rules[r].rhs[0].symbol;
		  int prec2 = rules[r].rhs[0].prec;
		  int nrPairs2 = ex.expansion.length();
		  for (int j = 0; j < nrPairs2; j++)
		    {
		      if (ex.expansion[j].symbol == nt2)
			{
			  if (ex.expansion[j].prec < prec2)
			    {
			      ex.expansion[j].prec = prec2;
			      expanded = true;
			    }
			  goto nextRule;
			}
		    }
		  ParserLog("expanding " << nonTerminal << " to " << nt2 << " using rule " << r);
		  ex.expansion.expandBy(1);
		  ex.expansion[nrPairs2].symbol = nt2;
		  ex.expansion[nrPairs2].prec = prec2;
		  expanded = true;
		}
	    nextRule:
	      ;
	    }
	} 
    }
  while (expanded);
}

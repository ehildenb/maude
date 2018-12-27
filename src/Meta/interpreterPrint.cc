
//
//	Code for printTerm() and parseQidList() messages.
//

bool
InterpreterManagerSymbol::printTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int printFlags;
  if (metaLevel->downPrintOptionSet(message->getArgument(4), printFlags))
    {
      Interpreter* interpreter;
      if (getInterpreter(message->getArgument(0), interpreter))
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(2), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      if (Term* t = metaLevel->downTerm(message->getArgument(3), m))
			{
			  Vector<int> buffer;
			  m->bufferPrint(buffer, t, printFlags);
			  t->deepSelfDestruct();

			  Vector<DagNode*> reply(3);
			  DagNode* target = message->getArgument(1);
			  reply[0] = target;
			  reply[1] = message->getArgument(0);
			  reply[2] = metaLevel->upQidList(buffer);
			  context.bufferMessage(target, printedTermMsg->makeDagNode(reply));
			  return true;
			}
		    }
		}
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::parseQidList(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      int id;
      if (metaLevel->downQid(message->getArgument(2), id))
	{
	  if (PreModule* pm = interpreter->getModule(id))
	    {
	      if (ImportModule* m = pm->getFlatModule())
		{
		  ConnectedComponent* component;
		  Vector<int> metaTokens;
		  if (metaLevel->downComponent(message->getArgument(4), m, component) &&
		      metaLevel->downQidList(message->getArgument(3), metaTokens))
		    {
		      //
		      //	Turn meta-tokens into tokens.
		      //
		      int nrTokens = metaTokens.length();
		      Vector<Token> tokens(nrTokens);
		      for (int i = 0; i < nrTokens; i++)
			tokens[i].tokenize(metaTokens[i], FileTable::META_LEVEL_CREATED);
		      //
		      //	We got this far so we are going to produce a reply,
		      //	even if it's a parse error.
		      //
		      Vector<DagNode*> reply(3);
		      DagNode* target = message->getArgument(1);
		      reply[0] = target;
		      reply[1] = message->getArgument(0);
		      //
		      //	Do the parse.
		      //
		      Term* parse1;
		      Term* parse2;
		      int firstBad;
		      switch(m->parseTerm2(tokens, component, parse1, parse2, firstBad))
			{
			case -1:	// bad token
			case 0:		// no parse
			  {
			    reply[2] = metaLevel->upNoParse(firstBad);
			    break;
			  }
			case 1:		// 1 parse
			  {
			    parse1->symbol()->fillInSortInfo(parse1);
			    reply[2] = metaLevel->upResultPair(parse1, m);
			    parse1->deepSelfDestruct();
			    break;
			  }
			default:	// more than 1 parse
			  {
			    parse1->symbol()->fillInSortInfo(parse1);
			    parse2->symbol()->fillInSortInfo(parse2);
			    reply[2] = metaLevel->upAmbiguity(parse1, parse2, m);
			    parse1->deepSelfDestruct();
			    parse2->deepSelfDestruct();
			  }
			}
		      context.bufferMessage(target, parsedQidListMsg->makeDagNode(reply));
		      return true;
		    }
		}
	    }
	}
    }
  return false;
}

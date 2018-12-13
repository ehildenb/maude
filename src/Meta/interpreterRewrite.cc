/*

    This file is part of the Maude 2 interpreter.

    Copyright 2018 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for rewriteTerm()/frewriteTerm()/erewrite() messages.
//

bool
InterpreterManagerSymbol::reduceTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
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
		       t = t->normalize(false);
		       DagNode* d = term2Dag(t);
		       t->deepSelfDestruct();
		       RewritingContext* objectContext =
			 context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
		       m->protect();
		       objectContext->reduce();
		       context.addInCount(*objectContext);

		       Vector<DagNode*> reply(5);
		       DagNode* target = message->getArgument(1);
		       reply[0] = target;
		       reply[1] = message->getArgument(0);
		       reply[2] = upRewriteCount(objectContext);

		       PointerMap qidMap;
		       PointerMap dagNodeMap;
		       DagNode* dagNode = objectContext->root();
		       reply[3] = metaLevel->upDagNode(dagNode, m, qidMap, dagNodeMap);
		       reply[4] = metaLevel->upType(dagNode->getSort(), qidMap);
		       context.bufferMessage(target, reducedTermMsg->makeDagNode(reply));

		       delete objectContext;
		       (void) m->unprotect();
		       return true;
		     }
		}
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::rewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      Int64 limit;
      if (metaLevel->downBound64(message->getArgument(2), limit))
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(3), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      if (Term* t = metaLevel->downTerm(message->getArgument(4), m))
			{
			  t = t->normalize(false);
			  DagNode* d = term2Dag(t);
			  t->deepSelfDestruct();
			  RewritingContext* objectContext =
			    context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
			  m->protect();
			  m->resetRules();
			  objectContext->ruleRewrite(limit);
			  context.addInCount(*objectContext);

			  Vector<DagNode*> reply(5);
			  DagNode* target = message->getArgument(1);
			  reply[0] = target;
			  reply[1] = message->getArgument(0);
			  reply[2] = upRewriteCount(objectContext);

			  PointerMap qidMap;
			  PointerMap dagNodeMap;
			  DagNode* dagNode = objectContext->root();
			  reply[3] = metaLevel->upDagNode(dagNode, m, qidMap, dagNodeMap);
			  reply[4] = metaLevel->upType(dagNode->getSort(), qidMap);
			  context.bufferMessage(target, rewroteTermMsg->makeDagNode(reply));
			  //
			  //	We should somehow save state for continuation.
			  //
			  delete objectContext;
			  (void) m->unprotect();
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
InterpreterManagerSymbol::frewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      Int64 limit;
      Int64 gas;
      if (metaLevel->downBound64(message->getArgument(2), limit) && limit != 0 &&
	  metaLevel->downSaturate64(message->getArgument(3), gas) && gas != 0)
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(4), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      if (Term* t = metaLevel->downTerm(message->getArgument(5), m))
			{
			  t = t->normalize(false);
			  DagNode* d = term2Dag(t);
			  t->deepSelfDestruct();
			  RewritingContext* objectContext =
			    context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
			  m->protect();
			  m->resetRules();
			  objectContext->fairRewrite(limit, gas);
			  objectContext->root()->computeTrueSort(*objectContext);  // needed so we have well defined sorts
			  context.addInCount(*objectContext);

			  Vector<DagNode*> reply(5);
			  DagNode* target = message->getArgument(1);
			  reply[0] = target;
			  reply[1] = message->getArgument(0);
			  reply[2] = upRewriteCount(objectContext);

			  PointerMap qidMap;
			  PointerMap dagNodeMap;
			  DagNode* dagNode = objectContext->root();
			  reply[3] = metaLevel->upDagNode(dagNode, m, qidMap, dagNodeMap);
			  reply[4] = metaLevel->upType(dagNode->getSort(), qidMap);
			  context.bufferMessage(target, frewroteTermMsg->makeDagNode(reply));
			  //
			  //	We should somehow save state for continuation.
			  //
			  delete objectContext;
			  (void) m->unprotect();
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
InterpreterManagerSymbol::erewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      Int64 limit;
      Int64 gas;
      if (metaLevel->downBound64(message->getArgument(2), limit) && limit != 0 &&
	  metaLevel->downSaturate64(message->getArgument(3), gas) && gas != 0)
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(4), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      if (Term* t = metaLevel->downTerm(message->getArgument(5), m))
			{
			  t = t->normalize(false);
			  DagNode* d = term2Dag(t);
			  t->deepSelfDestruct();
			  RewritingContext* subContext = context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
			  ObjectSystemRewritingContext* objectContext = safeCast(ObjectSystemRewritingContext*, subContext);
			  DebugAdvisory("created context " << (void*) objectContext);
			  objectContext->setObjectMode(ObjectSystemRewritingContext::EXTERNAL);
			  m->protect();
			  m->resetRules();
			  objectContext->fairStart(limit, gas);
			  objectContext->externalRewrite();
			  objectContext->root()->computeTrueSort(*objectContext);  // needed so we have well defined sorts
			  context.addInCount(*objectContext);

			  Vector<DagNode*> reply(5);
			  DagNode* target = message->getArgument(1);
			  reply[0] = target;
			  reply[1] = message->getArgument(0);
			  reply[2] = upRewriteCount(objectContext);

			  PointerMap qidMap;
			  PointerMap dagNodeMap;
			  DagNode* dagNode = objectContext->root();
			  reply[3] = metaLevel->upDagNode(dagNode, m, qidMap, dagNodeMap);
			  reply[4] = metaLevel->upType(dagNode->getSort(), qidMap);
			  context.bufferMessage(target, erewroteTermMsg->makeDagNode(reply));
			  //
			  //	We should somehow save state for continuation.
			  //
			  delete objectContext;
			  DebugAdvisory("deleted context " << (void*) objectContext);
			  (void) m->unprotect();
			  return true;
			}
		    }
		}
	    }
	}
    }
  return false;
}

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

View*
MetaLevel::downView(DagNode* metaView, Interpreter* owner)
{
  if (metaView->symbol() == viewSymbol)
    {
      //
      //	op view_from_to_is__endv : Header ModuleExpression ModuleExpression
      //	SortMappingSet OpMappingSet -> View .
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaView);
      int id;
      DagNode* metaParameterDeclList;
      if (downHeader(f->getArgument(0), id, metaParameterDeclList))
	{
	  if (metaParameterDeclList == 0)  // don't support parameterized views yet
	    {
	      if (ModuleExpression* fromTheory = downModuleExpression(f->getArgument(1)))
		{
		  if (ModuleExpression* toModule = downModuleExpression(f->getArgument(2)))
		    {
		      DagNode* metaOpMappings = f->getArgument(4);
		      Token name;
		      name.tokenize(id, FileTable::META_LEVEL_CREATED);
		      View* view = new MetaView(name, metaOpMappings, this, owner);
		      view->addFrom(fromTheory);
		      view->addTo(toModule);
		      if (downSortMappingSet(f->getArgument(3), view) &&
			  downOpMappingSet(metaOpMappings, view))
			return view;
		      delete view;
		    }
		  fromTheory->deepSelfDestruct();
		}
	    }
	}
    }
  return 0;
}

bool
MetaLevel::downSortMappingSet(DagNode* metaSortMappings, View* view)
{
  Symbol* ms = metaSortMappings->symbol();
  if (ms == sortMappingSetSymbol)
    {
      for (DagArgumentIterator i(metaSortMappings); i.valid(); i.next())
	{
	  if (!downSortMapping(i.argument(), view))
	    return false;
	}
    }
  else if (ms != emptySortMappingSetSymbol)
    return downSortMapping(metaSortMappings, view);
  return true;
}

bool
MetaLevel::downSortMapping(DagNode* metaSortMapping, View* view)
{
  Symbol* ms = metaSortMapping->symbol();
  if (ms == sortMappingSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaSortMapping);
      Token from;
      Token to;
      if (downToken(f->getArgument(0), from) && downToken(f->getArgument(1), to))
	{
	  view->addSortMapping(from, to);
	  return true;
	}
    }
  return false;
}

bool
MetaLevel::downOpMappingSet(DagNode* metaOpMappings, View* view)
{
  Symbol* mo = metaOpMappings->symbol();
  if (mo == opMappingSetSymbol)
    {
      for (DagArgumentIterator i(metaOpMappings); i.valid(); i.next())
	{
	  if (!downOpMapping(i.argument(), view))
	    return false;
	}
    }
  else if (mo != emptyOpMappingSetSymbol)
    return downOpMapping(metaOpMappings, view);
  return true;
}

bool
MetaLevel::downOpMapping(DagNode* metaOpMapping, View* view)
{
  Symbol* mo = metaOpMapping->symbol();
  if (mo == opMappingSymbol)
    {
      //
      //	  op (op_to_.) : Qid Qid -> OpMapping [ctor] .
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaOpMapping);
      Vector<Token> from(1);
      Vector<Token> to(1);
      if (downToken(f->getArgument(0), from[0]) && downToken(f->getArgument(1), to[0]))
	{
	  view->addOpMapping(from);
	  view->addOpTarget(to);
	  return true;
	}
    }
  else if (mo == opSpecificMappingSymbol)
    {
      //
      //	op (op_:_->_to_.) : Qid TypeList Type Qid -> OpMapping [ctor] .
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaOpMapping);
      Vector<Token> from(1);
      Vector<Token> to(1);
      if (downToken(f->getArgument(0), from[0]) && downToken(f->getArgument(3), to[0]))
	{
	  view->addOpMapping(from);
	  view->addOpTarget(to);
	  return downRenamingTypes(f->getArgument(1), view) &&
	    downRenamingType(f->getArgument(2), view);
	}
    }
  else if (mo == opTermMappingSymbol)
    return true;  // handle these in MetaView
  return false;
}

bool
MetaLevel::downOpTermMappings(DagNode* metaOpMappings,
			      MixfixModule* fromTheory,
			      MixfixModule* toModule,
			      Vector<Term*>& fromTerms,
			      Vector<Term*>& toTerms)
{
  Symbol* mo = metaOpMappings->symbol();
  if (mo == opMappingSetSymbol)
    {
      for (DagArgumentIterator i(metaOpMappings); i.valid(); i.next())
	{
	  if (!downOpTermMapping(i.argument(), fromTheory, toModule, fromTerms, toTerms))
	    return false;
	}
    }
  else if (mo != emptyOpMappingSetSymbol)
    return downOpTermMapping(metaOpMappings, fromTheory, toModule, fromTerms, toTerms);
  return true;
}

bool
MetaLevel::downOpTermMapping(DagNode* metaOpMapping,
			     MixfixModule* fromTheory,
			     MixfixModule* toModule,
			     Vector<Term*>& fromTerms,
			     Vector<Term*>& toTerms)
{
  if (metaOpMapping->symbol() == opTermMappingSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaOpMapping);
      if (Term* lhs = downTerm(f->getArgument(0), fromTheory))
	{
	  if (Term* rhs = downTerm(f->getArgument(1), toModule))
	    {
	      fromTerms.append(lhs);
	      toTerms.append(rhs);
	      return true;
	    }
	  lhs->deepSelfDestruct();
	}
      return false;
    }
  //
  //	Because it is not an op->term mapping We assume that metaOpMapping was
  //	previously processed without issue so we don't need to check it here.
  //
  return true;
}

ModuleExpression*
MetaLevel::downModuleExpression(DagNode* metaExpr)
{
  //
  //	Named module.
  //
  Token moduleName;
  if (downToken(metaExpr, moduleName))
    return new ModuleExpression(moduleName);

  Symbol* me = metaExpr->symbol();
  if (me == sumSymbol)
    {
      //
      //	<ModExpr> + ... + <ModExpr>
      //
      ModuleExpression* sum = 0;
      for (DagArgumentIterator i(metaExpr); i.valid(); i.next())
	{
	  if (ModuleExpression* summand = downModuleExpression(i.argument()))
	    sum = (sum == 0) ? summand : new ModuleExpression(sum, summand);
	  else
	    {
	      if (sum != 0)
		sum->deepSelfDestruct();
	      return 0;
	    }
	}
      return sum;
    }
  if (me == renamingSymbol)
    {
      //
      //	<ModExpr> * <Renaming>
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaExpr);
      Renaming* renaming = new Renaming();
      if (downRenamings(f->getArgument(1), renaming))
	{
	  if (ModuleExpression* renamed = downModuleExpression(f->getArgument(0)))
	    return new ModuleExpression(renamed, renaming);
	}
      delete renaming;
      return 0;
    }
  if (me == instantiationSymbol)
    {
      //
      //	Module instantiation.
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaExpr);
      Vector<Token> arguments;
      if (downInstantiationArguments(f->getArgument(1), arguments))
	{
	  if (ModuleExpression* instantiated = downModuleExpression(f->getArgument(0)))
	    return new ModuleExpression(instantiated, arguments);
	}
    }
  return 0;
}

bool
MetaLevel::downInstantiationArguments(DagNode* metaArguments, Vector<Token>& arguments)
{
  Assert(arguments.empty(), "nonempty arg list");
  if (metaArguments->symbol() == metaArgSymbol)
    {
      for (DagArgumentIterator i(metaArguments); i.valid(); i.next())
	{
	  int nrArguments = arguments.size();
	  arguments.resize(nrArguments + 1);
	  if (!downToken(i.argument(), arguments[nrArguments]))
	    return false;
	}
    }
  else
    {
      int nrArguments = arguments.size();
      arguments.resize(nrArguments + 1);
      if (!downToken(metaArguments, arguments[nrArguments]))
	return false;
    }
  return true;
}

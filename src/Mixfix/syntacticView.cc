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
//      Implementation for class SyntacticView.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "core.hh"
#include "interface.hh"
#include "variable.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "argumentIterator.hh"

//      variable class definitions
#include "variableTerm.hh"

//	front end class definitions
#include "token.hh"
#include "moduleExpression.hh"
#include "interpreter.hh"
#include "syntacticView.hh"

SyntacticView::SyntacticView(Token viewName, Interpreter* owner)
  : View(viewName, owner)
{
  newFromTheory = 0;
  newToModule = 0;
}

SyntacticView::~SyntacticView()
{
  if (newFromTheory != 0)
    newFromTheory->deepSelfDestruct();
  if (newToModule != 0)
    newToModule->deepSelfDestruct();
}

void
SyntacticView::addParameter2(Token name, ModuleExpression* theory)
{
  IssueAdvisory("parameterized views not yet supported");
}

void
SyntacticView::addVarDecl(Token varName)
{
  varDecls.push_back(VarDecl());
  varDecls.back().varName = varName;
  varDecls.back().lastWithCurrentDef = false;
}

void
SyntacticView::addType(bool kind, const Vector<Token>& tokens)
{
  if (varDecls.empty() || varDecls.back().lastWithCurrentDef)
    Renaming::addType(kind, tokens);  // not ours
  else
    {
      varDecls.back().lastWithCurrentDef = true;
      varDefs.push_back(Type());
      varDefs.back().kind = kind;
      varDefs.back().tokens = tokens;
    }
}

void
SyntacticView::addOpTermMapping(const Vector<Token>& fromOp, const Vector<Token>& toTerm)
{
  opTermList.push_back(BubblePair());
  opTermList.back().fromBubble = fromOp;  // deep copy
  opTermList.back().toBubble = toTerm;  // deep copy
}

void
SyntacticView::regretToInform(Entity* doomedEntity)
{
  if (doomedEntity == newFromTheory)
    {
      newFromTheory = 0;
      //
      //	The assumption is the the guts of the view will be invalidated
      //	and attempts to use the view will result in reevaluation which
      //	will replace newToModule. If we don't deal with any existing
      //	newToModule, then when the old newToModule gets deleted it
      //	will send us an unexpected regretToInfom().
      //
      if (newToModule != 0)
	newToModule->deepSelfDestruct();
    }
  else if (doomedEntity == newToModule)
    {
      newToModule = 0;
      //
      //	Same argument as above, but for newFromTheory.
      //
      if (newFromTheory != 0)
	newFromTheory->deepSelfDestruct();
    }
  else
    View::regretToInform(doomedEntity);
}

void
SyntacticView::finishModule(ImportModule* module)
{
  module->importSorts();
  module->closeSortSet();
  Assert(!(module->isBad()), "copy of a non-bad theory bad");
  module->importOps();
  Assert(!(module->isBad()), "copy of a non-bad theory bad");
  module->closeSignature();
  module->fixUpImportedOps();
  Assert(!(module->isBad()), "copy of a non-bad theory bad");
  module->closeFixUps();
  module->localStatementsComplete();
  module->resetImports();
}

bool
SyntacticView::handleVarDecls()
{
  Sort* fromSort = 0;
  Sort* toSort = 0;
  TypeList::const_iterator j = varDefs.begin();
  FOR_EACH_CONST(i, VarDeclList, varDecls)
    {
      if (fromSort == 0)
	{
	  int code = j->tokens[0].code();
	  fromSort = newFromTheory->findSort(code);
	  if (fromSort == 0)
	    {
	      IssueWarning(LineNumber(j->tokens[0].lineNumber()) <<
			   ": failed to find sort " << QUOTE(Token::sortName(code)) <<
			   " in " << QUOTE(newFromTheory) << '.');
	      return false;
	    }
	  toSort = newToModule->findSort(renameSort(code));
	  Assert(toSort != 0, "couldn't find translation of sort");
	  if (j->kind)
	    {
	      int nrTokens = j->tokens.size();
	      for (int k = 1; k < nrTokens; ++k)
		{
		  int code = j->tokens[k].code();
		  Sort* extraSort = newFromTheory->findSort(code);
		  if (extraSort == 0)
		    {
		      IssueWarning(LineNumber(j->tokens[k].lineNumber()) <<
				   ": failed to find sort " << QUOTE(Token::sortName(code)) <<
				   " in " << QUOTE(newFromTheory) << '.');
		      return false;
		    }
		  if (extraSort->component() != fromSort->component())
		    {
		      IssueWarning(LineNumber(j->tokens[k].lineNumber()) <<
				   ": sorts " << QUOTE(fromSort) << " and " <<
				   QUOTE(extraSort) << " are in different components.");
		      return false;
		    }
		}
	      fromSort = fromSort->component()->sort(Sort::KIND);
	      toSort = toSort->component()->sort(Sort::KIND);
	    }
	  ++j;
	}
      //
      //	We can add variable aliases to a module at any point before
      //	we first do parsing in the module.
      //
      newFromTheory->addVariableAlias(i->varName, fromSort);
      newToModule->addVariableAlias(i->varName, toSort);
      if (i->lastWithCurrentDef)
	fromSort = 0;
    }
  return true;
}

bool
SyntacticView::handleOpTermMappings()
{
  if (opTermList.empty())
    return true;  // nothing to do

  LineNumber lineNumber(FileTable::AUTOMATIC);
  //
  //	Because we have op->term mappings we need to make new modules to parse
  //	these mappings in, in order to exclude any existing variable aliases and
  //	add any new ones.
  //
  ImportModule* fromTheory = getFromTheory();
  newFromTheory = new ImportModule(fromTheory->id(),
				   fromTheory->getModuleType(),
				   ImportModule::VIEW_LOCAL,
				   this);
  newFromTheory->addImport(fromTheory, ImportModule::INCLUDING, lineNumber);
  finishModule(newFromTheory);

  ImportModule* toModule = getToModule();
  newToModule = new ImportModule(toModule->id(),
				 toModule->getModuleType(),
				 ImportModule::VIEW_LOCAL,
				 this);
  newToModule->addImport(toModule, ImportModule::INCLUDING, lineNumber);
  finishModule(newToModule);

  if (!varDecls.empty() && !handleVarDecls())
      return false;
  //
  //	Now deal with op->term mappings.
  //
  FOR_EACH_CONST(i, OpTermList, opTermList)
    {
      Term* fromTerm = newFromTheory->parseTerm(i->fromBubble);
      if (fromTerm == 0)
	return false;
      Symbol* fromSymbol = fromTerm->symbol();
      Term* toTerm = newToModule->parseTerm(i->toBubble, mapComponent(fromSymbol->rangeComponent(), newToModule), 1);
      if (toTerm == 0)
	{
	  fromTerm->deepSelfDestruct();
	  return false;
	}
      insertOpToTermMapping(i->fromBubble[0].lineNumber(),
			    fromTerm,
			    i->toBubble[1].lineNumber(),
			    toTerm,
			    newToModule);
    }
  //
  //	Dispense with bulky parsers.
  //
  newFromTheory->economize();
  newToModule->economize();
  return true;
}

void
SyntacticView::showView(ostream& s)
{
  s << "view " << static_cast<NamedEntity*>(this) << " from " <<
    getFrom() << " to " << getTo() << " is\n";
  printRenaming(s, "  ", " .\n  ");
  if (getNrSortMappings() > 0 || getNrOpMappings() > 0)
    s << " .\n";
  if (!varDecls.empty())
    {
      bool startNew = true;
      TypeList::const_iterator j = varDefs.begin();
      FOR_EACH_CONST(i, VarDeclList, varDecls)
	{
	  if (startNew)
	    {
	      s << "  var";
	      if (!(i->lastWithCurrentDef))
		s << 's';
	      startNew = false;
	    }
	  s << ' ' << i->varName;
	  if (i->lastWithCurrentDef)
	    {
	      s << " : " << *j << " .\n";
	      ++j;
	      startNew = true;
	    }
	}
    }
  
  const OpTermMap& opTermMap = getOpTermMap();
  FOR_EACH_CONST(i, OpTermMap, opTermMap)
    s << "  op " << i->second.first << " to term " << i->second.second << " .\n";
  s << "endv\n";
}

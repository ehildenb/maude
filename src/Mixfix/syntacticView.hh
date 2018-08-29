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
//      Class for views constructed from syntax; i.e. object level views.
//
#ifndef _syntacticView_hh_
#define _syntacticview_hh_
#include <list>
#include "view.hh"

class SyntacticView : public View
{
public:
  SyntacticView(Token viewName, Interpreter* owner);
  ~SyntacticView();
  //
  //	We need to deal with variable declarations and op->term mappings.
  //
  void addVarDecl(Token varName);
  void addType(bool kind, const Vector<Token>& tokens);
  void addOpTermMapping(const Vector<Token>& fromOp, const Vector<Token>& toTerm);
  //
  //	Show view command only makes sense for object level views.
  //
  void showView(ostream& s);
  
private:

  struct VarDecl
  {
    Token varName;
    bool lastWithCurrentDef;
  };

  struct BubblePair
  {
    Vector<Token> fromBubble;
    Vector<Token> toBubble;
  };

  typedef list<VarDecl> VarDeclList;
  typedef list<Type> TypeList;
  typedef list<BubblePair> OpTermList;

  //
  //	Override virtual functions from view.
  //
  void regretToInform(Entity* doomedEntity);
  bool handleOpTermMappings();
  //
  //	Internal functions.
  //
  void finishModule(ImportModule* module);
  bool handleVarDecls();

  ImportModule* newFromTheory;	// copy of fromTheory with different variable aliases
  ImportModule* newToModule;	// copy of toModule with different variable aliases

  VarDeclList varDecls;		// list of variable aliases declared
  TypeList varDefs;		// list of sorts used in variable declarations
  OpTermList opTermList;	// list of op->term mappings stored as unparsed bubble pairs
};

#endif

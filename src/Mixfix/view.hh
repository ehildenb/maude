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
//      Class for views.
//
#ifndef _view_hh_
#define _view_hh_
#include "namedEntity.hh"
#include "lineNumber.hh"
#include "entity.hh"
#include "renaming.hh"

class View
  : public NamedEntity,
    public LineNumber,
    public Entity,
    public Entity::User,
    public Renaming
{
  NO_COPYING(View);

public:
  enum ReturnValues
  {
    OP_TERM_MAPPING = -2
  };

  typedef multimap<int, pair<Term*, Term*> > OpTermMap;

  View(Token viewName, Interpreter* owner);
  ~View();

  void addFrom(ModuleExpression* expr);
  void addTo(ModuleExpression* expr);
  void finishView();
  bool evaluate();
  bool isComplete();
  ImportModule* getFromTheory() const;
  ImportModule* getToModule() const;
  Term* getOpMapTerm(Symbol* symbol) const;
  //
  //	These exist in order to up a view in the metalevel.
  //
  ModuleExpression* getFrom() const;
  ModuleExpression* getTo() const;
  const OpTermMap& getOpTermMap() const;  // maybe this exposes too much but we need it for the metalevel

protected:
  void regretToInform(Entity* doomedEntity);
  ConnectedComponent* mapComponent(const ConnectedComponent* component, ImportModule* module) const;
  //
  //	Op->Term mappings need to be handled by a subclass.
  //
  virtual bool handleOpTermMappings() = 0;
  bool insertOpToTermMapping(int fromLineNumber,
			     Term* fromTerm,
			     int toLineNumber,
			     Term* toTerm,
			     ImportModule* targetModule);

private:
  enum Status
    {
      INITIAL,
      GOOD,
      BAD,
      STALE
    };

  typedef map<int, pair<Sort*, int> > VarMap;

  static bool typeMatch(const ConnectedComponent* c1, const ConnectedComponent* c2);
  static bool typeMatch(const Symbol* s1, const Symbol* s2);

  void finishModule(ImportModule* module);
  bool checkSorts();
  bool checkOps();
  bool checkPolymorphicOps();
  void clearOpTermMap();
  Sort* mapSort(const Sort* sort, ImportModule* module) const;
  bool indexRhsVariables(Term* term, const VarMap& varMap, int lineNr);

  Interpreter* const owner;
  Status status;
  ModuleExpression* fromExpr;
  ModuleExpression* toExpr;
  ImportModule* fromTheory;
  ImportModule* toModule;
  OpTermMap opTermMap;		// map from op name to op->term mappings stored as term pairs
};

inline void
View::addFrom(ModuleExpression* expr)
{
  fromExpr = expr;
}

inline void
View::addTo(ModuleExpression* expr)
{
  toExpr = expr;
}

inline void
View::finishView()
{
  evaluate();
}

inline bool
View::isComplete()
{
  return status != INITIAL;
}

inline ImportModule*
View::getFromTheory() const
{
  Assert(status == GOOD, "view status not good");
  return fromTheory;
}

inline ImportModule*
View::getToModule() const
{
  Assert(status == GOOD, "view status not good");
  return toModule;
}

inline ModuleExpression*
View::getFrom() const
{
  return fromExpr;
}

inline ModuleExpression*
View::getTo() const
{
  return toExpr;
}

inline const View::OpTermMap&
View::getOpTermMap() const
{
  return opTermMap;
}

#ifndef NO_ASSERT
inline ostream&
operator<<(ostream& s, const View* v)
{
  //
  //	Needed to avoid ambiguity.
  //
  s << static_cast<const NamedEntity*>(v);
  return s;
}
#endif

#endif
